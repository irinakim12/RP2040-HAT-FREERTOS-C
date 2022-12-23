// Copyright 2021 WIZnet
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at

//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef _MLX90614_H_
#define _MLX90614_H_

typedef enum {
    MLX90614_CONTINUE_1LX_RES       = 0x10,   /*!< Command to set measure mode as Continuously H-Resolution mode*/
    MLX90614_CONTINUE_HALFLX_RES    = 0x11,   /*!< Command to set measure mode as Continuously H-Resolution mode2*/
    MLX90614_CONTINUE_4LX_RES       = 0x13,   /*!< Command to set measure mode as Continuously L-Resolution mode*/
    MLX90614_ONETIME_1LX_RES        = 0x20,   /*!< Command to set measure mode as One Time H-Resolution mode*/
    MLX90614_ONETIME_HALFLX_RES     = 0x21,   /*!< Command to set measure mode as One Time H-Resolution mode2*/
    MLX90614_ONETIME_4LX_RES        = 0x23,   /*!< Command to set measure mode as One Time L-Resolution mode*/
} mlx90614_cmd_measure_t;

typedef enum {
	TEMP_RAW,
	TEMP_K,
	TEMP_C,
	TEMP_F
} temperature_units;


#define MLX90614_I2C_ADDRESS   (0x5A)


//////////////////////////////////////
// MLX90614 RAM Addresses //
///////////////////////////////////////
#define MLX90614_TA      0x06 
#define MLX90614_TOBJ1	  0x07 
#define MLX90614_TOBJ2	  0x08 
//////////////////////////////////////
// MLX90614 EEPROM Addresses //
///////////////////////////////////////
#define MLX90614_TOMAX   0x20
#define MLX90614_TOMIN   0x21
#define MLX90614_PWMCTRL 0x22
#define MLX90614_TARANGE 0x23
#define MLX90614_KE      0x24 // EEPROM
#define MLX90614_CONFIG  0x25 
#define MLX90614_SMB_ADDRESS 0x2E

#define MLX90614_ID0     0x3C
#define MLX90614_ID1     0x3D
#define MLX90614_ID2     0x3E
#define MLX90614_ID3     0x3F
#define MLX90614_SLEEP   0xFF // Not really a register, but close enough


//SLEEP Data

#define MLX90614_NORMAL 0xE8
typedef void *mlx90614_handle_t;

#ifdef __cplusplus
extern "C" {
#endif


int  mlx90614_power_down(void);
int  mlx90614_sleep(void);
int  mlx90614_reset_data_register(void);
int  mlx90614_set_measure_mode( mlx90614_cmd_measure_t cmd_measure);
int  mlx90614_get_data(float *data);
int  mlx90614_get_light_intensity(mlx90614_cmd_measure_t cmd_measure, float *data);
int  mlx90614_change_measure_time(uint8_t measure_time);
uint8_t crc8(uint8_t crc, uint8_t inData);

#ifdef CONFIG_SENSOR_LIGHT_INCLUDED_MLX90614
int  light_sensor_mlx90614_init(i2c_bus_handle_t handle);
int  light_sensor_mlx90614_deinit(void);
int  light_sensor_mlx90614_test(void);
int  light_sensor_mlx90614_acquire_light(float *l);

#endif

#ifdef __cplusplus
}
#endif

#endif