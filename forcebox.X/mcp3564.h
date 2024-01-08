/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef MCP3564_H
#define	MCP3564_H

#define DUMMY 0x00

#define MCP356x_ADCDATA 0x0
#define MCP356x_CONFIG0 0x1
#define MCP356x_CONFIG1 0x2
#define MCP356x_CONFIG2 0x3
#define MCP356x_CONFIG3 0x4
#define MCP356x_IRQ 0x5
#define MCP356x_MUX 0x6
#define MCP356x_SCAN 0x7
#define MCP356x_TIMER 0x8
#define MCP356x_OFFSETCAL 0x9
#define MCP356x_GAINCAL 0xA
#define MCP356x_LOCK 0xD
#define MCP356x_CRCCFG 0xF

#define INT_MCP35xx_1 PORTCbits.RC2
#define INT_MCP35xx_2 PORTAbits.RA1
#define INT_MCP35xx_3 PORTAbits.RA3
#define INT_MCP35xx_4 PORTCbits.RC5
#define INT_MCP35xx_5 PORTAbits.RA7
#define INT_MCP35xx_6 PORTCbits.RC7

#include <xc.h> // include processor files - each processor file is guarded.  
/* Formule Temperature
    temp °C = 4,0096*1e-4*ADC Value*2,048-270,66
 */

typedef struct res_voie 
{
    long result;
    unsigned char voie;
} res_voie;

void spi_write(unsigned char data); // Compatibilite fonction avec divers matos
unsigned char spi_read();  // Compatibilite fonction avec divers matos
void cs_low(unsigned char comp_nbr);
void cs_high(unsigned char comp_nbr);

res_voie mcp356x_result32_channel_scan(unsigned char comp_nbr);
void mcp356x_result32_channel(unsigned char comp_nbr,unsigned char nbr_octet);
unsigned char mcp356x_readregister8(unsigned char address,unsigned char comp_nbr);
unsigned long mcp356x_readregister(unsigned char address,unsigned char size,unsigned char comp_nbr);
unsigned char mcp356x_writeregister8(unsigned char address,unsigned char data,unsigned char comp_nbr);
unsigned char mcp356x_writeregister(unsigned char address,unsigned long data,unsigned char size,unsigned char comp_nbr);
#endif	/* MCP3564_H */
