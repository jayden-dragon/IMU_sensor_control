IMU:imu.o tca9548.o
	gcc -o IMU imu.o tca9548.o -lmysqlclient
imu.o:imu.c
	gcc -c imu.c
tca9548.o:tca9548.c
	gcc -c tca9548.c
all:IMU
clean:
	rm -f *.o IMU
