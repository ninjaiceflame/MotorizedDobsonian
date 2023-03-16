/*************************************************************
* Author:			Nathan Wiley
* Filename:			sidereal.h
* Date Created:		3/12/2023
* Modifications:	3/16/2023
* Purpose:			Calculates local sidereal time
**************************************************************/
#include "sidereal.h"

/**********************************************************************
* Function:			displayTmMMDDYYYY
* Purpose: 			Provide an easy way to print tm structs day, month, and year to console
* Precondition:		Pass in an initialized tm struct to timeInfo
* Postcondition:	months, days, and years are printed out to console on a separate line
************************************************************************/
void sidereal::displayTmMMDDYYYY(tm timeInfo)
{
	cout << timeInfo.tm_mon << " " << timeInfo.tm_mday << " " << timeInfo.tm_year << endl;
}
/**********************************************************************
* Function:			displayHHMMSS
* Purpose: 			Provide an easy way to print tm structs hour, minute, and second to console
* Precondition:		Pass in an initialized tm struct to timeInfo
* Postcondition:	Hours, minutes, and seconds are printed out to console on a separate line
************************************************************************/
void sidereal::displayHHMMSS(hourMinuteSeconds timeInfo)
{
	cout << "H:" << timeInfo.hours << " M:" << timeInfo.minutes << " S:" << timeInfo.seconds << endl;
}

void sidereal::displayDms(degreeMinuteSeconds timeInfo)
{
	cout << timeInfo.degrees << "Deg " << timeInfo.minutes << "m " << timeInfo.seconds << "s" << endl;
}

/**********************************************************************
* Function:			hmsToDeg
* Purpose: 			Converts a struct of hours, minutes, and seconds to decimal degrees
* Precondition:		Pass in an initialized tm struct to timeInfo
* Postcondition:	Returns values as a single decimal degree
************************************************************************/
double sidereal::hmsToDeg(hourMinuteSeconds timeInfo)
{
	return (timeInfo.hours * 15.0) + (timeInfo.minutes / 4.0)) + (timeInfo.seconds / 240.0);
}

/**********************************************************************
* Function:			hmsToDeg (override
* Purpose: 			Converts hours, minutes, and seconds to decimal degrees
* Precondition:		Pass in hours, minute, and seconds to function
* Postcondition:	Returns a struct of hours, minutes, and seconds
************************************************************************/
double sidereal::hmsToDeg(double hours, double minutes, double seconds)
{
	return (hours * 15.0) + (minutes / 4.0) + (seconds / 240.0);
}

/**********************************************************************
* Function:			degToHms
* Purpose: 			Converts a decimal degree to a struct of hours, minutes, and seconds
* Precondition:		Pass in a valid angle in degrees
* Postcondition:	Returns a struct of hours, minutes, and seconds
************************************************************************/
hourMinuteSeconds sidereal::degToHms(double deg)
{
	hourMinuteSeconds timeInfo;

	timeInfo.hours = (floor(deg / 15));
	deg -= timeInfo.hours * 15;

	timeInfo.minutes = floor(deg * 4);
	deg -= (timeInfo.minutes / 4);

	timeInfo.seconds = (deg * 240);

	return timeInfo;
}

/**********************************************************************
* Function:			degToDMS
* Purpose: 			Convert degrees to Degree Minute Seconds
* Precondition:		pass in a double for degree you want to convert
* Postcondition:	Returns degreeMinuteSeconds struct with data members assigned
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
* Purpose: 			Converts Degrees Minute Seconds to a flat degree decimal
* Precondition:		Initialized degreeMinuteSeconds struct passed into function Ex: 4 Deg 50' 12"
* Postcondition:	Returns double contatining the degrees as a fraction is returned	Ex: 4.836667
************************************************************************/
double sidereal::dmsToDeg(degreeMinuteSeconds dms)
{
	return dms.degrees + (dms.minutes / 60.0) + (dms.seconds / 3600.0);
}

/**********************************************************************
* Function:			dmstoDeg (override)
* Purpose: 			Converts Degrees Minute Seconds to a flat degree decimal
* Precondition:		Pass in degree, minutes, and seconds into function Ex: 4 Deg 50' 12"
* Postcondition:	Returns double contatining the degrees as a fraction is returned	Ex: 4.836667
************************************************************************/
double sidereal::dmsToDeg(double degrees, double minutes, double seconds)
{
	return degrees + (minutes / 60.0) + (seconds / 3600.0);
}

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

