
// Filename : CPosition.h
// Description : interface of the CPosition class
// Author : Marc P. Bernasconi

#ifndef __CPOSITION_H__
#define __CPOSITION_H__

// includes


// constants

// macros

// class declarations

class CCursor;


class CPosition
{

public:
			CPosition();
			CPosition( const CPosition& rvalue );
	void	Melody( int iMeasures );
	void	Harmonize( const char* szGranularity, int iVoice, int iMeasures );
 
	void	Initialize( int iVoice, int iMeasure, int iTick, int iTicksPerMeasure );
	void	Initialize( int iVoice, int iAbsoluteTick, int iTicksPerMeasure );

	int 				operator>( const CPosition& rposRValue ) const;
	int 				operator<( const CPosition& rposRValue ) const;
	int 				operator==( const CPosition& rposRValue ) const;
	const CPosition&	operator=( const CPosition& rposRValue );
	
	int		GetVoice() const { return m_iVoice; }
	int		GetMeasure() const { return m_iMeasure; }
	int		GetTick() const { return m_iTick; }
	int		GetAbsoluteTick() const { return m_iAbsoluteTick; }

	void	SetVoice( int iVoice ) { m_iVoice = iVoice; }
	void	SetMeasure( int iMeasure );
	void	SetTick( int iTick );
	void	SetAbsoluteTick( int iAbsoluteTick );

protected:

	int		m_iVoice;
	int		m_iMeasure;
	int		m_iTick;
	int		m_iAbsoluteTick;

	int		m_iTicksPerMeasure;

private:

};


#endif __POSITION_H__


