
// CSong.cpp

#include "stdafx.h"
#include "winbase.h"

#ifndef __CSONG_H__
#include "CSong.h"
#endif

#ifndef __CVOICE_H__
#include "CVoice.h"
#endif

#ifndef __CAPPLICATION_H__
#include "CApplication.h"
#endif

#ifndef __CPOSITION_H__
#include "CPosition.h"
#endif

#ifndef __CONSTANT_H__
#include "Constant.h"
#endif

#ifndef __CNOTE_H__
#include "CNote.h"
#endif

#ifndef __CTICK_H__
#include "CTick.h"
#endif

#ifndef __CBMSTORAGE_H__
#include "CBMStorage.h"
#endif

#ifndef __CCURSOR_H__
#include "CCursor.h"
#endif

#ifndef __CMIDIFILE_H__
#include "CMidiFile.h"
#endif

#ifndef __CMIDIEVENT_H__
#include "CMidiEvent.h"
#endif

#include "CDialogSongSettings.h"
#include "CDialogPatchSettings.h"
#include "CDialogMidiInfo.h"
#include "CDialogRecord.h"

#include "GlobalDefs.h"


#define MEASURE_WIDTH		(NOTE_WIDTH * (m_iTicksPerMeasure + 2))



CSong::CSong() :
	CMusicalThing(),
	m_iVoices( SONG_NUMVOICES ),
	m_blModified( FALSE ),
	m_blNeverBeenSaved( TRUE ),
//	m_pWave( NULL ),
	m_iRecVoice( 0 ),
	m_iRecMaxTime( 40 )
{
	SetTempo( SONG_TEMPO );
	SetTimeSigN( SONG_TIMESIG_N );
	SetTimeSigD( SONG_TIMESIG_D );
	
	// the 10 is just for padding
	m_sizeLayout.cx = 10 + MEASURE_FIRSTX + MEASURE_WIDTH;
	m_sizeLayout.cy = STAFF_TOP + (VOICE_HEIGHT * SONG_NUMVOICES);

	InitVoices();

	// initialize staff
	m_Staff.m_iLength = 10 + MEASURE_FIRSTX + MEASURE_WIDTH;
	m_Staff.SetNumVoices( m_iVoices );

	m_GrandStaff.m_iLength = 10 + MEASURE_FIRSTX + MEASURE_WIDTH;
	m_GrandStaff.SetNumVoices( 2 );

	SetModified( FALSE );

	// prepare wave format
	m_wf.wFormatTag =			WAVE_FORMAT_PCM;
	m_wf.nChannels =			2; 
	m_wf.nSamplesPerSec =		11025; 
	m_wf.wBitsPerSample =		16; 
	m_wf.nBlockAlign =			m_wf.nChannels * m_wf.wBitsPerSample / 8;
	m_wf.nAvgBytesPerSec =		m_wf.nSamplesPerSec * m_wf.nChannels * m_wf.wBitsPerSample / 8;
	m_wf.cbSize = 0; 
}	

CSong::~CSong()
{
}

const CSong& CSong::operator=( const CSong& rvalue )
{
	CMusicalThing::operator=( rvalue );

	m_Staff = rvalue.m_Staff;
	m_GrandStaff = rvalue.m_GrandStaff;
	m_sizeLayout = rvalue.m_sizeLayout;
	m_VoiceList = rvalue.m_VoiceList;
	m_iVoices = rvalue.m_iVoices;
	m_blModified = rvalue.m_blModified;
	m_blNeverBeenSaved = rvalue.m_blNeverBeenSaved;

	return *this;
}

void CSong::Dump( char* szString ) const
{
	char sz[ 1024 ];

	sprintf( sz, "SONG:\r\n" );
	lstrcat( szString, sz );

	CMusicalThing::Dump( szString );

	m_Staff.Dump( szString );
	m_GrandStaff.Dump( szString );
	sprintf( sz, "m_sizeLayout.cx = %d\r\n", m_sizeLayout.cx );
	lstrcat( szString, sz );
	sprintf( sz, "m_sizeLayout.cy = %d\r\n", m_sizeLayout.cy );
	lstrcat( szString, sz );
	m_VoiceList.Dump( szString );
	sprintf( sz, "m_iVoices = %d\r\n", m_iVoices );
	lstrcat( szString, sz );
	sprintf( sz, "m_blModified = %d\r\n", m_blModified );
	lstrcat( szString, sz );
	sprintf( sz, "m_blNeverBeenSaved = %d\r\n", m_blNeverBeenSaved );
	lstrcat( szString, sz );
}

