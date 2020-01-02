
// Filename : cchord.h
// Description : interface of the CChord class
// Author : Marc P. Bernasconi

#ifndef __CCHORD_H__
#define __CCHORD_H__

// includes

#ifndef __CNOTE_H__
#include "CNote.h"
#endif

// constants

// macros

// class declarations

class CChord
{

public:

	// ctors for chords with 0 - 12 notes
	CChord();
	CChord( int p1 );
	CChord( int p1, int p2 );
	CChord( int p1, int p2, int p3 );
	CChord( int p1, int p2, int p3, int p4 );
	CChord( int p1, int p2, int p3, int p4, int p5 );
	CChord( int p1, int p2, int p3, int p4, int p5, int p6 );
	CChord( int p1, int p2, int p3, int p4, int p5, int p6, int p7 );
	CChord( int p1, int p2, int p3, int p4, int p5, int p6, int p7, int p8 );
	CChord( int p1, int p2, int p3, int p4, int p5, int p6, int p7, int p8, int p9 );
	CChord( int p1, int p2, int p3, int p4, int p5, int p6, int p7, int p8, int p9, int p10 );
	CChord( int p1, int p2, int p3, int p4, int p5, int p6, int p7, int p8, int p9, int p10, int p11 );
	CChord( int p1, int p2, int p3, int p4, int p5, int p6, int p7, int p8, int p9, int p10, int p11, int p12 );

	~CChord();

	void		SetPitches( int iPitches, int* pPitches );
	const char*	GetMacro();
	int			GetPitch( int iNumber );
	int			GetNotes() { return m_iNotes; }
	int			GetTicks() { return m_iTicks; }
	void		SetTicks( int iTicks ) { m_iTicks = iTicks; }
	void		StartMelody();
	int			GetMelodyPitch();

protected:

	CNote*	m_pNotes;
	int		m_iNotes;
	int		m_iTicks;
	char	m_szMacro[ 256 ];
	int		m_iMelodyNote;

	void	FreeMemory();
	void	CreateMacro();

private:

};


#endif __CHORD_H__
