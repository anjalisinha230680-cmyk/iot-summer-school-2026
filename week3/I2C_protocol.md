# I2C Communication protocol 
I2C stands for inter-integrated Circuit.
It is used to connect a microcontroller with multiple sensors.
It uses only 2 bi-directional open-drain lines for data communication
called SDA and SCL.
SDA:Transfer of data takes place through this pin.
SCL:IT carries the clock signal.
I2C Addressing
The address frame is the first frame after the start bit.
The address of the slave with which the master wants to communicate is sent by the master to every slave
connected with it then the slave compares its own address and sends ACK.
Three common Iot sensors which uses I2C are
1.BMP280
2.MPU6050
3.BH1750
