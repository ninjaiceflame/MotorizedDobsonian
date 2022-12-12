

#include <iostream>
#include <thread>
#include <chrono>
#include <pigpio.h>

using namespace std;


#define	LED	26
#define IN1 12 //GPIO 12
#define IN2 16 //GPIO 16
#define IN3 20 //GPIO 20
#define IN4 21 //GPIO 21
#define _DELAY 2000
#define _STEPS 32
#define _GEAR_RATIO 64


int main(void)
{
	int stepsPerRev = _STEPS * _GEAR_RATIO;

	//Initialise GPIO
	gpioInitialise();

	//Set up GPIO for LED
	gpioSetMode(LED, PI_OUTPUT);

	//Set up GPIO pins for stepper motor
	gpioSetMode(IN1, PI_OUTPUT);
	gpioSetMode(IN2, PI_OUTPUT);
	gpioSetMode(IN3, PI_OUTPUT);
	gpioSetMode(IN4, PI_OUTPUT);

	//Start at 0
	gpioWrite(IN1, PI_LOW);
	gpioWrite(IN2, PI_LOW);
	gpioWrite(IN3, PI_LOW);
	gpioWrite(IN4, PI_LOW);

	for (int i = 0; i < stepsPerRev/4; ++i)
	{
		//1
		gpioWrite(IN1, PI_HIGH);
		gpioWrite(IN2, PI_LOW);
		gpioWrite(IN3, PI_LOW);
		gpioWrite(IN4, PI_HIGH);
		std::this_thread::sleep_for(std::chrono::microseconds(_DELAY));

		//2
		gpioWrite(IN1, PI_HIGH);
		gpioWrite(IN2, PI_LOW);
		gpioWrite(IN3, PI_LOW);
		gpioWrite(IN4, PI_LOW);
		std::this_thread::sleep_for(std::chrono::microseconds(_DELAY));

		//3
		gpioWrite(IN1, PI_HIGH);
		gpioWrite(IN2, PI_HIGH);
		gpioWrite(IN3, PI_LOW);
		gpioWrite(IN4, PI_LOW);
		std::this_thread::sleep_for(std::chrono::microseconds(_DELAY));

		//4
		gpioWrite(IN1, PI_LOW);
		gpioWrite(IN2, PI_HIGH);
		gpioWrite(IN3, PI_LOW);
		gpioWrite(IN4, PI_LOW);
		std::this_thread::sleep_for(std::chrono::microseconds(_DELAY));

		//5
		gpioWrite(IN1, PI_LOW);
		gpioWrite(IN2, PI_HIGH);
		gpioWrite(IN3, PI_HIGH);
		gpioWrite(IN4, PI_LOW);
		std::this_thread::sleep_for(std::chrono::microseconds(_DELAY));

		//6
		gpioWrite(IN1, PI_LOW);
		gpioWrite(IN2, PI_LOW);
		gpioWrite(IN3, PI_HIGH);
		gpioWrite(IN4, PI_LOW);
		std::this_thread::sleep_for(std::chrono::microseconds(_DELAY));
		
		//7
		gpioWrite(IN1, PI_LOW);
		gpioWrite(IN2, PI_LOW);
		gpioWrite(IN3, PI_HIGH);
		gpioWrite(IN4, PI_HIGH);
		std::this_thread::sleep_for(std::chrono::microseconds(_DELAY));

		//8
		gpioWrite(IN1, PI_LOW);
		gpioWrite(IN2, PI_LOW);
		gpioWrite(IN3, PI_LOW);
		gpioWrite(IN4, PI_HIGH);
		std::this_thread::sleep_for(std::chrono::microseconds(_DELAY));
	}

	//Blink loop
	while (true)
	{
		cout << "Hello! - code from my pc" << endl;

		gpioWrite(LED, PI_HIGH);  // On
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));

		gpioWrite(LED, PI_LOW);	  // Off
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	}

	gpioTerminate();
	return 0;
}