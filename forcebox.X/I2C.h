
#ifndef I2C_H
#define I2C_H
#include <xc.h>

#define _XTAL_FREQ 32000000
#define I2C_BAUDRATE 100000UL
#define I2C_BRG ((_XTAL_FREQ/(4UL*I2C_BAUDRATE))-1)
#define EMC1001_W 0b01110000
#define EMC1001_R 0b01110001

void I2C_init();
void I2C_start(void);
void I2C_restart(void);
void I2C_stop(void);
unsigned char I2C_read(void);
void I2C_ack(void);
void I2C_nack(void);
void I2C_write(unsigned char I2C_data);
short EMC1001_getvalue();
#endif	/* I2C_H */