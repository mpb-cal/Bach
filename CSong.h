
// CSong.h

#ifndef __CSONG_H__
#define __CSONG_H__

#ifndef __CSTAFF_H__
#include "CStaff.h"
#endif

#ifndef __CMUSICALTHING_H__
#include "CMusicalThing.h"
#endif

#ifndef __CMIDIFILE_H__
#include "CMidiFile.h"
#endif

#ifndef __CWAVE_H__
//#include "..\\wavetestbed\\CWave.h"
#endif

#include "CLinkedList.h"

#include "CBachComponent.h"


// forward declarations

class CBachDoc;
class CNote;
class CCursor;
class CPosition;
class CTick;
class CBachSong;


class CSong : public CMusicalThing
{
public:

					CSong();
	virtual			~CSong();

public:

	const CSong&	operator=( const CSong& rvalue );

	// input and output methods
	virtual BOOL	Output( FILE* pFile );
	virtual BOOL	Input( FILE* pFile );

	void			Dump( char* szString ) const;

	void			InitVoices();

	void			Draw( HDC hdc ) const;

	int 			AddNote(	CNote* pNote, 
								int iTick, 
								int iVoice, 
								int iMeasure,
								BOOL blRest, 
								BOOL blSharp, 
								int& riCursorYpos, 
								BOOL blRememberAttack,
								BOOL blMake	);
	void			AddMeasure( int iKey );
	void			ChopMeasure();
	void			ChopEmptyMeasures();

	BOOL			IsModified() const;
	void			SetModified( BOOL blModified );
	BOOL			HasNeverBeenSaved() const;
	
	virtual void	ExportMidiFile(	const char* szTitle, 
									BOOL blSwingIt,
									CPosition& rposFrom, 
									CPosition& rposTo	);
	BOOL			ImportMidiFile( const char* szTitle );

	BOOL			PlayPrep( char szFilePath[], BOOL blRecording = FALSE );
	BOOL			Play( BOOL blLoop, BOOL blRecording );
	BOOL			PlayStop();

	BOOL			RecordPrep( char szFilePath[] );
	BOOL			Record();
	BOOL			RecordStop();

public:

	BOOL			IsPlaying();

	void			Make();
	void			Normalize();
	void			Reset();

	void			OnSettingsVoice( HINSTANCE hInstance, HWND hwndParent, int iVoice );
	int				OnSettingsSong( HINSTANCE hInstance, HWND hwndParent );
	void			OnSettingsPatch( HINSTANCE hInstance, HWND hwndParent );
	void			OnSettingsRecording( HINSTANCE hInstance, HWND hwndParent );

	SIZE			GetLayoutSize() const;
	RECT			GetRect( int iVoice, int iMeasure ) const;
	int				GetMeasureCount() const;
	int				GetVoiceTop( int iVoice ) const;
	int				GetVoiceAtPoint( POINT p ) const;
	int				GetTickAtPoint( POINT p ) const;
	int				GetNumVoices()  const { return m_iVoices; }
	int				GetChannel( int iVoice ) const;
	int				GetClef( int iVoice ) const;
	int				GetPatch( int iVoice ) const;
	
	HPEN			GetVoicePen( int iVoice ) const;

	void			SetNumVoices( int iNumVoices, int iKey );
	BOOL			SetKey( int iKey );
	BOOL			SetChannel( int iVoice, int iChannel );
	BOOL			SetClef( int iVoice, int iClef );
	BOOL			SetPatch( int iVoice, int iPatch );
	BOOL			SetTempo( int iTempo );
	BOOL			SetTimeSigN( int iTimeSigN );
	BOOL			SetTimeSigD( int iTimeSigD );

	void			GetTicks(	CTick* pTicks, 
								const CPosition& rposFrom, 
								const CPosition& rposTo	) const;
	void			PutTicks(	CTick* pTicks, 
								const CPosition& rposFrom, 
								const CPosition& rposTo, 
								int iKey	);

	void			TransposeNotes(	int iHalfSteps, 
									ENoteDirection Direction, 
									const CPosition& rposFrom, 
									const CPosition& rposTo	);
	void			TransposeDiatonic(	int iPitches, 
										ENoteDirection Direction, 
										const CPosition& rposFrom, 
										const CPosition& rposTo	);

	void			MuteVoice( int iVoice );
	BOOL			IsMute( int iVoice ) const;
	void			ClearVoice( int iVoice );
	void			RemoveAttacks( int iMeasure );

	void			LoadWaveFile( LPSTR lpstrFile, int iVoice );

protected:

	void		FindTicksPerMeasure();

	CStaff		m_Staff;
	CStaff		m_GrandStaff;
	SIZE		m_sizeLayout;
	CLinkedList	m_VoiceList;
	int			m_iVoices;

	BOOL		m_blModified;
	BOOL		m_blNeverBeenSaved;

	CMidiFile	m_MidiFile;

	WAVEFORMATEX	m_wf;
//	CWave*			m_pWave;

	int			m_iRecVoice;
	int			m_iRecMaxTime;
};



#endif __CSONG_H__
