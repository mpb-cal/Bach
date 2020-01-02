/*
	CMidiNote.cpp
*/

#include "stdafx.h"

#ifndef __CMIDINOTE_H__
#include "CMidiNote.h"
#endif

#ifndef __CONSTANT_H__
#include "Constant.h"
#endif

//musical construction definitions:
static const int major_chord[] = {0,4,3,5};
static const int minor_chord[] = {0,3,4,5};
static const int major_scale[] = {0,2,4,5,7,9,11};
static const int minor_scale[] = {0,2,3,5,7,8,10};
static const int major_pentatonic[] = {0,2,4,7,9};
static const int minor_pentatonic[] = {0,3,5,7,10};

int get_ticks(int note_value)
{
	int ticks;
	if(note_value == 0) return 0;
	if(note_value < 0)
	{
		ticks = 32 / abs(note_value);
		ticks += ticks / 2;
	}
	else ticks = 32 / note_value;
	return ticks;
}



//--------------------------------------------------------------------------
//CMidiNote class:


CMidiNote::CMidiNote( int iPitch, int iLength, int iChannel, int iVelocity )
{
	m_iPitch = iPitch;
	m_iLength = iLength;
	m_iChannel = iChannel;
	m_iVelocity = iVelocity;
}

void CMidiNote::Write( ostream& fout, int& riDelay )
{
	if (m_iPitch == NOTE_REST)
	{
		riDelay += m_iLength;
		return;
	}

	// delay before starting note
	int iDelaySize;
	BYTE a_bDelay[ 2 ];

	if(riDelay < 128)
	{
		iDelaySize = 1;
		a_bDelay[ 0 ] = (BYTE)riDelay;
	}
	else
	{
		iDelaySize = 2;

		BYTE length0 = (BYTE)((riDelay % 0x100) & 0x7f);
		BYTE length1 = (BYTE)((riDelay / 0x100) << 1);
		length1 = (BYTE)( length1 | 0x80 + (((riDelay % 0x100) & 0x80) >> 7) );
		a_bDelay[ 0 ] = length1;
		a_bDelay[ 1 ] = length0;
	}
	fout.write( a_bDelay, iDelaySize );

	riDelay = 0;

	BYTE a_bNoteon[ 3 ] =
	{
		(BYTE)(MN_NOTEON + m_iChannel),
		(BYTE)m_iPitch,
		(BYTE)m_iVelocity
	};
	fout.write( a_bNoteon, 3 );

	// delay before note ends
	if(m_iLength < 128)
	{
		iDelaySize = 1;
		a_bDelay[ 0 ] = (BYTE)m_iLength;
	}
	else
	{
		iDelaySize = 2;

		BYTE length0 = (BYTE)((m_iLength % 0x100) & 0x7f);
		BYTE length1 = (BYTE)((m_iLength / 0x100) << 1);
		length1 = (BYTE)( length1 | 0x80 + (((m_iLength % 0x100) & 0x80) >> 7) );
		a_bDelay[ 0 ] = length1;
		a_bDelay[ 1 ] = length0;
	}
	fout.write( a_bDelay, iDelaySize );

	//stop note:
	BYTE a_bNoteoff[ 3 ] =
	{
		(BYTE)(MN_NOTEOFF + m_iChannel),
		(BYTE)m_iPitch,
		(BYTE)m_iVelocity
	};
	fout.write( a_bNoteoff, 3 );
}
