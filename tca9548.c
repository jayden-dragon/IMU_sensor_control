#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#include <linux/types.h>
#include <string.h>
#include <linux/i2c-dev.h>


#include "tca9548.h"


#define SLAVE_TCA9548_ADDR  0x70


int open_master(const char *pname)
{
   int fd = -1 ;
   if(NULL == pname)
   {
	 return -1;
   }

   return open(I2C_CONTROLLER_NAME,O_RDWR);
}

int select_slave(const int fd,const unsigned int addr)
{
   return ioctl(fd,I2C_SLAVE,addr);
}

int enable_switcher_chan(const int fd,const int en_chx)
{
   int ret = -1;
   unsigned char val = (unsigned char)en_chx;

   if((en_chx < 0x01) || ((en_chx > 0x80)))
   {
     return -1;
   }


   ret = select_slave(fd,SLAVE_TCA9548_ADDR);
   if(ret < 0)
   {
     return -1;
   }
   return write_device(fd,&val,1);

}

int disable_switcher_chan(const int fd)
{
   int ret = -1;
   unsigned char val = DISABLE_TCA9548_ALLCH;

   ret = select_slave(fd,SLAVE_TCA9548_ADDR);
   if(ret < 0)
   {
     return -1;
   }
   return write_device(fd,&val,1);
}

int write_device(const int fd,const unsigned char *pdata,const unsigned int len)
{
	if((fd < 0) || (NULL == pdata) || (len < 0))
	{
       return -1;
	}
	return write(fd,pdata,len);
}

void close_master(const int *pfd)
{
    close(*pfd);
}

