
// Filename : CMusicalThing.cpp
// Description : implementation of the CMusicalThing class
// Author : Marc P. Bernasconi

#include "stdafx.h"

#ifndef __CMUSICALTHING_H__
#include "CMusicalThing.h"
#endif

#ifndef __CONSTANT_H__
#include "constant.h"
#endif


CMusicalThing::CMusicalThing() :
	m_iKey( 0 ),
	m_iClef( 0 ),
	m_iNumber( 0 ),
	m_iTicksPerMeasure( 0 ),
	m_iPatch( 0 ),
	m_iChannel( 0 ),
	m_iTempo( 0 ),
	m_iTimeSigN( 0 ),
	m_iTimeSigD( 0 ),
	m_iVelocity( 0 )
{
}

CMusicalThing::~CMusicalThing()
{
}

CMusicalThing::CMusicalThing( const CMusicalThing& rvalue )
{
	*this = rvalue;
}

void CMusicalThing::Dump( char* szString ) const
{
	char sz[ 1024 ];
	sprintf(	sz,
				"   MUSICALTHING:\r\n"
				"   m_iKey = %d\r\n"
				"   m_iClef = %d\r\n"
				"   m_iNumber = %d\r\n"
				"   m_iTicksPerMeasure = %d\r\n"
				"   m_iPatch = %d\r\n"
				"   m_iChannel = %d\r\n"
				"   m_iTempo = %d\r\n"
				"   m_iTimeSigN = %d\r\n"
				"   m_iTimeSigD = %d\r\n",
				m_iKey,
				m_iClef,
				m_iNumber,
				m_iTicksPerMeasure,
				m_iPatch,
				m_iChannel,
				m_iTempo,
				m_iTimeSigN,
				m_iTimeSigD,
				m_iVelocity	);

	lstrcat( szString, sz );
}

BOOL CMusicalThing::Output( FILE* pFile ) const
{
	if (fwrite( &m_iKey, sizeof( m_iKey ), 1, pFile ) < 1 ) return FALSE;
	if (fwrite( &m_iClef, sizeof( m_iClef ), 1, pFile ) < 1 ) return FALSE;
	if (fwrite( &m_iNumber, sizeof( m_iNumber ), 1, pFile ) < 1 ) return FALSE;
	if (fwrite( &m_iTicksPerMeasure, sizeof( m_iTicksPerMeasure ), 1, pFile ) < 1 ) return FALSE;
	if (fwrite( &m_iPatch, sizeof( m_iPatch ), 1, pFile ) < 1 ) return FALSE;
	if (fwrite( &m_iChannel, sizeof( m_iChannel ), 1, pFile ) < 1 ) return FALSE;
	if (fwrite( &m_iTempo, sizeof( m_iTempo ), 1, pFile ) < 1 ) return FALSE;
	if (fwrite( &m_iTimeSigN, sizeof( m_iTimeSigN ), 1, pFile ) < 1 ) return FALSE;
	if (fwrite( &m_iTimeSigD, sizeof( m_iTimeSigD ), 1, pFile ) < 1 ) return FALSE;
	if (fwrite( &m_iVelocity, sizeof( m_iVelocity ), 1, pFile ) < 1 ) return FALSE;

	return TRUE;
}

BOOL CMusicalThing::Input( FILE* pFile )
{
	if (fread( &m_iKey, sizeof( m_iKey ), 1, pFile ) < 1 ) return FALSE;
	if (fread( &m_iClef, sizeof( m_iClef ), 1, pFile ) < 1 ) return FALSE;
	if (fread( &m_iNumber, sizeof( m_iNumber ), 1, pFile ) < 1 ) return FALSE;
	if (fread( &m_iTicksPerMeasure, sizeof( m_iTicksPerMeasure ), 1, pFile ) < 1 ) return FALSE;
	if (fread( &m_iPatch, sizeof( m_iPatch ), 1, pFile ) < 1 ) return FALSE;
	if (fread( &m_iChannel, sizeof( m_iChannel ), 1, pFile ) < 1 ) return FALSE;
	if (fread( &m_iTempo, sizeof( m_iTempo ), 1, pFile ) < 1 ) return FALSE;
	if (fread( &m_iTimeSigN, sizeof( m_iTimeSigN ), 1, pFile ) < 1 ) return FALSE;
	if (fread( &m_iTimeSigD, sizeof( m_iTimeSigD ), 1, pFile ) < 1 ) return FALSE;
	if (fread( &m_iVelocity, sizeof( m_iVelocity), 1, pFile ) < 1 ) return FALSE;

	return TRUE;
}

