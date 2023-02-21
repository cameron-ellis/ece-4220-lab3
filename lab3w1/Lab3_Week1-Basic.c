

/*  Important Note: 
 *      1. Before running this thread make sure the "Lab1_Part2_ISR.ko" is
 *  installed as that is going to check the button press event.
 *      2. Compile this file using the command: gcc Lab3_W1.c -lwiringPi -lpthread -o Lab3_W1
 *      3. Run the program as a SUDO user: sudo ./Lab3_W1
 *  
 */

// http://wiringpi.com/reference/core-functions/

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sched.h>
#include <stdint.h>
#include <sys/timerfd.h>
#include <time.h>			
#include <semaphore.h>
#include <wiringPi.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>




int main(void){


    // Step-1
    // ------
    
    // Set-up of GPIO pins using WiringPi library
    wiringPiSetupGpio();
    pinMode(2, OUTPUT);
    pinMode(3, OUTPUT);
    pinMode(4, OUTPUT);
    pinMode(5, OUTPUT);

    pinMode(16, INPUT);
    pullUpDnControl(16, PUD_DOWN);



    // Step-2
    // ------
	// Run a infinite while loop and turn on/off one light at a time.
    // Turn off all LEDs
    digitalWrite(2, LOW);
    digitalWrite(3, LOW);
    digitalWrite(4, LOW);
    digitalWrite(5, LOW);

    while (1)
    {
        // Turn on/off green LED
        digitalWrite(4, HIGH);
        sleep(3);
        digitalWrite(4, LOW);
        //Turn on yellow LED and check for button
        digitalWrite(3, HIGH);
        sleep(3);
        if (digitalRead(16) == 1)
        {
            // If pressed turn off yellow and turn on red for 3 seconds
            digitalWrite(3, LOW);
            digitalWrite(2, HIGH);
            sleep(3);
            digitalWrite(2, LOW);
        }
        else
        {
            digitalWrite(3, LOW);
        }
        
    }
    
	return 0;
}
