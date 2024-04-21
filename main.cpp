/*
MIT License

Copyright (c) [2024] 
Organization: Perlatecnica APS ETS
Author: Mauro D'Angelo
Example to use the LSM6DSV16X Machine Learning Core

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "mbed.h"
#include "plt_iks4a1.h"
#include "lsm6dsv16x_activity_recognition_for_mobile.h"

LSM6DSV16X AccGyr(I2C_SDA, I2C_SCL); 
Serial pc(USBTX, USBRX);
DigitalOut led(LED1);
InterruptIn intPin(D5);

// MLC
ucf_line_t *ProgramPointer;
int32_t LineCounter;
int32_t TotalNumberOfLine;

void printMLCStatus(uint8_t status);

//Interrupts.
void INT1Event_cb();
volatile int mems_event = 0;


// main() runs in its own thread in the OS
int main()
{
    lsm6dsv16x_mlc_out_t  mlc_out;
    
    // Initialize serial for output.
    pc.baud(115200);

    // Initlialize components.
    AccGyr.begin();
    AccGyr.Enable_X();
    AccGyr.Enable_G();

    /* Feed the program to Machine Learning Core */
    /* Activity Recognition Default program */
    ProgramPointer = (ucf_line_t *)lsm6dsv16x_activity_recognition_for_mobile;
    TotalNumberOfLine = sizeof(lsm6dsv16x_activity_recognition_for_mobile) / sizeof(ucf_line_t);
    pc.printf("Activity Recognition for LSM6DSV16X MLC\r\n");
    pc.printf("UCF Number Line = %d",TotalNumberOfLine);

    for (LineCounter = 0; LineCounter < TotalNumberOfLine; LineCounter++) {
        if (AccGyr.writeRegister(ProgramPointer[LineCounter].address, &ProgramPointer[LineCounter].data, 1)) {
        pc.printf("Error loading the Program to LSM6DSV16XSensor at line: %d", LineCounter);
        
        while (1) {
            // Led blinking.
            led = 1;
            thread_sleep_for(250);
            led = 0;
            thread_sleep_for(250);
        }
        }
    }

    pc.printf("Program loaded inside the LSM6DSV16X MLC\r\n");

    //Interrupts.
    intPin.rise(&INT1Event_cb);

    /* We need to wait for a time window before having the first MLC status */
    thread_sleep_for(3000);

    AccGyr.Get_MLC_Output(&mlc_out);
    printMLCStatus(mlc_out.mlc1_src);

    while (true) {
        if (mems_event) {
            mems_event = 0;
            lsm6dsv16x_mlc_status_mainpage_t  status;
            AccGyr.Get_MLC_Status(&status);
            if (status.is_mlc1) {
            lsm6dsv16x_mlc_out_t mlc_out;
            AccGyr.Get_MLC_Output(&mlc_out);
            printMLCStatus(mlc_out.mlc1_src);
    }
  }
    }
}

void INT1Event_cb()
{
  mems_event = 1;
}

void printMLCStatus(uint8_t status)
{
  switch (status) {
    case 0:
      pc.printf("Activity: Stationary\r\n");
      break;
    case 1:
      pc.printf("Activity: Walking\r\n");
      break;
    case 4:
      pc.printf("Activity: Jogging\r\n");
      break;
    case 8:
      pc.printf("Activity: Biking\r\n");
      break;
    case 12:
      pc.printf("Activity: Driving\r\n");
      break;
    default:
      pc.printf("Activity: Unknown\r\n");
      break;
  }
}