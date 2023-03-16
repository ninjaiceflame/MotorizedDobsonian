/*************************************************************
* Author:			Nathan Wiley
* Filename:			sidereal.h
* Date Created:		3/12/2023
* Modifications:	3/15/2023
* Purpose:			Calculates local sidereal time
**************************************************************/
#include "sidereal.h"

/**********************************************************************
* Function:			getGMT
* Purpose: 			Prints out Greenwich Mean Time (GMT)
* Precondition:		none
* Postcondition:	Returns TM struct with GMT Month, Day, Year, and Hour, Minute, Seconds
************************************************************************/
tm sidereal::getGMT()
{
	time_t m_rawTime;
	//Get time
	time(&m_rawTime);

	//Set timeInfo struct equal to return value of gmtime()
	tm *timeInfo = gmtime(&m_rawTime);

	return *timeInfo;
}

void sidereal::displayTmMMDDYYYY(tm timeInfo)
{
	cout << timeInfo.tm_mon << " " << timeInfo.tm_mday << " " << timeInfo.tm_year << endl;
}

void sidereal::displayHHMMSS(hourMinuteSeconds timeInfo)
{
	cout << "H:" << timeInfo.hours << " M:" << timeInfo.minutes << " S:" << timeInfo.seconds << endl;
}

double sidereal::hmsToDeg(hourMinuteSeconds timeInfo)
{
	return (timeInfo.hours / 24) + (timeInfo.minutes / 1440) + (timeInfo.seconds / 86400);
}

hourMinuteSeconds sidereal::degToHms(double deg)
{
	hourMinuteSeconds timeInfo;

	timeInfo.hours = (floor(deg/15));
	deg -= timeInfo.hours*15;

	timeInfo.minutes = floor(deg * 4 );
	deg -= (timeInfo.minutes / 4);

	timeInfo.seconds = (deg * 240);

	return timeInfo;
}

/**********************************************************************
* Function:			getJulianDate
* Purpose: 			insert purpose
* Precondition:		insert precondition
* Postcondition:	insert postcondition
************************************************************************/
double sidereal::getJulianDate()
{
	double julianDate;
	struct tm timeInfo = getGMT();

	//Create a variable for amount of days this year including the fractional current day (current time, noon = .5 of a day), The + 1 on tm_yday is to account for it starting at 0 instead of 1 for Jan 1st.
	double days = (timeInfo.tm_yday + 1)+ (((timeInfo.tm_hour)) / 24.0) + (timeInfo.tm_min / 1440.0) + (timeInfo.tm_sec / 86400.0);

	//Correct for Leap year if applicable
	if (__isleap(timeInfo.tm_year) != 0)
	{
		days = -1;
	}

	//Julian date formula which accounts for leap years, 2451544.5 = Jan 01, 2000 at 00:00:00, The parts where the year is - 100 is to account for tm_Year starting at years since 1900, formula needs years since 2000
	//Formula came from here: https://space.stackexchange.com/questions/13809/time-utc-julian-date-tle-epoch-how-are-they-related-quantitatively and has been checked for accuracy
	julianDate = 2451544.5 + ((365 * (timeInfo.tm_year - 100)) + ((timeInfo.tm_year - 100) / 4) - ((timeInfo.tm_year - 100) / 100) + ((timeInfo.tm_year - 100) / 400) + days);

	return julianDate;
}

/**********************************************************************
* Function:			getERA
* Purpose: 			Calculate the Earth's Rotation Angle (ERA) as opposed to sidereal time
* Precondition:		none
* Postcondition:	double containing the ERA is returned
************************************************************************/
double sidereal::getERA()
{
	double theta = (2 * M_PI * (OFFSET + EARTHS_ROTATIONAL_SPEED * (getJulianDate() - 2451545.0)));

	theta = fmod(theta, 2 * M_PI);
	if (theta < 0)
	{
		theta += 2 * M_PI;
	}

	return theta;
}

double sidereal::getERAcomplex()
{
	double frac = fmod((getJulianDate()) - 2451545.0, 1);
	double theta = fmod((2 * M_PI * (OFFSET + 0.00273781191135448 * (getJulianDate() - 2451545.0) + frac)), (M_PI*2)); //eq 14

	if (theta < 0)
	{
		theta += 2 * M_PI;
	}
	return theta;
}

double sidereal::getGMSTinDEG()
{
	double GMST;
	double j2000 = getJulianDate() - 2451545.0;
	tm timeInfo = getGMT();
	GMST = 100.4606184 + (0.9856473662862 * j2000) + (15 *( timeInfo.tm_hour + (timeInfo.tm_min / 60) + (timeInfo.tm_sec / 3600)));

	GMST = fmod(GMST, 360);

	if (GMST < 0)
	{
		GMST += 360;
	}
	return GMST;
}

double sidereal::getGMSTinRads()
{
	double t = ((getJulianDate()- 2451545.0)/ 36525.0);

	double gmst = getERAcomplex() + (0.014506 + (4612.156534 * t) + (1.3915817 * t * t) - (0.00000044 * t * t * t) - (0.000029956 * t * t * t * t) - (0.0000000368 * t * t * t * t * t)) / 60.0 / 60.0 *( M_PI / 180.0);

	gmst = fmod(gmst,2 * M_PI);

	if (gmst < 0)
	{
		gmst += 2 * M_PI;
	}
	return gmst;
}

/**********************************************************************
* Function:			degToDMS
* Purpose: 			Convert degrees to Degree Minute Seconds
* Precondition:		pass in a double for degree you want to convert
* Postcondition:	degreeMinuteSeconds struct returned with data members assigned
************************************************************************/
degreeMinuteSeconds sidereal::degToDms(double deg)
{
	degreeMinuteSeconds dms;

	dms.degrees = floor(deg);
	deg -= dms.degrees;

	dms.minutes = floor(deg * 60);
	deg -= (dms.minutes / 60);

	dms.seconds = (deg * 3600);

	return dms;
}

/**********************************************************************
* Function:			dmstoDeg
* Purpose: 			Convert Degrees Minute Seconds to a flat degree decimal
* Precondition:		initialized degreeMinuteSeconds struct passed into function Ex: 4 Deg 50' 12" 
* Postcondition:	Double contatining the degrees as a fraction is returned	Ex: 4.836667
************************************************************************/
double sidereal::dmsToDeg(degreeMinuteSeconds dms)
{
	return dms.degrees + (dms.minutes / 60.0) + (dms.seconds / 3600.0);
}

double sidereal::getLMST(double rads, double longitudeWest)
{
	return ((rads*180)/M_PI - longitudeWest);
}
