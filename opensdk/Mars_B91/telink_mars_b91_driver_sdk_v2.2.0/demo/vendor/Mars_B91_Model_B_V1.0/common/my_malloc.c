#include "assert.h"
#include "string.h"
#include "my_malloc.h"
/*
 * FreeRTOS Kernel V10.5.1
 * Copyright (C) 2021 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * https://www.FreeRTOS.org
 * https://github.com/FreeRTOS
 *
 */

/*
 * A sample implementation of pvPortMalloc() and vPortFree() that combines
 * (coalescences) adjacent memory blocks as they are freed, and in so doing
 * limits memory fragmentation.
 *
 * See heap_1.c, heap_2.c and heap_3.c for alternative implementations, and the
 * memory management pages of https://www.FreeRTOS.org for more information.
 */

/* Define the linked list structure.  This is used to link free blocks in order
 * of their size. */
typedef struct A_BlockLink_t
{
    struct A_BlockLink_t * pxNextFreeBlock;  /*<< The next free block in the list. */
    u32 xBlockSize;                    		 /*<< The size of the free block. */
} BlockLink_t;

#ifndef portPOINTER_u32YPE
    #define portPOINTER_u32YPE    	  u32
#endif

/* A few bytes might be lost to byte aligning the heap start address. */
#define configADJUSTED_HEAP_SIZE    ( configTOTAL_HEAP_SIZE - portBYTE_ALIGNMENT )

#if portBYTE_ALIGNMENT == 32
    #define portBYTE_ALIGNMENT_MASK    ( 0x001f )
#elif portBYTE_ALIGNMENT == 16
    #define portBYTE_ALIGNMENT_MASK    ( 0x000f )
#elif portBYTE_ALIGNMENT == 8
    #define portBYTE_ALIGNMENT_MASK    ( 0x0007 )
#elif portBYTE_ALIGNMENT == 4
    #define portBYTE_ALIGNMENT_MASK    ( 0x0003 )
#elif portBYTE_ALIGNMENT == 2
    #define portBYTE_ALIGNMENT_MASK    ( 0x0001 )
#elif portBYTE_ALIGNMENT == 1
    #define portBYTE_ALIGNMENT_MASK    ( 0x0000 )
#else /* if portBYTE_ALIGNMENT == 32 */
    #error "Invalid portBYTE_ALIGNMENT definition"
#endif /* if portBYTE_ALIGNMENT == 32 */

__attribute__((aligned(portBYTE_ALIGNMENT))) static u8 ucHeap[configTOTAL_HEAP_SIZE];
static BlockLink_t xStart;
static BlockLink_t * pxEnd = NULL;
/* Keeps track of the number of calls to allocate and free memory as well as the
 * number of free bytes remaining, but says nothing about fragmentation. */
static u32 xFreeBytesRemaining = 0U;
static u32 xMinimumEverFreeBytesRemaining = 0U;
static u32 xNumberOfSuccessfulAllocations = 0;
static u32 xNumberOfSuccessfulFrees = 0;

/* The size of the structure placed at the beginning of each allocated memory
 * block must by correctly byte aligned. */
static const u32 xHeapStructSize = ( sizeof( BlockLink_t ) + ( ( u32 ) ( portBYTE_ALIGNMENT - 1 ) ) ) & ~( ( u32 ) portBYTE_ALIGNMENT_MASK );// 8
/* Block sizes must not get too small. */
#define heapMINIMUM_BLOCK_SIZE    				 ( (  u32  ) ( xHeapStructSize << 1 ) )

/* Assumes 8bit bytes! */
#define heapBITS_PER_BYTE           		  	 ( ( u32 ) 8 )
/* Max value that fits in a u32 type. */
#define heapSIZE_MAX                		  	 ( ~( ( u32 ) 0 ) )
/* Check if adding a and b will result in overflow. */
#define heapADD_WILL_OVERFLOW( a, b )         	 ( ( a ) > ( heapSIZE_MAX - ( b ) ) )
/* Check if multiplying a and b will result in overflow. */
#define heapMULTIPLY_WILL_OVERFLOW( a, b )    	 ( ( ( a ) > 0 ) && ( ( b ) > ( heapSIZE_MAX / ( a ) ) ) )
/* Check if adding a and b will result in overflow. */
#define heapADD_WILL_OVERFLOW( a, b )        	 ( ( a ) > ( heapSIZE_MAX - ( b ) ) )
/* MSB of the xBlockSize member of an BlockLink_t structure is used to track
 * the allocation status of a block.  When MSB of the xBlockSize member of
 * an BlockLink_t structure is set then the block belongs to the application.
 * When the bit is free the block is still part of the free heap space. */
