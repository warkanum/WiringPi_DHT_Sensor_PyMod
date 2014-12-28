
#include "dht11_sensor.h"
#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sched.h>

int dht11_dat[5] = { 0, 0, 0, 0, 0 };

void read_dht11_data(int pin, sensor_data * returnData)
{
	uint8_t laststate	= HIGH;
	uint8_t counter		= 0;
	uint8_t j		= 0, i;

	if (returnData == NULL)
		return;

	returnData->valid = 0;

	dht11_dat[0] = dht11_dat[1] = dht11_dat[2] = dht11_dat[3] = dht11_dat[4] = 0;

	if (wiringPiSetup() == -1)
	{
		returnData->valid = 0;
		strcpy(returnData->errmsg, "Failed to setup Wiring PI\0");

		return;
	}

	set_max_priority(); //Ramp up priority

	//pull pin down for 18 milliseconds
	pinMode(pin, OUTPUT);
	digitalWrite(pin, LOW);
	delay( 18 );
	
	//then pull it up for 40 microseconds
	digitalWrite(pin, HIGH);
	delayMicroseconds( 40 );
	
	//prepare to read the pin
	pinMode(pin, INPUT);

	//detect change and read data
	for ( i = 0; i < MAXTIMINGS; i++ )
	{
		counter = 0;
		while (digitalRead(pin) == laststate)
		{
			counter++;
			delayMicroseconds( 1 );
			if ( counter == 255 )
			{
				break;
			}
		}
		laststate = digitalRead(pin);

		if ( counter == 255 )
			break;

		//ignore first 3 transitions
		if ( (i >= 4) && (i % 2 == 0) )
		{
			//shove each bit into the storage bytes
			dht11_dat[j / 8] <<= 1;
			if ( counter > 16 )
				dht11_dat[j / 8] |= 1;
			j++;
		}
	}

	//check we read 40 bits (8bit x 5 ) + verify checksum in the last byte
	if ( (j >= 40) &&
	     (dht11_dat[4] == ( (dht11_dat[0] + dht11_dat[1] + dht11_dat[2] + dht11_dat[3]) & 0xFF) ) )
	{
		returnData->humidity = dht11_dat[0];
		returnData->humidity2 = dht11_dat[1];
		returnData->temperature = dht11_dat[2];
		returnData->temperature2 = dht11_dat[3];
		returnData->valid = 1;
		strcpy(returnData->errmsg, "\0");
	}
	else  
	{
		returnData->valid = 0;
		strcpy(returnData->errmsg, "Checksum verify failed.\0");
	}

	set_default_priority(); //normal priority

}


void set_max_priority(void) 
{
	struct sched_param sched;
	memset(&sched, 0, sizeof(sched));
	// Use FIFO scheduler with highest priority for the lowest chance of the kernel context switching.
	sched.sched_priority = sched_get_priority_max(SCHED_FIFO);
	sched_setscheduler(0, SCHED_FIFO, &sched);
}

void set_default_priority(void) 
{
	struct sched_param sched;
	memset(&sched, 0, sizeof(sched));
	// Go back to default scheduler with default 0 priority.
	sched.sched_priority = 0;
	sched_setscheduler(0, SCHED_OTHER, &sched);
}