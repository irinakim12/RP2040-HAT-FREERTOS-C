
#include "hardware/i2c.h"
#include <stdio.h>
#include "mlx90614.h"

#define WRITE_BIT  I2C_MASTER_WRITE  /*!< I2C master write */
#define READ_BIT   I2C_MASTER_READ   /*!< I2C master read */
#define ACK_CHECK_EN   0x1           /*!< I2C master will check ack from slave*/
#define ACK_CHECK_DIS  0x0           /*!< I2C master will not check ack from slave */
#define ACK_VAL    0x0               /*!< I2C ack value */
#define NACK_VAL   0x1               /*!< I2C nack value */

#define BH_1750_MEASUREMENT_ACCURACY    1.2    /*!< the typical measurement accuracy of  BH1750 sensor */

#define BH1750_POWER_DOWN        0x00    /*!< Command to set Power Down*/
#define BH1750_POWER_ON          0x01    /*!< Command to set Power On*/
#define BH1750_DATA_REG_RESET    0x07    /*!< Command to reset data register, not acceptable in power down mode*/



int mlx90614_write(uint8_t* cmd,uint8_t cmdlen)
{
    int ret;
    uint8_t crc;

    crc = crc8(0, MLX90614_I2C_ADDRESS<<1);
    for(int i =0; i<cmdlen+1 ; i++)
      crc = crc8(crc,cmd[i]);
    *cmd += crc;
  
    
    ret = i2c_write_blocking(i2c_default, MLX90614_I2C_ADDRESS, cmd, cmdlen+2, true);
    if(ret == PICO_ERROR_TIMEOUT)
    {
        printf("i2c_write_blocking() PICO_ERROR_TIMEOUT\n");
        ret = -1;
    }
    else if(ret == PICO_ERROR_GENERIC)
    {
        printf("i2c_write_blocking() PICO_ERROR_GENERIC\n");
        ret = -1;
    }
    else if(ret == PICO_ERROR_NO_DATA)
    {
        printf("i2c_write_blocking() PICO_ERROR_NO_DATA\n");
        ret = -1;
    }
    else
    {
#ifdef LOG_ERR_ALL
        printf("i2c_write_blocking() ret %d\n", ret);
#endif
        ret = 0;
    }

    return ret;
}
int mlx90416_read(uint8_t* cmd, uint8_t* data)
{
    int ret;
    uint8_t crc;
    ret = i2c_write_blocking(i2c_default, MLX90614_I2C_ADDRESS, cmd, 1, true);
    if(ret == PICO_ERROR_TIMEOUT)
    {
        printf("i2c_write_blocking() PICO_ERROR_TIMEOUT\n");
        ret = -1;
    }
    else if(ret == PICO_ERROR_GENERIC)
    {
        printf("i2c_write_blocking() PICO_ERROR_GENERIC\n");
        ret = -1;
    }
    else if(ret == PICO_ERROR_NO_DATA)
    {
        printf("i2c_write_blocking() PICO_ERROR_NO_DATA\n");
        ret = -1;
    }
    else
    {
#ifdef LOG_ERR_ALL
        printf("i2c_write_blocking() ret %d\n", ret);
#endif
        ret = 0;
    }
    
    ret = i2c_read_blocking(i2c_default, MLX90614_I2C_ADDRESS, data, 3, false);
    if(ret == PICO_ERROR_TIMEOUT)
    {
        printf("i2c_read_blocking() PICO_ERROR_TIMEOUT\n");
        ret = -1;
    }
    else if(ret == PICO_ERROR_GENERIC)
    {
        printf("i2c_read_blocking() PICO_ERROR_GENERIC\n");
        ret = -1;
    }
    else if(ret == PICO_ERROR_NO_DATA)
    {
        printf("i2c_read_blocking() PICO_ERROR_NO_DATA\n");
        ret = -1;
    }
    else
    {
#ifdef LOG_ERR_ALL
        printf("i2c_read_blocking() ret %d\n", ret);
#endif
      crc = crc8(0,(MLX90614_I2C_ADDRESS<<1));
      crc = crc8(crc, cmd);
      crc = crc8(crc, (MLX90614_I2C_ADDRESS<<1)+1);
      for(int i = 0; i<3 ;i++)  
        crc = crc8(crc, data[i]);
      if(crc ==  data[2])
      {
        ret = 0;
      }
       ret = -1;
    }

    return ret;
}

int mlx90614_get_deviceid(uint8_t *deviceid)
{
    uint8_t cmd;
    uint8_t data;
    int ret;
    uint8_t tmp;

    cmd = MLX90614_SMB_ADDRESS;
    ret = mlx90614_read(&cmd,1,&data,1);
    if(ret !=0)
    {
      printf("mlx906142_read() failed\n");
    }
    printf("deviceid : %x\n",data);
    *deviceid = data;
    return ret;

}
int  mlx90614_init(void)
{
    uint8_t mlx90614_deviceid;
 
    int ret = mlx90614_get_deviceid(&mlx90614_deviceid);
    if(ret !=0)
    {
      printf("mlx906142_read() failed\n");
       return -1;
    }
    return ret;
 
}

