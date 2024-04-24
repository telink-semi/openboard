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
#ifdef __cplusplus
extern "C" {
#endif

FRESULT ff_exf_getfree(const TCHAR *drv,unsigned int *tot_sect,unsigned int *fre_sect);
FRESULT ff_scan_files(const TCHAR * path);
FRESULT ff_scan_root_lua_files(const TCHAR * path,const char* suffix);
FRESULT ff_unlink(const TCHAR * path);
FRESULT ff_read_file(const TCHAR* path);
FRESULT ff_write_file(const TCHAR* path,const void* buff);
FRESULT ff_write_read_file(const TCHAR* path,const void* buff);
FRESULT fs_mkfs( const TCHAR* path );

#ifdef __cplusplus
}
#endif

#endif