BOOL CSong::Output( FILE* pFile )
{
	if (!CMusicalThing::Output( pFile )) return FALSE;

	if (fwrite( &m_iVoices, sizeof( m_iVoices ), 1, pFile ) < 1 ) return FALSE;
	if (fwrite( &m_sizeLayout, sizeof( m_sizeLayout ), 1, pFile ) < 1 ) return FALSE;

	for (int i = 0; i < m_iVoices; i++)
	{
		if (!((CVoice*)m_VoiceList[ i ])->Output( pFile )) return FALSE;
	}

	if (!m_Staff.Output( pFile )) return FALSE;
	if (!m_GrandStaff.Output( pFile )) return FALSE;

	SetModified( FALSE );
	m_blNeverBeenSaved = FALSE;

	return TRUE;
}

BOOL CSong::Input( FILE* pFile )
{
	if (!CMusicalThing::Input( pFile )) return FALSE;

	if (fread( &m_iVoices, sizeof( m_iVoices ), 1, pFile ) < 1 ) return FALSE;
	if (fread( &m_sizeLayout, sizeof( m_sizeLayout ), 1, pFile ) < 1 ) return FALSE;

	m_VoiceList.RemoveAll();
	InitVoices();

	for (int i = 0; i < m_iVoices; i++)
	{
		if (!((CVoice*)m_VoiceList[ i ])->Input( pFile )) return FALSE;
	}

	if (!m_Staff.Input( pFile )) return FALSE;
//	if (!m_GrandStaff.Input( pFile )) return FALSE;
	
	Normalize();

	SetModified( FALSE );
	m_blNeverBeenSaved = FALSE;

	return TRUE;
}

void CSong::InitVoices()
{
	for (int i = 0; i < m_iVoices; i++)
	{
		CVoice* pVoice = new CVoice( i );
		m_VoiceList.AppendItem( pVoice );

		int iChannel;

		if (i >= 9 && i <= 14)		// save channel 10 for drums
		{
			iChannel = i + 2;
		}
		else if (i > 14)	// drums
		{
			iChannel = 10;
		}
		else
		{
			iChannel = i + 1;
		}

		pVoice->Init(	iChannel, 
						VOICE_VELOCITY,
						VOICE_HEIGHT * i, 
						CLEF_TREBLE, 
						m_iTicksPerMeasure	);
	}

}

void CSong::Draw( HDC hdc ) const
{
	int iXOrigin = CApplication::GetXOrigin();
	int iYOrigin = CApplication::GetYOrigin();

	if (CApplication::IsGrandStaff())
	{
		m_GrandStaff.Draw( hdc );
	}
	else
	{
		m_Staff.Draw( hdc );
	}

	// select black pen
	HPEN hOldPen = (HPEN)::SelectObject( hdc, ::GetStockObject( BLACK_PEN ) );

	for (int i = 0; i < m_iVoices; i++)
	{
		((CVoice*)m_VoiceList[ i ])->Draw( hdc );
	}

	// clean up
	::SelectObject( hdc, hOldPen );
}

BOOL CSong::HasNeverBeenSaved() const
{
	return m_blNeverBeenSaved;
}

int CSong::AddNote(	CNote* pNote, 
					int iTick, 
					int iVoice, 
					int iMeasure,
					BOOL blRest, 
					BOOL blSharp, 
					int& riCursorYpos, 
					BOOL blRememberAttack, 
					BOOL blMake	)
{
	// returns:
	//	UPDATE_MEASURE if a new note is added
	//  UPDATE_SONG if space between voices changes

	SetModified( TRUE );

	int iUpdate = ((CVoice*)m_VoiceList[ iVoice ])->AddNote(	pNote, 
																iTick, 
																iMeasure,
																blRest, 
																blSharp, 
																riCursorYpos, 
																blRememberAttack	);

	if (iUpdate & UPDATE_MEASURE) return UPDATE_MEASURE;

	if (iUpdate & UPDATE_SONG)
	{
		if (blMake) Make();

		return UPDATE_SONG;
	}

	return UPDATE_NONE;
}

