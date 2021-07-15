#ifndef _LINUX_TCA9548_H
#define _LINUX_TCA9548_H


#define I2C_CONTROLLER_NAME "/dev/i2c-1"

#define DISABLE_TCA9548_ALLCH   0x00
#define ENABLE_TCA9548_CH7      0x80
#define ENABLE_TCA9548_CH4      0x10
#define ENABLE_TCA9548_CH3      0x08  //connect to PCF8563
#define ENABLE_TCA9548_CH2      0x04  //connect to BQ27621
#define ENABLE_TCA9548_CH1      0x02
#define ENABLE_TCA9548_CH0      0x01  //connect to ADV7513 hdmi
#define ENABLE_TCA9548_CH5      0x20  //connect to CH7026  image-fcouser
#define ENABLE_INFRARED_CH6     0x40  //connect to infrared sensor
#define ENABLE_ADT7410_CH3      0x08  //connect to ADT7410


int open_master(const char *pname);
void close_master(const int *pfd);

int enable_switcher_chan(const int fd,const int en_chx);
int disable_switcher_chan(const int fd);

int select_slave(const int fd,const unsigned int addr);
int write_device(const int fd,const unsigned char *pdata,const unsigned int len);


#endif
