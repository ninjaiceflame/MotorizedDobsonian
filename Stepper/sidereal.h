/*************************************************************
* Author:			Nathan Wiley
* Filename:			sidereal.h
* Date Created:		3/12/2023
* Modifications:	3/12/2023
* Purpose:			Calculates local sidereal time
**************************************************************/
#pragma once

#define EARTHS_ROTATIONAL_SPEED 1.00273781191135448
#define OFFSET 0.7790572732640

#include <iostream> //For cout
//#include <cmath>
#include <math.h>	//M_PI, floor()
//#include <chrono>
#include <time.h>	//time(), time_t, tm, and __isleap()

using namespace std;
//using namespace std::chrono;

/************************************************************************
* Struct: 		degreeMinuteSeconds
* Purpose:		Holds data for degree minute seconds format. Ex 57 Deg,29',32"
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
* Methods:		getGMT
*				getJulianDate
*				getERA
*				getGMSTinDEG
*				degToDms
*				dmsToDeg
*				getGMST
*				getLMST
*************************************************************************/
class sidereal
{
	//Methods:
	public:

		/***************************Utility Functions********************************/
		static void displayTmMMDDYYYY(tm timeInfo);
		static void displayHHMMSS(hourMinuteSeconds timeInfo);
		static double hmsToDeg(hourMinuteSeconds timeInfo);
		static hourMinuteSeconds degToHms(double deg);
		static degreeMinuteSeconds degToDms(double deg); //Returns degreeMinuteSeconds struct after converting a degree
		static double dmsToDeg(degreeMinuteSeconds dms); //Returns deouble degree after converting a degreeMinuteSecond struct

		/*************************Time and Date Getters******************************/
		//Returns Greenwich Mean Time – GMT Time Zone / Coordinated Universal Time (Standard Time)
		static tm getGMT();

		//Returns local julian date
		static double getJulianDate();


		//Sidereal time Ordered most accurate to least accurate
		static double getERA();			//Returns Earth Rotation Angle (ERA) in radians
		static double getERAcomplex();	//Another method of getting Earth's Rotation Angle (ERA) in radians
		static double getGMSTinRads();	//Returns Greenwich Mean Sidereal Time (GMST) in radians
		static double getGMSTinDEG();	//Returns Greenwich Mean Sidereal Time (GMST) in degrees

		//Returns Local Mean Sidereal Time
		static double getLMST(double rads, double longitudeWest);
		 
	private:
};

