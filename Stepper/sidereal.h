/*************************************************************
* Author:			Nathan Wiley
* Filename:			sidereal.h
* Date Created:		3/12/2023
* Modifications:	3/12/2023
* Purpose:			Calculates local sidereal time
**************************************************************/
#pragma once
#include <iostream>
#include <cmath>
#include <math.h>
#include <chrono>
#include <time.h>

using namespace std;

/************************************************************************
* Class: 		sidereal
* Purpose:		Use local time and coordinates to calculate local sidereal time
* Data members:	myDataMembers
* Methods:		myMethods
*************************************************************************/
class sidereal
{
	public: 

		time_t m_rawTime;
		struct tm* timeInfo;
		
	void getTime();
};

