#ifndef __SPI_FUNCTIONS_H
#define __SPI_FUNCTIONS_H

#include "nrf.h"
#include "bsp.h"
#include "ff.h"
#include "diskio_blkdev.h"
#include "nrf_block_dev_sdc.h"


#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

#include "pid_functions.h"

void fatfs_initialization(void);

void fatfs_write(void);

#endif // __SPI_FUNCTONS_H