#define heapBLOCK_ALLOCATED_BITMASK   			 ( ( ( u32 ) 1 ) << ( ( sizeof( u32 ) * heapBITS_PER_BYTE ) - 1 ) )
#define heapBLOCK_SIZE_IS_VALID( xBlockSize )    ( ( ( xBlockSize ) & heapBLOCK_ALLOCATED_BITMASK ) == 0 )
#define heapBLOCK_IS_ALLOCATED( pxBlock )        ( ( ( pxBlock->xBlockSize ) & heapBLOCK_ALLOCATED_BITMASK ) != 0 )
#define heapALLOCATE_BLOCK( pxBlock )            ( ( pxBlock->xBlockSize ) |= heapBLOCK_ALLOCATED_BITMASK )
#define heapFREE_BLOCK( pxBlock )                ( ( pxBlock->xBlockSize ) &= ~heapBLOCK_ALLOCATED_BITMASK )

#ifndef configHEAP_CLEAR_MEMORY_ON_FREE
    #define configHEAP_CLEAR_MEMORY_ON_FREE      0
#endif

#ifndef configASSERT
    #define configASSERT( x )    				 assert(x);
#endif

#ifndef mtCOVERAGE_TEST_MARKER
    #define mtCOVERAGE_TEST_MARKER()
#endif

void vPortInitialiseBlocks( void )
{
	memset( ucHeap, 0, configTOTAL_HEAP_SIZE);
}

static void	prvHeapInit(void)
{
	BlockLink_t* pxFirstFreeBlock;
	u8* pucAlignedHeap;

    portPOINTER_u32YPE uxAddress;
    u32 xTotalHeapSize = configTOTAL_HEAP_SIZE;

    /* Ensure the heap starts on a correctly aligned boundary. */
    uxAddress = ( portPOINTER_u32YPE ) ucHeap;

    if( ( uxAddress & portBYTE_ALIGNMENT_MASK ) != 0 )
    {
        uxAddress += ( portBYTE_ALIGNMENT - 1 );
        uxAddress &= ~( ( portPOINTER_u32YPE ) portBYTE_ALIGNMENT_MASK );
        xTotalHeapSize -= uxAddress - ( portPOINTER_u32YPE ) ucHeap;
    }

    pucAlignedHeap = ( u8 * ) uxAddress;

    /* xStart is used to hold a pointer to the first item in the list of free
     * blocks.  The void cast is used to prevent compiler warnings. */
    xStart.pxNextFreeBlock = ( void * ) pucAlignedHeap;
    xStart.xBlockSize = ( u32 ) 0;

    /* pxEnd is used to mark the end of the list of free blocks and is inserted
     * at the end of the heap space. */
    uxAddress = ( ( portPOINTER_u32YPE ) pucAlignedHeap ) + xTotalHeapSize;
    uxAddress -= xHeapStructSize;
    uxAddress &= ~( ( portPOINTER_u32YPE ) portBYTE_ALIGNMENT_MASK );
    pxEnd = ( BlockLink_t * ) uxAddress;
    pxEnd->xBlockSize = 0;
    pxEnd->pxNextFreeBlock = NULL;

    /* To start with there is a single free block that is sized to take up the
     * entire heap space, minus the space taken by pxEnd. */
    pxFirstFreeBlock = ( BlockLink_t * ) pucAlignedHeap;
    pxFirstFreeBlock->xBlockSize = ( u32 ) ( uxAddress - ( portPOINTER_u32YPE ) pxFirstFreeBlock );
    pxFirstFreeBlock->pxNextFreeBlock = pxEnd;

    /* Only one block exists - and it covers the entire usable heap space. */
    xMinimumEverFreeBytesRemaining = pxFirstFreeBlock->xBlockSize;
    xFreeBytesRemaining = pxFirstFreeBlock->xBlockSize;
}

