#ifndef __H_dht11_Sensor__
#define __H_dht11_Sensor__


#define MAXTIMINGS	85

typedef struct sensor_data {
	unsigned short valid;
	int humidity;
	int humidity2;
	int temperature;
	int temperature2;
	char errmsg[300];
} sensor_data;

// Setup Wiring PI m
int setupSensor();

// Read data from sensor on specified pin.
void read_dht11_data(int pin, sensor_data * return_data);

// Increase scheduling priority and algorithm to try to get 'real time' results.
void set_max_priority(void);

// Drop scheduling priority back to normal/default.
void set_default_priority(void);


#endif 
