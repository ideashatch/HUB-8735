#include "Arduino.h"
#include "HUB8735FatFS.h"
#include "flash_api.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "rtl8735b.h"

#ifdef __cplusplus
}
#endif

extern phal_sdhost_adapter_t psdioh_adapter;

HUB8735FatFS::HUB8735FatFS(void) {
    // init to no card intialized
    fatfs_sd.drv_num = -1;
}

bool HUB8735FatFS::begin(void) {
    FRESULT res;

    res = (FRESULT)fatfs_sd_init();
    if (res != 0) {
        printf("fatfs_sd_init fail (%d)\r\n", res);
        return (res == FR_OK);
    }
    fatfs_sd_get_param(&fatfs_sd);

    return (res == FR_OK);
}

void HUB8735FatFS::end(void) {
    fatfs_sd_close();
    memset(&fatfs_sd, 0, sizeof(fatfs_sd_params_t));
    fatfs_sd.drv_num = -1;
}

File HUB8735FatFS::open(const String& path) {
    return open(path.c_str());
}

File HUB8735FatFS::open(const char* path) {
    if (fatfs_sd.drv_num < 0) {
        return File();
    }

    return File(path);
}

bool HUB8735FatFS::exists(const String& path) {
    return exists(path.c_str());
}

bool HUB8735FatFS::exists(const char* path) {
    if (fatfs_sd.drv_num < 0) {
        return 0;
    }

    FRESULT ret;
    FILINFO finfo;

    ret = f_stat(path, &finfo);
    return (ret == FR_OK);
}

bool HUB8735FatFS::remove(const String& path) {
    return remove(path.c_str());
}

bool HUB8735FatFS::remove(const char* path) {
    if (fatfs_sd.drv_num < 0) {
        return 0;
    }

    FRESULT ret = FR_OK;

    ret = f_unlink(path);
    return (ret == FR_OK);
}

bool HUB8735FatFS::rename(const String& pathFrom, const String& pathTo) {
    return rename(pathFrom.c_str(), pathTo.c_str());
}

bool HUB8735FatFS::rename(const char* pathFrom, const char* pathTo) {
    if (fatfs_sd.drv_num < 0) {
        return 0;
    }

    FRESULT ret = FR_OK;

    ret = f_rename(pathFrom, pathTo);
    return (ret == FR_OK);
}

bool HUB8735FatFS::mkdir(const String &path) {
    return mkdir(path.c_str());
}

bool HUB8735FatFS::mkdir(const char *path) {
    if (fatfs_sd.drv_num < 0) {
        return 0;
    }

    FRESULT ret = FR_OK;
    ret = f_mkdir(path);
    return (ret == FR_OK);
}

bool HUB8735FatFS::rmdir(const String &path) {
    return rmdir(path.c_str());
}

bool HUB8735FatFS::rmdir(const char *path) {
    if (fatfs_sd.drv_num < 0) {
        return 0;
    }

    FRESULT ret = FR_OK;

    ret = f_unlink(path);
    return (ret == FR_OK);
}

char *HUB8735FatFS::getRootPath(void) {
    if (fatfs_sd.drv_num < 0) {
        return NULL;
    } else {
        return fatfs_sd.drv;
    }
}

