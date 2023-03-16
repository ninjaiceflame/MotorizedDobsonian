/*************************************************************
* Author:			Nathan Wiley
* Filename:			coordinate.h
* Date Created:		3/16/2023
* Modifications:	3/16/2023
* Purpose:			Provide conversion between equatorial Right Ascension / Declination and local Altitude / Azimuth coordinates
**************************************************************/
#pragma once

#include <math.h>		//M_PI
#include <cmath>		//atan2()
#include "sidereal.h"	//degree and hour minute second structs, getLMST()

/************************************************************************
* Struct: 		twoAxisDeg
* Purpose:		Holds data for 2 decimal degree values like latitude and longitude, RA / Dec, or Alt / Az
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
* Purpose:		Holds data for 2 radian values like latitude and longitude, RA / Dec, or Alt / Az
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
* Purpose:		Holds data for 2 degree minute seconds values like latitude and longitude, RA / Dec, or Alt / Az
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
* Purpose:		Provide conversion between equatorial Right Ascension / Declination and local Altitude / Azimuth coordinates
* Data members:	double Alt
*				double Az
* 
* Methods:		myMethods
*************************************************************************/
class coordinate
{
	public:
		static twoAxisDeg equatorialToLocalJ2000(double RA, double Dec, twoAxisDeg myPositionDeg);
};