static void prvInsertBlockIntoFreeList( BlockLink_t * pxBlockToInsert ) /* PRIVILEGED_FUNCTION */
{
    BlockLink_t * pxIterator;
    u8 * puc;

    /* Iterate through the list until a block is found that has a higher address
     * than the block being inserted. */
    for( pxIterator = &xStart; pxIterator->pxNextFreeBlock < pxBlockToInsert; pxIterator = pxIterator->pxNextFreeBlock )
    {
        /* Nothing to do here, just iterate to the right position. */
    }

    /* Do the block being inserted, and the block it is being inserted after
     * make a contiguous block of memory? */
    puc = ( u8 * ) pxIterator;

    if( ( puc + pxIterator->xBlockSize ) == ( u8 * ) pxBlockToInsert )
    {
        pxIterator->xBlockSize += pxBlockToInsert->xBlockSize;
        pxBlockToInsert = pxIterator;
    }
    else
    {
        mtCOVERAGE_TEST_MARKER();
    }

    /* Do the block being inserted, and the block it is being inserted before
     * make a contiguous block of memory? */
    puc = ( u8 * ) pxBlockToInsert;

    if( ( puc + pxBlockToInsert->xBlockSize ) == ( u8 * ) pxIterator->pxNextFreeBlock )
    {
        if( pxIterator->pxNextFreeBlock != pxEnd )
        {
            /* Form one big block from the two blocks. */
            pxBlockToInsert->xBlockSize += pxIterator->pxNextFreeBlock->xBlockSize;
            pxBlockToInsert->pxNextFreeBlock = pxIterator->pxNextFreeBlock->pxNextFreeBlock;
        }
        else
        {
            pxBlockToInsert->pxNextFreeBlock = pxEnd;
        }
    }
    else
    {
        pxBlockToInsert->pxNextFreeBlock = pxIterator->pxNextFreeBlock;
    }

    /* If the block being inserted plugged a gab, so was merged with the block
     * before and the block after, then it's pxNextFreeBlock pointer will have
     * already been set, and should not be set here as that would make it point
     * to itself. */
    if( pxIterator != pxBlockToInsert )
    {
        pxIterator->pxNextFreeBlock = pxBlockToInsert;
    }
    else
    {
        mtCOVERAGE_TEST_MARKER();
    }
}

void *pvPortMalloc( u32 xWantedSize )
{
    BlockLink_t * pxBlock;
    BlockLink_t * pxPreviousBlock;
    BlockLink_t * pxNewBlockLink;
    void * pvReturn = NULL;
    u32 xAdditionalRequiredSize;

    {
        /* If this is the first call to malloc then the heap will require
         * initialisation to setup the list of free blocks. */
        if( pxEnd == NULL )
        {
            prvHeapInit();
        }
        else
        {
            mtCOVERAGE_TEST_MARKER();
        }

        if( xWantedSize > 0 )
        {
            /* The wanted size must be increased so it can contain a BlockLink_t
             * structure in addition to the requested amount of bytes. Some
             * additional increment may also be needed for alignment. */
            xAdditionalRequiredSize = xHeapStructSize + portBYTE_ALIGNMENT - ( xWantedSize & portBYTE_ALIGNMENT_MASK );

            if( heapADD_WILL_OVERFLOW( xWantedSize, xAdditionalRequiredSize ) == 0 )
            {
                xWantedSize += xAdditionalRequiredSize;
            }
            else
            {
                xWantedSize = 0;
            }
        }
        else
        {
            mtCOVERAGE_TEST_MARKER();
        }
        /* Check the block size we are trying to allocate is not so large that the
         * top bit is set.  The top bit of the block size member of the BlockLink_t
         * structure is used to determine who owns the block - the application or
         * the kernel, so it must be free. */
        if( heapBLOCK_SIZE_IS_VALID( xWantedSize ) != 0 )
        {
            if( ( xWantedSize > 0 ) && ( xWantedSize <= xFreeBytesRemaining ) )
            {
                /* Traverse the list from the start (lowest address) block until
                 * one of adequate size is found. */
                pxPreviousBlock = &xStart;
                pxBlock = xStart.pxNextFreeBlock;

                while( ( pxBlock->xBlockSize < xWantedSize ) && ( pxBlock->pxNextFreeBlock != NULL ) )
                {
                    pxPreviousBlock = pxBlock;
                    pxBlock = pxBlock->pxNextFreeBlock;
                }

                /* If the end marker was reached then a block of adequate size
                 * was not found. */
                if( pxBlock != pxEnd )
                {
                    /* Return the memory space pointed to - jumping over the
                     * BlockLink_t structure at its start. */
                    pvReturn = ( void * ) ( ( ( uint8_t * ) pxPreviousBlock->pxNextFreeBlock ) + xHeapStructSize );

                    /* This block is being returned for use so must be taken out
                     * of the list of free blocks. */
                    pxPreviousBlock->pxNextFreeBlock = pxBlock->pxNextFreeBlock;

                    /* If the block is larger than required it can be split into
                     * two. */
                    if( ( pxBlock->xBlockSize - xWantedSize ) > heapMINIMUM_BLOCK_SIZE )
                    {
                        /* This block is to be split into two.  Create a new
                         * block following the number of bytes requested. The void
                         * cast is used to prevent byte alignment warnings from the
                         * compiler. */
                        pxNewBlockLink = ( void * ) ( ( ( uint8_t * ) pxBlock ) + xWantedSize );
                        configASSERT( ( ( ( u32 ) pxNewBlockLink ) & portBYTE_ALIGNMENT_MASK ) == 0 );

                        /* Calculate the sizes of two blocks split from the
                         * single block. */
                        pxNewBlockLink->xBlockSize = pxBlock->xBlockSize - xWantedSize;
                        pxBlock->xBlockSize = xWantedSize;

                        /* Insert the new block into the list of free blocks. */
                        prvInsertBlockIntoFreeList( pxNewBlockLink );
                    }
                    else
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }

                    xFreeBytesRemaining -= pxBlock->xBlockSize;

                    if( xFreeBytesRemaining < xMinimumEverFreeBytesRemaining )
                    {
                        xMinimumEverFreeBytesRemaining = xFreeBytesRemaining;
                    }
                    else
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }

                    /* The block is being returned - it is allocated and owned
                     * by the application and has no "next" block. */
                    heapALLOCATE_BLOCK( pxBlock );
                    pxBlock->pxNextFreeBlock = NULL;
                    xNumberOfSuccessfulAllocations++;
                }
                else
                {
                    mtCOVERAGE_TEST_MARKER();
                }
            }
            else
            {
                mtCOVERAGE_TEST_MARKER();
            }
        }
        else
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }
    configASSERT( ( ( ( u32 ) pvReturn ) & ( u32 ) portBYTE_ALIGNMENT_MASK ) == 0 );
    return pvReturn;
}

