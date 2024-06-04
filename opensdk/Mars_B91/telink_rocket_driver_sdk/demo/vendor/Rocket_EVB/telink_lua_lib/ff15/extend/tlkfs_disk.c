#include "tlkfs_disk.h"
#include "../source/ff.h"

#define TLKFS_DISK_MAX_NUMB        FF_VOLUMES

static tlkfs_disk_t *spTlkFsDisks[TLKFS_DISK_MAX_NUMB] = {0};

tlkfs_disk_t *tlkfs_getDisk(unsigned char volNum)
{
	if(volNum >= TLKFS_DISK_MAX_NUMB) return nullptr;
	return spTlkFsDisks[volNum];
}

int tlkfs_addDisk(tlkfs_disk_t *pDisk, unsigned char volNum)
{
	if(pDisk == nullptr || pDisk->sectorSize == 0 || (pDisk->sectorSize & 0x1F) != 0
		|| pDisk->sectorCount < (128 + 2) || pDisk->Read == nullptr){
		return -1;
	}
	if(volNum >= TLKFS_DISK_MAX_NUMB) return -2;
	spTlkFsDisks[volNum] = pDisk;
	return 0;
}

void tlkfs_delDisk(tlkfs_disk_t *pDisk)
{
	unsigned char index;
	if(pDisk == nullptr) return;
	for(index=0; index<TLKFS_DISK_MAX_NUMB; index++){
		if(spTlkFsDisks[index] == pDisk) break;
	}
	if(index == TLKFS_DISK_MAX_NUMB) return;
	spTlkFsDisks[index] = nullptr;
}

void tlkfs_clrDisk(void)
{
	unsigned char index;
	for(index=0; index<TLKFS_DISK_MAX_NUMB; index++){
		spTlkFsDisks[index] = nullptr;
	}
}

