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

/************************************************************************
* Class: 		sidereal
* Purpose:		Use local time and coordinates to calculate local sidereal time
* Data members:	myDataMembers
* Methods:		myMethods
*************************************************************************/
class sidereal
{
	//Data Members
	public: 

		time_t m_rawTime;
		struct tm* timeInfo;

	private:

	//Methods:
	public:
		//Returns Greenwich Mean Time – GMT Time Zone / Coordinated Universal Time (Standard Time)
		tm getGMT();

		//Returns Earth rotation angle in radians
		double getERA();

		//Returns Greenwich Mean Sidereal Time
		tm getGMST();

		//Returns Local Mean Sidereal Time
		tm getLMST();
	private:
};

