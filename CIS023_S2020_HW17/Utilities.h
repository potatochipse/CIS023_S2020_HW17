// Module:		Utilities.h
// Author:		Mark Berrett
// Date:		February 27, 2019
// Purpose:		Template class functions
//				usefull utilities

//				utility					usage
//
//				CompareSize()			value = CompareSize(item, item);
//				TCHARtoInt()			int = TCHARtoInt(TCHAR);
//				InttoTCHAR()			InttoTCHAR(int, TCHAR);
//				TCHARtoDouble()			double = TCHARtoDouble(TCHAR);
//				DoubletoTCHAR()			DoubletoTCHAR(double TCHAR, int);
//				TCHARtoString()			TCHARtoString(TCHAR, &string);
//				StringtoTCHAR()			size_t = StringtoTCHAR(&string, TCHAR);

#pragma once

#define TCHAR_SIZE 128					// default array size for TCHAR's

#include <string>						// need for TCHARtoString() and StringtoTCHAR()
#include <sstream>						// need for StringtoTCHAR()

using namespace std;					// standard namespace


// compares two items of similar basic data type
// returns -1, 0, 1 based on comparison
template <class Type>
Type CompareSize(Type x, Type y)
{
	if (x < y)							// -1 for less
		return -1;
	else if (x == y)					// 0 for equal
		return 0;
	else return 1;						// for greater than
}

// converts TCHAR* to int 
template <class Type>
int TCHARtoInt(Type* szIn)
{
	// attempt to convert to integer
	int iReturn = (int)_tstoi(szIn);	// _tstoi() return UINT, must be cast to int

	// if overflow, return MAXINT
	// if 0, then may be an invalid TCHAR, look for that
	if (lstrlen(szIn) > 0 && iReturn == 0)
		iReturn = MININT;				// MAZINT and MININT are error message

	return iReturn;						// return converted number
}


// converts int to TCHAR
// assumes base 10
template <class Type>
void InttoTCHAR(Type iIn, TCHAR* szIn)
{
	_itot_s(iIn, szIn, TCHAR_SIZE, 10);	// assumes base 10
}

// converts TCHAR to double
template <class Type>
double TCHARtoDouble(Type* szIn)
{
	return _tcstod(szIn, NULL);			// assumes 2nd parameter not used
}

// convert double to TCHAR
// iPrecision determines the number of decimal places
// return point to local TCHAR
template <class Type>
void DoubletoTCHAR(Type dIn, TCHAR* szReturn, int iPrecision)
{
	TCHAR szControl[10];				// convertion (printf) mask
	TCHAR szWork[TCHAR_SIZE];			// completed TCHAR

	_stprintf_s(szControl,				// create mask
		TEXT("%%.%df"),					// mask to spedify precision
		iPrecision);					// mask, iPrecision # of decimal places

	_stprintf_s(szWork,					// convert double to TCHAR
		szControl,						// using mask
		dIn);							// value to convert

	_tcscpy_s(							// copy a TCHAR* to a TCHAR[]
		szReturn,						// destination TCHAR
		TCHAR_SIZE,						// max # of chars to copy
		szWork);						// source TCHAR
}

// converts TCHAr to string
// uses point to string (calling code needs &string)
template <class Type>
void TCHARtoString(TCHAR* szIn, Type* strIn)
{
	strIn->erase();						// clear output string

	if (lstrlen(szIn) > 0)				// must be characters to convert
	{
		wstring temp(szIn);				// convert TCHAR to stream
		string str(temp.begin(),		// convert stream to string
			temp.end());

		strIn->append(str);				// copy (append) temp string to return 
	}
}

// converts string to TCHAR
// modifies TCHAR in place
// returns size_t code from mbstowcs_s()
// max TCHAR_SIZE TCHAR length
template <class Type>
size_t StringtoTCHAR(Type* strIn, TCHAR* szIn)
{
	size_t pReturnValue = 0;			// return code from mbstowcs_s()

	if (strIn->length() == 0 ||			// cannot be an empty string
		strIn->length() > TCHAR_SIZE)	// connot be longer than the TCHAR
	{
		memset(szIn, 0, sizeof(szIn));	// then return empty TCHAR
	}
	else
	{
		mbstowcs_s(						// convert ASCII to Unicode
			&pReturnValue,				// return value (# of chars successfully converted)
			szIn,						// source TCHAR
			TCHAR_SIZE,					// size of TCHAR
			strIn->c_str(),				// input string
			TCHAR_SIZE);				// # of chars to copy
	}
	return pReturnValue;				// return mbstowcs_s() error code
}