int  mlx90614_sleep(void)
{ 
    uint8_t cmd[2];
    int ret;
    cmd[0] = MLX90614_SLEEP;
    cmd[1] = MLX90614_NORMAL;


    ret = mlx90614_write(cmd,2);
    if(ret == PICO_ERROR_TIMEOUT)
    {
        printf("mlx90614_write() PICO_ERROR_TIMEOUT\n");
        ret = -1;
    }
    else if(ret == PICO_ERROR_GENERIC)
    {
        printf("mlx90614_write() PICO_ERROR_GENERIC\n");
        ret = -1;
    }
    else if(ret == PICO_ERROR_NO_DATA)
    {
        printf("mlx90614_write() PICO_ERROR_NO_DATA\n");
        ret = -1;
    }
    else
    {
#ifdef LOG_ERR_ALL
        printf("mlx90614_write() ret %d\n", ret);
#endif
        ret = 0;
    } 
   /* 
    ret = mlx90614_write(cmd, 2);
    if(ret !=0)
    {
      printf("mlx90614_write() failed\n");
      return -1;
    }
    return 0;
    */

}
///GPIO Control 
/*
**  GPIO Control로 변경해야함
*/
int  mlx90614_wakeup(void)
{
    uint8_t cmd[2];
    int ret;
    cmd[0] = MLX90614_SLEEP;
    cmd[1] = MLX90614_NORMAL;
    
    ret = mlx90614_write(cmd, 2);
    if(ret !=0)
    {
      printf("mlx90614_write() failed\n");
      return -1;
    }
    cmd[0] = BH1750_POWER_ON;
    ret = mlx90614_write(cmd,1);
    return ret;
}

int mxl90614_get_object_tempC(uint8_t *temp)
{
     uint8_t cmd[2];
    int ret;
    uint8_t data;
    uint8_t tmp;


    cmd[0] = MLX90614_TOBJ1;
    ret =  mlx90614_read(cmd,1, &data,1);
    if(ret != 0)
    {
      printf(" mlx90614_read() failed\n");
      return -1;
    }
    *temp = data;
    
    return 0;
}
int mxl90614_get_ambient_tempC(uint8_t *temp)
{
      uint8_t cmd[2];
    int ret;
    uint8_t data;
    uint8_t tmp;


    cmd[0] = MLX90614_TA;
    ret =  mlx90614_read(cmd,1, &data,1);
    if(ret != 0)
    {
      printf(" mlx90614_read() failed\n");
      return -1;
    }
    *temp = data;
    
    return 0;
}
int mxl90614_get_object_tempF(uint8_t *temp)
{
      uint8_t cmd[2];
    int ret;
    uint8_t data;
    uint8_t tmp;


    cmd[0] = MLX90614_TOBJ1;
    ret =  mlx90614_read(cmd,1, &data,1);
    if(ret != 0)
    {
      printf(" mlx90614_read() failed\n");
      return -1;
    }
    *temp = (data*9  /5) + 32;
    //*temp = data;
    
    return 0;
}
int mxl90614_get_ambient_tempF(uint8_t *temp)
{
      uint8_t cmd[2];
    int ret;
    uint8_t data;
    uint8_t tmp;


    cmd[0] = MLX90614_TA;
    ret =  mlx90614_read(cmd,1, &data,1);
    if(ret != 0)
    {
      printf(" mlx90614_read() failed\n");
      return -1;
    }
    *temp = (data* 9 / 5) + 32;
    
    return 0;
}
int mxl90614_get_emissivityReg(uint8_t *temp)
{
      uint8_t cmd[2];
    int ret;
    uint8_t data;
    uint8_t tmp;


    cmd[0] = MLX90614_KE;
    ret =  mlx90614_read(cmd,1, &data,1);
    if(ret != 0)
    {
      printf(" mlx90614_read() failed\n");
      return -1;
    }
    *temp = data;
    
    return 0;
}
int mxl90614_set_emissivityReg(uint8_t temp)
{  uint8_t cmd[2];
    int ret;
    uint8_t data;
    uint8_t tmp;


    cmd[0] = MLX90614_KE;
    cmd[1] = temp;
    ret =  mlx90614_write(cmd,2);
    if(ret != 0)
    {
      printf(" mlx90614_write() failed\n");
      return -1;
    }
    
    return 0;
}
/*
int mxl90614_get_emissivity(uint8_t temp)
{
      uint8_t cmd[2];
    int ret;
    uint8_t data;
    uint8_t tmp;


    cmd[0] = MLX90614_KE;
    ret =  mlx90614_read(cmd,1, &data,1);
    if(ret != 0)
    {
      printf(" mlx90614_read() failed\n");
      return -1;
    }
    *temp = data;
    
    return 0;
}
int mxl90614_set_emissivity(uint8_t temp)

*/



uint8_t crc8(uint8_t crc, uint8_t inData)
{
    uint8_t i;
	uint8_t data;
	data = crc ^ inData;
	for ( i = 0; i < 8; i++ )
	{
		if (( data & 0x80 ) != 0 )
		{
			data <<= 1;
			data ^= 0x07;
		}
		else
		{
			data <<= 1;
		}
	}
	return data;
}