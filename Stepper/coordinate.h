/*************************************************************
* Author:			Nathan Wiley
* Filename:			coordinate.h
* Date Created:		3/16/2023
* Modifications:	3/16/2023
* Purpose:			Provide conversion between equatorial Right Ascension / Declination and local Altitude / Azimuth coordinates
**************************************************************/
#pragma once

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
#define _GEAR_RATIO 100 * 2 * 2.5
#define _STEP_RESOLUTION _STEPS * _GEAR_RATIO
#define _STEP_SIZE 1/_STEP_RESOLUTION
//Controller pins
#define D_BTN 5
#define C_BTN 6
#define B_BTN 13
#define A_BTN 19

#include <math.h>		//M_PI
#include <cmath>		//atan2()
#include "sidereal.h"	//degree and hour minute second structs, getLMST()

using std::cin;

/************************************************************************
* Struct: 		twoAxisDeg
* Purpose:		Holds data for 2, decimal degree values like latitude and longitude, RA / Dec, or Alt / Az
* Data members:	x - Holds latitude, RA, or Alt
*				y - Holds longitude, Dec, or Az
*************************************************************************/
typedef struct twoAxisDeg
{
	double x;
	double y;
};

/************************************************************************
* Struct: 		twoAxisRads
* Purpose:		Holds data for 2, radian values like latitude and longitude, RA / Dec, or Alt / Az
* Data members:	x - Holds latitude, RA, or Alt
*				y - Holds longitude, Dec, or Az
*************************************************************************/
typedef struct twoAxisRads
{
	double x;
	double y;
};


/************************************************************************
* Struct: 		twoAxisDms
* Purpose:		Holds data for 2, degree minute seconds values like latitude and longitude, RA / Dec, or Alt / Az
* Data members:	x - Holds latitude, RA, or Alt
*				y - Holds longitude, Dec, or Az
*************************************************************************/
typedef struct twoAxisDms
{
	degreeMinuteSeconds x;
	degreeMinuteSeconds y;
};

/************************************************************************
* Class: 		coordinate
* Purpose:		Provide conversion from equatorial Right Ascension / Declination to local Altitude / Azimuth coordinates
* Data members:	double Alt
*				double Az
* 
* Methods:		myMethods
*************************************************************************/
class coordinate
{
	public:
		static twoAxisDeg equatorialToLocal(double RA, double Dec, twoAxisDeg myPositionDeg);
		static void calibrate();
		static void manualControl();
		static void gotoCoordsDeg(twoAxisDeg targetRaDec);
		static void stepRight();
		static void stepLeft();
		static void stepUp();
		static void stepDown();
	private:
		static twoAxisDeg currentCelestialPosDeg;
		static twoAxisDeg currentLocalPosDeg;
		static twoAxisDeg currentLatLongDeg;
};

