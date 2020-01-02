
// Filename : CMusicalThing.h
// Description : interface of the CMusicalThing class
// Author : Marc P. Bernasconi

#ifndef __CMUSICALTHING_H__
#define __CMUSICALTHING_H__

// includes
#ifndef __CBASEOBJECT_H__
#include "CBaseObject.h"
#endif


// constants

// macros

// class declarations

class CMusicalThing : public CBaseObject
{

public:

	// default constructor
			CMusicalThing::CMusicalThing();
	// virtual destructor
	virtual	CMusicalThing::~CMusicalThing();
	
	// copy constructor
			CMusicalThing( const CMusicalThing& rvalue );

	// assignment operator
	virtual const CMusicalThing&	operator= ( const CMusicalThing& rvalue );
	// comparison operators
	virtual BOOL					operator==( const CMusicalThing& rvalue ) const;
	virtual BOOL					operator!=( const CMusicalThing& rvalue ) const;

	virtual void	Dump( char* szString ) const;

	// input and output methods
	virtual	BOOL	Output( FILE* pFile ) const;
	virtual	BOOL	Input( FILE* pFile );

	// "Get" methods
	INT		GetKey()				const { return m_iKey; }
	INT		GetClef()				const { return m_iClef; }
	INT		GetNumber()				const { return m_iNumber; }
	INT		GetTicksPerMeasure()	const { return m_iTicksPerMeasure; }
	INT		GetPatch()				const { return m_iPatch; }
	INT		GetChannel()			const { return m_iChannel; }
	INT		GetTempo()				const { return m_iTempo; }
	INT		GetTimeSigN()			const { return m_iTimeSigN; }
	INT		GetTimeSigD()			const { return m_iTimeSigD; }
	INT		GetVelocity()			const { return m_iVelocity; }

	// virtual "Set" methods
	virtual	BOOL	SetKey( INT nKey );
	virtual	BOOL	SetClef( INT nClef );
	virtual	BOOL	SetNumber( INT nNumber );
	virtual	BOOL	SetTicksPerMeasure( INT nTicksPerMeasure );
	virtual	BOOL	SetPatch( INT nPatch );
	virtual	BOOL	SetChannel( INT nChannel );
	virtual	BOOL	SetTempo( INT nTempo );
	virtual	BOOL	SetTimeSigN( INT nTimeSigN );
	virtual	BOOL	SetTimeSigD( INT nTimeSigD );
	virtual	BOOL	SetVelocity( INT nVelocity );

protected:

	INT		m_iKey;
	INT		m_iClef;
	INT		m_iNumber;
	INT		m_iTicksPerMeasure;
	INT		m_iPatch;
	INT		m_iChannel;
	INT		m_iTempo;
	INT		m_iTimeSigN;
	INT		m_iTimeSigD;
	INT		m_iVelocity;

private:

};


#endif __MUSICALTHING_H__
