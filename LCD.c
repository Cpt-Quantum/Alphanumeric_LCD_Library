/*
 * File:   LCD.c
 * Author: SAI-Connor
 *
 * Created on 13 June 2017, 16:42
 */

#include <xc.h>
#include <stdint.h>
#include "LCD.h"

//Define constants to choose which pins to use for each signal on the LCD
#define RS LATAbits.LATA5    //Register select pin
#define EN LATAbits.LATA4    //Enable pin
#define D4 LATAbits.LATA0    
#define D5 LATAbits.LATA1
#define D6 LATAbits.LATA2
#define D7 LATAbits.LATA3

//Define oscillator frequency for delay function use
#define _XTAL_FREQ 1000000

//Define required delay times for data to be read from pins DB7-DB4
#define min_delay 5 //ms

//Define minimum instruction time for LCD functions
#define min_instruct_time 40 //microseconds

//Define functions
void LCD_Port(char a){
    if (a & 1)
        D4 = 1;
    else 
        D4 = 0;
    
    if (a & 2)
        D5 = 1;
    else 
        D5 = 0;
    
    if (a & 4)
        D6 = 1;
    else
        D6 = 0;
    
    if (a & 8)
        D7 = 1;
    else
        D7 = 0; 
}
void LCD_Cmd(char a){
    RS = 0;
    LCD_Port(a);
    EN = 1;
    
    __delay_ms(min_delay);
    
    EN = 0;
}
void LCD_Clear(){
    LCD_Cmd(0);
    LCD_Cmd(1);
}

void LCD_Cursor_Shift(char a){
    char temp;
    temp = 0x10 | a;
    LCD_Cmd(temp);
}

//This sets the cursor position to a desired value, (x,y) out of (16,2)
void LCD_Cursor_Position(char x, char y){
    char temp, left_bits, right_bits;
    if (y==1){
        //1st line addresses start from 0x80 (0x00 with a 1 in the MSB)
        temp = 0x80 + x - 1;
        //Get the most significant bits and shift them right by 4
        left_bits = temp >> 4;
        //Get the least significant bits
        right_bits = temp & 0x0F;
    }
    else if (y==2){
        //2nd line addresses start from 0xC0 (0x40 with a 1 in the MSB)
        temp = 0xC0 + x - 1;
        //Get the most significant bits and shift them right by 4
        left_bits = temp >> 4;
        //Get the least significant bits
        right_bits = temp & 0x0F;
    }
    LCD_Cmd(left_bits);
    LCD_Cmd(right_bits);
}

void LCD_Write_Char(char a){
    //Define temporary variables to hold the top 4 bits (left) 
    // and bottom 4 bits (right).
    char left_bits, right_bits;
    left_bits=a&0xF0;
    right_bits=a&0x0F;

    RS = 1;
    //Send the most significant bits first
    LCD_Port(left_bits>>4);
    EN = 1;
    __delay_us(min_instruct_time);
    EN = 0;
    LCD_Port(right_bits);
    EN = 1;
    __delay_us(min_instruct_time);
    EN = 0;
}
void LCD_Write_String(char *a, uint8_t array_length){
    for(uint8_t i = 0; i < array_length; i++){
        LCD_Write_Char(a[i]);
    };
}

//Note that these are available only for the first 16 bits of RAM (0x00-0x0F)
void LCD_CGRAM_Write(char *a, char custom_char_length, char address){
    //Separate address into left and right nibbles
    char left_bits = (0x30 & address);
    char right_bits = (0x0F & address);
    //Go to CGRAM address
    LCD_Cmd(0x4| left_bits);
    LCD_Cmd(right_bits);
    //Write custom char to memory
    for(char i = 0; i<custom_char_length; i++){
        LCD_Write_Char(a[i]);
    }
    //Return home
    LCD_Cmd(0);
    LCD_Cmd(2); 
}

/* Function notes */
/* This function takes inputs to allow for configuration of the way    */
/*  the LCD is initialised.                                            */
/* num_display_lines = 0 for 1 display lines                           */
/* num_display_lines = 1 for 2 display lines                           */
/* font_type = 0 for 5x8 font display type                             */
/* font_type = 1 for 5x10 font display type                            */
/* cursor_state = 0 for cursor off                                     */
/* cursor_state = 1 for cursor on                                      */
/* cursor_blink_state = 0 for cursor to blink                          */
/* cursor_blink_state = 1 for no cursor blink                          */
void LCD_Initialise(uint8_t num_display_lines, uint8_t font_type,
        uint8_t cursor_state, uint8_t cursor_blink_state ){
    //Initialise all pins to 0
    LCD_Port(0x00);
    RS = 0;
    EN = 0;
        
    //Set the states according the function inputs
    char display_control
                =( 0x0C | (cursor_state<<1) | (cursor_blink_state) );
    char function_set
                =( 0x00 | (num_display_lines<<3) | (font_type<<2) );
    
    
    /* Device starts off in 8 bit mode, although lower 4 bits aren't used */
    __delay_ms(40);
    LCD_Cmd(0x03);
    __delay_ms(5);
    LCD_Cmd(0x03);
    __delay_ms(1);
    LCD_Cmd(0x03);
    __delay_ms(1);
    
    LCD_Cmd(0x02);
    __delay_ms(1);
    /* End of startup*/
        //Function set: This is where the 4/8 bit mode is set, as well as font 
    // and line mode. 
    LCD_Cmd(0x02);
    LCD_Cmd(function_set);
    //Turn display off
    LCD_Cmd(0x00);
    LCD_Cmd(0x08);
    //Clear display
    LCD_Cmd(0x00);
    LCD_Cmd(0x01);
    __delay_ms(5);
    //Entry mode set
    LCD_Cmd(0x00);
    LCD_Cmd(0x06);
    
    //Wait to ensure the LCD is finished initialising
    __delay_ms(20);
    
    //Turn on the display again
    LCD_Cmd(0x00);
    LCD_Cmd(display_control);
    __delay_ms(20);
}


