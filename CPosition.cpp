
// Filename : CPosition.cpp
// Description : implementation of the CPosition class
// Author : Marc P. Bernasconi

#include "stdafx.h"

#ifndef __CPOSITION_H__
#include "CPosition.h"
#endif

#ifndef __CONSTANT_H__
#include "Constant.h"
#endif

 
CPosition::CPosition() :
	m_iVoice( 0 ),
	m_iMeasure( 0 ),
	m_iTick( 0 ),
	m_iAbsoluteTick( 0 ),
	m_iTicksPerMeasure( 0 )
{
}

CPosition::CPosition( const CPosition& rvalue )
{	
	m_iVoice = rvalue.m_iVoice;
	m_iMeasure = rvalue.m_iMeasure;
	m_iTick = rvalue.m_iTick;
	m_iAbsoluteTick = rvalue.m_iAbsoluteTick;

	m_iTicksPerMeasure = rvalue.m_iTicksPerMeasure;
}

void CPosition::Initialize( int iVoice, int iMeasure, int iTick, int iTicksPerMeasure )
{
	m_iVoice = iVoice;
	m_iMeasure = iMeasure;
	m_iTick = iTick;

	m_iAbsoluteTick = m_iMeasure * iTicksPerMeasure + m_iTick;

	m_iTicksPerMeasure = iTicksPerMeasure;
}

void CPosition::Initialize( int iVoice, int iAbsoluteTick, int iTicksPerMeasure )
{
	m_iAbsoluteTick = iAbsoluteTick;
	m_iVoice = iVoice;

	m_iMeasure = m_iAbsoluteTick / iTicksPerMeasure;
	m_iTick = m_iAbsoluteTick % iTicksPerMeasure;

	m_iTicksPerMeasure = iTicksPerMeasure;
}

int CPosition::operator>( const CPosition& rposRValue ) const
{
	return (m_iAbsoluteTick > rposRValue.m_iAbsoluteTick);
}

int CPosition::operator<( const CPosition& rposRValue ) const
{
	return (m_iAbsoluteTick < rposRValue.m_iAbsoluteTick);
}

int CPosition::operator==( const CPosition& rposRValue ) const
{
	return (m_iAbsoluteTick == rposRValue.m_iAbsoluteTick);
}

const CPosition& CPosition::operator=( const CPosition& rposRValue )
{
	m_iAbsoluteTick = rposRValue.m_iAbsoluteTick;
	m_iMeasure = rposRValue.m_iMeasure;
	m_iTick = rposRValue.m_iTick;
	m_iVoice = rposRValue.m_iVoice;

	return *this;
}

void CPosition::SetMeasure( int iMeasure )
{
	m_iMeasure = iMeasure;

	m_iAbsoluteTick = m_iMeasure * m_iTicksPerMeasure + m_iTick;
}

void CPosition::SetTick( int iTick )
{
	m_iTick = iTick;

	m_iAbsoluteTick = m_iMeasure * m_iTicksPerMeasure + m_iTick;
}

void CPosition::SetAbsoluteTick( int iAbsoluteTick )
{
	m_iAbsoluteTick = iAbsoluteTick;

	m_iMeasure = iAbsoluteTick / m_iTicksPerMeasure;
	m_iTick = iAbsoluteTick % m_iTicksPerMeasure;
}