void vPortFree( void * pv )
{
    u8 * puc = ( u8 * ) pv;
    BlockLink_t * pxLink;

    if( pv != NULL )
    {
        /* The memory being freed will have an BlockLink_t structure immediately
         * before it. */
        puc -= xHeapStructSize;

        /* This casting is to keep the compiler from issuing warnings. */
        pxLink = ( void * ) puc;

        configASSERT( heapBLOCK_IS_ALLOCATED( pxLink ) != 0 );
        configASSERT( pxLink->pxNextFreeBlock == NULL );

        if( heapBLOCK_IS_ALLOCATED( pxLink ) != 0 )
        {
            if( pxLink->pxNextFreeBlock == NULL )
            {
                /* The block is being returned to the heap - it is no longer
                 * allocated. */
                heapFREE_BLOCK( pxLink );
                #if ( configHEAP_CLEAR_MEMORY_ON_FREE == 1 )
                {
                    ( void ) memset( puc + xHeapStructSize, 0, pxLink->xBlockSize - xHeapStructSize );
                }
                #endif
                {
                    /* Add this block to the list of free blocks. */
                    xFreeBytesRemaining += pxLink->xBlockSize;
                    prvInsertBlockIntoFreeList( ( ( BlockLink_t * ) pxLink ) );
                    xNumberOfSuccessfulFrees++;
                }
            }
            else
            {
                mtCOVERAGE_TEST_MARKER();
            }
        }
        else
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }
}

void *pvPortReMalloc(void *ptr,u32 xWantedSize )
{
	void *relocal = NULL;
	relocal = pvPortMalloc(xWantedSize);
	if(relocal==NULL)return NULL;
	else
	{
	    memcpy(relocal,ptr,xWantedSize);        // 拷贝旧内存内容到新内存
	    vPortFree(ptr);  						// 释放旧内存
		return (void*)(relocal);  				// 返回新内存首地址
	}
}

