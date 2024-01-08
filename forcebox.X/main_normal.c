/*
 * File:   main.c
 * Author: xgaltier
 *
 * Created on 11 décembre 2023, 17:02
 */

// CONFIG1
#pragma config FEXTOSC = OFF    // External Oscillator mode selection bits (Oscillator not enabled)
#pragma config RSTOSC = HFINT32// Power-up default value for COSC bits (HFINTOSC with 2x PLL, with OSCFRQ = 16 MHz and CDIV = 1:1 (FOSC = 32 MHz))
#pragma config CLKOUTEN = OFF   // Clock Out Enable bit (CLKOUT function is disabled; i/o or oscillator function on OSC2)
#pragma config CSWEN = OFF      // Clock Switch Enable bit (The NOSC and NDIV bits cannot be changed by user software)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (FSCM timer disabled)

// CONFIG2
#pragma config MCLRE = ON       // Master Clear Enable bit (MCLR pin is Master Clear function)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config LPBOREN = OFF    // Low-Power BOR enable bit (ULPBOR disabled)
#pragma config BOREN = OFF      // Brown-out reset enable bits (Brown-out reset disabled)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (VBOR) set to 1.9V on LF, and 2.45V on F Devices)
#pragma config ZCD = OFF        // Zero-cross detect disable (Zero-cross detect circuit is disabled at POR.)
#pragma config PPS1WAY = OFF    // Peripheral Pin Select one-way control (The PPSLOCK bit can be set and cleared repeatedly by software)
#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable bit (Stack Overflow or Underflow will cause a reset)

// CONFIG3
#pragma config WDTCPS = WDTCPS_31// WDT Period Select bits (Divider ratio 1:65536; software control of WDTPS)
#pragma config WDTE = OFF       // WDT operating mode (WDT Disabled, SWDTEN is ignored)
#pragma config WDTCWS = WDTCWS_7// WDT Window Select bits (window always open (100%); software control; keyed access not required)
#pragma config WDTCCS = SC      // WDT input clock selector (Software Control)

// CONFIG4
#pragma config WRT = OFF        // UserNVM self-write protection bits (Write protection off)
#pragma config SCANE = available// Scanner Enable bit (Scanner module is available for use)
#pragma config LVP = OFF         // Low Voltage Programming Enable bit (Low Voltage programming enabled. MCLR/Vpp pin function is MCLR.)

// CONFIG5
#pragma config CP = OFF         // UserNVM Program memory code protection bit (Program Memory code protection disabled)
#pragma config CPD = OFF        // DataNVM code protection bit (Data EEPROM code protection disabled)

#define _XTAL_FREQ 32000000
#define SCAN_TIME_MCP 0x006FFF

#include <xc.h>
#include "mcp3564.h"
#include "I2C.h"
#include "UART.h"

void config_chip();
res_voie conv_xg[5];
unsigned char nbr_module=1;
volatile unsigned long temps=0; 

void __interrupt() my_isr(void)
{
    if (PIE0bits.TMR0IE && PIR0bits.TMR0IF)
    {
        temps++;
        PIR0bits.TMR0IF=0; 
    }
}

