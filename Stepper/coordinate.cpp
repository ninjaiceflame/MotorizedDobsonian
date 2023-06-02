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
void coordinate::calibrate(twoAxisDeg latLong)
{
	//hourMinuteSeconds RA;
	//degreeMinuteSeconds Dec;

	//degreeMinuteSeconds Lat;
	//degreeMinuteSeconds Long;

	////No input error checking yet
	//cout << "\n\nEnter calibration target RA in HMS:\nHour: ";
	//cin >> RA.hours;
	//cout << "\nMinutes: ";
	//cin >> RA.minutes;
	//cout << "\nSeconds: ";
	//cin >> RA.seconds;

	//cout << "\n\nEnter calibration target Dec in DMS:\nDegrees: ";
	//cin >> Dec.degrees;
	//cout << "\nMinutes: ";
	//cin >> Dec.minutes;
	//cout << "\nSeconds: ";
	//cin >> Dec.seconds;

	/*cout << "\n\nEnter Latitude in DMS:\nDegrees: ";
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
	cin >> Long.seconds;*/

	//Test with north coordinate 6/1/2023
	twoAxisDeg RaDecInput;
	RaDecInput.x = sidereal::hmsToDeg(1, 23, 14.6);
	RaDecInput.y = sidereal::dmsToDeg(50, 14, 23.3);

	//Store the RA/Dec coordinates
	/*currentCelestialPosDeg.x = sidereal::hmsToDeg(RA);
	currentCelestialPosDeg.y = sidereal::dmsToDeg(Dec);*/

	//Store the Lat/Long coordinates
	//currentLatLongDeg.x = sidereal::dmsToDeg(latLong.x);
	//currentLatLongDeg.y = sidereal::dmsToDeg(latLong.y);
	currentLatLongDeg.x = latLong.x;
	currentLatLongDeg.y = latLong.y;
	//Store the Alt/Az coordinates
	currentAltAz = equatorialToLocal(RaDecInput.x, RaDecInput.y, latLong);

	int x = 0;
}

void coordinate::manualControl()
{
	//Keyboard control:
	char key = NULL;
	while (1)
	{
		key = getchar();
		switch (key)
		{
			//Up
		case 'w':
			
			for (int i = 0; i < 1000; i++)
			{
				stepUp();
			}
			break;

			//Down
		case 's':
			for (int i = 0; i < 1000; i++)
			{
				stepDown();
			}
			break;

			//Left
		case 'a':
			for (int i = 0; i < 1000; i++)
			{
				stepLeft();
			}
			break;

			//Right
		case 'd':
			for (int i = 0; i < 1000; i++)
			{
				stepRight();
			}
			break;

		case 'x':
			return;
		}

		//Reset
		key = NULL;
		gpioDelay(_DELAY);
	}

}

