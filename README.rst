20x04 I2C LCD Library
==================================================
:Author: Yicheng Qian
:Course: ECE387
:Project: Midterm Project

This library provides a set of functions for interfacing an 20x04 I2C LCD with ATmega 328P and a sample program for guiding you to start using it. Note the original authors of this library is Davide Gironi(Enable I2C on LCD) and Peter Fleury(LCD Driver and TWI/I2C). The library has almost all the functions that we normally use except for clear a specific line and print integer on the LCD. I have added those two functions into the library so that we can use it like what we did in our LCD lab.


Installation
------------
You can install the I2C LCD library:

::

    git clone git@github.com:qiany21/ECE387_Midterm_Project.git

Note you also need a complete avr-gcc toolchain to make it work:

::

    sudo apt-get update
    sudo apt-get upgrade all
    sudo apt-get install gcc-avr binutils-avr avr-libc
    sudo apt-get install gdb-avr
    sudo apt-get install avrdude
    
Now you are all set! To compile the sample program(I borrowed the python script from Dr.Peter Jamieson):

::

    python ./compile_script.py 4 com3

where the number 4 indicates the level of compilation(there are a total of 4 levels) and com3 indicates the port used by Arduino(This can be different in your case, make sure you have the correct port number and change both the command line arguments and the setting in the script).

Note you might be using an I2C LCD with different size and/or other microcontrollers. Please refer to `Library`_ section and make relevant changes.
  
Hardware
-----------------

What is an I2C LCD?
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

An I2C LCD is an LCD display screen with an I2C interface. It can be used to display a range of sizes. There are usually two typical sizes: 16x02 and 20x04(16 characters on 2 lines or 20 characters on 4 lines). The reason we need such an LCD with I2C is a normal LCD would need over 10 wires to connect to an Arduino while an I2C LCD would only use 4 wires to connect to an Arduino. 2 data pins are used to control the LCD. Adding an I2C interface reduces complexity and difficulty. In addition, more port resources are saved. An I2C LCD is made up of a PCF8574T port expander and an HD44780 LCD. Usually, these two parts will be soldered together when you purchase one. We will have a brief review of these two components.

HD44780 LCD
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The HD44780 LCD is a dot-matrix liquid crystal display controller driving 2 lines by 16 characters or 4 lines by 20 characters. (With expansion chip, it can go up to 80 characters). The following is a list of pins of HD44780 LCD that can be found at the topn of the LCD in the following picture. (Refer to https://wiki2.org/en/Hitachi_HD44780_LCD_controller)

- Ground
- VCC +3.3 to +5V (typical)
- Contrast adjustment (VO) This is an analog input, typically connected to a potentiometer. The user must be able to control this voltage independent of all other adjustments, - in order to optimise visibility of the display that varies i.a. with temperature, and, in some cases height above the sea level. With a wrong adjustment the display will seem to malfunction.
- Register Select (RS). RS=0: Command, RS=1: Data
- Read/Write (R/W). R/W=0: Write, R/W=1: Read (In most applications reading from the HD44780 makes no sense. In that case this pin can be permanently connected to ground and no io pins need to be allocated to steer it.)
- Clock (Enable). Falling edge triggered
- Bit 0 (Not used in 4-bit operation)
- Bit 1 (Not used in 4-bit operation)
- Bit 2 (Not used in 4-bit operation)
- Bit 3 (Not used in 4-bit operation)
- Bit 4
- Bit 5
- Bit 6
- Bit 7
- Backlight Anode (+) (If applicable)
- Backlight Cathode (-) (If applicable)

.. image:: https://github.com/qiany21/ECE387_Midterm_Project/blob/main/lcd_20_04.jpg

PCF8574T Port Expander
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The PCF8574 is an I2C bus to 8-bit parallel bus IO expander IC. The following board which is configured to be interfaced with 16×2 or 20×4 LCD Displays. The address pins are pulled HIGH(which simply means the address of the device is fixed to be 0x27. This is important!!!) and you do not have access to all the IO pins and the INT pin. The interface for this IC is I2C using SDA and SCL lines. By I2C, The data is transmitted in the form of frame which is a bit sequence(The sequence is made up of start condition, address frame/data frame, and ACK message), and it is controlled by changing SDA and SCLK. For more information about PCF8574T, please refer to https://www.ti.com/lit/ds/symlink/pcf8574.pdf. For more information about I2C Communication Protocol, please refer to https://learn.sparkfun.com/tutorials/i2c/all

.. image:: https://github.com/qiany21/ECE387_Midterm_Project/blob/main/pcf8574t.jpg

Library
-----------------

i2chw
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
This I2C library is implemented as a compact assembler software implementation of the I2C protocol which runs on any AVR (i2cmaster.S) and as a TWI hardware interface for all AVR with built-in TWI hardware (twimaster.c). TWI is the name used by Atmel but is the same as I2C. If you cannot understand what twimaster.c does, you can check any AVR chip's datasheet for its TWI module.

| Important:
|
| To make your program work with this I2C LCD library. You should know the microprocessor on your Arduino. For example, this library has been modified to work on Atmega328P. The F_CPU in twimaster.c has been changed to 16MHz in order to make it work. Please adjust this parameter in case of using other microprocessors.

Another important parameter is SCL_CLOCK, adjust this to change the speed of displaying characters.

pcf8574
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
This PCF8574 library is implemented as a helper library to set the status of those pins that are connected to the LCD. Meanwhile, it will be used to initialize the I2C communication. I changed PCF8574_ADDRBASE to be 0x27 which corresponds to have A0, A1, A2 of PCF8574 set to high. This is the library that will be used in library lcdpcf8574. 

| Important:
|
| If you have PCF8574T and HD44780 separated and you solder them together by yourself, it is most likely you can set the address of the device. In this case, change PCF8574_ADDRBASE in pcf8574.h to match the address you set.


lcdpcf8574
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
This LCD library includes some functions to control and write to the LCD. Two additional methods that help users program the LCD have been added.

- void lcd_clear(int line) : This method clears a given line. There was only one method to clear the whole screen, and that is not helpful.

- void lcd_puti(int i) : This method prints a given integer onto the LCD. There were methods to print char and string onto the LCD. This method helps when we want to print the loop control variable or something else that is an integer type.

| Important:
|
| If you are using an LCD with diffrent size, please make changes to LCD_LINES, LCD_DISP_LENGTH, LCD_WRAP_LINES in lcdpcf8574.h so that those parameters match your LCD display.

Note UART has nothing to do with the I2C LCD module, it is designed for debugging purpose.


Sample Program and Circuit Diagram
----------------------------------------
