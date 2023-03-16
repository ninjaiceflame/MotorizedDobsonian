/*************************************************************
* Author:			Nathan Wiley
* Filename:			sidereal.h
* Date Created:		3/12/2023
* Modifications:	3/116/2023
* Purpose:			Calculates local sidereal time
**************************************************************/
#pragma once

#include <iostream> //For cout
#include <math.h>	//M_PI, floor()
#include <time.h>	//time(), time_t, tm, and __isleap()

#define EARTHS_ROTATIONAL_SPEED 1.00273781191135448
#define OFFSET 0.7790572732640

using std::cout;
using std::endl;

/************************************************************************
* Struct: 		degreeMinuteSeconds
* Purpose:		Holds data for degree minute seconds format. Ex. 57 Deg,29',32"
* Data members:	degrees	- Holds degrees
*				minutes	- Holds minutes
*				seconds	- Holds seconds
*************************************************************************/
typedef struct degreeMinuteSeconds
{
	double degrees;
	double minutes;
	double seconds;
} degreeMinuteSeconds;

/************************************************************************
* Struct: 		degreeMinuteSeconds
* Purpose:		Holds data for hour minute seconds format. Ex. 23H 32M 14.23S
* Data members:	hours	- Holds hours
*				minutes	- Holds minutes
*				seconds	- Holds seconds
*************************************************************************/
typedef struct hourMinuteSeconds
{
	double hours;
	double minutes;
	double seconds;
} hourMinuteSeconds;

/************************************************************************
* Class: 		sidereal
* Purpose:		Use local time and coordinates to calculate local sidereal time, includes a toolkit of handy functions
* Data members:	none
* Methods:		displayTmMMDDYYYY
*				displayHHMMSS
*				hmsToDeg
*				degToHms
*				degToDms
*				dmsToDeg
				getGMT
*				getLMST
*				getJulianDate
*				getERA
*				getERAcomplex
*				getGMSTinDEG
*************************************************************************/
class sidereal
{
	//Methods:
	public:

		/*************************** Utility Functions ********************************/
		static void displayTmMMDDYYYY(tm timeInfo);
		static void displayHHMMSS(hourMinuteSeconds timeInfo);
		static void displayDms(degreeMinuteSeconds timeInfo);
		static double hmsToDeg(hourMinuteSeconds timeInfo);
		static double hmsToDeg(double hours, double minutes, double seconds);
		static hourMinuteSeconds degToHms(double deg);
		static degreeMinuteSeconds degToDms(double deg); //Returns degreeMinuteSeconds struct after converting a degree
		static double dmsToDeg(degreeMinuteSeconds dms); //Returns deouble degree after converting a degreeMinuteSecond struct
		static double dmsToDeg(double degrees, double minutes, double seconds);

		/************************* Time and Date Getters ******************************/
		//Returns Greenwich Mean Time – GMT Time Zone / Coordinated Universal Time (Standard Time)
		static tm getGMT();

		//Returns Local Mean Sidereal Time - Use getGMSTinRads for best accuracy
		static double getLMST(double rads, double longitudeEast);

		//Returns local julian date
		static double getJulianDate();

		//Sidereal time - Ordered most accurate to least accurate
		static double getGMSTinRads();	//Returns Greenwich Mean Sidereal Time (GMST) in radians. Precisely in tune with apparant local sidereal time calculations
		static double getERA();			//Returns Earth Rotation Angle (ERA) in radians
		static double getERAcomplex();	//Another method of getting Earth's Rotation Angle (ERA) in radians
		static double getGMSTinDEG();	//Returns Greenwich Mean Sidereal Time (GMST) in degrees
};