void CSong::AddMeasure( int iKey )
{
	SetModified( TRUE );

	for (int i = 0; i < m_iVoices; i++)
	{
		((CVoice*)m_VoiceList[ i ])->AddMeasure( iKey );
	}

	m_sizeLayout.cx += MEASURE_WIDTH;

	// update length of staff
	m_Staff.m_iLength = 10 + MEASURE_FIRSTX + (GetMeasureCount() * MEASURE_WIDTH);
	m_GrandStaff.m_iLength = 10 + MEASURE_FIRSTX + (GetMeasureCount() * MEASURE_WIDTH);

	CApplication::UpdateDocumentSize();
}

void CSong::ChopMeasure()
{
	SetModified( TRUE );

	for (int i = 0; i < m_iVoices; i++)
	{
		((CVoice*)m_VoiceList[ i ])->ChopMeasure();
	}

	m_sizeLayout.cx -= MEASURE_WIDTH;

	// update length of staff
	m_Staff.m_iLength = 10 + MEASURE_FIRSTX + (GetMeasureCount() * MEASURE_WIDTH);
	m_GrandStaff.m_iLength = 10 + MEASURE_FIRSTX + (GetMeasureCount() * MEASURE_WIDTH);

	CApplication::UpdateDocumentSize();
}

void CSong::ChopEmptyMeasures()
{
	SetModified( TRUE );

	BOOL blNotesFound = FALSE;

	while (((CVoice*)m_VoiceList[ 0 ])->GetMeasureCount() > 1)
	{
		for (int i = 0; i < m_iVoices; i++)
		{
			if (((CVoice*)m_VoiceList[ i ])->HasNotesInLastMeasure())
			{
				blNotesFound = TRUE;
				break;
			}
		}

		if (!blNotesFound)
		{
			for (int i = 0; i < m_iVoices; i++)
			{
				((CVoice*)m_VoiceList[ i ])->ChopMeasure();
			}

			m_sizeLayout.cx -= MEASURE_WIDTH;
			CApplication::UpdateDocumentSize();
		}
		else
		{
			break;
		}
	}

	// update length of staff
	m_Staff.m_iLength = 10 + MEASURE_FIRSTX + (GetMeasureCount() * MEASURE_WIDTH);
	m_GrandStaff.m_iLength = 10 + MEASURE_FIRSTX + (GetMeasureCount() * MEASURE_WIDTH);
}

void CSong::ExportMidiFile(	const char* szTitle,
							BOOL blSwingIt,
							CPosition& rposFrom, 
							CPosition& rposTo	)
{
	CMidiFile mfile( m_iVoices, m_iTempo, 0, 0, 4, 4 );

	for (int i = 0; i < m_iVoices; i++)
	{
		((CVoice*)m_VoiceList[ i ])->ExportMidiFile( mfile, i, blSwingIt, rposFrom, rposTo );
	}

	char szFilePathMidi[ MAX_PATH ] = "";
	lstrcat( szFilePathMidi, szTitle );
	lstrcat( szFilePathMidi, ".mid" );

	ofstream fout( szFilePathMidi, ios::binary );
	mfile.Write( fout );
	fout.close();
}

