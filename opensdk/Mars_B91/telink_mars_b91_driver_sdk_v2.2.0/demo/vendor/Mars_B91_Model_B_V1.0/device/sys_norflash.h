#ifndef  __SYS_NORFLASH_H__
#define  __SYS_NORFLASH_H__
/* Enable C linkage for C++ Compilers: */
#if defined(__cplusplus)
extern "C" {
#endif

#ifndef SYS_NORFLASH_DISK_ADDRESS
#define SYS_NORFLASH_DISK_ADDRESS 	0x80000
#endif
#ifndef SYS_NORFLASH_DISK_SIZE
#define SYS_NORFLASH_DISK_SIZE 		400*1024 // Must be greater than  (128 + N_SEC_TRACK)*GET_SECTOR_SIZE   Must be greater than or equal to 32K
#endif
int sys_norflash_write_lba(unsigned char * buffer, unsigned int lba, unsigned int count);
int sys_norflash_read_lba(unsigned char * buffer, unsigned int lba, unsigned int count);
int sys_norflash_erase_lba(unsigned int lba, unsigned int total_bytes);
void sys_norflash_clear_dcachetag(void);

/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif
#endif
