
// Filename : ccursor.h
// Description : interface of the CCursor class
// Author : Marc P. Bernasconi

#ifndef __CCURSOR_H__
#define __CCURSOR_H__

// includes

#ifndef __CSELECTION_H__
#include "CSelection.h"
#endif

// constants

// enums

enum
{
	MOVE_NONE,
	MOVE_RIGHT,
	MOVE_RIGHTCTL,
	MOVE_LEFT,
	MOVE_LEFTCTL,
	MOVE_UPSHORT,
	MOVE_UPLONG,
	MOVE_DOWNSHORT,
	MOVE_DOWNLONG,
	MOVE_HOMESHORT,
	MOVE_HOMELONG,
	MOVE_ENDSHORT,
	MOVE_ENDLONG,
	MOVE_VOICEUPSHORT,
	MOVE_VOICEUPLONG,
	MOVE_VOICEDOWNSHORT,
	MOVE_VOICEDOWNLONG
};

enum
{
	COLOR_RED = RGB( 255, 0, 0 ),
	COLOR_GREEN = RGB( 0, 255, 0 ),
	COLOR_BLUE = RGB( 0, 0, 255 )
};

// macros

// forward declarations

class CNote;


// class declarations

class CCursor
{

friend class CApplication;

public:
				CCursor();
				~CCursor();

	int 		AddNote( BOOL blRest, BOOL blSharp );
	int			AddNote2( int iAbsolutePitch, int iTicks );
	int			AddChord( BOOL blRest, BOOL blSharp, int iMode );
	void		AddMeasure();
	int			OnKeyDown( unsigned int uiChar, HDC hdc );

	int			GetCaretBitmapId() const;
	int			GetKey() const;
	int			GetMeasure() const { return m_iMeasure; }
	int			GetVoice() const { return m_iVoice; }
	int			GetTick() const { return m_iTick; }
	int			GetAbsoluteTick() const { return m_iAbsoluteTick; }
	int			GetVoicesInChord() const { return m_iVoicesInChord; }
	int			GetMove() const { return m_iMove; }
	int			GetMoveCtl() const { return m_iMoveCtl; }
	int			GetTicksPerMeasure() const { return m_iTicksPerMeasure; }
	int			GetXpos() const;
	int			GetYpos() const;
	int			GetXOrigin() const { return m_iXOrigin; };
	int			GetYOrigin() const { return m_iYOrigin; };
	void		GetMode( char* szMode ) const;
	int			GetPan( int iChannel ) const { return m_iPan[ iChannel - 1 ]; };

	void		SetXOrigin( int iXOrigin );
	void		SetYOrigin( int iYOrigin );
	void		SetKey( int iKey );
	void		SetTicksPerMeasure( int iTicksPerMeasure );

	void		Reset();
	void		ResetPitch();

	int			Move( int iDirection, BOOL blShift, HDC hdc );
	void		MoveToAbsoluteTick( int iAbsoluteTick );
	void		MoveToDefaultPitch( int iClef );
	void		MoveToVoice( int iVoice );
	void		MoveToMeasure( int iMeasure );

	BOOL		ComposeMacro( HINSTANCE hInstance, HWND hwndParent );
	BOOL		ComposeDrumPart( HINSTANCE hInstance, HWND hwndParent );
	BOOL		ComposeClickTrack( HINSTANCE hInstance, HWND hwndParent );
	void		DrumPart(	int iKickVoice, 
							int iSnareVoice, 
							int iHiHatVoice, 
							int iTom1Voice, 
							int iTom2Voice, 
							int iMeasures, 
							int iStyle, 
							int iStartMeasure	);

	BOOL		ComposeGuitarChord( HINSTANCE hInstance, HWND hwndParent );
	BOOL		GuitarChord( int iTicks, int iChord, BOOL blChord, BOOL blBassNote, BOOL blMelody );
	BOOL		ComposeChordProgression( HINSTANCE hInstance, HWND hwndParent );
	BOOL		ComposeMelody( HINSTANCE hInstance, HWND hwndParent );
	BOOL		ComposeHarmonize( HINSTANCE hInstance, HWND hwndParent );
	void		ComposeSong( BOOL blCreateMelody );
	void		ComposeSongAABA();
	void		ComposeAddRestsToChords( int iPatternLength );
	void		ComposeSection( int iRepetitions, int* pnStartMeasures, BOOL blCreateMelody );
	void		ComposeSectionAABA( int iRepetitions, int* pnStartMeasures );
	void		ComposeSetEntryPoint( int iVoice );
	void		Melody( int iMeasures );
	void		Harmonize( const char* szGranularity, int iVoice, int iMeasures );

	void		OnPercussion();

	void		OnEditCopy();
	void		OnEditCut();
	void		OnEditPaste();
	void		OnDestroyClipboard();

	BOOL		TransposeNotes( HINSTANCE hInstance, HWND hwndParent );
	BOOL		TransposeDiatonic( HINSTANCE hInstance, HWND hwndParent );

	void		HiliteVoice( HDC hdc, int iNewVoice, int iOldVoice );
	void		HiliteVoice( HDC hdc );

	void		InsertionModeCut();
	void		InsertionModeCopy();
	void		InsertionModePaste( CPosition& rposFrom );

	void		ChopMeasure();
	void		ChopEmptyMeasures();

	BOOL		OnSettings( HINSTANCE hInstance, HWND hwndParent );
	BOOL		OnSettingsChannel( HINSTANCE hInstance, HWND hwndParent );

protected:

	CNote*				m_pNote;

	int					m_iMeasure;
	int					m_iVoice;
	int					m_iTick;
	int					m_iAbsoluteTick;
	int					m_iVoicesInChord;
	int					m_iMove;
	int					m_iMoveCtl;
	int					m_iTicksPerMeasure;
	int					m_iPan[ 16 ];

	BOOL				m_blChord;
	BOOL				m_blRememberAttack;
	BOOL				m_blInsertMode;
	BOOL				m_blMakeOnAddNote;
	BOOL				m_blRetainPitch;
	BOOL				m_blAutoAdvance;

	int					m_iXpos;
	int					m_iYpos;
	int					m_iXOrigin;
	int					m_iYOrigin;

	CSelection			m_Selection;
	CSelection			m_TempSelection;

	char				m_szPreviousMacro[ 1024 ];
	int					m_iPreviousGuitarChordTicks;
	int					m_iPreviousGuitarChord;
	BOOL				m_blPreviousGuitarChordChord;
	BOOL				m_blPreviousGuitarChordBassNote;
	BOOL				m_blPreviousGuitarChordMelody;
	int					m_iConsecutiveGuitarChords;

	HGLOBAL				m_hClipboardMem;

	void		MoveToPitch( int iAbsolutePitch );
	void		Macro( const char* szMacro, int iRepetitions = 1 );

private:

};


#endif __CURSOR_H__