int HUB8735FatFS::readDir(char *path, char *result_buf, unsigned int bufsize) {
    FRESULT ret = FR_OK;
    FILINFO fno;
    DIR dir;

    char *fn;
    unsigned int fnlen;
    int bufidx = 0;

#if _USE_LFN && ((!defined(FATFS_R_13C)) && (!defined(FATFS_R_14B)))
    char lfn[(_MAX_LFN + 1)];
    fno.lfname = lfn;
    fno.lfsize = sizeof(lfn);
#endif

    do {
        if (fatfs_sd.drv_num < 0) {
            ret = FR_DISK_ERR;
            break;
        }

        ret = f_opendir(&dir, path);
        if (ret != FR_OK) {
            break;
        }

        memset(result_buf, 0, bufsize);

        while (1) {
            // call f_readdir repeatedly until all files in directory have been processed.
            ret = f_readdir(&dir, &fno);
            if ((ret != FR_OK) || (fno.fname[0] == 0)) {
                break;
            }

#if _USE_LFN && ((!defined(FATFS_R_13C)) && (!defined(FATFS_R_14B)))
            if (*fno.lfname)
            {
                fn = fno.lfname;
                fnlen = fno.lfsize;
            }
            else
#endif
            {
                fn = fno.fname;
                fnlen = strlen(fn);
            }

            // print file names into buffer
            if ((bufidx + fnlen + 1) < bufsize) {
                bufidx += sprintf((result_buf + bufidx), "%s", fn);
                //bufidx++;
            }
        }
    } while (0);

    return (-ret);
}

bool HUB8735FatFS::isDir(char *path) {
    unsigned char attr;
    if (getAttribute(path, &attr) >= 0) {
        if (attr & AM_DIR) {
            return 1;
        }
    }
    return 0;
}

bool HUB8735FatFS::isFile(char *path) {
    unsigned char attr;
    if (getAttribute(path, &attr) >= 0) {
        if (attr & AM_ARC) {
            return 1;
        }
    }
    return 0;
}

int HUB8735FatFS::getLastModTime(char *path, uint16_t *year, uint16_t *month, uint16_t *date, uint16_t *hour, uint16_t *minute, uint16_t *second) {
    FRESULT ret = FR_OK;
    FILINFO fno;
#if _USE_LFN && ((!defined(FATFS_R_13C)) && (!defined(FATFS_R_14B)))
    char lfn[(_MAX_LFN + 1)];
    fno.lfname = lfn;
    fno.lfsize = sizeof(lfn);
#endif

    do {
        if (fatfs_sd.drv_num < 0) {
            ret = FR_DISK_ERR;
            break;
        }

        ret = f_stat(path, &fno);
        if (ret != FR_OK) {
            break;
        }

        *year   = (fno.fdate >> 9) + 1980;
        *month  = (fno.fdate >> 5) & 0x0F;
        *date   = (fno.fdate & 0x1F);
        *hour   = (fno.ftime >> 11);
        *minute = (fno.ftime >> 5) & 0x3F;
        *second = (fno.ftime & 0x1F) * 2;

    } while (0);

    return (-ret);
}

int HUB8735FatFS::setLastModTime(char *path, uint16_t year, uint16_t month, uint16_t date, uint16_t hour, uint16_t minute, uint16_t second) {
    FRESULT ret = FR_OK;

//#if FF_USE_CHMOD && !FF_FS_READONLY // f_utime is not available if these macros are not set
    FILINFO fno;

#if _USE_LFN && ((!defined(FATFS_R_13C)) && (!defined(FATFS_R_14B)))
    char lfn[(_MAX_LFN + 1)];
    fno.lfname = lfn;
    fno.lfsize = sizeof(lfn);
#endif

    do {
        if (fatfs_sd.drv_num < 0) {
            ret = FR_DISK_ERR;
            break;
        }

        fno.fdate = 0x0000 | ((year - 1980) <<  9) | ((month  & 0x0F) << 5) | (date & 0x1F);
        fno.ftime = 0x0000 | ((hour & 0x1F) << 11) | ((minute & 0x3F) << 5) | ((second/2) & 0x1F) ;
        ret = f_utime(path, &fno);
        if (ret != FR_OK) {
            break;
        }
    } while (0);
    
//#endif
    return (-ret);
}


