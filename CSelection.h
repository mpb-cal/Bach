
// Filename : CSelection.h
// Description : interface of the CSelection class
// Author : Marc P. Bernasconi

#ifndef __CSELECTION_H__
#define __CSELECTION_H__

// includes

#ifndef __CPOSITION_H__
#include "CPosition.h"
#endif

#include "CNote.h"

// constants

// macros

// class declarations

class CCursor;
class CTick;
class CSong;

class CSelection
{

friend class CCursor;

public:

				CSelection();
				~CSelection();
	BOOL		IsEnabled() const	{ return m_blEnabled; }
	BOOL		HasData() const		{ return (m_iTicks > 0); }

	void		Enable(	int iVoice, 
						int iMeasure, 
						int iTick, 
						int iX, 
						int iFirstY, 
						int iCurrentY,
						int iTicksPerMeasure	);
	void		Disable();
	void		SetX( int iVoice, int iMeasure, int iTick, int iXpos, int iTicksPerMeasure );
	void		SetY( int iVoice, int iMeasure, int iTick, int iYpos, int iTicksPerMeasure );
	void		LoadTicks( CSong* pSong );
	void		PutTicks( CSong* pSong, CPosition posFrom, int iTicksPerMeasure, int iKey );
	int			GetNumberTicks() const	{ return m_iTicks; }
	int			GetNumberVoices() const	{ return m_iVoices; }
	CPosition	GetStartPos() const		{ return m_posStart; }
	CPosition	GetStopPos() const		{ return m_posStop; }
	void		GetTicks( char* pcBuffer );
	void		SetTicks( char* pcBuffer );
	void		Fill( int iPitch, int iTicks );
	void		TransposeNotes( CSong* pSong, int iHalfSteps, ENoteDirection Direction );
	void		TransposeDiatonic( CSong* pSong, int iPitches, ENoteDirection Direction );
	BOOL		DrawNowX( HDC hdc ) const;
	BOOL		DrawNowY( HDC hdc ) const;
	BOOL		Draw( HDC hdc ) const;
	void		FreeTicks();

protected:
	
	int			m_iTicks;
	int			m_iVoices;
	CTick**		m_ppTicks;
	CPosition	m_posStart;
	CPosition	m_posStop;
	BOOL		m_blEnabled;

	int			m_iCurrentX;
	int			m_iPreviousX;
	int			m_iFirstX;

	int			m_iCurrentY;
	int			m_iPreviousY;
	int			m_iFirstY;

private:

};



#endif __SELECTION_H__
