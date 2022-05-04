// 4180 Final Project Code - Nikhil and Audrey

// Product will have two inputs: passwords from bluetooth (lab3_5) and touchpad (lab1_6) 
// If the password is correct, a DC motor will run and unlock the door
// Instructions will be displayed on a uLCD (lab2_3)


// 1-9: numbers, 10: back, 11: 0, 12: enter 
// 8 digit password 


#include "mbed.h"
#include "LSM9DS1.h"
#include "uLCD_4DGL.h"
#include <mpr121.h>
#include "Motor.h"
#include "XNucleo53L0A1.h"
#include <stdio.h>
#include "rtos.h"
//#include <VL53L0X.h>


char passcode[9] = {'1', '2', '3', '4', '4', '3', '2', '1'};

DevI2C i2c(p28, p27);//DevI2C i2c(p9, p10); // Setup the i2c bus on pins 9 and 10
uLCD_4DGL uLCD(p13,p14,p20);//uLCD_4DGL uLCD(p13,p14,p11); // serial tx, serial rx, reset pin;
InterruptIn interrupt(p24);// Create the interrupt receiver object on pin 24

Serial pc(USBTX, USBRX);// Setup the Serial to the PC for debugging
RawSerial dev(p9, p10); //RawSerial dev(p28,p27);
Motor m(p23, p6, p7); // pwm, fwd, rev LEFT motor
//Motor m(p22, p25, p26);// Motor m(p23, p6, p5); //pwm, fwd, rev
DigitalOut shdn(p15);//DigitalOut shdn(p26); //Lidar sensor

DigitalOut mtr1(p6);//DigitalOut shdn(p26); //Lidar sensor
DigitalOut mtr2(p7);//DigitalOut shdn(p26); //Lidar sensor


//VL53L0X sensor(SENS1_PIN4, SENS1_PIN3);
DigitalIn limSwitch(p5);

Mutex LCD;
Timer t;

DigitalOut led1(LED1);
DigitalOut led2(LED2);
DigitalOut led3(LED3);
DigitalOut led4(LED4);


//I2C sensor pins - CHANGE PIN NUMBERS ?
#define VL53L0_I2C_SDA   p28
#define VL53L0_I2C_SCL   p27

// Setup the Mpr121: Capacitive Touch
Mpr121 mpr121(&i2c, Mpr121::ADD_VSS); // constructor(i2c object, i2c address of the mpr121)

//Global Variables
char tpCode[9];
int tpCount = 0;
char btCode[9];

const int kMaxBufferSize = 100;
char      buffer[kMaxBufferSize];
int       len = 0;

char      buffer2[kMaxBufferSize];
int       len2 = 0;

volatile int btCount = 0;
volatile bool codeCorrect = false;

volatile int value; 
volatile bool keyDetect = false;

volatile bool newCode = false;
volatile char temp;

void fallInterrupt() {
    led3 = !led3;
    value = mpr121.read(0x00);
    value += mpr121.read(0x01)<<8; //**What does this do? 
    keyDetect = true;
    switch (value) {
        case 0:
            temp = 'x'; //no key
            break;
        case 1:
            temp = 'c';
            break;
        case 2:
            temp = '7';
            break;
        case 4:
            temp = '4';
            break;
        case 8:
            temp = '1';
            break;
        case 16:
            temp = '0';
            break;
        case 32:
            temp = '8';
            break;
        case 64:
            temp = '5';
            break;
        case 128:
            temp = '2';
            break;
        case 256:
            temp = 'e';
            break;
        case 512:
            temp = '9';
            break;
        case 1024:
            temp = '6';
            break;
        case 2048:
            temp = '3';
            break;
    }
    if(temp == 'e') {
        newCode = true;
        buffer2[len2] = '\0';
        buffer2[8] = '\0';
        len2 = 0;
    } else if (temp == 'c') {
        len2 = 0;
        buffer2[len2] = '\0';
    } else if (temp == 'x') {
        // do nothing?
    }else {
        buffer2[len2++] = temp;
        buffer2[len2] = '\0';
    }
}

volatile int closed;

void motor(void const *args) {
    limSwitch.mode(PullUp);

    while(1) {
        led2 = !limSwitch;
        if(codeCorrect) {
            led4 = 1;
            closed = limSwitch;
            m.speed(1);
            t.start();
            for(int i = 0; i < 5000; i++){
                if (!limSwitch) break;
                Thread::wait(1);
            }
            m.speed(0);
            t.stop();
            led4 = 0;
            Thread::wait(5000);
            led4 = 1;
            float spinTime = t.read();
            t.reset();
            m.speed(-1);
            LCD.lock();
            uLCD.locate(0,12);
            uLCD.printf("sTime: %f", spinTime);
            Thread::wait(50);
            LCD.unlock();
            Thread::wait(spinTime * 1000);
            m.speed(0);
            codeCorrect = false;
        }
        led4 = 0;
        Thread::wait(100);
    }    
}

volatile bool newThing = false;

void dev_recv() {
    led1 = !led1;
    
    while(dev.readable()) {
        char new_char = dev.getc();          
        buffer[len++] = new_char;    
    }
    buffer[len] = '\0';
    buffer[8] = '\0';          
    newThing = true;

}

int main() {
    uLCD.cls(); // clear LCD
    uLCD.text_height(1); // Set text size to 2
    uLCD.text_width(1); // Needed w/ prev line
    uLCD.color(BLUE); // Set color to: (BLACK, RED, GREEN, BLUE, RED+GREEN, ect.)
    uLCD.locate(0,1); // Set cursor to (col, row)
    uLCD.printf("Enter passcode on phone or keypad");

    
    interrupt.fall(&fallInterrupt);
    interrupt.mode(PullUp);
    
    dev.baud(9600);
    dev.attach(&dev_recv, Serial::RxIrq);
    Thread thread1(motor);
    
    while(1) {
        LCD.lock();
        if(newThing) {
            len = 0;
            //uLCD.locate(0, 6); // Set cursor to (col, row)
            //uLCD.printf("buffer: %s", buffer);
            uLCD.locate(0, 7); // Set cursor to (col, row)
            if(strcmp(buffer, passcode) == 0) {
                uLCD.printf("RIGHT");
                codeCorrect = true;
            } else {
                uLCD.printf("WRONG");
                codeCorrect = false;
            }
            newThing = false;
        } else if(newCode) {
            uLCD.locate(0, 9); // Set cursor to (col, row)
            if(strcmp(buffer2, passcode) == 0) {
                uLCD.printf("RIGHT");
                codeCorrect = true;
            } else {
                uLCD.printf("WRONG");
                codeCorrect = false;
            }
            newCode = false;
        }
        if (led4) {
            uLCD.locate(0, 10); // Set cursor to (col, row)
            uLCD.printf("Closed: %d", closed);
        }
        if(keyDetect) {
            uLCD.locate(0, 4); // Set cursor to (col, row)
            uLCD.printf("         ");
            uLCD.locate(0, 4); // Set cursor to (col, row)
            uLCD.printf("t: %c", temp);
            uLCD.locate(0, 8); // Set cursor to (col, row)
            uLCD.printf("              ");
            uLCD.locate(0, 8); // Set cursor to (col, row)
            uLCD.printf("b2: %s", buffer2);
            keyDetect = false;
        }
        LCD.unlock();
        
        Thread::wait(50);
    }
}



