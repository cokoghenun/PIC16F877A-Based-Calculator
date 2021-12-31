/* Name     : main.c
*  Purpose  : Main file for make calculator using keypad and PIC16F877.
*  Author   : Amlendra Kumar
*  Website  : https://aticleworld.com
*/
#include<htc.h>

// Configuration word for PIC16F877A
__CONFIG( FOSC_HS & WDTE_OFF & PWRTE_ON & CP_OFF & BOREN_ON
          & LVP_OFF & CPD_OFF & WRT_OFF & DEBUG_OFF);



// Define CPU Frequency
// This must be defined, if __delay_ms() or
// __delay_us() functions are used in the code
#define _XTAL_FREQ   20000000


// Define Pins for LCD
#define LCD_E    			RD0     // Enable pin for LCD
#define LCD_RS	 			RD1     // RS pin for LCD
#define LCD_Data_Bus_D4		RD4		// Data bus bit 4
#define LCD_Data_Bus_D5		RD5		// Data bus bit 5
#define LCD_Data_Bus_D6		RD6		// Data bus bit 6
#define LCD_Data_Bus_D7		RD7		// Data bus bit 7

// Define Pins direction registrers
#define LCD_E_Dir     			TRISD0
#define LCD_RS_Dir   	 		TRISD1
#define LCD_Data_Bus_Dir_D4   	TRISD4
#define LCD_Data_Bus_Dir_D5     TRISD5
#define LCD_Data_Bus_Dir_D6  	TRISD6
#define LCD_Data_Bus_Dir_D7 	TRISD7

// Define which port is being used for data bus
#define LCD_PORT	PORTD

// Constants
#define E_Delay       500



// Define pins for keypad
#define RowA		RB0
#define RowB		RB1
#define RowC		RB2
#define RowD		RB3
#define C1			RB4
#define C2			RB5
#define C3			RB6
#define C4			RB7

#define Keypad_PORT			PORTB
#define Keypad_PORT_Dir		TRISB


// Function Declarations for LCD
void WriteCommandToLCD(unsigned char);
void WriteDataToLCD(char);
void InitLCD(void);
void WriteStringToLCD(const char*);
void ClearLCDScreen(void);


// Function declarations for keypad
void InitKeypad(void);
char GetKey(void);


// define Error (to be used inside calculator functions)
#define Error   13

// Some function declarations
int get_num(char);
char get_func(char);
void DispError(int);
void disp_num(int);



int main(void)
{
    char key;           // Key char for keeping record of pressed key
    int num1 = 0;       // First number
    char func = '+';    // Function to be performed among two numbers
    int num2 = 0;       // Second number

    InitKeypad();       // Initialize Keypad
    InitLCD();			// Initialize LCD

    while(1)
    {
        //get numb1
        key = GetKey();
        ClearLCDScreen();	    	// Clear LCD screen
        WriteDataToLCD(key);       // Echo the key pressed to LCD
        num1 = get_num(key);       // Get int number from char value, it checks for wrong input as well

        if(num1!=Error)            // If correct input then proceed, num1==Error means wrong input
        {
            //get function
            key = GetKey();
            WriteDataToLCD(key);             //Echo the key pressed to LCD
            func = get_func(key);            //it checks for wrong func

            if(func!='e')                    //if correct input then proceed, func=='e' means wrong input
            {
                //get numb2
                key = GetKey();
                WriteDataToLCD(key);   //Echo the key pressed to LCD
                num2 = get_num(key);  //Get int number from char value, it checks for wrong input as well

                if(num2!=Error)  //if correct input then proceed, num2==Error means wrong input
                {
                    //get equal sign
                    key = GetKey();
                    WriteDataToLCD(key); //Echo the key pressed to LCD

                    if(key == '=')    //if = is pressed then proceed
                    {
                        switch(func)    //switch on function
                        {
                        case '+':
                            disp_num(num1+num2);
                            break;
                        case '-':
                            disp_num(num1-num2);
                            break;
                        case 'x':
                            disp_num(num1*num2);
                            break;
                        case '/':
                            disp_num(num1/num2);
                            break;
                        }
                    }
                    else	 //key other then = here means error wrong input
                    {
                        if(key == 'C')     //if clear screen is pressed then clear screen and reset
                            ClearLCDScreen(); // Clear LCD screen
                        else
                            DispError(0); 	  //Display wrong input error
                    }
                }
            }
        }
    }
    return 0;
}


