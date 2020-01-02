
// Filename : CBaseObject.h
// Description : interface of the CBaseObject class
// Author : Marc P. Bernasconi

#ifndef __CBASEOBJECT_H__
#define __CBASEOBJECT_H__

// includes
#include <windows.h>
#include <stdio.h>

// constants

// macros

// class declarations

class CBaseObject
{

public:

	// default constructor
			CBaseObject::CBaseObject() {}
	// virtual destructor
	virtual	CBaseObject::~CBaseObject() {}

	virtual void	Dump( char* szString ) const = 0;
		
	// input and output methods
	virtual	BOOL	Output( FILE* pFile ) const = 0;
	virtual	BOOL	Input( FILE* pFile ) = 0;

protected:

private:

};


#endif __BASEOBJECT_H__
