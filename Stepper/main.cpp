/*************************************************************
* Author:			Nathan Wiley
* Filename:			main.cpp
* Date Created:		1/17/2023
* Modifications:	3/16/2023
* Purpose:			Control 2 stepper motors at a time, prints Julian date, ERA, and GMST to console
**************************************************************/
#include <iostream>		//cout, endl
#include <pigpio.h>		//gpio access for Raspberry Pi

#include "sidereal.h"	//Custom class for calculating time and time angles
#include "coordinate.h" //Custom class for calculating coordinates and reference frames
#include <chrono>		//Used for testing
#include <thread>		//Used for testing

using std::cout;
using std::endl;
using std::fixed;

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
#define _GEAR_RATIO 100 * 2.5
#define _STEPRESOLUTION _STEPS * _GEAR_RATIO
#define _STEPSIZE 1/_STEPRESOLUTION
//Controller pins
#define D_BTN 5
#define C_BTN 6
#define B_BTN 13
#define A_BTN 19

int main(void)
{
	int stepsPerRev = _STEPS * _GEAR_RATIO;
	bool myBool = false;
	
	//Initialize GPIO
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
	latitude.seconds = 29.53;

	degreeMinuteSeconds longitude;
	longitude.degrees = 121;
	longitude.minutes = 46;
	longitude.seconds = 54.01;

	//Converting coordinates from degree minute seconds to degrees decimal
	double latitudeDeg = sidereal::dmsToDeg(latitude);
	double longitudeDeg = sidereal::dmsToDeg(longitude);
	
	//Test with moon
	twoAxisDeg RaDecInput;
	RaDecInput.x = sidereal::hmsToDeg(14, 50, 50);
	RaDecInput.y = sidereal::dmsToDeg(-18, 36, 33.9);

	twoAxisDeg latLong;
	latLong.x = latitudeDeg;
	latLong.y = -longitudeDeg;

	twoAxisDeg temp;
	twoAxisDms AltAz;

	coordinate telescope;
	while (1)
	{
		telescope.manualControl();
		telescope.calibrate(latLong);
		telescope.gotoCoordsDeg(RaDecInput);
		//Get local sidereal time using getGMSTinRads() and longitude in degrees
		//double LMST = sidereal::getLMST(sidereal::getGMSTinRads(),-longitudeDeg);
		//cout << "Current Time (UTC/GMT): ";	sidereal::displayHHMMSS(sidereal::getGMT());
		////Status for console
		//cout << endl << endl;
		//cout << "Lat: " << latitudeDeg << " Long: " << longitudeDeg << endl;
		//cout << fixed << sidereal::getJulianDate() << endl;
		//cout << fixed << "GMST = " << sidereal::getGMSTinRads() << " (in Radians)" << endl;
		//cout << "LMST in Deg: " << LMST << endl;

		//cout << "LMST: ";
		//sidereal::displayHHMMSS(sidereal::degToHms(LMST));	
		//cout << "RA Deg input: " << RaDecInput.x << " Dec Deg input: " << RaDecInput.y << endl;
		//cout << "Moon RA:  "; sidereal::displayHHMMSS(sidereal::degToHms(RaDecInput.x));
		//cout << "Moon Dec: "; sidereal::displayDms(sidereal::degToDms(RaDecInput.y));

		//temp = coordinate::equatorialToLocal(RaDecInput.x, RaDecInput.y, latLong);
		//AltAz.x = sidereal::degToDms(temp.x);
		//AltAz.y = sidereal::degToDms(temp.y);
		//cout << "Alt: " << temp.x << " Az: " << temp.y << endl;

		//double ha = LMST - RaDecInput.x;



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

	gpioTerminate();
	return 0;
}