// Function name: InitKeypad
void InitKeypad(void)
{
    Keypad_PORT	    = 0x00;	// Set Keypad port pin values zero
    Keypad_PORT_Dir = 0xF0;	// Last 4 pins input, First 4 pins output

    // Enable weak internal pull up on input pins
    OPTION_REG &= 0x7F;
}


// Scan all the keypad keys to detect any pressed key.
char READ_SWITCHES(void)	
{	
  
  
  RowA = 0; RowB = 1; RowC = 1; RowD = 1; 	//Test Row A

  if (C1 == 0) { __delay_ms(250); while (C1==0); return '7'; }
  if (C2 == 0) { __delay_ms(250); while (C2==0); return '8'; }
  if (C3 == 0) { __delay_ms(250); while (C3==0); return '9'; }
  if (C4 == 0) { __delay_ms(250); while (C4==0); return '/'; }

  RowA = 1; RowB = 0; RowC = 1; RowD = 1; 	//Test Row B

  if (C1 == 0) { __delay_ms(250); while (C1==0); return '4'; }
  if (C2 == 0) { __delay_ms(250); while (C2==0); return '5'; }
  if (C3 == 0) { __delay_ms(250); while (C3==0); return '6'; }
  if (C4 == 0) { __delay_ms(250); while (C4==0); return 'x'; }
  
  RowA = 1; RowB = 1; RowC = 0; RowD = 1; 	//Test Row C

  if (C1 == 0) { __delay_ms(250); while (C1==0); return '1'; }
  if (C2 == 0) { __delay_ms(250); while (C2==0); return '2'; }
  if (C3 == 0) { __delay_ms(250); while (C3==0); return '3'; }
  if (C4 == 0) { __delay_ms(250); while (C4==0); return '-'; }
  
  RowA = 1; RowB = 1; RowC = 1; RowD = 0; 	//Test Row D

  if (C1 == 0) { __delay_ms(250); while (C1==0); return 'C'; }
  if (C2 == 0) { __delay_ms(250); while (C2==0); return '0'; }
  if (C3 == 0) { __delay_ms(250); while (C3==0); return '='; }
  if (C4 == 0) { __delay_ms(250); while (C4==0); return '+'; }

  return 'n';           	// Means no key has been pressed
}

// Function name: GetKey
// Read pressed key value from keypad and return its value
char GetKey(void)           	 // Get key from user
{
    char key = 'n';              // Assume no key pressed

    while(key=='n')              // Wait untill a key is pressed
        key = READ_SWITCHES();   // Scan the keys again and again

    return key;                  //when key pressed then return its value
}



void ToggleEpinOfLCD(void)
{
    LCD_E = 1;                // Give a pulse on E pin
    __delay_us(E_Delay);      // so that LCD can latch the
    LCD_E = 0;                // data from data bus
    __delay_us(E_Delay);
}


void WriteCommandToLCD(unsigned char Command)
{
    LCD_RS = 0;				 	     // It is a command

    LCD_PORT &= 0x0F;		  		 // Make Data pins zero
    LCD_PORT |= (Command&0xF0);		 // Write Upper nibble of data
    ToggleEpinOfLCD();		   		 // Give pulse on E pin

    LCD_PORT &= 0x0F;		   		 // Make Data pins zero
    LCD_PORT |= ((Command<<4)&0xF0); // Write Lower nibble of data
    ToggleEpinOfLCD();		   		 // Give pulse on E pin
}


void WriteDataToLCD(char LCDChar)
{
    LCD_RS = 1;				 		 // It is data

    LCD_PORT &= 0x0F;			 	 // Make Data pins zero
    LCD_PORT |= (LCDChar&0xF0);  	 // Write Upper nibble of data
    ToggleEpinOfLCD();		 		 // Give pulse on E pin

    LCD_PORT &= 0x0F;		    	 // Make Data pins zero
    LCD_PORT |= ((LCDChar<<4)&0xF0); // Write Lower nibble of data
    ToggleEpinOfLCD();		 		 // Give pulse on E pin
}