const CMusicalThing& 
CMusicalThing::operator= ( const CMusicalThing& rvalue )
{
	m_iKey = rvalue.m_iKey;
	m_iClef = rvalue.m_iClef;
	m_iNumber = rvalue.m_iNumber;
	m_iTicksPerMeasure = rvalue.m_iTicksPerMeasure;
	m_iPatch = rvalue.m_iPatch;
	m_iChannel = rvalue.m_iChannel;
	m_iTempo = rvalue.m_iTempo;
	m_iTimeSigN = rvalue.m_iTimeSigN;
	m_iTimeSigD = rvalue.m_iTimeSigD;
	m_iVelocity= rvalue.m_iVelocity;
	
	return *this;
}

BOOL 
CMusicalThing::operator==( const CMusicalThing& rvalue ) const
{
	if (m_iKey == rvalue.m_iKey &&
		m_iClef == rvalue.m_iClef &&
		m_iNumber == rvalue.m_iNumber &&
		m_iTicksPerMeasure == rvalue.m_iTicksPerMeasure &&
		m_iPatch == rvalue.m_iPatch &&
		m_iChannel == rvalue.m_iChannel &&
		m_iTempo == rvalue.m_iTempo &&
		m_iTimeSigN == rvalue.m_iTimeSigN &&
		m_iTimeSigD == rvalue.m_iTimeSigD &&
		m_iVelocity == rvalue.m_iVelocity)
	{
		return TRUE;
	}

	return FALSE;
}

BOOL 
CMusicalThing::operator!=( const CMusicalThing& rvalue ) const
{
	if (m_iKey != rvalue.m_iKey ||
		m_iClef != rvalue.m_iClef ||
		m_iNumber != rvalue.m_iNumber ||
		m_iTicksPerMeasure != rvalue.m_iTicksPerMeasure ||
		m_iPatch != rvalue.m_iPatch ||
		m_iChannel != rvalue.m_iChannel ||
		m_iTempo != rvalue.m_iTempo ||
		m_iTimeSigN != rvalue.m_iTimeSigN ||
		m_iTimeSigD != rvalue.m_iTimeSigD ||
		m_iVelocity!= rvalue.m_iVelocity)
	{
		return TRUE;
	}

	return FALSE;
}

BOOL
CMusicalThing::SetKey( INT nKey )
{
	ASSERT( nKey >= 0 && nKey < LastKeySignature );

	m_iKey = nKey;

	return TRUE;
}

BOOL
CMusicalThing::SetClef( INT nClef )
{
	ASSERT( nClef >= 0 && nClef < LastClef );

	m_iClef = nClef;

	return TRUE;
}

BOOL
CMusicalThing::SetNumber( INT nNumber )
{
	ASSERT( nNumber >= 0 );

	m_iNumber = nNumber;

	return TRUE;
}

BOOL
CMusicalThing::SetTicksPerMeasure( INT nTicksPerMeasure )
{
	ASSERT( nTicksPerMeasure > 0 );

	m_iTicksPerMeasure = nTicksPerMeasure;

	return TRUE;
}

BOOL
CMusicalThing::SetPatch( INT nPatch )
{
	ASSERT( nPatch >= PATCH_MIN && nPatch <= PATCH_MAX );

	m_iPatch = nPatch;

	return TRUE;
}

BOOL
CMusicalThing::SetChannel( INT nChannel )
{
	ASSERT( nChannel >= CHANNEL_MIN && nChannel <= CHANNEL_MAX );

	m_iChannel = nChannel;

	return TRUE;
}

BOOL
CMusicalThing::SetTempo( INT nTempo )
{
	ASSERT( nTempo > 0 );

	m_iTempo = nTempo;

	return TRUE;
}

BOOL
CMusicalThing::SetTimeSigN( INT nTimeSigN )
{
	ASSERT( nTimeSigN > 0 );

	m_iTimeSigN = nTimeSigN;

	return TRUE;
}

BOOL
CMusicalThing::SetTimeSigD( INT nTimeSigD )
{
	ASSERT( nTimeSigD > 0 );

	m_iTimeSigD = nTimeSigD;

	return TRUE;
}

BOOL
CMusicalThing::SetVelocity( INT nVelocity )
{
	ASSERT( nVelocity > 0 );

	m_iVelocity = nVelocity;

	return TRUE;
}


