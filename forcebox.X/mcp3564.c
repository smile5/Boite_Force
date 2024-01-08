/*
 * File:   mcp3564.c
 * Author: xgaltier
 *
 * Created on 11 décembre 2023, 17:04
 */


#include <xc.h>
#include "mcp3564.h"
#include "UART.h"

void mcp356x_result32_channel(unsigned char comp_nbr,unsigned char nbr_octet) {
    unsigned char real_address = 0, j, i, test[4];
    unsigned long result = 0;
    res_voie tmp[5];

    cs_low(comp_nbr);
    real_address = (0x01 << 6) + (0x0A << 2) + 0x0;
    spi_write(real_address);
    cs_high(comp_nbr);


    for (i = 0; i < nbr_octet; i++) {
        switch (comp_nbr) {
            case 1:
                while (INT_MCP35xx_1 == 1);
                break;
            case 2:
                while (INT_MCP35xx_2 == 1);
                break;
            case 3:
                while (INT_MCP35xx_3 == 1);
                break;
            case 4:
                while (INT_MCP35xx_4 == 1);
                break;
            case 5:
                while (INT_MCP35xx_5 == 1);
                break;
            case 6:
                while (INT_MCP35xx_6 == 1);
                break;
        }
        cs_low(comp_nbr);
        real_address = (0x01 << 6) + (0 << 2) + 0x01;
        spi_write(real_address);
        spi_read(); //read status
        spi_write(DUMMY);
        j = spi_read(); // read channel + signe
        tmp[i].voie = (j & 0xF0) >> 4;
        spi_write(DUMMY);
        result = (unsigned long) (spi_read()) << 16;
        spi_write(DUMMY);
        result = result + ((unsigned long) (spi_read()) << 8);
        spi_write(DUMMY);
        result = result + (unsigned long) spi_read();
        cs_high(comp_nbr);
        if (j & 0x01) {
            tmp[i].result = 0xFF000000 + result;
        } else {
            tmp[i].result = result;
        }
    }
    for (i = 0; i < nbr_octet; i++) {
        UART_write('?');
        UART_write(0x30 + comp_nbr);
        UART_write(tmp[i].voie);
        UART_write_long(tmp[i].result);
        UART_write('!');
    }
}

res_voie mcp356x_result32_channel_scan(unsigned char comp_nbr)
{
    unsigned char real_address = 0,i,test[4];
    unsigned long result=0;
    res_voie tmp;
    tmp.result=0;
    cs_low(comp_nbr);
    real_address= (0x01 << 6) + (0 << 2) + 0x01;
    spi_write(real_address);
    spi_read(); //read status
    switch(comp_nbr)
    {
        case 1:
            while(INT_MCP35xx_1==1);
            break;
        case 2:
            while(INT_MCP35xx_2==1);
            break;
        case 3:
            while(INT_MCP35xx_3==1);
            break;
        case 4:
            while(INT_MCP35xx_4==1);
            break;
        case 5:
            while(INT_MCP35xx_5==1);
            break;
        case 6:
            while(INT_MCP35xx_6==1);
            break;
    }
    spi_write(DUMMY);
    i=spi_read(); // read channel + signe
    test[0]=i;
    tmp.voie=(i & 0xF0) >> 4;
    spi_write(DUMMY);
    test[1]=spi_read();
    result= (unsigned long)(spi_read()) << 16;
    spi_write(DUMMY);
    test[2]=spi_read();
    result= result +((unsigned long)(spi_read()) << 8);
    spi_write(DUMMY);
    test[3]=spi_read();
    result= result +(unsigned long)spi_read();
    cs_high(comp_nbr);
    if (i & 0x01)
    {
        tmp.result= 0xFF000000 + result ;
    }
    else
    {
        tmp.result= result;
    }
    switch(comp_nbr)
    {
        case 1:
            while(INT_MCP35xx_1==0);
            break;
        case 2:
            while(INT_MCP35xx_2==0);
            break;
        case 3:
            while(INT_MCP35xx_3==0);
            break;
        case 4:
            while(INT_MCP35xx_4==0);
            break;
        case 5:
            while(INT_MCP35xx_5==0);
            break;
        case 6:
            while(INT_MCP35xx_6==0);
            break;
    }
    return tmp;
}

