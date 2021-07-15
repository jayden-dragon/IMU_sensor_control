#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <sys/stat.h>
#include <mysql/mysql.h>
#include <time.h>
#include <math.h>

#define DBHOST "127.0.0.1"
#define DBUSER "root"
#define DBPASS "1"
#define DBNAME "jayden"

MYSQL *connector;
MYSQL_RES *result;
MYSQL_ROW row;

#include <linux/i2c-dev.h>


#include "tca9548.h"

#define MPU_ADDRESS 0x68
#define MPU_POWER1 0x6B
#define MPU_ACCEL_XOUT1 0x3B
#define MPU_ACCEL_XOUT2 0x3C
#define MPU_ACCEL_YOUT1 0x3D
#define MPU_ACCEL_YOUT2 0x3E
#define MPU_ACCEL_ZOUT1 0x3F
#define MPU_ACCEL_ZOUT2 0x40
#define MPU_GYRO_XOUT1 0x43
#define MPU_GYRO_XOUT2 0x44
#define MPU_GYRO_YOUT1 0x45
#define MPU_GYRO_YOUT2 0x46
#define MPU_GYRO_ZOUT1 0x47
#define MPU_GYRO_ZOUT2 0x48

int fd = -1;


int linux_delay_ms(unsigned long num_ms);
void mpu_power_on(int i2c_handle);
void mpu_write_register(int fd, uint8_t reg, uint8_t value);
uint8_t mpu_read_register(int i2c_mpu, uint8_t reg);
int16_t mpu_read_register_pair(int i2c_mpu, uint8_t reg);

int16_t mpu_read_register_pair(int i2c_mpu, uint8_t reg)
{
	uint8_t data[3];
	data[0] = reg;
	if (write(i2c_mpu, data, 1) != 1)
	{
		printf("error sending read request via i2c\n");
		return -1;
	}
	if (read(i2c_mpu, data, 2) != 2)
		printf("error getting read request via i2c\n");

	return (data[0] << 8 | data[1]);

}
uint8_t mpu_read_register(int i2c_mpu, uint8_t reg)
{
	unsigned char data[3];
	data[0] = reg;

	if(write(i2c_mpu, data, 1) != 1)
	{
		printf("read request via i2c\n");
		return -1;
	}
	
	if(read(i2c_mpu, data,1) != 1)
		printf("read i2c \n");
	return data[0];
}

void mpu_write_register(int fd, uint8_t reg, uint8_t value)
{
	uint8_t data[2];
	data[0] = reg;
	data[1] = value;
	int byte_num;

	byte_num = write(fd,data,2);
	/*if(byte_num != 2)
	{
		printf("write register error\n");
	}*/
	//printf("%d\n",byte_num);

}

void mpu_power_on(int i2c_handle)
{
	mpu_write_register(i2c_handle, MPU_POWER1, 0x00);
}

int linux_delay_ms(unsigned long num_ms)
{
	struct timespec ts;
	
	ts.tv_sec = num_ms /1000;
	ts.tv_nsec = (num_ms % 1000) * 1000000;

	return nanosleep(&ts, NULL);
}

