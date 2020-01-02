
// Filename : CTick.cpp
// Description : implementation of the CTick class
// Author : Marc P. Bernasconi

#include "stdafx.h"

#ifndef __CTICK_H__
#include "CTick.h"
#endif

#ifndef __CONSTANT_H__
#include "Constant.h"
#endif

 
CTick::CTick() :
	m_ucPitch( NOTE_REST ),
	m_blAttack( FALSE )
{
}

const CTick& CTick::operator=( const CTick& rValue )
{
	m_ucPitch = rValue.m_ucPitch;
	m_blAttack = rValue.m_blAttack;

	return *this;
}

void CTick::Dump( char* szString ) const
{
	char sz[ 512 ];
	sprintf( sz, "%d/%d ", m_blAttack, m_ucPitch );
	lstrcat( szString, sz );
}

void CTick::SetPitch( unsigned char ucPitch )
{
	m_ucPitch = ucPitch;
}

void CTick::SetAttack( BOOL blAttack )
{
	m_blAttack = blAttack;
}

unsigned char CTick::GetPitch() const
{
	return m_ucPitch;
}

BOOL CTick::GetAttack() const
{
	return m_blAttack;
}

BOOL CTick::Output( FILE* pFile ) const
{
	if (fwrite( &m_ucPitch, sizeof( m_ucPitch ), 1, pFile ) < 1) return FALSE;
	if (fwrite( &m_blAttack, sizeof m_blAttack, 1, pFile ) < 1) return FALSE;

	return TRUE;
}

BOOL CTick::Input( FILE* pFile )
{
	if (fread( &m_ucPitch, sizeof( m_ucPitch ), 1, pFile ) < 1) return FALSE;
	if (fread( &m_blAttack, sizeof m_blAttack, 1, pFile ) < 1) return FALSE;

	return TRUE;
}


