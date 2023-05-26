/*************************************************************
* Author:			Nathan Wiley
* Filename:			coordinate.cpp
* Date Created:		3/16/2023
* Modifications:	3/16/2023
* Purpose:			Insert description
**************************************************************/
#include "coordinate.h"

/**********************************************************************
* Function:			equatorialToLocal
* Purpose: 			Converts Equatorial / Celestial coordinates (RA, Dec) to Local (Alt, Az) coordinates.
* Precondition:		Pass in Right Ascention to RA and Declination to Dec as a doubles containing degrees.
*					Pass in twoAxisDeg holding Latitute and Longitude as degrees
* Postcondition:	An instance of twoAxisDeg is returned containing the Alt/Az coordinates of the target in degrees.
************************************************************************/
twoAxisDeg coordinate::equatorialToLocal(double Ra, double Dec, twoAxisDeg myPositionDeg)
{
	//Create struct for Alt/Az and Lat/Long
	twoAxisDeg AltAz;
	twoAxisRads latLong;
	twoAxisRads RaDec;

	//Convert and store myPositionDeg from degrees to Radians 
	latLong.x = myPositionDeg.x * (M_PI / 180.0);
	latLong.y = myPositionDeg.y * (M_PI / 180.0);

	//Convert and store Ra and Dec from degrees to Radians
	RaDec.x = Ra * (M_PI / 180.0);
	RaDec.y = Dec * (M_PI / 180.0);

	//Get Local Mean Sidereal Time using getGMSTinRads and inputing longitude through myPosition (note west needs negative number, east needs positive)
	double LMST = sidereal::getLMST(sidereal::getGMSTinRads(), myPositionDeg.y);

	//Convert and store LMST decimal degrees to Radians
	LMST = LMST*(M_PI / 180.0);

	//Get Hour Angle from subtracting Right Ascension (RA) from Meridian (LMST)
	double hourAngle = LMST - RaDec.x;

	//Quadrant correction math
	if (hourAngle < 0)
	{
		hourAngle += 2 * M_PI;
	}
	if (hourAngle > M_PI)
	{
		hourAngle -= 2 * M_PI;
	}

	//Set Azimuth
	AltAz.y = (atan2(sin(hourAngle), cos(hourAngle) * sin(latLong.x) - tan(RaDec.y) * cos(latLong.x)));
	//Set Altitude
	AltAz.x = (asin(sin(latLong.x) * sin(RaDec.y) + cos(latLong.x) * cos(RaDec.y) * cos(hourAngle)));

	
	AltAz.y -= M_PI;

	if (AltAz.y < 0)
	{
		AltAz.y += 2 * M_PI;
	}

	//Convert to degrees decimal for output
	AltAz.x = AltAz.x * (180 / M_PI);
	AltAz.y = AltAz.y * (180 / M_PI);

	return AltAz;
}
 

/**********************************************************************
* Function:			calibrate
* Purpose: 			Aligns coordinates when given target
* Precondition:		Telescope must be level and pointed at object using manualControl() to work
* Postcondition:	The Coordinate class will know where the telescope is pointed, and can now point to another target
************************************************************************/
void coordinate::calibrate()
{
	hourMinuteSeconds RA;
	degreeMinuteSeconds Dec;

	degreeMinuteSeconds Lat;
	degreeMinuteSeconds Long;

	//No input error checking yet
	cout << "\n\nEnter calibration target RA in HMS:\nHour: ";
	cin >> RA.hours;
	cout << "\nMinutes: ";
	cin >> RA.minutes;
	cout << "\nSeconds: ";
	cin >> RA.seconds;

	cout << "\n\nEnter calibration target Dec in DMS:\nDegrees: ";
	cin >> Dec.degrees;
	cout << "\nMinutes: ";
	cin >> Dec.minutes;
	cout << "\nSeconds: ";
	cin >> Dec.seconds;

	cout << "\n\nEnter Latitude in DMS:\nDegrees: ";
	cin >> Lat.degrees;
	cout << "\nMinutes: ";
	cin >> Lat.minutes;
	cout << "\nSeconds: ";
	cin >> Lat.seconds;

	cout << "\n\nEnter Longitude in DMS:\nDegrees: ";
	cin >> Long.degrees;
	cout << "\nMinutes: ";
	cin >> Long.minutes;
	cout << "\nSeconds: ";
	cin >> Long.seconds;

	//Store the RA/Dec coordinates
	currentCelestialPosDeg.x = sidereal::hmsToDeg(RA);
	currentCelestialPosDeg.y = sidereal::dmsToDeg(Dec);

	//Store the Lat/Long coordinates
	currentLatLongDeg.x = sidereal::dmsToDeg(Lat);
	currentLatLongDeg.y = sidereal::dmsToDeg(Long);

	//Store the Alt/Az coordinates
	currentLocalPosDeg = equatorialToLocal(currentCelestialPosDeg.x, currentCelestialPosDeg.y, currentLatLongDeg);
}

void coordinate::manualControl()
{

}