void InitLCD(void)
{
    // Firstly make all pins output
    LCD_E  		 		 = 0;   // E  = 0
    LCD_RS  	 		 = 0;   // RS = 0
    LCD_Data_Bus_D4		 = 0;  	// Data bus = 0
    LCD_Data_Bus_D5		 = 0;  	// Data bus = 0
    LCD_Data_Bus_D6		 = 0;  	// Data bus = 0
    LCD_Data_Bus_D7		 = 0;  	// Data bus = 0
    LCD_E_Dir    		 = 0;   // Make Output
    LCD_RS_Dir    	 	 = 0;   // Make Output
    LCD_Data_Bus_Dir_D4  = 0;   // Make Output
    LCD_Data_Bus_Dir_D5  = 0;   // Make Output
    LCD_Data_Bus_Dir_D6  = 0;   // Make Output
    LCD_Data_Bus_Dir_D7  = 0;   // Make Output

    ///////////////// Reset process from datasheet //////////////
    __delay_ms(40);

    LCD_PORT &= 0x0F;		  // Make Data pins zero
    LCD_PORT |= 0x30;		  // Write 0x3 value on data bus
    ToggleEpinOfLCD();		  // Give pulse on E pin

    __delay_ms(6);

    LCD_PORT &= 0x0F;		  // Make Data pins zero
    LCD_PORT |= 0x30;		  // Write 0x3 value on data bus
    ToggleEpinOfLCD();		  // Give pulse on E pin

    __delay_us(300);

    LCD_PORT &= 0x0F;		  // Make Data pins zero
    LCD_PORT |= 0x30;		  // Write 0x3 value on data bus
    ToggleEpinOfLCD();		  // Give pulse on E pin

    __delay_ms(2);

    LCD_PORT &= 0x0F;		  // Make Data pins zero
    LCD_PORT |= 0x20;		  // Write 0x2 value on data bus
    ToggleEpinOfLCD();		  // Give pulse on E pin

    __delay_ms(2);
    /////////////// Reset Process End ////////////////
    WriteCommandToLCD(0x28);    //function set
    WriteCommandToLCD(0x0c);    //display on,cursor off,blink off
    WriteCommandToLCD(0x01);    //clear display
    WriteCommandToLCD(0x06);    //entry mode, set increment
}


void WriteStringToLCD(const char *s)
{
    while(*s)
        WriteDataToLCD(*s++);   // print first character on LCD
}


void ClearLCDScreen(void)       // Clear the Screen and return cursor to zero position
{
    WriteCommandToLCD(0x01);    // Clear the screen
    __delay_ms(2);              // Delay for cursor to return at zero position
}




/*
* Functions used inside main for
* making calculator are shown below
*/

int get_num(char ch)         //convert char into int
{
    int num = 0;

    switch(ch)
    {
    case '0':
        num = 0;
        break;
    case '1':
        num = 1;
        break;
    case '2':
        num = 2;
        break;
    case '3':
        num = 3;
        break;
    case '4':
        num = 4;
        break;
    case '5':
        num = 5;
        break;
    case '6':
        num = 6;
        break;
    case '7':
        num = 7;
        break;
    case '8':
        num = 8;
        break;
    case '9':
        num = 9;
        break;
    case 'C':
        ClearLCDScreen();
        num = Error;
        break;  //this is used as a clear screen and then reset by setting error
    default:
        DispError(0);
        num = Error;
        break;       //it means wrong input
    }

    return num;
}

char get_func(char chf)            //detects the errors in inputted function
{
    if(chf=='C')                   //if clear screen then clear the LCD and reset
    {
        ClearLCDScreen();          //clear display
        return 'e';
    }

    if( chf!='+' && chf!='-' && chf!='x' && chf!='/' )  //if input is not from allowed funtions then show error
    {
        DispError(1);
        return 'e';
    }

    return chf;                        //function is correct so return the correct function
}


void DispError(int numb)           //displays differet error messages
{
    ClearLCDScreen();              //clear display

    switch(numb)
    {
    case 0:
        WriteStringToLCD("Wrong Input");
        break;
    case 1:
        WriteStringToLCD("Wrong Function");
        break;
    default:
        WriteStringToLCD("Wrong Input");
        break;
    }
}

void disp_num(int numb)            //displays number on LCD
{
    unsigned char UnitDigit  = 0;  //It will contain unit digit of numb
    unsigned char TenthDigit = 0;  //It will contain 10th position digit of numb

    if(numb<0)
    {
        numb = -1*numb;          // Make number positive
        WriteDataToLCD('-');	 // Display a negative sign on LCD
    }

    TenthDigit = (numb/10);	                  // Find out Tenth Digit

    if( TenthDigit != 0)	                  // If it is zero, then don't display
        WriteDataToLCD(TenthDigit+0x30);	  // Make Char of TenthDigit and then display it on LCD

    UnitDigit = numb - TenthDigit*10;

    WriteDataToLCD(UnitDigit+0x30);	  // Make Char of UnitDigit and then display it on LCD
}