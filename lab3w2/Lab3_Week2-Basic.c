

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

// Define priority levels
#define GREEN_PRIORITY      (48)
#define YELLOW_PRIORITY     (48)
#define RED_PRIORITY        (49)

// Create global semaphore shared between threads
sem_t led_sem;

void *GLedHandler(void *data){
    // Declare it as a real time task and pass the necessary params to the scheduler 
	struct sched_param param;
    param.sched_priority = GREEN_PRIORITY;
    if (sched_setscheduler(0, SCHED_FIFO, &param) == -1) {
        printf("Run the program as a sudo user\n");
 	    perror("sched_setscheduler failed, thread 1");
    	exit(20);
    }

    while(1)
    {
        sem_wait(&led_sem);
        digitalWrite(4, HIGH);
        sleep(3);
        digitalWrite(4, LOW);
        sem_post(&led_sem);
        usleep(10000);
    }

    pthread_exit(0);
}

void *YLedHandler(void *data){
    // Declare it as a real time task and pass the necessary params to the scheduler 
	struct sched_param param;
    param.sched_priority = YELLOW_PRIORITY;
    if (sched_setscheduler(0, SCHED_FIFO, &param) == -1) {
        printf("Run the program as a sudo user\n");
 	    perror("sched_setscheduler failed, thread 1");
    	exit(20);
    }

    while(1)
    {
        sem_wait(&led_sem);
        digitalWrite(3, HIGH);
        sleep(3);
        digitalWrite(3, LOW);
        sem_post(&led_sem);
        usleep(10000);
    }

    pthread_exit(0);
}

void *RLedHandler(void *data){
    // Declare it as a real time task and pass the necessary params to the scheduler 
	struct sched_param param;
    param.sched_priority = RED_PRIORITY;
    if (sched_setscheduler(0, SCHED_FIFO, &param) == -1) {
        printf("Run the program as a sudo user\n");
 	    perror("sched_setscheduler failed, thread 1");
    	exit(20);
    }

    while (1)
    {
        if (digitalRead(16) == 1)
        {
            sem_wait(&led_sem);
            digitalWrite(3, LOW);
            digitalWrite(4, LOW);
            digitalWrite(2, HIGH);
            sleep(3);
            digitalWrite(2, LOW);
            sem_post(&led_sem);
        }
        usleep(10000);
    }

    pthread_exit(0);
}


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

    // Turn off all LEDs initially
    digitalWrite(2, LOW);
    digitalWrite(3, LOW);
    digitalWrite(4, LOW);
    digitalWrite(5, LOW);

    // Step-2
    // ------

    // Initialize semaphore
    sem_init(&led_sem, 0, 1);

    // Create threads for handling LEDs
    pthread_t gthread, ythread, rthread;
    pthread_create(&gthread, NULL, &GLedHandler, NULL);
    pthread_create(&ythread, NULL, &YLedHandler, NULL);
    pthread_create(&rthread, NULL, &RLedHandler, NULL);

    pthread_join(gthread, NULL);
    pthread_join(ythread, NULL);
    pthread_join(rthread, NULL);

	return 0;
}
