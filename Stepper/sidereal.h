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

#include <iostream>
#include <cmath>
#include <math.h>
#include <chrono>
#include <time.h>

using namespace std;
using namespace std::chrono;

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
		//Returns Greenwich Mean Time – GMT Time Zone / Coordinated Universal Time (Standard Time)
		static tm getGMT();

		//Returns local julian date
		static double getJulianDate();

		//Returns Earth rotation angle in radians
		static double getERA();

		//Returns Greenwich Mean Sidereal Time in degrees
		static double getGMSTinDEG();

		//Returns degreeMinuteSeconds struct after converting a degree
		static degreeMinuteSeconds degToDms(double deg);

		//Returns deouble degree after converting a degreeMinuteSecond struct
		static double dmsToDeg(degreeMinuteSeconds dms);

		//Returns a tm struct for current Greenwich Mean Sidereal Time
		static struct degreeMinuteSeconds getGMST();

		//Returns Local Mean Sidereal Time
		static double getLMST();
		 
	private:
};