void main(void) {
    unsigned char i, j;
    short test;
    /* Debut config MCU */
    config_chip();
    UART_init();
    temps = 0;
    /* configuration MCU terminer*/
    /* Config 1° module ADC MCP3564
     si on ne recoit pas de nbr de module on a 1 seul module */
    mcp356x_writeregister(MCP356x_TIMER, SCAN_TIME_MCP, 3, 1);
    mcp356x_writeregister(MCP356x_SCAN, 0x001f00, 3, 1);
    mcp356x_writeregister8(MCP356x_CONFIG3, 0xB0, 1);
    mcp356x_writeregister8(MCP356x_CONFIG0, 0xE2, 1);
    mcp356x_writeregister8(MCP356x_CONFIG1, 0x18, 1);
    while (1) {
        // On verifie si on a recu qque chose du port serie
        if (PIR3bits.RCIF) { // On a recu qque chose
            //Gestion Erreur
            if (RC1STAbits.OERR == 1) // EUSART error - restart
            {
                RC1STAbits.CREN = 0;
                RC1STAbits.CREN = 1;
            }
            if ((RC1REG == '1') || (RC1REG == '2') || (RC1REG == '3') || (RC1REG == '4') || (RC1REG == '5') || (RC1REG == '6')) { // on a change de nbre de module
                nbr_module = RC1REG - 0x30;
                // FAIRE CONFIGURATION DES MODULES SUPPLEMENTAIRES
                switch (nbr_module) {
                    case 6:
                        mcp356x_writeregister(MCP356x_TIMER, SCAN_TIME_MCP, 3, 6);
                        mcp356x_writeregister(MCP356x_SCAN, 0x000f00, 3, 6);
                        mcp356x_writeregister8(MCP356x_CONFIG3, 0xF0, 6);
                        mcp356x_writeregister8(MCP356x_CONFIG0, 0xE3, 6);
                    case 5:
                        mcp356x_writeregister(MCP356x_TIMER, SCAN_TIME_MCP, 3, 5);
                        mcp356x_writeregister(MCP356x_SCAN, 0x000f00, 3, 5);
                        mcp356x_writeregister8(MCP356x_CONFIG3, 0xF0, 5);
                        mcp356x_writeregister8(MCP356x_CONFIG0, 0xE3, 5);
                    case 4:
                        mcp356x_writeregister(MCP356x_TIMER, SCAN_TIME_MCP, 3, 4);
                        mcp356x_writeregister(MCP356x_SCAN, 0x000f00, 3, 4);
                        mcp356x_writeregister8(MCP356x_CONFIG3, 0xF0, 4);
                        mcp356x_writeregister8(MCP356x_CONFIG0, 0xE3, 4);
                    case 3:
                        mcp356x_writeregister(MCP356x_TIMER, SCAN_TIME_MCP, 3, 3);
                        mcp356x_writeregister(MCP356x_SCAN, 0x000f00, 3, 3);
                        mcp356x_writeregister8(MCP356x_CONFIG3, 0xF0, 3);
                        mcp356x_writeregister8(MCP356x_CONFIG0, 0xE3, 3);
                    case 2:
                        mcp356x_writeregister(MCP356x_TIMER, SCAN_TIME_MCP, 3, 2);
                        mcp356x_writeregister(MCP356x_SCAN, 0x000f00, 3, 2);
                        mcp356x_writeregister8(MCP356x_CONFIG3, 0xF0, 2);
                        mcp356x_writeregister8(MCP356x_CONFIG0, 0xE3, 2);
                    case 1:
                        mcp356x_writeregister(MCP356x_TIMER, SCAN_TIME_MCP, 3, 1);
                        mcp356x_writeregister(MCP356x_SCAN, 0x001f00, 3, 1);
                        mcp356x_writeregister8(MCP356x_CONFIG3, 0xF0, 1);
                        mcp356x_writeregister8(MCP356x_CONFIG0, 0xE3, 1);
                        break;
                }
            }
            if (RC1REG == 'R') { // on demande un Read
                //mcp356x_result32_channel(1,5);

                switch (nbr_module) {
                    case 6:
                        mcp356x_result32_channel(6, 4);
                    case 5:
                        mcp356x_result32_channel(5, 4);
                    case 4:
                        mcp356x_result32_channel(4, 4);
                    case 3:
                        mcp356x_result32_channel(3, 4);
                    case 2:
                        mcp356x_result32_channel(2, 4);
                    case 1:
                    default: // on traite comme un seul module
                        mcp356x_result32_channel(1, 5);
                }
            }
        }
        // On doit faire les mesures des differents modules et recuperer les valeurs
        // on ne mesure la temperature que sur le 1° module

    }
    return;
}




void config_chip()
{
    // PWM pour clk sur RC7
    
    // MOSI sur RB5 - Output
    // MISO sur RB4 - Input
    // SCK sur RB3 - Output
    // CS sur RB2 - Output
    // PPS Unlock sequence

    PPSLOCK = 0x55;
    PPSLOCK = 0xAA;
    PPSLOCKbits.PPSLOCKED = 0x00;
    ANSELB=0;
    TRISBbits.TRISB5=0;
    RB5PPS=0x17;
    TRISBbits.TRISB4=1;
    SSP2DATPPS=0x0C;
    TRISBbits.TRISB3=0;
    RB3PPS=0x16;
    TRISBbits.TRISB2=0;
    SSP2SSPPS=0x0A;
    SSP2STATbits.CKE=1;
    SSP2STATbits.SMP=1;
    //PORTBbits.RB2=1;
    SSP2CON1bits.SSPM=2;
    SSP2CON1bits.SSPEN=1;
    // LED sur RA0
    ANSELA=0;
    TRISAbits.TRISA0=0;
    TRISAbits.TRISA1=1;
    TRISAbits.TRISA2=0;
    TRISAbits.TRISA3=1;
    TRISAbits.TRISA4=0;
    TRISAbits.TRISA5=1;
    TRISAbits.TRISA6=0;
    TRISAbits.TRISA7=1;
    ANSELC=0;
    TRISCbits.TRISC2=1;
    TRISCbits.TRISC3=1;
    TRISCbits.TRISC4=1;
    TRISCbits.TRISC6=0;
    TRISCbits.TRISC7=1;
    // Config Timer0 pour gestion du temps
    PIE0bits.TMR0IE=1; //ISR Timer0 ON
    INTCON=0xC1; // GIE PEIE ...  ISR ON
    T0CON1bits.T0CS=3 ; // Fosc 32MHz
    T0CON1bits.T0CKPS=0x7 ; // Horloge 32e6 divise par 32768 soit une ISR toute les 1ms env.
    T0CON1bits.T0ASYNC=0;
    T0CON0bits.T016BIT=0;;
    T0CON0bits.T0EN=1; // on demarre Timer0
   
}