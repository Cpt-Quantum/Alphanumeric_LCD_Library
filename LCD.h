/* 
 * File:   LCD.h
 * Author: SAI-Connor
 *
 * Created on 13 June 2017, 16:42
 */

#ifndef LCD
#define	LCD

#include <stdint.h>

//#ifdef	__cplusplus
//extern "C" {
//#endif



//Function to set bits on each data pin
void LCD_Port(char a);

//Function to send the LCD a predefined function call
void LCD_Cmd(char a);

//Function to clear the LCD
void LCD_Clear();

//Function to move cursor or shift display
void LCD_Cursor_Shift(char a);

//Function to move the cursor to a desired position (including home)
void LCD_Cursor_Position(char x, char y);

//Functions to write data to the LCD screen
void LCD_Write_Char(char a);
void LCD_Write_String(char *a, char length);

//Function to write custom character to CGRAM
void LCD_CGRAM_Write(char *a, char custom_char_length, char address);

//Function to initialise the LCD at startup
void LCD_Initialise(uint8_t num_display_lines, uint8_t font_type,
        uint8_t cursor_state, uint8_t cursor_blink_state );

//#ifdef	__cplusplus
//}
//#endif

#endif	/* LCD_H */