/**********************************************************************
* Function:			getLMST
* Purpose: 			Calculate local sidereal time
* Precondition:		Pass in a double containing GMST or ERA in radians, and longitude coordinate in degrees (positive number for East, negative for West)
* Postcondition:	Returns a double containing decimal degrees of Local Mean Sidereal Time
					When using getERA(), Approx 1m 13s behind https://www.localsiderealtime.com/
					When using getERA(), Approx 1m 30s behind stellarium
					When using getGMSTinRads(), precisely in time with https://www.localsiderealtime.com/ using -121.7817 longitude
					When using getGMSTinRads(), precisely in time with stellarium "Apparant Sidereal Time" using -121.7817 longitude
************************************************************************/
double sidereal::getLMST(double rads, double longitudeEast)
{
	double LMST;

	//Correct if negative
	LMST = ((rads * 180) / M_PI + longitudeEast);
	if (LMST < 0)
	{
		LMST += 360.0;
	}

	return LMST;
}

/**********************************************************************
* Function:			getJulianDate
* Purpose: 			Calculates current julian date based on getGMT() and account for leap years
* Precondition:		None
* Postcondition:	Returns a double containing the julian date including fracional days\
* Sources:			https://space.stackexchange.com/questions/13809/time-utc-julian-date-tle-epoch-how-are-they-related-quantitatively 
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
	julianDate = 2451544.5 + ((365 * (timeInfo.tm_year - 100)) + ((timeInfo.tm_year - 100) / 4) - ((timeInfo.tm_year - 100) / 100) + ((timeInfo.tm_year - 100) / 400) + days);

	return julianDate;
}

/**********************************************************************
* Function:			getGMSTinRads
* Purpose: 			Calculate GMST and return it as a double in Radians
* Precondition:		None
* Postcondition:	Returns a double containing GMST in radians - Most accurate with databases for stellarium - speed comparable to getERAcomplex(), (0.000006s)
* Sources:			https://astrogreg.com/convert_ra_dec_to_alt_az.html
					https://www.atnf.csiro.au/iau-comm31/pdf/2009_IAUGA_JD6/JD06_capitaine_wallace.pdf
************************************************************************/
double sidereal::getGMSTinRads()
{
	//Get julian date and convert to j2000, then divide by centuries
	double t = ((getJulianDate() - 2451545.0) / 36525.0);

	//Formula for GMST from IAU 2000 expressed in seconds, and converted to Rads
	double GMST = getERA() + (0.014506 + (4612.156534 * t) + (1.3915817 * t * t) - (0.00000044 * t * t * t) - (0.000029956 * t * t * t * t) - (0.0000000368 * t * t * t * t * t)) / 60.0 / 60.0 * (M_PI / 180.0);

	GMST = fmod(GMST, 2 * M_PI);

	//Correct if negative
	if (GMST < 0)
	{
		GMST += 2 * M_PI;
	}

	return GMST;
}

/**********************************************************************
* Function:			getERA
* Purpose: 			Calculate the Earth's Rotation Angle (ERA) as opposed to sidereal time
* Precondition:		None
* Postcondition:	Returns a double containing the ERA in radians - outputs the same as getERAcomplex() but is twice as fast (0.000003s)
* Sources:			https://www.atnf.csiro.au/iau-comm31/pdf/2009_IAUGA_JD6/JD06_capitaine_wallace.pdf
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

/**********************************************************************
* Function:			getERAcomplex
* Purpose: 			Provide an alternate way to calculate ERA, code taken from source with slight modifications for clairity and C++ convention
* Precondition:		None
* Postcondition:	Returns a double containing the ERA in radians - outputs the same as getERA() but is twice as slow (0.000006s)
* Sources:			https://astrogreg.com/convert_ra_dec_to_alt_az.html
************************************************************************/
double sidereal::getERAcomplex()
{
	double frac = fmod((getJulianDate()) - 2451545.0, 1);
	double theta = fmod((2 * M_PI * (OFFSET + 0.00273781191135448 * (getJulianDate() - 2451545.0) + frac)), (M_PI*2));

	//Correct if negative
	if (theta < 0)
	{
		theta += 2 * M_PI;
	}

	return theta;
}

/**********************************************************************
* Function:			getGMSTinDEG - Do not use!
* Purpose: 			Provide an alternate way to calculate GMST (for some reason this method was much less accurate)
* Precondition:		None
* Postcondition:	Returns a double containing GMST in decimal degrees
************************************************************************/
double sidereal::getGMSTinDEG()
{
	double GMST;
	double j2000 = getJulianDate() - 2451545.0;
	tm timeInfo = getGMT();
	GMST = 100.4606184 + (0.9856473662862 * j2000) + (15 *( timeInfo.tm_hour + (timeInfo.tm_min / 60) + (timeInfo.tm_sec / 3600)));

	GMST = fmod(GMST, 360);

	//Correct if negative
	if (GMST < 0)
	{
		GMST += 360;
	}

	return GMST;
}