void coordinate::gotoCoordsDeg(twoAxisDeg targetRaDec)
{
	//Convert Ra Dec to Alt Az
	twoAxisDeg targetAltAz = equatorialToLocal(targetRaDec.x, targetRaDec.y, currentLatLongDeg);
	
	double xToMove = targetAltAz.x - currentAltAz.x;
	double yToMove = targetAltAz.y - currentAltAz.y;

	double gear_ratio = 100 * 2.5;
	double step_resolution = _STEPS * gear_ratio;

	double step_size = 360 / step_resolution;
	while (1)
	{
		//If the y degrees to move is more than a step size in degrees
		if (abs(yToMove) >= (step_size))
		{
			//0 to 360 degrees, 0 = North, 90 = East, 180 = South, 270 = West
			
			//Step if positive AND not out of bounds
			if (yToMove > 0 && targetAltAz.y < 360)
			{
				coordinate::stepRight();
				currentAltAz.y += step_size;
			}
			//Step if negative AND not out of bounds
			if (yToMove < 0 && targetAltAz.y > 0)
			{
				coordinate::stepLeft();
				currentAltAz.y -= step_size;
			}
		}

		//If the x degrees to move is more than a step size in degrees
		if (abs(xToMove) >= (step_size))
		{
			//0 to 90 degrees, 0 = Horizontal, 90 = Vertical, anything > 90 or < 0 is ignored
			// 
			//Step if positive
			if (xToMove > 0 && targetAltAz.x < 90)
			{
				coordinate::stepUp();
				currentAltAz.x += step_size;

			}
			//Step if negative
			if (xToMove < 0 && targetAltAz.x > 0)
			{
				coordinate::stepDown();
				currentAltAz.x -= step_size;
			}
		}
		//Update target Alt Az
		targetAltAz = coordinate::equatorialToLocal(targetRaDec.x, targetRaDec.y, currentLatLongDeg);
		xToMove = targetAltAz.x - currentAltAz.x;
		yToMove = targetAltAz.y - currentAltAz.y;
	}


	//Step
	//char key = NULL;

	//key = getchar();
	//switch (key)
	//{
	//	//Up
	//case 'w':
	//	gpioWrite(DIR2, PI_HIGH);
	//	for (int i = 0; i < 1000; i++)
	//	{
	//		gpioWrite(PUL2, PI_HIGH);
	//		gpioDelay(_DELAY);
	//		gpioWrite(PUL2, PI_LOW);
	//		gpioDelay(_DELAY);
	//	}
	//	break;

	//	//Down
	//case 's':
	//	gpioWrite(DIR2, PI_LOW);
	//	for (int i = 0; i < 1000; i++)
	//	{
	//		gpioWrite(PUL2, PI_HIGH);
	//		gpioDelay(_DELAY);
	//		gpioWrite(PUL2, PI_LOW);
	//		gpioDelay(_DELAY);
	//	}
	//	break;

	//	//Left
	//case 'a':
	//	gpioWrite(DIR1, PI_HIGH);
	//	for (int i = 0; i < 1000; i++)
	//	{
	//		gpioWrite(PUL1, PI_HIGH);
	//		gpioDelay(_DELAY);
	//		gpioWrite(PUL1, PI_LOW);
	//		gpioDelay(_DELAY);
	//	}
	//	break;

	//	//Right
	//case 'd':
	//	gpioWrite(DIR1, PI_LOW);
	//	for (int i = 0; i < 1000; i++)
	//	{
	//		gpioWrite(PUL1, PI_HIGH);
	//		gpioDelay(_DELAY);
	//		gpioWrite(PUL1, PI_LOW);
	//		gpioDelay(_DELAY);
	//	}
	//	break;
	//}

	////Reset
	//key = NULL;
	//gpioDelay(_DELAY);

}

void coordinate::stepRight()
{	//Set direction
	gpioWrite(DIR1, PI_LOW);
	//Step
	gpioWrite(PUL1, PI_HIGH);
	gpioDelay(_DELAY);
	gpioWrite(PUL1, PI_LOW);
	gpioDelay(_DELAY);
}

void coordinate::stepLeft()
{	//Set direction
	gpioWrite(DIR1, PI_HIGH);
	//Step 
	gpioWrite(PUL1, PI_HIGH);
	gpioDelay(_DELAY);
	gpioWrite(PUL1, PI_LOW);
	gpioDelay(_DELAY);
}

void coordinate::stepUp()
{		
	//Set direction
	gpioWrite(DIR2, PI_LOW);
	//Step
	gpioWrite(PUL2, PI_HIGH);
	gpioDelay(_DELAY);
	gpioWrite(PUL2, PI_LOW);
	gpioDelay(_DELAY);
}

void coordinate::stepDown()
{
	//Set direction
	gpioWrite(DIR2, PI_HIGH);
	//Step
	gpioWrite(PUL2, PI_HIGH);
	gpioDelay(_DELAY);
	gpioWrite(PUL2, PI_LOW);
	gpioDelay(_DELAY);
}
