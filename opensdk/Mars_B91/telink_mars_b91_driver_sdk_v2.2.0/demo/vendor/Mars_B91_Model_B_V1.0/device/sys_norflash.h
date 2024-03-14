#ifndef  __SYS_NORFLASH_H__
#define  __SYS_NORFLASH_H__
/* Enable C linkage for C++ Compilers: */
#if defined(__cplusplus)
extern "C" {
#endif
#define SYS_NORFLASH_DISK_ADDRESS 	0x80000
#define SYS_NORFLASH_DISK_SIZE 		400*1024//400k  Must be greater than or equal to 32K

void sys_norflash_write_lba(unsigned char * buffer, unsigned int lba, unsigned int count);
void sys_norflash_read_lba(unsigned char * buffer, unsigned int lba, unsigned int count);
void sys_norflash_erase_lba(unsigned int lba, unsigned int total_bytes);

/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif
#endif
