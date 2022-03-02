20x04 I2C LCD Library
==================================================
:Author: Yicheng Qian
:Course: ECE387
:Project: Midterm Project

This library provides a set of functions for interfacing the I2C LCD with ATmega 328P and a sample program for guiding you to start using it. Note the original authors of this library is Davide Gironi(Enable I2C on LCD) and Peter Fleury(LCD Driver). The library has almost all the functions that we normally use except for clear a specific line and print integer on the LCD. I have added those two functions into the library so that we can use it like what we did in our LCD lab.


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
  
Hardware Overview
-----------------

What is an I2C LCD?
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

An I2C LCD is a LCD display screen with I2C interface. It can be used to display a range of sizes. There are usually two typical sizes: 16x02 and 20x04(16 characters on 2 lines or 20 characters on 4 lines). The reason we need such a LCD with I2C is a normal LCD would need over 10 wires to connect to an Arduino while an I2C LCD would only use 4 wires to connect to an Arduino. 2 data pins are used to control the LCD. Addding an I2C interface reduces complexity and difficulty. In addition, more port resources are saved. An I2C LCD is maade up with a PCF8574T port expander and a HD44780 LCD. We will have a brief review of these two components.

HD44780 LCD
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The HD44780 LCD is a dot-matrix liquid crystal display controller driving 2 lines by 16 characters or 4 lines by 20 characters. (With expansion chip, it can go up to 80 characters). The following are the pins of HD44780 LCD that can be found at the topn of the LCD in the following picture. (Refer to https://wiki2.org/en/Hitachi_HD44780_LCD_controller)

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

The PCF8574 is an I2C bus to 8-bit parallel bus IO expander IC. The following board which is configured to be interfaced with 16×2 or 20×4 LCD Displays. The address pins are pulled HIGH(which simply means the address of the device is fixed to be 0x27. This is important!!!) and you do not have access to all the IO pins and the INT pin. The interface for this IC is I2C using SDA and SCL lines.

.. image:: https://github.com/qiany21/ECE387_Midterm_Project/blob/main/pcf8574t.jpg

Library Overview
-----------------

i2chw
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

pcf8574
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

lcdpcf8574
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


Note UART has nothing to do with the I2C LCD module, it is designed for debugging purpose.



