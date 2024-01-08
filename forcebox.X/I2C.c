
#include "I2C.h"

void I2C_init(void)
{
    SSP1CLKPPS = 0x14;      //RC4->MSSP1:SCL1;    
    RC3PPS = 0x15;          //RC3->MSSP1:SDA1;    
    RC4PPS = 0x14;          //RC4->MSSP1:SCL1;    
    SSP1DATPPS = 0x13;      //RC3->MSSP1:SDA1;    
    
    SSP1STAT = 0b10000000;     
	SSP1CON1 = 0b00101000;    
	SSP1CON3 = 0x00;
	SSP1ADD = I2C_BRG;
}

 void I2C_start(void)
 {   
    SSP1CON2bits.SEN = 1;           
    while(SSP1CON2bits.SEN == 1);
}

void I2C_restart(void)
{    
    SSP1CON2bits.RSEN = 1;          
    while(SSP1CON2bits.RSEN == 1); 
}

void I2C_stop(void)
{    
    SSP1CON2bits.PEN = 1;           
    while(SSP1CON2bits.PEN == 1);
}

unsigned char I2C_read(void)
{
    PIR3bits.SSP1IF = 0;         	//clear SSP interrupt bit
    SSP1CON2bits.RCEN = 1;       	//set the receive enable bit to initiate a read of 8 bits from the serial eeprom
    while(PIR3bits.SSP1IF == 0); 	//Wait for interrupt flag to go high indicating transmission is complete  
    return SSP1BUF;                 //Data from eeprom is now in the SSPBUF so return that value
}

void I2C_ack(void)
{
    PIR3bits.SSP1IF = 0;            //clear SSP interrupt bit
    SSP1CON2bits.ACKDT = 0;         //clear the Acknowledge Data Bit - this means we are sending an Acknowledge or 'ACK'
    SSP1CON2bits.ACKEN = 1;         //set the ACK enable bit to initiate transmission of the ACK bit to the serial eeprom
    while(PIR3bits.SSP1IF == 0);    //Wait for interrupt flag to go high indicating transmission is complete
}

void I2C_nack(void)
{
    PIR3bits.SSP1IF = 0;            //clear SSP interrupt bit
    SSP1CON2bits.ACKDT = 1;         //set the Acknowledge Data Bit- this means we are sending a No-Ack or 'NAK'
    SSP1CON2bits.ACKEN = 1;         //set the ACK enable bit to initiate transmission of the ACK bit to the serial eeprom
    while(PIR3bits.SSP1IF == 0);    // Wait for interrupt flag to go high indicating transmission is complete
}

void I2C_write(unsigned char I2C_data)
{    
    PIR3bits.SSP1IF = 0;            
    SSP1BUF = I2C_data;             
    while(PIR3bits.SSP1IF == 0);	// Wait for interrupt flag to go high indicating transmission is complete
}

short EMC1001_getvalue()
{
    unsigned char high_b;
    unsigned char low_b;
    short result;
        I2C_start();
        I2C_write(EMC1001_W); 
        I2C_write(0);
        I2C_restart();
        I2C_write(EMC1001_R); 
        high_b = I2C_read();
        I2C_nack();
        I2C_stop();
        I2C_start();
        I2C_write(EMC1001_W);  
        I2C_write(2);
        I2C_restart();
        I2C_write(EMC1001_R); 
        low_b = I2C_read();
        I2C_nack();
        I2C_stop();
        result=((short)high_b<<2)+(short)(low_b>>6);
        if(high_b>128)
        {
            result=result | 0xFC00;
        }   
        return result;
}