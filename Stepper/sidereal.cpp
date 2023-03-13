/*************************************************************
* Author:			Nathan Wiley
* Filename:			sidereal.h
* Date Created:		3/12/2023
* Modifications:	
* Purpose:			Calculates local sidereal time
**************************************************************/
#include "sidereal.h"

/**********************************************************************
* Function:			getTime
* Purpose: 			Prints out local time
* Precondition:		none
* Postcondition:	none
************************************************************************/
void sidereal::getTime()
{
	time(&m_rawTime);

	timeInfo = gmtime(&m_rawTime);

	//Outputs GMT Month, Day, Year, and Hour, Minute, Second. These values will be used for calculations in the future
	cout << (timeInfo->tm_mon + 1) << " " << timeInfo->tm_mday << " " << (timeInfo->tm_year + 1900) << " " << timeInfo->tm_hour<< " " << timeInfo->tm_min << " " << timeInfo->tm_sec << endl;

}
