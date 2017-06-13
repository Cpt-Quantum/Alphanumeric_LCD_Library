/*
 * File:   main.c
 * Author: SAI-Connor
 *
 * Created on 13 June 2017, 16:42
 */

#include <xc.h>
#include <stdint.h>
#include "LCD.h"
/***********************************************************************************************************************************************/
// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

// CONFIG1L
#pragma config PLLSEL = PLL4X   // PLL Selection (4x clock multiplier)
#pragma config CFGPLLEN = OFF   // PLL Enable Configuration bit (PLL Disabled (firmware controlled))
#pragma config CPUDIV = NOCLKDIV// CPU System Clock Postscaler (CPU uses system clock (no divide))
#pragma config LS48MHZ = SYS24X4// Low Speed USB mode with 48 MHz system clock (System clock at 24 MHz, USB clock divider is set to 4)

// CONFIG1H
#pragma config FOSC = INTOSCIO  // Oscillator Selection (Internal oscillator)
#pragma config PCLKEN = ON      // Primary Oscillator Shutdown (Primary oscillator enabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config nPWRTEN = OFF    // Power-up Timer Enable (Power up timer disabled)
#pragma config BOREN = SBORDIS  // Brown-out Reset Enable (BOR enabled in hardware (SBOREN is ignored))
#pragma config BORV = 190       // Brown-out Reset Voltage (BOR set to 1.9V nominal)
#pragma config nLPBOR = OFF     // Low-Power Brown-out Reset (Low-Power Brown-out Reset disabled)

// CONFIG2H
#pragma config WDTEN = OFF      // Watchdog Timer Enable bits (WDT disabled in hardware (SWDTEN ignored))
#pragma config WDTPS = 32768    // Watchdog Timer Postscaler (1:32768)

// CONFIG3H
#pragma config CCP2MX = RC1     // CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
#pragma config PBADEN = OFF     // PORTB A/D Enable bit (PORTB<5:0> pins are configured as digital I/O on Reset)
#pragma config T3CMX = RC0      // Timer3 Clock Input MUX bit (T3CKI function is on RC0)
#pragma config SDOMX = RB3      // SDO Output MUX bit (SDO function is on RB3)
#pragma config MCLRE = ON       // Master Clear Reset Pin Enable (MCLR pin enabled; RE3 input disabled)

// CONFIG4L
#pragma config STVREN = ON      // Stack Full/Underflow Reset (Stack full/underflow will cause Reset)
#pragma config LVP = OFF        // Single-Supply ICSP Enable bit (Single-Supply ICSP disabled)
#pragma config ICPRT = OFF      // Dedicated In-Circuit Debug/Programming Port Enable (ICPORT disabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled)

// CONFIG5L
#pragma config CP0 = OFF        // Block 0 Code Protect (Block 0 is not code-protected)
#pragma config CP1 = OFF        // Block 1 Code Protect (Block 1 is not code-protected)
#pragma config CP2 = OFF        // Block 2 Code Protect (Block 2 is not code-protected)
#pragma config CP3 = OFF        // Block 3 Code Protect (Block 3 is not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protect (Boot block is not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protect (Data EEPROM is not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Block 0 Write Protect (Block 0 (0800-1FFFh) is not write-protected)
#pragma config WRT1 = OFF       // Block 1 Write Protect (Block 1 (2000-3FFFh) is not write-protected)
#pragma config WRT2 = OFF       // Block 2 Write Protect (Block 2 (04000-5FFFh) is not write-protected)
#pragma config WRT3 = OFF       // Block 3 Write Protect (Block 3 (06000-7FFFh) is not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Registers Write Protect (Configuration registers (300000-3000FFh) are not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protect (Boot block (0000-7FFh) is not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protect (Data EEPROM is not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Block 0 Table Read Protect (Block 0 is not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Block 1 Table Read Protect (Block 1 is not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Block 2 Table Read Protect (Block 2 is not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF      // Block 3 Table Read Protect (Block 3 is not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protect (Boot block is not protected from table reads executed in other blocks)
/***********************************************************************************************************************************************/

//Define oscillator frequency for delay function use
#define _XTAL_FREQ 1000000

#define Program_Good LATEbits.LATE0 //Signal to show the PIC has programmed

void main(void) {
    //Turn off comparitors
    CM1CON0 = 0x07;
    CM2CON0 = 0x07;

    //Turn off analogue functions to appropriate pins
    ANSELA = 0x00;
    ANSELE = 0x00;
    
    //Setup appropriate pins as outputs
    TRISA= 0x00;
    TRISE0 = 0;
    
    //Set the program check low
    Program_Good = 0;
        
    //Initialise LCD
    LCD_Initialise(1,0,1,1);
    
    //Move to start position
    LCD_Cursor_Position(1,1);
    
    //Create a custom character
    char a[8];
    
    a[0] = 0x00;
    a[1] = 0x0A;
    a[2] = 0x1F;
    a[3] = 0x1F;
    a[4] = 0x1F;
    a[5] = 0x0E;
    a[6] = 0x04;
    a[7] = 0x00;
    
    LCD_CGRAM_Write(a, 8, 0x00);
    
    //Write H
    LCD_Write_Char('I');
    LCD_Write_Char(0x00);
    LCD_Write_Char('M');
    LCD_Write_Char('a');
    LCD_Write_Char('n');
    LCD_Write_Char('c');
    LCD_Write_Char('h');
    LCD_Write_Char('e');
    LCD_Write_Char('s');
    LCD_Write_Char('t');
    LCD_Write_Char('e');
    LCD_Write_Char('r');
    
    
    Program_Good = 1;
    while(1){
    for (int i=0; i<1000; i++){
        __delay_ms(1);
    }
//    Program_Good = !(Program_Good);
    }
}