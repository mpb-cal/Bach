
// CVoice.h

#ifndef __CVOICE_H__
#define __CVOICE_H__

#ifndef __CMUSICALTHING_H__
#include "CMusicalThing.h"
#endif

#ifndef __CWAVE_H__
//#include "..\\wavetestbed\\CWave.h"
#endif

#include "CNote.h"

#include "CLinkedList.h"

class CNote;
class CMeasure;
class CMidiFile;
class CPosition;
class CTick;

class CVoice : public CMusicalThing, public CLinkedListItem
{

public:

					CVoice( int iNumber );
	virtual			~CVoice();

	void			MakeCopy( CLinkedListItem*& rpNewCopy );

	void			Dump( char* szString ) const;

	void			Init(	int iChannel, 
							int iVelocity,
							int iTop, 
							int iClef, 
							int iTicksPerMeasure	); 

	void			Reset();
	void			Draw( HDC hdc ) const;
	void			Make( int& rnTop );
	void			Normalize( int& rnTop );
	BOOL			HasNotesInLastMeasure() const;
	void			Mute( BOOL blYesOrNo ) { m_blMute = blYesOrNo; }
	BOOL			IsMute() const { return m_blMute; }

	int 			AddNote(	CNote* pNote, 
								int iTick, 
								int iMeasure, 
								BOOL blRest,
								BOOL blSharp, 
								int& rnCursorYpos, 
								BOOL blRememberAttack	);
	// input and output methods
	BOOL		Output( FILE* pFile ) const;
	BOOL		Input( FILE* pFile );

	void		ClearNotes();
	void		AddMeasure( int iKey );
	BOOL		ChopMeasure();
	void		RemoveAttacks( int iMeasure );
	void		ExportMidiFile(	CMidiFile& mfile, 
								int iNumber, 
								BOOL blSwingIt,
								CPosition& rposFrom, 
								CPosition& rposTo	) const;

	int			OnSettings( HINSTANCE hInstance, HWND hwndParent );

	BOOL		SetClef( int iClef );
	BOOL		SetTicksPerMeasure( int iTicksPerMeasure );
	
	int			GetTop() const { return m_iTop; }
	int			GetRoomAbove() const { return m_iRoomAbove; }
	int			GetRoomBelow() const { return m_iRoomBelow; }
	int			GetClefBitmap() const { return m_iClefBitmap; }
	int			GetMeasureCount() const;
	RECT		GetRect( int iMeasure ) const;
	int			GetMeasureAtPoint( POINT p ) const;
	int			GetTickAtPoint( POINT p ) const;
	COLORREF	GetColor() const { return m_crColor; }
	void		SetColor( COLORREF crColor );
	HPEN		GetPen() const { return m_hPen; }

	void		GetTicks(	CTick* pTicks, 
							const CPosition& rposFrom, 
							const CPosition& rposTo	) const;
	void		PutTicks(	CTick* PTicks, 
							const CPosition& rposFrom, 
							const CPosition& rposTo	);

	void		TransposeNotes(	int iHalfSteps, 
								ENoteDirection Direction, 
								const CPosition& rposFrom, 
								const CPosition& rposTo	);
	void		TransposeDiatonic(	int iPitches, 
									ENoteDirection Direction, 
									const CPosition& rposFrom, 
									const CPosition& rposTo	);

	void		SetWaveFileName( LPSTR lpstrFile );
	void		GetWaveFileName( LPSTR lpstrFile );
	BOOL		RecordPrep( char szFilePath[], WAVEFORMATEX& wf, int iMaxTime );
	BOOL		RecordStart();
	BOOL		RecordStop();

protected:
	
	CMeasure*		GetMeasure( int iMeasure ) const;
	CMeasure*		GetLastMeasure() const;

	CLinkedList		m_MeasureList;

	int				m_iTop;
	int				m_iRoomAbove;
	int				m_iRoomBelow;
	int				m_iClefBitmap;

	BOOL			m_blMute;

	COLORREF		m_crColor;
	HPEN			m_hPen;

//	CWave*			m_pWave;
	char			m_szWaveFile[ 1000 ];
};



#endif __VOICE_H__
