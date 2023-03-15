/*************************************************************
* Author:			Nathan Wiley
* Filename:			main.cpp
* Date Created:		1/17/2023
* Modifications:	3/15/2023
* Purpose:			Control 2 stepper motors at a time, prints Julian date, ERA, and GMST to console
**************************************************************/
#include <iostream>
#include <thread>
#include <chrono>
#include <pigpio.h>
#include <stdio.h>
#include <pthread.h>
#include <thread>
#include <cmath>

#include "sidereal.h"

using std::cout;
using std::endl;

//Stepper motor 1 - Horizontal
#define	ENA1 2
#define DIR1 3
#define PUL1 4

//Stepper motor 2 - Vertical
#define ENA2 17
#define DIR2 27
#define PUL2 22

//#define _DELAY 64
#define _DELAY 100
#define _STEPS 1600
#define _GEAR_RATIO 100 * 2

//Controller pins
#define D_BTN 5
#define C_BTN 6
#define B_BTN 13
#define A_BTN 19

int main(void)
{
	int stepsPerRev = _STEPS * _GEAR_RATIO;
	// For back and forth loop: bool myBool = false;
	
	//Initialise GPIO
	gpioInitialise();

	//Set up GPIO pins for stepper motor
	gpioSetMode(ENA1, PI_OUTPUT);
	gpioSetMode(DIR1, PI_OUTPUT);
	gpioSetMode(PUL1, PI_OUTPUT);

	//Set up GPIO pins for stepper motor
	gpioSetMode(ENA2, PI_OUTPUT);
	gpioSetMode(DIR2, PI_OUTPUT);
	gpioSetMode(PUL2, PI_OUTPUT);

	//Set Controller pins to input
	gpioSetMode(D_BTN, PI_INPUT);
	gpioSetMode(C_BTN, PI_INPUT);
	gpioSetMode(B_BTN, PI_INPUT);
	gpioSetMode(A_BTN, PI_INPUT);

	//Start at 0
	gpioWrite(ENA1, PI_LOW);
	gpioWrite(DIR1, PI_LOW);
	gpioWrite(PUL1, PI_LOW);

	//Start at 0
	gpioWrite(ENA2, PI_LOW);
	gpioWrite(DIR2, PI_LOW);
	gpioWrite(PUL2, PI_LOW);


	//Custom coordinates 
	degreeMinuteSeconds latitude;
	latitude.degrees = 42;
	latitude.minutes = 13;
	latitude.seconds = 29.7048;

	degreeMinuteSeconds longitude;
	longitude.degrees = 121;
	longitude.minutes = 46;
	longitude.seconds = 51.4272;

	double latitudeDeg = sidereal::dmsToDeg(latitude);
	double longitudeDeg = sidereal::dmsToDeg(longitude);


	while (1)
	{
		sidereal::getGMT();
		cout << "Lat: " << latitudeDeg << " Long: " << longitudeDeg << endl;
		cout << fixed << sidereal::getJulianDate() << endl;
		cout << fixed << "ERA  = " << sidereal::getERA() << endl;
		cout << fixed << "GMST = " << sidereal::getGMSTinDEG() << endl;

		////				Begin Back and forth loop				//
		//for (int i = 0; i < 16000; i++)
		//{
		//	cout << "stepping" << endl;
		//	gpioWrite(PUL1, PI_HIGH);
		//	gpioWrite(PUL2, PI_HIGH);
		//	gpioDelay(_DELAY);
		//	gpioWrite(PUL1, PI_LOW);
		//	gpioWrite(PUL2, PI_LOW);
		//	gpioDelay(_DELAY);
		//}
		//
		////Switch Directions
		//if (myBool == false)
		//{
		//	
		//	gpioWrite(DIR1, PI_HIGH);
		//	gpioWrite(DIR2, PI_HIGH);
		//}
		//else
		//{
		//	gpioWrite(DIR1, PI_LOW);
		//	gpioWrite(DIR2, PI_LOW);
		//}
		//cout << "switching directions = " << myBool << endl;
		//myBool = !myBool;
		////				End Back and Forth Loop					//

	

		//				Begin Keyboard Directional Code				//
		//Keyboard control:
		char key = NULL;

		key = getchar();
		switch (key)
		{
			//Up
			case 'w':
				gpioWrite(DIR2, PI_HIGH);
				for (int i = 0; i < 1000; i++)
				{
					gpioWrite(PUL2, PI_HIGH);
					gpioDelay(_DELAY);
					gpioWrite(PUL2, PI_LOW);
					gpioDelay(_DELAY);
				}
				break;
					
			//Down
			case 's':
				gpioWrite(DIR2, PI_LOW);
				for (int i = 0; i < 1000; i++)
				{
					gpioWrite(PUL2, PI_HIGH);
					gpioDelay(_DELAY);
					gpioWrite(PUL2, PI_LOW);
					gpioDelay(_DELAY);
				}
				break;

			//Left
			case 'a':
				gpioWrite(DIR1, PI_HIGH);
				for (int i = 0; i < 1000; i++)
				{
					gpioWrite(PUL1, PI_HIGH);
					gpioDelay(_DELAY);
					gpioWrite(PUL1, PI_LOW);
					gpioDelay(_DELAY);
				}
				break;

			//Right
			case 'd':
				gpioWrite(DIR1, PI_LOW);
				for (int i = 0; i < 1000; i++)
				{
					gpioWrite(PUL1, PI_HIGH);
					gpioDelay(_DELAY);
					gpioWrite(PUL1, PI_LOW);
					gpioDelay(_DELAY);
				}
				break;
		}

		//Reset
		key = NULL;
		gpioDelay(_DELAY);

		////			End Keyboard Directional Code				//

		//				Begin Controller Code						//

		//If Up or Down:
		//
		//if (!gpioRead(A_BTN))
		//{
		//	//Set Direction
		//	gpioWrite(DIR2, PI_HIGH);
		//	gpioDelay(_DELAY);

		//	//Pulse
		//	gpioWrite(PUL2, PI_HIGH);
		//	gpioDelay(_DELAY);
		//	gpioWrite(PUL2, PI_LOW);
		//	gpioDelay(_DELAY);
		//}
		//else if (!gpioRead(C_BTN))
		//{
		//	//Set Direction
		//	gpioWrite(DIR2, PI_LOW);
		//	gpioDelay(_DELAY);

		//	//Pulse
		//	gpioWrite(PUL2, PI_HIGH);
		//	gpioDelay(_DELAY);
		//	gpioWrite(PUL2, PI_LOW);
		//	gpioDelay(_DELAY);

		//}

		////If Left or Right
		//if (!gpioRead(D_BTN))
		//{
		//	//Set Direction Left
		//	gpioWrite(DIR1, PI_HIGH);
		//	gpioDelay(_DELAY);

		//	//Pulse
		//	gpioWrite(PUL1, PI_HIGH);
		//	gpioDelay(_DELAY);
		//	gpioWrite(PUL1, PI_LOW);
		//	gpioDelay(_DELAY);
		//}
		//else if (!gpioRead(B_BTN))
		//{
		//	//Set Direction Right
		//	gpioWrite(DIR1, PI_LOW);
		//	gpioDelay(_DELAY);

		//	gpioWrite(PUL1, PI_HIGH);
		//	gpioDelay(_DELAY);
		//	gpioWrite(PUL1, PI_LOW);
		//	gpioDelay(_DELAY);
		//}
		//				End Controller Code							//
	}

	//Code for calibration
	//bool calibrated = false;
	//bool horizontally_calibrated = false;
	//bool vertically_calibrated = false;

	//while(!calibrated)
	//{ 
	//	//Subtract horizontal
	//	if (!horizontally_calibrated)
	//	{
	//		//gpioWrite(PUL1, PI_HIGH);
	//		//gpioDelay(_DELAY);
	//	}

	//	//Subtract vertical
	//	if (!vertically_calibrated)
	//	{

	//	}
	//}


	//Psuedocode for controller demo

	gpioTerminate();
	return 0;
}

