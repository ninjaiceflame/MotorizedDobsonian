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
	return (2 * M_PI * (OFFSET + EARTHS_ROTATIONAL_SPEED * (getJulianDate() - 2451545.0)));
}


double sidereal::getGMSTinDEG()
{
	double GMST;
	double j2000 = getJulianDate() - 2451545.0;
	GMST = 100.4606184 + (0.9856473662862 * j2000);

	return GMST;
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

struct degreeMinuteSeconds sidereal::getGMST()
{
	struct degreeMinuteSeconds dms;

	return dms;
}


double sidereal::getLMST()
{
	double LMST;

	return LMST;
}
