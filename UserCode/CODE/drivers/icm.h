/*
 * icm.h
 *
 *  Created on: Jul 1, 2022
 *      Author: xff20
 */

#ifndef ICM_H_
#define ICM_H_

#include "os/os_dev.h"

typedef struct {
    int16 wx,wy,wz;
    int16 ax,ay,az;
}icm_data;

#define icm_init icm20602_init_spi
#define icm_gyro get_icm20602_gyro_spi
#define icm_acc  get_icm20602_accdata_spi
void icm_get_raw(icm_data *dat_ptr);
void icm_get(icm_data *dat_ptr);
void icm_data_calib(icm_data *dat_ptr,icm_data *calib);
void icm_data_clear(icm_data *dat_ptr);
void icm_auto_calib();

#endif /* ICM_H_ */