void HUB8735FatFS::sd_upgrade(void){
    FIL m_file;
    //fatfs_sd_params_t fatfs_sd;
    flash_t flash;
    FRESULT res;
    char fw_name[12] = "hub8735.bin";	//arduino size must string size + 1
    char path[32] = {0};
    unsigned int fw_len, addr, r_len;
    unsigned short block_max = 256;
    unsigned short i, w_num, presentage;
    unsigned char *r_buf;

	res = (FRESULT)fatfs_sd_init();
	if (res < 0) {
		printf("fatfs_sd_init fail (%d)\n", res);
		goto fail;
	}

	printf("sd_upgrade enter\r\n");
	//printf("[%s] fw_nam %s\r\n", __FUNCTION__, fw_name);
    //Get SD parameters
    fatfs_sd_get_param(&fatfs_sd);
    
    //Pack FW file path
    sprintf(path, "%s%s",fatfs_sd.drv, fw_name);

    //Open FW file
    res = f_open(&m_file, path, FA_OPEN_EXISTING | FA_READ);

    //Check if FW file exist
    if(res != FR_OK)
    {
        //FW file doesn't esixt
        printf("[%s] Not found upgrade file 'hub8735.bin'\r\n", __FUNCTION__);
        return;
        //goto fail;
    }
    else
    {
        //Get FW file length
        fw_len = f_size(&m_file);
        
        //Check if file length is valid
        if(fw_len > 0)
        {
            printf("[%s] %s size %d\r\n",__FUNCTION__, fw_name, fw_len);
            
            //Erase flash
            addr = 0;
            presentage = 100;
            
            for(i=0;i<block_max;i++)
            {
                flash_erase_block(&flash, addr+(i*64*1024));
                if(presentage != (((i+1)*100)/block_max))
                {
                	presentage = (((i+1)*100)/block_max);
                	printf("[%s] Erasing... %d%%\r\n",__FUNCTION__,presentage);
                }
            }
            //Write to flash
            addr = 0;
            presentage = 100;
            
            w_num = fw_len / (4*1024);
            if((fw_len % (4*1024)))
            {
               w_num++;
            }
            
            r_buf = (unsigned char *)malloc(4*1024);
            if(r_buf == 0)
            {
                f_close(&m_file);
                return;
            }
            
            for(i=0;i<w_num;i++)
            {
                if(f_read(&m_file, r_buf, 4*1024, (u32 *)&r_len) != FR_OK)
                {
                    //Read failed
                    free(r_buf);
                    f_close(&m_file);
                    printf("[%s] Read file failed",__FUNCTION__);
                    return;
                }
               
                flash_stream_write(&flash, addr, r_len, r_buf);
                //flash_stream_read(&flash, addr, r_len, r_buf);                
                addr += r_len;
                if(presentage != (((i+1)*100)/w_num))
                {
                	presentage = (((i+1)*100)/w_num);
                	printf("[%s] Upgrading.... %d%% \r\n",__FUNCTION__, presentage);
                }
            }
            printf("[%s] Upgrade done\r\n",__FUNCTION__);
        	free(r_buf);
        }
        else
        {
            printf("[%s] %s is empty\r\n",__FUNCTION__, fw_name);
        }
        f_close(&m_file);
                
      
        f_unlink(path);
        printf("[%s] Delete file & Hold system, please reset board\r\n", __FUNCTION__);       
        while(1);
    }
    
    fail:

	fatfs_sd_close();

}

int HUB8735FatFS::status(void) {
    return fatfs_sd_is_inited();
}

int HUB8735FatFS::getAttribute(char *path, unsigned char *attr) {
    FRESULT ret = FR_OK;
    FILINFO fno;
#if _USE_LFN && ((!defined(FATFS_R_13C)) && (!defined(FATFS_R_14B)))
    char lfn[(_MAX_LFN + 1)];
    fno.lfname = lfn;
    fno.lfsize = sizeof(lfn);
#endif

    do {
        if (fatfs_sd.drv_num < 0) {
            ret = FR_DISK_ERR;
            break;
        }

        ret = f_stat(path, &fno);
        if (ret != FR_OK) {
            break;
        }

        *attr = fno.fattrib;
    } while (0);

    return -ret;
}