BOOL CSong::ImportMidiFile( const char* szTitle )
{
    HANDLE hFile = ::CreateFile( szTitle, GENERIC_READ, NULL, NULL, 
        OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
/*
    if (hFile == INVALID_HANDLE_VALUE)
    {
		::MessageBox( NULL, "Unable to open file", NULL, MB_OK );
		return FALSE;
    }
*/
	if (!m_MidiFile.Read( hFile ))
	{
/*		::MessageBox(	CApplication::GetMainWindowHandle(),
						"Midi file corrupt.",
						NULL,
						MB_ICONEXCLAMATION	);
	    ::CloseHandle( hFile );
		return FALSE;*/
	}
    ::CloseHandle( hFile );

	return TRUE;
}

BOOL CSong::PlayPrep( char szFilePath[], BOOL blRecording )
{
	// prepare midi file
	char szFilePathMidi[ MAX_PATH ] = "";
	lstrcat( szFilePathMidi, szFilePath );
	lstrcat( szFilePathMidi, ".mid" );

	if (!ImportMidiFile( szFilePathMidi )) return FALSE;

/*
//	if (!blRecording)
	{
		// mix all wave tracks

		char szFilePathWave[ MAX_PATH ] = "";
		lstrcat( szFilePathWave, szFilePath );
		lstrcat( szFilePathWave, ".wav" );

		if (m_pWave) delete m_pWave;

		m_pWave = new CWave(	szFilePathWave,
								m_wf.wFormatTag,
								m_wf.nChannels,
								m_wf.nSamplesPerSec,
								m_wf.wBitsPerSample	);

		for (int i = 0; i < m_iVoices; i++)
		{
			char szWaveFile[ 1000 ] = "";

			((CVoice*)m_VoiceList[ i ])->GetWaveFileName( szWaveFile );
			if (lstrcmp( szWaveFile, ""))
			{
				CWave* pVoiceWave = new CWave( szWaveFile );
				*m_pWave = *m_pWave + *pVoiceWave;
				delete pVoiceWave;
			}
		}

		if (m_pWave->HasData())
		{
			if (!m_pWave->Write()) return FALSE;
			if (!m_pWave->Read()) return FALSE;
			if (!m_pWave->PlayPrep()) return FALSE;
		}
		else
		{
			delete m_pWave;
			m_pWave = NULL;
		}
	}
*/
	return TRUE;
}

BOOL CSong::Play( BOOL blLoop, BOOL blRecording )
{
/*
//	if (!blRecording)
	{
		// play wave file
		if (m_pWave)
		{
			if (!m_pWave->PlayStart()) return FALSE;
		}
	}
*/

	// play midi file
	return m_MidiFile.Play( m_iTempo, blLoop );
}

BOOL CSong::PlayStop()
{
	m_MidiFile.Stop();
/*
	if (m_pWave)
	{
		m_pWave->PlayStop();
		delete m_pWave;
		m_pWave = NULL;
	}
*/

	return TRUE;
}

void CSong::OnSettingsRecording( HINSTANCE hInstance, HWND hwndParent )
{
	// create dialog
	CDialogRecord dr;

	dr.m_iMaxTime = m_iRecMaxTime;
	
	switch (m_wf.nSamplesPerSec)
	{
		case 11025 :
			dr.m_iSampleRate = 0;
			break;
	}

	dr.m_iBitsPerSample = m_wf.wBitsPerSample / 8 - 1;
	dr.m_iMode = m_wf.nChannels - 1;

	// display dialog
	if (dr.DoModal( hInstance, hwndParent ) == IDOK)
	{
		m_iRecMaxTime = dr.m_iMaxTime;
		switch (dr.m_iSampleRate)
		{
			case 0 :
				m_wf.nSamplesPerSec = 11025;
				break;
		}
		m_wf.wBitsPerSample = (dr.m_iBitsPerSample + 1) * 8;
		m_wf.nChannels = dr.m_iMode + 1;
	}
}

BOOL CSong::RecordPrep( char szFilePath[] )
{
	m_iRecVoice = TheCursor.GetVoice();

	char szFilePathWave[ MAX_PATH ];
	sprintf( szFilePathWave, "%s.voice%d.wav", szFilePath, m_iRecVoice + 1 );

	return ((CVoice*)m_VoiceList[ m_iRecVoice ])->RecordPrep( szFilePathWave, m_wf, m_iRecMaxTime );
}

BOOL CSong::Record()
{
	return ((CVoice*)m_VoiceList[ m_iRecVoice ])->RecordStart();
}

BOOL CSong::RecordStop()
{
	m_MidiFile.Stop();
	return ((CVoice*)m_VoiceList[ m_iRecVoice ])->RecordStop();
}

BOOL CSong::IsPlaying()
{
	return m_MidiFile.IsPlaying();
}

void CSong::OnSettingsVoice( HINSTANCE hInstance, HWND hwndParent, int iVoice )
{
	int iUpdate = ((CVoice*)m_VoiceList[ iVoice ])->OnSettings( hInstance, hwndParent );
	
	if (iUpdate & UPDATE_DOC)
	{
		SetModified( TRUE );
		Make();
	}
	
	if (iUpdate & UPDATE_VIEW)
	{
		Normalize();
		Make();
		TheCursor.ResetPitch();
		CApplication::Redraw();
	}
}

void CSong::OnSettingsPatch( HINSTANCE hInstance, HWND hwndParent )
{
	CDialogPatchSettings dps;

	// see if all voices have the same patch setting
	int iPreviousPatch = ((CVoice*)m_VoiceList[ 0 ])->GetPatch();
	int iPatch;

	for (int i = 1; i < m_iVoices; i++)
	{
		iPatch = ((CVoice*)m_VoiceList[ i ])->GetPatch();
		if (iPatch != iPreviousPatch)
		{
			iPatch = 0;
			break;
		}
		iPreviousPatch = iPatch;
	}
	dps.m_iPatch = iPatch;

	// display dialog
	if (dps.DoModal( hInstance, hwndParent ) == IDOK)
	{
		// get patch for all voices from dialog
		for (i = 0; i < m_iVoices; i++)
		{
			((CVoice*)m_VoiceList[ i ])->SetPatch( dps.m_iPatch );
		}

		SetModified( TRUE );
		CApplication::Redraw();
	}
}

BOOL CSong::OnSettingsSong( HINSTANCE hInstance, HWND hwndParent )
{
	BOOL blResetCursor = FALSE;

	int iUpdate = UPDATE_NONE;

	// create dialog
	CDialogSongSettings dss;

	dss.m_iVoices = m_iVoices;
	dss.m_iTempo = m_iTempo;
	dss.m_iTimeSigN = m_iTimeSigN;
	dss.m_iTimeSigD = m_iTimeSigD;

	int iOldTimeSigN = m_iTimeSigN;
	int iOldTimeSigD = m_iTimeSigD;

	// display dialog
	if (dss.DoModal( hInstance, hwndParent ))
	{
		SetNumVoices( dss.m_iVoices, TheCursor.GetKey() );
		SetTempo( dss.m_iTempo );

		iUpdate |= UPDATE_DOC;

		// display warning if time signature changes
		if (dss.m_iTimeSigN != iOldTimeSigN || dss.m_iTimeSigD != iOldTimeSigD)
		{
			if (MessageBox( NULL, "Changing the time signature will cause all notes "
				"to be erased! Change time signature?",
				"Warning",
				MB_ICONWARNING | MB_TASKMODAL | MB_YESNO ) == IDYES)
			{
				SetTimeSigN( dss.m_iTimeSigN );
				SetTimeSigD( dss.m_iTimeSigD );

				blResetCursor = TRUE;

				iUpdate |= UPDATE_VIEW | UPDATE_DOC;
		
				for (int i = 0; i < m_iVoices; i++)
				{
					((CVoice*)m_VoiceList[ i ])->SetTicksPerMeasure( m_iTicksPerMeasure );
				}

				// delete all measures but one
				Reset();
			}
		}
	}

	if (iUpdate & UPDATE_DOC)
	{
//		m_pdocDocument->SetModifiedFlag( TRUE );
	}
	
	if (iUpdate & UPDATE_VIEW)
	{
//		m_pdocDocument->UpdateAllViews( NULL );
	}

	return blResetCursor;
}

void CSong::Make()
{
// rebuilds score layout
// called when space between notes changes

	int iTop = ((CVoice*)m_VoiceList[ 0 ])->GetTop();

	for (int i = 0; i < m_iVoices; i++)
	{
		int iRoomAbove = ((CVoice*)m_VoiceList[ i ])->GetRoomAbove();

		// adjust staff positions	
		m_Staff.m_iYpos[ i ] = iTop + iRoomAbove;
		
		// rebuild this voice and get top of next voice
		((CVoice*)m_VoiceList[ i ])->Make( iTop );
	}

	m_sizeLayout.cy = iTop;

	CApplication::UpdateDocumentSize();
}

void CSong::Normalize()
{
// called by:
//  CBachView::OnEditPaste()
//  CSong::OnSettingsSong()
//  CSong::OnSettingsVoice()
// adjusts space between voices to fit highest and lowest notes


	// get top of first voice
	int iTop = ((CVoice*)m_VoiceList[ 0 ])->GetTop();

	// loop through all voices
	for (int i = 0; i < m_iVoices; i++)
	{
		// save top of current voice
		int iThisTop = iTop;

		// normalize this voice and get top of next voice
		((CVoice*)m_VoiceList[ i ])->Normalize( iTop );

		// get room above this voice
		int iRoomAbove = ((CVoice*)m_VoiceList[ i ])->GetRoomAbove();

		// adjust staff positions	
		m_Staff.m_iYpos[ i ] = iThisTop + iRoomAbove;
	}

	m_sizeLayout.cy = iTop;

	CApplication::UpdateDocumentSize();
}

void CSong::PutTicks(	CTick* pTicks, 
						const CPosition& rposFrom, 
						const CPosition& rposTo,
						int iKey	)
{
	SetModified( TRUE );

	// add measures if rposTo is beyond current last measure
	
	int iToMeasure = rposTo.GetMeasure();
	int iMeasureCount = GetMeasureCount();
	int iNewMeasures = iToMeasure + 1 - iMeasureCount;
	for (int i = 0; i < iNewMeasures; i++)
	{
		AddMeasure( iKey );
	}

	int iVoice = rposFrom.GetVoice();
	if (iVoice < m_iVoices)
	{
		((CVoice*)m_VoiceList[ iVoice ])->PutTicks( pTicks, rposFrom, rposTo );
	}

//	m_pdocDocument->SetModifiedFlag( TRUE );
}

void CSong::TransposeNotes(	int iHalfSteps, 
							ENoteDirection Direction, 
							const CPosition& rposFrom, 
							const CPosition& rposTo	)
{
	SetModified( TRUE );

	for (int i = rposFrom.GetVoice(); i <= rposTo.GetVoice(); i++)
	{
		((CVoice*)m_VoiceList[ i ])->TransposeNotes( iHalfSteps, Direction, rposFrom, rposTo );
	}
}

void CSong::TransposeDiatonic(	int iPitches, 
								ENoteDirection Direction, 
								const CPosition& rposFrom, 
								const CPosition& rposTo	)
{
	SetModified( TRUE );

	for (int i = rposFrom.GetVoice(); i <= rposTo.GetVoice(); i++)
	{
		((CVoice*)m_VoiceList[ i ])->TransposeDiatonic( iPitches, Direction, rposFrom, rposTo );
	}
}

void CSong::Reset()
{
	// chop all measures except first
	int iMeasures = GetMeasureCount();
	for (int i = 0; i < iMeasures - 1; i++)
	{
		ChopMeasure();
	}

	// clear notes from first measure, delete wave tracks
	for (i = 0; i < m_iVoices; i++)
	{
		((CVoice*)m_VoiceList[ i ])->ClearNotes();
		((CVoice*)m_VoiceList[ i ])->Reset();
	}

	// reset size
	m_sizeLayout.cx = 10 + MEASURE_FIRSTX + MEASURE_WIDTH;
	m_sizeLayout.cy = STAFF_TOP + (VOICE_HEIGHT * SONG_NUMVOICES);

	// rebuild score
	Normalize();

	// update length of staff
	m_Staff.m_iLength = 10 + MEASURE_FIRSTX + (GetMeasureCount() * MEASURE_WIDTH);
	m_GrandStaff.m_iLength = 10 + MEASURE_FIRSTX + (GetMeasureCount() * MEASURE_WIDTH);

	// reset cursor
	TheCursor.SetTicksPerMeasure( m_iTicksPerMeasure );
	TheCursor.SetKey( CMAJOR );
	TheCursor.Reset();

	m_blNeverBeenSaved = TRUE;

	SetModified( FALSE );
	CApplication::Redraw();
}

void CSong::MuteVoice( int iVoice )
{
	if (((CVoice*)m_VoiceList[ iVoice ])->IsMute()) ((CVoice*)m_VoiceList[ iVoice ])->Mute( FALSE );
	else ((CVoice*)m_VoiceList[ iVoice ])->Mute( TRUE );

	SetModified( TRUE );
}

void CSong::FindTicksPerMeasure()
{
	switch (m_iTimeSigD)
	{
		case 2 :
			m_iTicksPerMeasure = NOTE_HALF * m_iTimeSigN;
			break;

		case 4:
			m_iTicksPerMeasure = NOTE_QUARTER * m_iTimeSigN;
			break;

		case 8 :
			m_iTicksPerMeasure = NOTE_EIGHTH * m_iTimeSigN;
			break;

		case 16:
			m_iTicksPerMeasure = NOTE_SIXTEENTH * m_iTimeSigN;
			break;
	}
}

void CSong::ClearVoice( int iVoice )
{
	SetModified( TRUE );

	((CVoice*)m_VoiceList[ iVoice ])->ClearNotes();

//	m_pdocDocument->SetModifiedFlag( TRUE );

	Make();

	Normalize();

//	m_pdocDocument->UpdateAllViews( NULL );
}

void CSong::RemoveAttacks( int iMeasure )
{
	SetModified( TRUE );

	// iMeasure = -1 : all measures

	for (int i = 0; i < m_iVoices; i++)
	{
		((CVoice*)m_VoiceList[ i ])->RemoveAttacks( iMeasure );
	}
}

HPEN CSong::GetVoicePen( int iVoice ) const
{
	return ((CVoice*)m_VoiceList[ iVoice ])->GetPen();
}

SIZE CSong::GetLayoutSize() const
{
	return m_sizeLayout; 
}

int CSong::GetMeasureCount() const
{
	return ((CVoice*)m_VoiceList[ 0 ])->GetMeasureCount();
}

RECT CSong::GetRect( int iVoice, int iMeasure ) const
{
	// iVoice = -1 : all voices
	// iMeasure = -1 : all measures

	RECT rc;

	if (iVoice == -1)
	{
		// get rect for first voice
		rc = ((CVoice*)m_VoiceList[ 0 ])->GetRect( iMeasure );
		// stretch bottom of rect to height of song
		InflateRect( &rc, 0, (VOICE_HEIGHT * (m_iVoices - 1)) / 2 );
		// reset top to 0
		OffsetRect( &rc, 0, -rc.top );
	}
	else
	{
		rc = ((CVoice*)m_VoiceList[ iVoice ])->GetRect( iMeasure );
	}

	return rc;
}

int CSong::GetVoiceTop( int iVoice ) const
{
	return ((CVoice*)m_VoiceList[ iVoice ])->GetTop() + ((CVoice*)m_VoiceList[ iVoice ])->GetRoomAbove();
}

void CSong::GetTicks(	CTick* pTicks, 
						const CPosition& rposFrom, 
						const CPosition& rposTo	) const
{
	((CVoice*)m_VoiceList[ rposFrom.GetVoice() ])->GetTicks( pTicks, rposFrom, rposTo );
}

int CSong::GetVoiceAtPoint( POINT p ) const
{
	for (int i = 0; i < m_iVoices; i++)
	{
		RECT rc = ((CVoice*)m_VoiceList[ i ])->GetRect( -1 );
		if (::PtInRect( &rc, p ))
		{
			return i;
		}
	}

	return -1;
}

int CSong::GetTickAtPoint( POINT p ) const
{
	int iVoice = GetVoiceAtPoint( p );

	if (iVoice != -1)
	{
		return ((CVoice*)m_VoiceList[ iVoice ])->GetTickAtPoint( p );
	}
	
	return -1;
}

int CSong::GetChannel( int iVoice ) const
{
	return ((CVoice*)m_VoiceList[ iVoice ])->GetChannel();
}

int CSong::GetClef( int iVoice ) const
{
	return ((CVoice*)m_VoiceList[ iVoice ])->GetClef();
}

int CSong::GetPatch( int iVoice ) const
{
	if (m_VoiceList[ iVoice ])
	{
		return ((CVoice*)m_VoiceList[ iVoice ])->GetPatch();
	}

	return -1;
}

void CSong::SetModified( BOOL blModified )
{
	m_blModified = blModified;
	CApplication::SetModified( blModified );
}

BOOL CSong::SetKey( int iKey )
{
	for (int i = 0; i < m_iVoices; i++)
	{
		((CVoice*)m_VoiceList[ i ])->SetKey( iKey );
	}

	SetModified( TRUE );
	
	return TRUE;
}

BOOL CSong::SetChannel( int iVoice, int iChannel )
{
	SetModified( TRUE );

	((CVoice*)m_VoiceList[ iVoice ])->SetChannel( iChannel );

	return TRUE;
}

BOOL CSong::SetClef( int iVoice, int iClef )
{
	SetModified( TRUE );

	((CVoice*)m_VoiceList[ iVoice ])->SetClef( iClef );

//	m_pdocDocument->SetModifiedFlag( TRUE );

	Make();

	Normalize();

//	m_pdocDocument->UpdateAllViews( NULL );

	return TRUE;
}

BOOL CSong::SetPatch( int iVoice, int iPatch )
{
	SetModified( TRUE );

	((CVoice*)m_VoiceList[ iVoice ])->SetPatch( iPatch );

//	m_pdocDocument->SetModifiedFlag( TRUE );

	return TRUE;
}

BOOL CSong::SetTempo( int iTempo )
{
	if (iTempo == m_iTempo) return FALSE;
	
	m_iTempo = iTempo;

	SetModified( TRUE );

	return TRUE;
}

BOOL CSong::SetTimeSigN( int iTimeSigN )
{
	if (iTimeSigN == m_iTimeSigN) return FALSE;
	
	m_iTimeSigN = iTimeSigN;

	FindTicksPerMeasure();

	SetModified( TRUE );
	CApplication::Redraw();

	return TRUE;
}

BOOL CSong::SetTimeSigD( int iTimeSigD )
{
	if (iTimeSigD == m_iTimeSigD) return FALSE;
	
	m_iTimeSigD = iTimeSigD;

	FindTicksPerMeasure();

	SetModified( TRUE );
	CApplication::Redraw();

	return TRUE;
}

void CSong::SetNumVoices( int iNumVoices, int iKey )
{
	if (iNumVoices == m_iVoices)
	{
		return;
	}
	else if (iNumVoices > m_iVoices)	// add voices
	{
		TheCursor.Reset();

		// this block is similar to CSong::InitVices()
		for (int i = m_iVoices; i < iNumVoices; i++)
		{
			CVoice* pVoice = new CVoice( i );
			m_VoiceList.AppendItem( pVoice );

			int iChannel;

			if (i >= 9 && i <= 14)		// save channel 10 for drums
			{
				iChannel = i + 2;
			}
			else if (i > 14)	// drums
			{
				iChannel = 10;
			}
			else
			{
				iChannel = i + 1;
			}

			pVoice->Init(	iChannel, 
							VOICE_VELOCITY,
							VOICE_HEIGHT * i, 
							CLEF_TREBLE, 
							m_iTicksPerMeasure	);

			int iAddMeasures = GetMeasureCount() - 1;
			for (int j = 0; j < iAddMeasures; j++)
			{
				pVoice->AddMeasure( iKey );
			}
		}

		m_iVoices = iNumVoices;

		m_Staff.SetNumVoices( iNumVoices );

		Make();
	}
	else	// remove voices
	{
		TheCursor.Reset();

		for (int i = m_iVoices; i > iNumVoices; i--)
		{
			m_VoiceList.ChopItem();
		}

		m_iVoices = iNumVoices;

		m_Staff.SetNumVoices( iNumVoices );

		Make();
	}

	SetModified( TRUE );
	CApplication::Redraw();
}

BOOL CSong::IsModified() const
{
	return m_blModified;
}

BOOL CSong::IsMute( int iVoice ) const
{
	return ((CVoice*)m_VoiceList[ iVoice ])->IsMute();
}

void CSong::LoadWaveFile( LPSTR lpstrFile, int iVoice )
{
	((CVoice*)m_VoiceList[ iVoice ])->SetWaveFileName( lpstrFile );
}



