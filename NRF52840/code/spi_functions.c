#include "nrf.h"
#include "bsp.h"
#include "ff.h"
#include "diskio_blkdev.h"
#include "nrf_block_dev_sdc.h"


#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

#include "pid_functions.h"

#define SDC_SCK_PIN   14    ///< SDC serial clock (SCK) pin.
#define SDC_MOSI_PIN  13  ///< SDC serial data in (DI) pin.
#define SDC_MISO_PIN  15  ///< SDC serial data out (DO) pin.
#define SDC_CS_PIN    24  ///< SDC chip select (CS) pin.

#define FILE_NAME   "NORDIC.TXT"

extern volatile  int e;
extern volatile int deltaT;
extern volatile int target;
char time_buffer[5];
char pos_buffer[6];
char target_buffer[6];
char u_buffer[5];
char error_buffer[7];

NRF_BLOCK_DEV_SDC_DEFINE(
        m_block_dev_sdc,
        NRF_BLOCK_DEV_SDC_CONFIG(
                SDC_SECTOR_SIZE,
                APP_SDCARD_CONFIG(SDC_MOSI_PIN, SDC_MISO_PIN, SDC_SCK_PIN, SDC_CS_PIN)
         ),
         NFR_BLOCK_DEV_INFO_CONFIG("Nordic", "SDC", "1.00")
);

static FATFS fs;
static DIR dir;
static FILINFO fno;
static FIL file;

uint32_t bytes_written;
FRESULT ff_result;
DSTATUS disk_state = STA_NOINIT;

void fatfs_initialization()
{
    // Initialize FATFS disk I/O interface by providing the block device.
    static diskio_blkdev_t drives[] =
    {
            DISKIO_BLOCKDEV_CONFIG(NRF_BLOCKDEV_BASE_ADDR(m_block_dev_sdc, block_dev), NULL)
    };

    diskio_blockdev_register(drives, ARRAY_SIZE(drives));

    NRF_LOG_INFO("Initializing disk 0 (SDC)...");
    for (uint32_t retries = 3; retries && disk_state; --retries)
    {
        disk_state = disk_initialize(0);
    }
    if (disk_state)
    {
        NRF_LOG_INFO("Disk initialization failed.");
        return;
    }

    uint32_t blocks_per_mb = (1024uL * 1024uL) / m_block_dev_sdc.block_dev.p_ops->geometry(&m_block_dev_sdc.block_dev)->blk_size;
    uint32_t capacity = m_block_dev_sdc.block_dev.p_ops->geometry(&m_block_dev_sdc.block_dev)->blk_count / blocks_per_mb;
    NRF_LOG_INFO("Capacity: %d MB", capacity);

    NRF_LOG_INFO("Mounting volume...");
    ff_result = f_mount(&fs, "", 1);
    if (ff_result)
    {
        NRF_LOG_INFO("Mount failed.");
        return;
    }

    NRF_LOG_INFO("\r\n Listing directory: /");
    ff_result = f_opendir(&dir, "/");
    if (ff_result)
    {
        NRF_LOG_INFO("Directory listing failed!");
        return;
    }

    do
    {
        ff_result = f_readdir(&dir, &fno);
        if (ff_result != FR_OK)
        {
            NRF_LOG_INFO("Directory read failed.");
            return;
        }

        if (fno.fname[0])
        {
            if (fno.fattrib & AM_DIR)
            {
                NRF_LOG_RAW_INFO("   <DIR>   %s",(uint32_t)fno.fname);
            }
            else
            {
                NRF_LOG_RAW_INFO("%9lu  %s", fno.fsize, (uint32_t)fno.fname);
            }
        }
    }
    while (fno.fname[0]);

}



void fatfs_write()
{
 NRF_LOG_INFO("Writing to file " FILE_NAME "...");
    ff_result = f_open(&file, FILE_NAME, FA_READ | FA_WRITE | FA_OPEN_APPEND);
    if (ff_result != FR_OK)
    {
        NRF_LOG_INFO("Unable to open or create file: " FILE_NAME ".");
        return;
    }
    
    // write the time interval
    f_write(&file, "Elapsed time: ", 14, (UINT *) &bytes_written);
    snprintf(time_buffer,sizeof(time_buffer),"%d",deltaT);
    f_write(&file, time_buffer, sizeof(time_buffer), (UINT *) &bytes_written);
    
    //write position of motor shaft
    f_write(&file, "position: ", 10, (UINT *) &bytes_written);
    snprintf(pos_buffer,sizeof(pos_buffer),"%d",pos);
    f_write(&file, pos_buffer, sizeof(pos_buffer), (UINT *) &bytes_written);
    
    // write target angle in pulses
    f_write(&file, "target: ", 8, (UINT *) &bytes_written);
    snprintf(target_buffer,sizeof(target_buffer),"%d",target);
    f_write(&file,target_buffer, sizeof(target_buffer), (UINT *) &bytes_written);
  
    //write control signal
    f_write(&file, "u: ", 3, (UINT *) &bytes_written);
    snprintf(u_buffer,sizeof(u_buffer),"%d",u);
    f_write(&file, u_buffer, sizeof(u_buffer), (UINT *) &bytes_written);
  
    //write error in pulese
    f_write(&file, "error: ", 7, (UINT *) &bytes_written);
    snprintf(error_buffer,sizeof(error_buffer),"%d",e);
    f_write(&file, error_buffer, sizeof(error_buffer), (UINT *) &bytes_written);
   
    //return carriage and create new line
    f_write(&file, "\r\n", 2, (UINT *) &bytes_written);

    // close file
    (void) f_close(&file);
    return;

}