unsigned char mcp356x_readregister8(unsigned char address,unsigned char comp_nbr)
{
    unsigned char real_address = 0;
    
    cs_low(comp_nbr);
    if (address > 0x0F)
    {
        spi_write(address);
    }
    else
    {
        real_address= (0x01 << 6) + (address << 2) + 0x01;
        spi_write(real_address);
    }
    spi_read(); //read status
    spi_write(DUMMY);
    cs_high(comp_nbr);
    return spi_read(); //read data
}

unsigned long mcp356x_readregister(unsigned char address,unsigned char size,unsigned char comp_nbr)
{
    unsigned char real_address = 0,i;
    unsigned long result=0;
    cs_low(comp_nbr);
    if (address > 0x0F)
    {
        spi_write(address);
    }
    else
    {
        real_address= (0x01 << 6) + (address << 2) + 0x01;
        spi_write(real_address);
    }
    spi_read(); //read status
    for (i=0;i<size;i++)
    {
        spi_write(DUMMY);
        result= (result<< (8))+spi_read();
    }
    cs_high(comp_nbr);
    return result; //read data
}

unsigned char mcp356x_writeregister8(unsigned char address,unsigned char data,unsigned char comp_nbr)
{
    unsigned char real_address = 0,status;
    
    cs_low(comp_nbr);
    if (address > 0x0F)
    {
        spi_write(address);
    }
    else
    {
        real_address= (0x01 << 6) + (address << 2) + 0x02;
        spi_write(real_address);
    }
    status=spi_read(); //read status
    spi_write(data);
    cs_high(comp_nbr);
    spi_read();
    return status; //read data
}

unsigned char mcp356x_writeregister(unsigned char address,unsigned long data,unsigned char size,unsigned char comp_nbr)
{
    unsigned char real_address = 0,status,i;
    unsigned char tmp;
    unsigned long ll32;
    cs_low(comp_nbr);
    if (address > 0x0F)
    {
        spi_write(address);
    }
    else
    {
        real_address= (0x01 << 6) + (address << 2) + 0x02;
        spi_write(real_address);
    }
    status=spi_read(); //read status
    for(i=size;i>0;i--)
    {
       tmp=(unsigned char) ((data>>((i-1)*8)) & 0x000000FF); 
       spi_write(tmp);
       spi_read();
    }
    cs_high(comp_nbr);
    return status; //read data
    
}

void cs_low(unsigned char comp_nbr)
{
    switch(comp_nbr)
    {
        case 1:
        LATBbits.LATB2=0;
        break;
        case 2:
        LATAbits.LATA0=0;
        break;
        case 3:
        LATAbits.LATA2=0;
        break;
        case 4:
        LATAbits.LATA4=0;
        break;
        case 5:
        LATAbits.LATA6=0;
        break;
        case 6:
        LATCbits.LATC6=0;
        break;
        default:  // 1 seul
        LATBbits.LATB2=0;
        break;  
    }
}

void cs_high(unsigned char comp_nbr)
{
    switch(comp_nbr)
    {
        case 1:
        LATBbits.LATB2=1;
        break;
        case 2:
        LATAbits.LATA0=1;
        break;
        case 3:
        LATAbits.LATA2=1;
        break;
        case 4:
        LATAbits.LATA4=1;
        break;
        case 5:
        LATAbits.LATA6=1;
        break;
        case 6:
        LATCbits.LATC6=1;
        break;
        default:  // 1 seul
        LATBbits.LATB2=1;
        break;
    }
}
void spi_write(unsigned char data)
{
    SSP2BUF=data;
    while(SSP2STATbits.BF ==0 );
}
unsigned char spi_read()
{
    return SSP2BUF;
}
