/*************************************************************
* Author:			Nathan Wiley
* Filename:			coordinate.cpp
* Date Created:		3/16/2023
* Modifications:	3/16/2023
* Purpose:			Insert description
**************************************************************/
#include "coordinate.h"

twoAxisDeg coordinate::equatorialToLocalJ2000(double Ra, double Dec, twoAxisDeg myPositionDeg)
{
	//Create struct for Alt/Az and Lat/Long
	twoAxisDeg AltAz;
	twoAxisRads latLong;
	twoAxisRads RaDec;

	//Get Local Mean Sidereal Time using getGMSTinRads and inputing longitude through myPosition (note west needs negative number, east needs positive)
	double LMST = sidereal::getLMST(sidereal::getGMSTinRads(), myPositionDeg.y);

	//Convert and store LMST decimal degrees to Radians
	LMST = LMST*(M_PI / 180.0);

	//Convert and store myPositionDeg to Radians 
	latLong.x = myPositionDeg.x * (M_PI / 180.0);
	latLong.y = myPositionDeg.y * (M_PI / 180.0);

	//Convert and store Ra and Dec from degrees to Radians
	RaDec.x = Ra * (M_PI / 180.0);
	RaDec.y = Dec * (M_PI / 180.0);

	//Get Hour Angle from subtracting Right Ascension (RA) from Meridian (LMST)
	double hourAngle = LMST - Ra;	

	//Quadrant correction math
	if (hourAngle < 0)
	{
		hourAngle += 2 * M_PI;
	}
	if (hourAngle > M_PI)
	{
		hourAngle -= 2 * M_PI;
	}

	//Set Altitude
	AltAz.x = asin( sin(latLong.x) * sin(RaDec.y) + cos(latLong.x) * cos(RaDec.y) * cos(hourAngle));

	//Set Azimuth
	AltAz.y = atan2( sin(hourAngle), cos(hourAngle) * sin(latLong.x) - tan(RaDec.y) * cos(latLong.x));
	
	AltAz.y -= M_PI;

	if (AltAz.y < 0)
	{
		AltAz.y += 2 * M_PI;
	}

	return AltAz;
}