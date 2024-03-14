#include "fatfs_extend.h"

FATFS g_fs[FF_VOLUMES]; //	564B*FF_VOLUMES
FIL g_file;				//	554B
FILINFO g_fileinfo; 	//	278B
DIR g_dir;				//	48B
UINT g_br,g_bw;

FRESULT ff_exf_getfree(const TCHAR *drv)
{
	FATFS *fs1;
	FRESULT res;
	unsigned int fre_clust=0, fre_sect=0, tot_sect=0;
    res = f_getfree(drv, (DWORD*)&fre_clust, &fs1);
    if(res == FR_OK)
	{
	    tot_sect = (fs1->n_fatent-2)*fs1->csize;
	    fre_sect = fre_clust*fs1->csize;
#if FF_MAX_SS != 512
		tot_sect *= fs1->ssize/512;
		fre_sect *= fs1->ssize/512;
#endif
		if(tot_sect > 1024<<6){// > 32MB
			printf("%s total:%d MB, free:%d MB\r\n",drv,tot_sect>>11,fre_sect>>11);
		}
		else{
			printf("%s total:%d KB, free:%d KB\r\n",drv,tot_sect>>1,fre_sect>>1);
		}
 	}
	return res;
}

FRESULT ff_scan_files(const TCHAR * path)
{
	FRESULT res;
	g_fileinfo.fsize = FF_MAX_LFN * 2 + 1;
	res = f_opendir(&g_dir,(const TCHAR*)path);
	if (res == FR_OK)
	{
		while(1)
		{
			res = f_readdir(&g_dir, &g_fileinfo);
			if (res != FR_OK || g_fileinfo.fname[0] == 0)
				break;
			if (g_fileinfo.fname[0] == '.') continue;
			printf("%s/%s\r\n",path,g_fileinfo.fname);
		}
	}
	return res;
}

FRESULT ff_unlink(const TCHAR * path)
{
	return f_unlink(path); // Delete a File/Directory
}

FRESULT ff_read_file(const TCHAR* path)
{
	FRESULT res;
	unsigned int read_len   = 0;
	unsigned int read_total = 0;
	unsigned int read_size  = 0;
	unsigned int file_size  = -1;
	char buf[1024];
	memset(buf,0,1024);
	res = f_open(&g_file,path,FA_OPEN_ALWAYS|FA_READ);
	if(FR_OK == res){
	    file_size = f_size(&g_file);
	    printf("Read File Size: %d bytes\r\n", file_size);
	    printf("Read: ");
		do{
			if(file_size >= 1024){
				read_size = 1024;
				file_size -= 1024;
			}
			else {
				read_size = file_size;
				buf[read_size] = 0;
				file_size = 0;
			}
			res = f_read(&g_file,buf,read_size, &read_len);
			if(FR_OK == res){
				read_total += read_len;
				printf(buf);
			}
			else
				return res;

		}while(file_size);
		printf("\r\n");
		if(read_total == file_size)   return f_close(&g_file);
		return -1;
	}
	return res;
}

FRESULT ff_write_file(const TCHAR* path,const void* buff)
{
	FRESULT res;
	res = f_open(&g_file,path,FA_OPEN_ALWAYS|FA_WRITE);
	if(FR_OK == res){
		res = f_write(&g_file,buff,strlen(buff),&g_bw);
		return f_close(&g_file);
	}
	return res;
}

FRESULT ff_write_read_file(const TCHAR* path,const void* buff)
{
	FRESULT res;
	unsigned int read_len   = 0;
	unsigned int read_total = 0;
	unsigned int read_size  = 0;
	unsigned int file_size  = -1;
	char buf[1024];
	memset(buf,0,1024);
	res = f_open(&g_file,path,FA_OPEN_ALWAYS|FA_READ|FA_WRITE);
#if 1
	if(FR_OK == res){
		res = f_write(&g_file,buff,strlen(buff),&g_bw);
	   //  printf("f_sync: %d \r\n", f_sync(&g_file));
	    f_lseek(&g_file,0);
	}
#else
	if(FR_OK == res && FR_OK == f_write(&g_file,buff,strlen(buff),&g_bw))
		res = f_close(&g_file);
	if(FR_OK == res)
		res = f_open(&g_file,path,FA_OPEN_ALWAYS|FA_READ|FA_WRITE);
#endif
	if(FR_OK == res){
	    file_size = f_size(&g_file);
	    printf("Read File Size: %d bytes\r\n", file_size);
	    printf("Read: ");
		do{
			if(file_size >= 1024){
				read_size = 1024;
				file_size -= 1024;
			}
			else {
				read_size = file_size;
				buf[read_size] = 0;
				file_size = 0;
			}
			res = f_read(&g_file,buf,read_size, &read_len);
			if(FR_OK == res){
				read_total += read_len;
				printf(buf);
			}
			else
				return res;

		}while(file_size);
		printf("\r\n");
		if(read_total == file_size)   return f_close(&g_file);
		return -1;
	}
	return res;
}

FRESULT fs_mkfs( const TCHAR* path )
{
	FRESULT result = FR_OK;
    BYTE work[FF_MAX_SS]; /* Work area (larger is better for processing time) */
    result = f_mkfs(path, 0, work, sizeof work);
	printf("fs_mkfs result:%d\r\n",result);
    return result;
}

static BYTE convert_fopen_mode(const char *type) {
    BYTE mode = 0;
    if (strcmp(type, "r") == 0 || strcmp(type, "rb") == 0) {
        mode |= FA_READ;
    } else if (strcmp(type, "w") == 0) {
        mode |= FA_WRITE | FA_CREATE_ALWAYS;
    } else if (strcmp(type, "a") == 0) {
        mode |= FA_WRITE | FA_OPEN_ALWAYS | FA_OPEN_APPEND;
    } else if (strcmp(type, "r+") == 0) {
        mode |= FA_READ | FA_WRITE;
    } else if (strcmp(type, "w+") == 0) {
        mode |= FA_READ | FA_WRITE | FA_CREATE_ALWAYS;
    } else if (strcmp(type, "a+") == 0) {
        mode |= FA_READ | FA_WRITE | FA_OPEN_ALWAYS | FA_OPEN_APPEND;
    }
    else{
    	mode |= FA_READ | FA_WRITE;
    }
    return mode;
}

FILE* fopen(const char* __restrict _name, const char* __restrict _type){
	return f_open(&g_file,_name,convert_fopen_mode(_type)) ? NULL : (FILE *)(&g_file);
}

FILE* freopen(const char* __restrict _name, const char* __restrict _type, FILE* __restrict fp){
	return f_open((FIL*)fp,_name,convert_fopen_mode(_type)) ? NULL : (FILE *)(fp);
}

int	fclose(FILE* fp){
	return f_close((FIL*)fp);
}

size_t	fread(_PTR __restrict buff, size_t _size, size_t _n, FILE* __restrict fp){
	size_t len;
	return f_read ((FIL*)fp,buff,_size*_n, &len) ? 0 : len;
}

size_t	fwrite(const _PTR __restrict buff, size_t _size, size_t _n, FILE* fp){
	size_t len;
	return f_write ((FIL*)fp,buff,_size*_n, &len) ? 0 : len;
}

long ftell(FILE* fp){
	return f_tell((FIL*)fp);
}

void rewind(FILE* fp){
	f_rewind((FIL*)fp);
}

int	fseek(FILE* fp, long offset, __attribute__((unused)) int whence){
	return f_lseek((FIL*)fp, (DWORD)offset);
}

int	rename(const char* path_old, const char* path_new){
	return f_rename((const TCHAR*)path_old, (const TCHAR*)path_new);
}
