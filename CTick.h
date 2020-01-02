
// Filename : CTick.h
// Description : interface of the CTick class
// Author : Marc P. Bernasconi

#ifndef __CTICK_H__
#define __CTICK_H__

// includes
#ifndef __CBASEOBJECT_H__
#include "CBaseObject.h"
#endif

// constants

// macros

// class declarations

class CTick : public CBaseObject
{
public:
					CTick();

	const CTick&	operator=( const CTick& rValue );

	void			Dump( char* szString ) const;

	void			SetPitch( unsigned char ucPitch );
	void			SetAttack( BOOL blAttack );

	unsigned char	GetPitch() const;
	BOOL			GetAttack() const;

	// input and output methods
	BOOL	Output( FILE* pFile ) const;
	BOOL	Input( FILE* pFile );

protected:

	unsigned char	m_ucPitch;
	BOOL			m_blAttack;

private:

};


#endif __TICK_H__
