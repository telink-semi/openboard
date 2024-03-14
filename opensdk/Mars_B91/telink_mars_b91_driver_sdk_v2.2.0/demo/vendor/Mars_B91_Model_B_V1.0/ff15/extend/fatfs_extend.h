#ifndef _FATFS_EXTEND_DEFINED
#define _FATFS_EXTEND_DEFINED
#include <string.h>
#include <stdio.h>
#include "../source/ff.h"

extern FATFS g_fs[FF_VOLUMES]; 	//	564B*FF_VOLUMES
extern FIL g_file;				//	554B
extern FILINFO g_fileinfo; 		//	278B
extern DIR g_dir;				//	48B
extern UINT g_br,g_bw;
//extern unsigned char g_fatbuf;//SD卡数据缓存区

#ifdef __cplusplus
extern "C" {
#endif

FRESULT ff_exf_getfree(const TCHAR *drv);
FRESULT ff_scan_files(const TCHAR * path);
FRESULT ff_unlink(const TCHAR * path);
FRESULT ff_read_file(const TCHAR* path);
FRESULT ff_write_file(const TCHAR* path,const void* buff);
FRESULT ff_write_read_file(const TCHAR* path,const void* buff);
FRESULT fs_mkfs( const TCHAR* path );

#ifdef __cplusplus
}
#endif

#endif