u8 vPortPerused(void)
{
    unsigned int used =  configTOTAL_HEAP_SIZE - xFreeBytesRemaining;
    return (used*100)/(configTOTAL_HEAP_SIZE);
}
#if 0
void free_rtos_heap4_test()
{
	u8 *t1 = (u8 *)pvPortMalloc(80);
	t1[0] = 0x12;
	t1[1] = 0x34;
	t1[2] = 0x56;
	t1[20] = 0x78;
	printf("xFreeBytesRemaining:%d\r\n",xFreeBytesRemaining);

	u8 *t2 = (u8 *)pvPortMalloc(1024*5);
	t2[0] = 0x12;
	t2[1] = 0x34;
	t2[2] = 0x56;
	t2[20] = 0x78;
	printf("xFreeBytesRemaining:%d\r\n",xFreeBytesRemaining);
	printf("xmem_perused:%d\r\n",vPortPerused());

	vPortFree(t2);
	printf("xFreeBytesRemaining:%d\r\n",xFreeBytesRemaining);
	printf("xmem_perused:%d\r\n",vPortPerused());
}
#endif
#if 0
u32 xPortGetFreeHeapSize( void )
{
    return xFreeBytesRemaining;
}

u32 xPortGetMinimumEverFreeHeapSize( void )
{
    return xMinimumEverFreeBytesRemaining;
}

void * pvPortCalloc( u32 xNum,
                     u32 xSize )
{
    void * pv = NULL;
    if( heapMULTIPLY_WILL_OVERFLOW( xNum, xSize ) == 0 )
    {
        pv = pvPortMalloc( xNum * xSize );

        if( pv != NULL )
        {
            ( void ) memset( pv, 0, xNum * xSize );
        }
    }
    return pv;
}

/* Used to pass information about the heap out of vPortGetHeapStats(). */
typedef struct xHeapStats
{
    u32 xAvailableHeapSpaceInBytes;      /* The total heap size currently available - this is the sum of all the free blocks, not the largest block that can be allocated. */
    u32 xSizeOfLargestFreeBlockInBytes;  /* The maximum size, in bytes, of all the free blocks within the heap at the time vPortGetHeapStats() is called. */
    u32 xSizeOfSmallestFreeBlockInBytes; /* The minimum size, in bytes, of all the free blocks within the heap at the time vPortGetHeapStats() is called. */
    u32 xNumberOfFreeBlocks;             /* The number of free memory blocks within the heap at the time vPortGetHeapStats() is called. */
    u32 xMinimumEverFreeBytesRemaining;  /* The minimum amount of total free memory (sum of all free blocks) there has been in the heap since the system booted. */
    u32 xNumberOfSuccessfulAllocations;  /* The number of calls to pvPortMalloc() that have returned a valid memory block. */
    u32 xNumberOfSuccessfulFrees;        /* The number of calls to vPortFree() that has successfully freed a block of memory. */
} HeapStats_t;
void vPortGetHeapStats( HeapStats_t * pxHeapStats )
{
    BlockLink_t * pxBlock;
    u32 xBlocks = 0, xMaxSize = 0, xMinSize = 0xffffffffUL; /* portMAX_DELAY used as a portable way of getting the maximum value. */

    {
        pxBlock = xStart.pxNextFreeBlock;

        /* pxBlock will be NULL if the heap has not been initialised.  The heap
         * is initialised automatically when the first allocation is made. */
        if( pxBlock != NULL )
        {
            while( pxBlock != pxEnd )
            {
                /* Increment the number of blocks and record the largest block seen
                 * so far. */
                xBlocks++;

                if( pxBlock->xBlockSize > xMaxSize )
                {
                    xMaxSize = pxBlock->xBlockSize;
                }

                if( pxBlock->xBlockSize < xMinSize )
                {
                    xMinSize = pxBlock->xBlockSize;
                }

                /* Move to the next block in the chain until the last block is
                 * reached. */
                pxBlock = pxBlock->pxNextFreeBlock;
            }
        }
    }
    pxHeapStats->xSizeOfLargestFreeBlockInBytes = xMaxSize;
    pxHeapStats->xSizeOfSmallestFreeBlockInBytes = xMinSize;
    pxHeapStats->xNumberOfFreeBlocks = xBlocks;
    {
        pxHeapStats->xAvailableHeapSpaceInBytes = xFreeBytesRemaining;
        pxHeapStats->xNumberOfSuccessfulAllocations = xNumberOfSuccessfulAllocations;
        pxHeapStats->xNumberOfSuccessfulFrees = xNumberOfSuccessfulFrees;
        pxHeapStats->xMinimumEverFreeBytesRemaining = xMinimumEverFreeBytesRemaining;
    }
}
#endif