int main(int argc, char **argv)
{
	char query_a[1024];
	char query_b[1024];

	connector = mysql_init(NULL);
	if(!mysql_real_connect(connector, DBHOST, DBUSER, DBPASS, DBNAME, 3306, NULL, 0))
	{
		fprintf(stderr, "%s\n", mysql_error(connector));
		return 0;
	}
	printf("MySQL(RpiDB) opened.\n");

	int16_t accel_x[8], accel_y[8], accel_z[8];
	int16_t gyro_x[8], gyro_y[8], gyro_z[8];
	char *fileName = "/dev/i2c-1";
	
	fd = open_master("/dev/i2c-1");
	if (fd < 0)
	{
		printf("failed to open i2c port\n");
		exit(1);
	}
	
		
	if (ioctl(fd, I2C_SLAVE, MPU_ADDRESS) < 0)
	{
		printf("unable to get bus access to talk to slave\n");
		exit(1);
	}
	
		
	while(1)
	{
				 
		mpu_power_on(fd);
		enable_switcher_chan(fd, ENABLE_TCA9548_CH3);
		select_slave(fd, 0x68);
		accel_x[0] = mpu_read_register_pair(fd, MPU_ACCEL_XOUT1);
		accel_y[0] = mpu_read_register_pair(fd, MPU_ACCEL_YOUT1);
		accel_z[0] = mpu_read_register_pair(fd, MPU_ACCEL_ZOUT1);
		gyro_x[0] = mpu_read_register_pair(fd, MPU_GYRO_XOUT1);
		gyro_y[0] = mpu_read_register_pair(fd, MPU_GYRO_YOUT1);
		gyro_z[0] = mpu_read_register_pair(fd, MPU_GYRO_ZOUT1);
		disable_switcher_chan(DISABLE_TCA9548_ALLCH);
		
		mpu_power_on(fd);		
		enable_switcher_chan(fd, ENABLE_TCA9548_CH2);
		select_slave(fd, 0x68);
		accel_x[1] = mpu_read_register_pair(fd, MPU_ACCEL_XOUT1);
		accel_y[1] = mpu_read_register_pair(fd, MPU_ACCEL_YOUT1);
		accel_z[1] = mpu_read_register_pair(fd, MPU_ACCEL_ZOUT1);
		gyro_x[1] = mpu_read_register_pair(fd, MPU_GYRO_XOUT1);
		gyro_y[1] = mpu_read_register_pair(fd, MPU_GYRO_YOUT1);
		gyro_z[1] = mpu_read_register_pair(fd, MPU_GYRO_ZOUT1);
		disable_switcher_chan(DISABLE_TCA9548_ALLCH);
				
		mpu_power_on(fd);
		enable_switcher_chan(fd, ENABLE_TCA9548_CH7);
		select_slave(fd, 0x68);
		accel_x[2] = mpu_read_register_pair(fd, MPU_ACCEL_XOUT1);
		accel_y[2] = mpu_read_register_pair(fd, MPU_ACCEL_YOUT1);
		accel_z[2] = mpu_read_register_pair(fd, MPU_ACCEL_ZOUT1);
		gyro_x[2] = mpu_read_register_pair(fd, MPU_GYRO_XOUT1);
		gyro_y[2] = mpu_read_register_pair(fd, MPU_GYRO_YOUT1);
		gyro_z[2] = mpu_read_register_pair(fd, MPU_GYRO_ZOUT1);
		disable_switcher_chan(DISABLE_TCA9548_ALLCH);
		
		mpu_power_on(fd);
		enable_switcher_chan(fd, ENABLE_TCA9548_CH4);
		select_slave(fd, 0x68);
		accel_x[3] = mpu_read_register_pair(fd, MPU_ACCEL_XOUT1);
		accel_y[3] = mpu_read_register_pair(fd, MPU_ACCEL_YOUT1);
		accel_z[3] = mpu_read_register_pair(fd, MPU_ACCEL_ZOUT1);
		gyro_x[3] = mpu_read_register_pair(fd, MPU_GYRO_XOUT1);
		gyro_y[3] = mpu_read_register_pair(fd, MPU_GYRO_YOUT1);
		gyro_z[3] = mpu_read_register_pair(fd, MPU_GYRO_ZOUT1);
		disable_switcher_chan(DISABLE_TCA9548_ALLCH);

		mpu_power_on(fd);
		enable_switcher_chan(fd, ENABLE_TCA9548_CH5);
		select_slave(fd, 0x68);
		accel_x[4] = mpu_read_register_pair(fd, MPU_ACCEL_XOUT1);
		accel_y[4] = mpu_read_register_pair(fd, MPU_ACCEL_YOUT1);
		accel_z[4] = mpu_read_register_pair(fd, MPU_ACCEL_ZOUT1);
		gyro_x[4] = mpu_read_register_pair(fd, MPU_GYRO_XOUT1);
		gyro_y[4] = mpu_read_register_pair(fd, MPU_GYRO_YOUT1);
		gyro_z[4] = mpu_read_register_pair(fd, MPU_GYRO_ZOUT1);
		disable_switcher_chan(DISABLE_TCA9548_ALLCH);

		mpu_power_on(fd);
		enable_switcher_chan(fd, ENABLE_TCA9548_CH6);
		select_slave(fd, 0x68);
		accel_x[5] = mpu_read_register_pair(fd, MPU_ACCEL_XOUT1);
		accel_y[5] = mpu_read_register_pair(fd, MPU_ACCEL_YOUT1);
		accel_z[5] = mpu_read_register_pair(fd, MPU_ACCEL_ZOUT1);
		gyro_x[5] = mpu_read_register_pair(fd, MPU_GYRO_XOUT1);
		gyro_y[5] = mpu_read_register_pair(fd, MPU_GYRO_YOUT1);
		gyro_z[5] = mpu_read_register_pair(fd, MPU_GYRO_ZOUT1);
		disable_switcher_chan(DISABLE_TCA9548_ALLCH);

		mpu_power_on(fd);
		enable_switcher_chan(fd, ENABLE_TCA9548_CH0);
		select_slave(fd, 0x68);
		accel_x[6] = mpu_read_register_pair(fd, MPU_ACCEL_XOUT1);
		accel_y[6] = mpu_read_register_pair(fd, MPU_ACCEL_YOUT1);
		accel_z[6] = mpu_read_register_pair(fd, MPU_ACCEL_ZOUT1);
		gyro_x[6] = mpu_read_register_pair(fd, MPU_GYRO_XOUT1);
		gyro_y[6] = mpu_read_register_pair(fd, MPU_GYRO_YOUT1);
		gyro_z[6] = mpu_read_register_pair(fd, MPU_GYRO_ZOUT1);
		disable_switcher_chan(DISABLE_TCA9548_ALLCH);

		mpu_power_on(fd);
		enable_switcher_chan(fd, ENABLE_TCA9548_CH1);
		select_slave(fd, 0x68);
		accel_x[7] = mpu_read_register_pair(fd, MPU_ACCEL_XOUT1);
		accel_y[7] = mpu_read_register_pair(fd, MPU_ACCEL_YOUT1);
		accel_z[7] = mpu_read_register_pair(fd, MPU_ACCEL_ZOUT1);
		gyro_x[7] = mpu_read_register_pair(fd, MPU_GYRO_XOUT1);
		gyro_y[7] = mpu_read_register_pair(fd, MPU_GYRO_YOUT1);
		gyro_z[7] = mpu_read_register_pair(fd, MPU_GYRO_ZOUT1);
		disable_switcher_chan(DISABLE_TCA9548_ALLCH);


		printf("\rAccel %d %d %d | %d %d %d | %d %d %d | %d %d %d | %d %d %d | %d %d %d | %d %d %d | %d %d %d        ", 
		(int)accel_x[0], (int)accel_y[0], (int)accel_z[0], 
		(int)accel_x[1], (int)accel_y[1], (int)accel_z[1], 
		(int)accel_x[2], (int)accel_y[2], (int)accel_z[2],
		(int)accel_x[3], (int)accel_y[3], (int)accel_z[3],
		(int)accel_x[4], (int)accel_y[4], (int)accel_z[4],
		(int)accel_x[5], (int)accel_y[5], (int)accel_z[5],
		(int)accel_x[6], (int)accel_y[6], (int)accel_z[6],
		(int)accel_x[7], (int)accel_y[7], (int)accel_z[7]);

		/*printf("\rGyro %d %d %d | %d %d %d | %d %d %d | %d %d %d | %d %d %d | %d %d %d | %d %d %d | %d %d %d        ",
		(int)gyro_x[0], (int)gyro_y[0], (int)gyro_z[0],
		(int)gyro_x[1], (int)gyro_y[1], (int)gyro_z[1],
		(int)gyro_x[2], (int)gyro_y[2], (int)gyro_z[2],
		(int)gyro_x[3], (int)gyro_y[3], (int)gyro_z[3],
		(int)gyro_x[4], (int)gyro_y[4], (int)gyro_z[4],
		(int)gyro_x[5], (int)gyro_y[5], (int)gyro_z[5],
		(int)gyro_x[6], (int)gyro_y[6], (int)gyro_z[6],
		(int)gyro_x[7], (int)gyro_y[7], (int)gyro_z[7]);*/

		sprintf(query_a, "insert into accelerometer values (%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d )",
			accel_x[0], accel_y[0], accel_z[0],
			accel_x[1], accel_y[1], accel_z[1],
			accel_x[2], accel_y[2], accel_z[2]
			accel_x[3], accel_y[3], accel_z[3],
			accel_x[4], accel_y[4], accel_z[4],
			accel_x[5], accel_y[5], accel_z[5],
			accel_x[6], accel_y[6], accel_z[6],
			accel_x[7], accel_y[7], accel_z[7]);
		if(mysql_query(connector, query_a))
		{
			fprintf(stderr, "%s\n", mysql_error(connector));
			printf("Write DB error\n");
		}
		sprintf(query_b, "insert into gyro values (%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d )",
			gyro_x[0], gyro_y[0], gyro_z[0],
			gyro_x[1], gyro_y[1], gyro_z[1],
			gyro_x[2], gyro_y[2], gyro_z[2],
			gyro_x[3], gyro_y[3], gyro_z[3],
			gyro_x[4], gyro_y[4], gyro_z[4],
			gyro_x[5], gyro_y[5], gyro_z[5],
			gyro_x[6], gyro_y[6], gyro_z[6],
			gyro_x[7], gyro_y[7], gyro_z[7]);

		if(mysql_query(connector, query_b))
		{
			fprintf(stderr, "%s\n", mysql_error(connector));
			printf("Write DB error\n");
		}		
		fflush(stdout);
		linux_delay_ms(98);
					
	}

	return 0;
}
