#ifndef TLKFS_DISK_H
#define TLKFS_DISK_H

#ifndef nullptr
#define nullptr 	((void *)0)
#endif

typedef struct{
	unsigned char isReady;
	unsigned char blockSize;
	unsigned short sectorSize;
	unsigned int sectorCount;
	unsigned int(*Init)(void);
	int(*Read)(unsigned char *pBuff, unsigned int blkOffs, unsigned int blkNumb);
	int(*Write)(unsigned char *pData, unsigned int blkOffs, unsigned int blkNumb);
}tlkfs_disk_t;

tlkfs_disk_t *tlkfs_getDisk(unsigned char volNum);
int  tlkfs_addDisk(tlkfs_disk_t *pDisk, unsigned char volNumb);
void tlkfs_delDisk(tlkfs_disk_t *pDisk);
void tlkfs_clrDisk(void);

#endif


