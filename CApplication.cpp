
#include "resource.h"

#ifndef __CAPPLICATION_H__
#include "CApplication.h"
#endif

#include "CBachComponent.h"

#ifndef __CSTRUCTUREDSONG_H__
#include "CStructuredSong.h"
#endif

#ifndef __CBMSTORAGE_H__
#include "CBMStorage.h"
#endif

#ifndef __GLOBALDEFS_H__
#include "GlobalDefs.h"
#endif

#include "CNote.h"
#include "CStructuredSong.h"

#include "DialogAboutBox.h"
#include "CDialogSongSettings.h"
#include "CDialogChordSettings.h"
#include "CDialogKeySettings.h"
#include "CDialogDumpSong.h"

#ifndef _ER_DEBUG_H
#include "er_debug.h"
#endif

#include "windef.h"
#include "winbase.h"

LRESULT CALLBACK MainWindowProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK ClientWindowProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK OverviewWindowProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam );


// global variables for clipboard

const char* g_szClipboardFormat = "BachClipboardFormat";
unsigned int g_uiClipboardFormat;


// CApplication constants

const int APPLICATION_TOOLBAR1ID		= 1;
const int APPLICATION_STATUSBARID		= 2;
const int APPLICATION_CLIENTWINDOWID	= 3;
const int APPLICATION_OVERVIEWWINDOWID	= 4;

const char APPLICATION_APPNAME[]			= "Bach2";
const char APPLICATION_OPENFILEFILTER[]		= "Bach Files (*.bch)\0*.bch\0";
const char APPLICATION_DEFAULTEXTENSION[]	= "bch";
const char APPLICATION_IMPORTFILEFILTER[]	= "MIDI Files (*.mid)\0*.mid\0";
const char APPLICATION_LOADWAVEFILTER[]		= "Wave Files (*.wav)\0*.wav\0";

// status bar constants

const int  APPLICATION_SBPANES				= 5;
const int  APPLICATION_SBPANE1WIDTH			= 150;
const int  APPLICATION_SBPANE2WIDTH			= 175;
const int  APPLICATION_SBPANE3WIDTH			= 75;
const int  APPLICATION_SBPANE4WIDTH			= 75;


// Big 3 global objects

CSong			TheSong;
CStructuredSong	TheStructuredSong;
CCursor			TheCursor;


// define static members of CApplication

HINSTANCE	CApplication::m_hInstance = NULL;
int			CApplication::m_iCmdShow = 0;
char		CApplication::m_szCommandLine[ 1000 ];
int			CApplication::m_iReturnValue = 0;
HWND		CApplication::m_hwndMainWindow = NULL;
HWND		CApplication::m_hwndClientWindow = NULL;
HWND		CApplication::m_hwndOverviewWindow = NULL;
HWND		CApplication::m_hwndToolbar1 = NULL;
HWND		CApplication::m_hwndStatusbar = NULL;
HACCEL		CApplication::m_hAccel = NULL;
HCURSOR		CApplication::m_hWaitCursor = NULL;
HMENU		CApplication::m_hPopupMenu = NULL;
int			CApplication::m_iMainWindowHeight = 0;
int			CApplication::m_iMainWindowWidth = 0;
int			CApplication::m_iClientWindowHeight = 0;
int			CApplication::m_iClientWindowWidth = 0;
int			CApplication::m_iOverviewWindowHeight = 0;
int			CApplication::m_iOverviewWindowWidth = 0;
int			CApplication::m_iToolbar1Height = 0;
int			CApplication::m_iToolbar1Width = 0;
int			CApplication::m_iStatusbarHeight = 0;
int			CApplication::m_iStatusbarWidth = 0;
int			CApplication::m_iZoomDivisor = 1;
int			CApplication::m_iVScrollPos = 0;
int			CApplication::m_iVScrollMax = 0;
int			CApplication::m_iHScrollPos = 0;
int			CApplication::m_iHScrollMax = 0;
int			CApplication::m_iOverviewVScrollPos = 0;
int			CApplication::m_iOverviewVScrollMax = 0;
int			CApplication::m_iOverviewHScrollPos = 0;
int			CApplication::m_iOverviewHScrollMax = 0;
BOOL		CApplication::m_blSwingIt = FALSE;
BOOL		CApplication::m_blCursorIsShowing = TRUE;
BOOL		CApplication::m_blGrandStaff = FALSE;
char		CApplication::m_szFilePath[ MAX_PATH ];
char		CApplication::m_szDumpString[ 0x10000 ] = "";
BOOL		CApplication::m_blPlayingSongs = FALSE;
BOOL		CApplication::m_blPlayingWave = FALSE;


void CApplication::Run(	HINSTANCE hInstance, 
						char* szCommandLine, 
						int iCmdShow	)
{
	// seed random numbers
	srand( (unsigned)time( NULL ) );

	m_hInstance = hInstance;
	m_iCmdShow = SW_MAXIMIZE;
	lstrcpy( m_szCommandLine, szCommandLine );

	::InitCommonControls();

//	ERDebugStream::InitDebugLevel( hInstance );
	
	CBMStorage::Initialize( m_hInstance );

	// prepare wait cursor
	m_hWaitCursor = ::LoadCursor( NULL, IDC_WAIT );

	// prepare clipboard format
	g_uiClipboardFormat = ::RegisterClipboardFormat( g_szClipboardFormat );

	if (!CreateMainWindow()) m_iReturnValue = -1;

	if (m_iReturnValue == 0)
	{
		// Show the main window and paint its contents.
		::ShowWindow( m_hwndMainWindow, m_iCmdShow );
		::UpdateWindow( m_hwndMainWindow );
	}

	if (m_iReturnValue == 0)
	{
		::SendMessage( m_hwndMainWindow, WM_COMMAND, ID_FILE_NEW, 0 );
	}

	// process command line
	if (m_szCommandLine[ 0 ] != '\0')
	{
		lstrcpy( m_szFilePath, m_szCommandLine );

		FILE* pFile = fopen( m_szCommandLine, "rb" );
		if (pFile)
		{
			TheStructuredSong.Input( pFile );
			fclose( pFile );
		}
	}

	// load keyboard accelerator table
	m_hAccel = LoadAccelerators( m_hInstance, (LPCTSTR)IDR_ACCELERATOR );

	if (m_iReturnValue == 0)
	{
		MessageLoop();
	}
}


void CApplication::MessageLoop()
{
	MSG msg;
	while (::GetMessage( &msg, (HWND)NULL, 0, 0 ))
	{
		if (!TranslateAccelerator( m_hwndMainWindow, m_hAccel, &msg ))
		{
			::TranslateMessage( &msg );
			::DispatchMessage( &msg );
		}
	}

	m_iReturnValue = msg.wParam;
}

BOOL CApplication::OKToCloseFile()
{
	BOOL blResult = FALSE;

	if (TheSong.IsModified())
	{
		char szMessage[ MAX_PATH + _MAX_FNAME ] = "Save changes to ";
		lstrcat( szMessage, m_szFilePath );
		lstrcat( szMessage, "?" );

		int iResult = MessageBox(	m_hwndMainWindow,
									szMessage,
									APPLICATION_APPNAME,
									MB_YESNOCANCEL	);

		switch (iResult)
		{
			case IDYES :
				blResult = (BOOL)::SendMessage( m_hwndMainWindow, WM_COMMAND, ID_FILE_SAVE, 0 );
				break;

			case IDNO :
				blResult = TRUE;
				break;

			case IDCANCEL :
				blResult = FALSE;
				break;

			default :
				blResult = FALSE;
		}
	}
	else
	{
		blResult = TRUE;
	}

	return blResult;
}

BOOL CApplication::OnFileExit()
{
	OnPlayStop( TRUE );

	if (OKToCloseFile())
	{
		PostQuitMessage( 0 );
		return TRUE;
	}

	return FALSE;
}


///////////////////////////////////////////////////////////////////////
//	window creation methods

BOOL CApplication::CreateMainWindow()
{
	WNDCLASS wc; 

	// Register the window class for the main window.
	wc.style			= CS_DBLCLKS;
	wc.lpfnWndProc		= (WNDPROC)::MainWindowProc;
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance		= m_hInstance;
	wc.hIcon			= LoadIcon( m_hInstance, "Bach2App" );
	wc.hCursor			= LoadCursor( m_hInstance, IDC_ARROW );
	wc.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wc.lpszMenuName		= MAKEINTRESOURCE( IDR_MAINMENU );
	wc.lpszClassName	= "MainWindowClass";

	if (!::RegisterClass( &wc ))
	{
		return FALSE;
	}

	// Create the main window. 
	m_hwndMainWindow = ::CreateWindowEx(	NULL,
											"MainWindowClass",
											APPLICATION_APPNAME,
											WS_OVERLAPPEDWINDOW,
											CW_USEDEFAULT,
											CW_USEDEFAULT,
											CW_USEDEFAULT,
											CW_USEDEFAULT,
											(HWND)NULL,
											(HMENU)NULL,
											m_hInstance,
											(LPVOID)NULL	);

	// If the main window cannot be created, terminate
	// the application.
	if (!m_hwndMainWindow)
	{
		return FALSE;
	}

	// save window's dimensions
	RECT rc;
	::GetWindowRect( m_hwndMainWindow, &rc );
	m_iMainWindowHeight = rc.bottom - rc.top;
	m_iMainWindowWidth = rc.right - rc.left;

	// load the popup menu
	HMENU hMenu = ::LoadMenu( m_hInstance, MAKEINTRESOURCE( IDR_POPUPMENU ) );
	m_hPopupMenu = ::GetSubMenu( hMenu, 0 );

	return TRUE;
}


BOOL CApplication::CreateClientWindow( HWND hwndParent )
{
	WNDCLASS wc; 

	// Register the window class for the main window's client area
	wc.style			= CS_DBLCLKS;
	wc.lpfnWndProc		= (WNDPROC)::ClientWindowProc;
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance		= m_hInstance;
	wc.hIcon			= ::LoadIcon( (HINSTANCE) NULL, IDI_APPLICATION );
	wc.hCursor			= ::LoadCursor( m_hInstance, (LPCTSTR)IDC_CROSSHAIRS );
	wc.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wc.lpszMenuName		= NULL;
	wc.lpszClassName	= "ClientWindowClass";

	if (!::RegisterClass( &wc ))
	{
		return FALSE;
	}

	// Create the main window's client area window
	m_hwndClientWindow = ::CreateWindowEx(	WS_EX_CLIENTEDGE,
											"ClientWindowClass",
											NULL,
											WS_CHILD | 
												WS_VISIBLE |
												WS_HSCROLL |
												WS_VSCROLL,
											0,
											0,
											0,
											0,
											hwndParent,
											(HMENU) APPLICATION_CLIENTWINDOWID,
											m_hInstance,
											(LPVOID)NULL	);

	if (!m_hwndClientWindow)
	{
		DWORD dwLastError = GetLastError();
		return FALSE;
	}

	// save window's dimensions
	RECT rc;
	::GetWindowRect( m_hwndClientWindow, &rc );
	m_iClientWindowHeight = rc.bottom - rc.top;
	m_iClientWindowWidth = rc.right - rc.left;

	return TRUE;
}

BOOL CApplication::CreateOverviewWindow( HWND hwndParent )
{
	WNDCLASS wc; 

	// Register the window class for the main window's client area
	wc.style			= 0;
	wc.lpfnWndProc		= (WNDPROC)::OverviewWindowProc;
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance		= m_hInstance;
	wc.hIcon			= ::LoadIcon( (HINSTANCE) NULL, IDI_APPLICATION );
	wc.hCursor			= ::LoadCursor( m_hInstance, (LPCTSTR)IDC_CROSSHAIRS );
	wc.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wc.lpszMenuName		= NULL;
	wc.lpszClassName	= "OverviewWindowClass";

	if (!::RegisterClass( &wc ))
	{
		return FALSE;
	}

	// Create the main window's client area window
	m_hwndOverviewWindow = ::CreateWindowEx(	WS_EX_CLIENTEDGE,
												"OverviewWindowClass",
												NULL,
												WS_CHILD | 
													WS_VISIBLE |
													WS_HSCROLL |
													WS_VSCROLL,
												0,
												0,
												0,
												0,
												hwndParent,
												(HMENU) APPLICATION_OVERVIEWWINDOWID,
												m_hInstance,
												(LPVOID)NULL	);

	if (!m_hwndOverviewWindow)
	{
		DWORD dwLastError = GetLastError();
		return FALSE;
	}

	// save window's dimensions
	RECT rc;
	::GetWindowRect( m_hwndOverviewWindow, &rc );
	m_iOverviewWindowHeight = rc.bottom - rc.top;
	m_iOverviewWindowWidth = rc.right - rc.left;

	return TRUE;
}

BOOL CApplication::CreateToolbars( HWND hwndParent )
{
	TBBUTTON tbButton[] =
	{
		0,			0,					TBSTATE_ENABLED,	TBSTYLE_SEP,	0, 0, 0, 0,
		STD_FILENEW,	ID_FILE_NEW,	TBSTATE_ENABLED,	TBSTYLE_BUTTON, 0, 0, 0, 0,
		STD_FILEOPEN,	ID_FILE_OPEN,	TBSTATE_ENABLED,	TBSTYLE_BUTTON, 0, 0, 0, 0,
		STD_FILESAVE,	ID_FILE_SAVE,	TBSTATE_ENABLED,	TBSTYLE_BUTTON, 0, 0, 0, 0
	};

	m_hwndToolbar1 = ::CreateToolbarEx(	hwndParent,
										WS_CHILD |
											WS_VISIBLE |
											WS_CLIPSIBLINGS |
											CCS_TOP |
											TBSTYLE_TOOLTIPS,
										APPLICATION_TOOLBAR1ID,
										4,
										HINST_COMMCTRL,
										IDB_STD_SMALL_COLOR,
										tbButton,
										4,
										0,
										0,
										0,
										0,
										sizeof TBBUTTON	);

	TBADDBITMAP tbbitmap =
	{
		m_hInstance, IDB_NOTE_CHORD
	};
	int iBitmap = (int)::SendMessage( m_hwndToolbar1, TB_ADDBITMAP, 1, (LPARAM)&tbbitmap );

	TBBUTTON tbMyButton[] =
	{
		0,			0,				TBSTATE_ENABLED,	TBSTYLE_SEP, 0, 0, 0, 0,
		iBitmap,	ID_NOTE_CHORD,	TBSTATE_ENABLED,	TBSTYLE_BUTTON, 0, 0, 0, 0
	};
	::SendMessage( m_hwndToolbar1, TB_ADDBUTTONS, 2, (LPARAM)tbMyButton );

	if (!m_hwndToolbar1)
	{
		return FALSE;
	}

	// save window's dimensions
	RECT rc;
	::GetWindowRect( m_hwndToolbar1, &rc );
	m_iToolbar1Height = rc.bottom - rc.top;
	m_iToolbar1Width = rc.right - rc.left;

	return TRUE;
}

BOOL CApplication::CreateStatusbar( HWND hwndParent )
{
	BOOL blResult = FALSE;

	m_hwndStatusbar = ::CreateStatusWindow(	WS_CHILD |
												WS_VISIBLE |
												WS_CLIPSIBLINGS |
												SBARS_SIZEGRIP |
												CCS_BOTTOM,
											"Ready",
											hwndParent,
											APPLICATION_STATUSBARID	);

	if (m_hwndStatusbar)
	{
		blResult = TRUE;
	}

	// save window's dimensions
	RECT rc;
	::GetWindowRect( m_hwndStatusbar, &rc );
	m_iStatusbarHeight = rc.bottom - rc.top;
	m_iStatusbarWidth = rc.right - rc.left;

	return blResult;
}


void CApplication::SetMainWindowTitle()
{
	char szWindowTitle[ _MAX_FNAME ];

	lstrcpy( szWindowTitle, APPLICATION_APPNAME );
	lstrcat( szWindowTitle, " - " );
	lstrcat( szWindowTitle, m_szFilePath );

	if (TheSong.IsModified())
	{
		lstrcat( szWindowTitle, "*" );
	}

	::SetWindowText( m_hwndMainWindow, szWindowTitle );
}

void CApplication::SetModified( BOOL blModified )
{
	SetMainWindowTitle();
}

void CApplication::UpdateDocumentSize()
{
	RECT rc;
	::GetClientRect( m_hwndClientWindow, &rc );
	::PostMessage(	m_hwndClientWindow, 
					WM_SIZE, 
					SIZE_RESTORED, 
					MAKELPARAM( rc.left - rc.right, rc.bottom - rc.top )	);
}

void CApplication::Redraw()
{
	UpdateStatusBar();
	::InvalidateRect( m_hwndClientWindow, NULL, TRUE );
}

void CApplication::MakeCaret()
{
	if (m_iZoomDivisor == 1)
	{
		int iCaretBitmapId = TheCursor.m_pNote->GetCaretBitmapId();
		
		HBITMAP hCaret = ::LoadBitmap( m_hInstance, MAKEINTRESOURCE( iCaretBitmapId ) );

		::CreateCaret( m_hwndClientWindow, hCaret, 0, 0 );

		::SetCaretPos(	TheCursor.GetXpos() - m_iHScrollPos,
						TheCursor.GetYpos() - m_iVScrollPos	);

		::ShowCaret( m_hwndClientWindow );

		::DeleteObject( hCaret );
	}
	else
	{
		::CreateCaret( m_hwndClientWindow, NULL, 1, 20 );

		::SetCaretPos(	TheCursor.GetXpos() - m_iHScrollPos,
						TheCursor.GetYpos() - m_iVScrollPos	);

		::ShowCaret( m_hwndClientWindow );
	}
}

void CApplication::NoteMenuSetChecks( HMENU hMenu, int iSelection, WORD wParam )
{
	CheckMenuItem( hMenu, ID_NOTE_THIRTYSECOND,	MF_UNCHECKED );
	CheckMenuItem( hMenu, ID_NOTE_SIXTEENTH,	MF_UNCHECKED );
	CheckMenuItem( hMenu, ID_NOTE_SIXTEENTHD,	MF_UNCHECKED );
	CheckMenuItem( hMenu, ID_NOTE_EIGHTH,		MF_UNCHECKED );
	CheckMenuItem( hMenu, ID_NOTE_EIGHTHD,		MF_UNCHECKED );
	CheckMenuItem( hMenu, ID_NOTE_QUARTER,		MF_UNCHECKED );
	CheckMenuItem( hMenu, ID_NOTE_QUARTERD,		MF_UNCHECKED );
	CheckMenuItem( hMenu, ID_NOTE_HALF,			MF_UNCHECKED );
	CheckMenuItem( hMenu, ID_NOTE_HALFD,		MF_UNCHECKED );
	CheckMenuItem( hMenu, ID_NOTE_WHOLE,		MF_UNCHECKED );

	CheckMenuItem( hMenu, wParam, MF_CHECKED );
	TheCursor.m_pNote->SetValue( iSelection );
	MakeCaret();
}

void CApplication::ExportMidiFile( CSong* pSong, BOOL blFromCursor )
{
	CPosition posFrom;
	CPosition posTo;

	if (blFromCursor)
	{
		if (TheCursor.m_Selection.IsEnabled())
		{
			posFrom = TheCursor.m_Selection.GetStartPos();
			posTo = TheCursor.m_Selection.GetStopPos();
		}
		else
		{
			posFrom.SetMeasure( TheCursor.m_iMeasure );
			posTo.SetMeasure( TheSong.GetMeasureCount() );
		}
	}
	else
	{
		posFrom.SetMeasure( 0 );
		posTo.SetMeasure( TheSong.GetMeasureCount() );
	}

	pSong->ExportMidiFile(	m_szFilePath, 
							m_blSwingIt,
							posFrom,
							posTo	);
}

void CApplication::OnPlayGo( CSong* pSong, BOOL blFromCursor, BOOL blLoop )
{
	OnPlayStop( TRUE );

	ExportMidiFile( pSong, blFromCursor );

	char szPane[ 100 ] = "Ready";
	if (!pSong->PlayPrep( m_szFilePath )) goto err;
	if (!pSong->Play( blLoop, FALSE )) goto err;

	m_blPlayingWave = TRUE;

	lstrcpy( szPane, "Playing" );
	::SendMessage( m_hwndStatusbar, SB_SETTEXT, 0, (LPARAM)szPane );

	return; 

	err:
	{
		lstrcpy( szPane, "Playing Failed" );
		::SendMessage( m_hwndStatusbar, SB_SETTEXT, 0, (LPARAM)szPane );
	}
}

void CApplication::OnPlayStop( BOOL blAlways )
{
	if (!blAlways)
	{
		// don't stop if MIDI is still going
		if (TheSong.IsPlaying() || TheStructuredSong.IsPlaying()) return;

		// don't stop if wave is still going
		if (m_blPlayingWave) return;
	}

	char szPane[ 100 ] = "Playback Stopped";
	::SendMessage( m_hwndStatusbar, SB_SETTEXT, 0, (LPARAM)szPane );

	TheSong.PlayStop();
	TheStructuredSong.PlayStop();
	m_blPlayingSongs = FALSE;
}

void CApplication::OnRecordGo()
{
	OnRecordStop();

	ExportMidiFile( &TheSong, FALSE );

	char szPane[ 100 ] = "Recording";
	::SendMessage( m_hwndStatusbar, SB_SETTEXT, 0, (LPARAM)szPane );

	if (!TheSong.RecordPrep( m_szFilePath )) goto err;
	if (!TheSong.PlayPrep( m_szFilePath, TRUE )) goto err;
	
	TheSong.Play( FALSE, TRUE );
	m_blPlayingWave = TRUE;
	TheSong.Record();

	return;

	err:
	{
		OnRecordStop( FALSE );
	}
}

void CApplication::OnRecordStop( BOOL success )
{
	OnPlayStop( TRUE );
	TheSong.RecordStop();

	char szPane[ 100 ];
	if (success) lstrcpy( szPane, "Done Recording");
	else lstrcpy( szPane, "Recording Failed");
	::SendMessage( m_hwndStatusbar, SB_SETTEXT, 0, (LPARAM)szPane );
}

DWORD PlaySongsThread( LPVOID pParam )
{
	CApplication::m_blPlayingSongs = TRUE;

	while (CApplication::m_blPlayingSongs)
	{
		switch( rand() % 2 )
		{
			case 0 :
				CApplication::OnComposeSong( TRUE );
				break;

			case 1 :
				CApplication::OnComposeSongAABA();
				break;
		}
		CApplication::OnPlayGo( &TheSong, FALSE, FALSE );
		::SleepEx( 5000, FALSE );
		while (TheSong.IsPlaying())
		{
			::SleepEx( 1000, FALSE );
		}
	}

	return 0;
}

void CApplication::OnComposeSongs()
{
	DWORD dwThreadId;
	::CreateThread(	NULL, 
					0, 
					(LPTHREAD_START_ROUTINE)PlaySongsThread, 
					0, 
					0, 
					&dwThreadId	);
}

void CApplication::OnComposeSong( BOOL blCreateMelody )
{
	// save top voice
	if (!blCreateMelody)
	{
		TheCursor.Reset();
		TheCursor.Macro( "bs r32 ", 24 );
		TheCursor.OnEditCopy();
		TheCursor.Reset();
	}

	// clear out song and set to 10 voices
	TheSong.Reset();

	// paste top voice back in
	if (!blCreateMelody)
	{
		TheCursor.Reset();
		TheCursor.OnEditPaste();
		TheCursor.Reset();
	}

	// move caret home
	::SetCaretPos(	TheCursor.GetXpos() - m_iHScrollPos, 
					TheCursor.GetYpos() - m_iVScrollPos	);

	TheSong.SetNumVoices( 10, 0 );

	// key
	if (blCreateMelody)
	{
		int iKey = rand() % LastKeySignature;
		TheCursor.m_pNote->SetKey( iKey );
	}


	// channels
	TheSong.SetChannel( 0, 1 );
	TheSong.SetChannel( 1, 2 );
	TheSong.SetChannel( 2, 3 );
	TheSong.SetChannel( 3, 4 );
	TheSong.SetChannel( 4, 5 );
	TheSong.SetChannel( 5, 10 );
	TheSong.SetChannel( 6, 10 );
	TheSong.SetChannel( 7, 10 );
	TheSong.SetChannel( 8, 10 );
	TheSong.SetChannel( 9, 10 );


	/////////////////// patches

	// melody
	int iPatch = rand() % 128;

	// avoid Synth SFX and SFX
	if (iPatch >=  96 && iPatch <= 103) iPatch -= 96;
	if (iPatch >= 120 && iPatch <= 127) iPatch -= 104;;

	TheSong.SetPatch( 0, iPatch );

	// chords
	int iPatch1, iPatch2, iPatch3, iPatch4;
	int iChordOrchestra = rand() % 9;
	switch (iChordOrchestra)
	{
		case 0 :	// piano
			iPatch1 = iPatch2 = iPatch3 = iPatch4 = rand() % 8;
			break;

		case 1 :	// chromatic percussion
			iPatch1 = iPatch2 = iPatch3 = iPatch4 = rand() % 8 + 8;
			break;

		case 2 :	// organ
			iPatch1 = iPatch2 = iPatch3 = iPatch4 = rand() % 8 + 16;
			break;

		case 3 :	// guitar
			iPatch1 = iPatch2 = iPatch3 = iPatch4 = rand() % 8 + 24;
			break;

		case 4 :	// string quartet
			iPatch1 = 40;
			iPatch2 = 41;
			iPatch3 = 42;
			iPatch4 = 43;
			break;

		case 5 :	// brass quartet
			iPatch1 = 56;
			iPatch2 = 57;
			iPatch3 = 57;
			iPatch4 = 58;
			break;

		case 6 :	// sax quartet
			iPatch1 = 64;
			iPatch2 = 65;
			iPatch3 = 66;
			iPatch4 = 67;
			break;

		case 7 :	// woodwind quartet
			iPatch1 = 73;
			iPatch2 = 68;
			iPatch3 = 71;
			iPatch4 = 70;
			break;

		case 8 :	// random homogenous
			iPatch = rand() % 128;
			// avoid Synth SFX and SFX
			if (iPatch >=  96 && iPatch <= 103) iPatch -= 96;
			if (iPatch >= 120 && iPatch <= 127) iPatch -= 104;;
			iPatch1 = iPatch2 = iPatch3 = iPatch4 = iPatch;
			break;

	}

	TheSong.SetPatch( 1, iPatch1 );
	TheSong.SetPatch( 2, iPatch2 );
	TheSong.SetPatch( 3, iPatch3 );
	TheSong.SetPatch( 4, iPatch4 );

	////////////////// clefs
	TheSong.SetClef( 3, CLEF_BASS );
	TheSong.SetClef( 4, CLEF_BASS );

	TheCursor.ComposeSong( blCreateMelody );

	// unmute drum kit and extra drums
	if (TheSong.IsMute( 5 )) TheSong.MuteVoice( 5 );
	if (TheSong.IsMute( 6 )) TheSong.MuteVoice( 6 );
	if (TheSong.IsMute( 7 )) TheSong.MuteVoice( 7 );
	if (TheSong.IsMute( 8 )) TheSong.MuteVoice( 8 );
	if (TheSong.IsMute( 9 )) TheSong.MuteVoice( 9 );

	// mute drum kit (1 in 4 chance)
	if (rand() % 4 == 0)
	{
		TheSong.MuteVoice( 7 );
		TheSong.MuteVoice( 8 );
		TheSong.MuteVoice( 9 );
	}

	// mute extra drums (1 in 2 chance)
	if (rand() % 2 == 0)
	{
		TheSong.MuteVoice( 5 );
		TheSong.MuteVoice( 6 );
	}

	TheSong.Normalize();
	Redraw();
	MakeCaret();

	if (rand() % 2 == 0) m_blSwingIt = TRUE;
	else m_blSwingIt = FALSE;
}

void CApplication::OnComposeSongAABA()
{
	// clear out song and set to 10 voices
	TheSong.Reset();

	// move caret home
	::SetCaretPos(	TheCursor.GetXpos() - m_iHScrollPos, 
					TheCursor.GetYpos() - m_iVScrollPos	);

	TheSong.SetNumVoices( 5, 0 );

	// key
	int iKey = rand() % LastKeySignature;
	TheCursor.m_pNote->SetKey( iKey );


	// channels
	TheSong.SetChannel( 0, 1 );
	TheSong.SetChannel( 1, 1 );
	TheSong.SetChannel( 2, 1 );
	TheSong.SetChannel( 3, 1 );
	TheSong.SetChannel( 4, 1 );


	/////////////////// patches

	// melody
	TheSong.SetPatch( 0, 0 );
	TheSong.SetPatch( 1, 0 );
	TheSong.SetPatch( 2, 0 );
	TheSong.SetPatch( 3, 0 );
	TheSong.SetPatch( 4, 0 );

	////////////////// clefs
	TheSong.SetClef( 3, CLEF_BASS );
	TheSong.SetClef( 4, CLEF_BASS );

	TheCursor.ComposeSongAABA();

	TheSong.Normalize();
	Redraw();
	MakeCaret();

	if (rand() % 2 == 0) m_blSwingIt = TRUE;
	else m_blSwingIt = FALSE;
}

void CApplication::DumpSong( CSong* pSong )
{
	m_szDumpString[ 0 ] = 0;
	pSong->Dump( m_szDumpString );

	CDialogDumpSong dds;
	lstrcpy( dds.m_szText, m_szDumpString );

	dds.DoModal( m_hInstance, m_hwndMainWindow );
}

void CApplication::UpdateStatusBar()
{
	char szPane1[ 100 ];
	char szPane2[ 100 ];
	char szPane3[ 100 ];
	char szPane4[ 100 ];

	char szMode[ 100 ];
	TheCursor.GetMode( szMode );

	sprintf( szPane1, "Mode : %s", szMode );
	sprintf( szPane2, "Voice : %d (%s)",
		TheCursor.GetVoice() + 1,
		g_pszPatches[ TheSong.GetPatch( TheCursor.GetVoice() ) ] );
	sprintf( szPane3, "Measure : %d", TheCursor.GetMeasure() + 1 );
	sprintf( szPane4, "Beat : %d.%d",  
		TheCursor.GetTick() / NOTE_QUARTER + 1,
		TheCursor.GetTick() % NOTE_QUARTER + 1 );

	::SendMessage( m_hwndStatusbar, SB_SETTEXT, 1, (LPARAM)szPane1 );
	::SendMessage( m_hwndStatusbar, SB_SETTEXT, 2, (LPARAM)szPane2 );
	::SendMessage( m_hwndStatusbar, SB_SETTEXT, 3, (LPARAM)szPane3 );
	::SendMessage( m_hwndStatusbar, SB_SETTEXT, 4, (LPARAM)szPane4 );
}

//////////////////////////////////////////////////////////////////////////////
//	Main window proc

// stub proc
LRESULT CALLBACK MainWindowProc(	HWND hwnd,
									UINT uMsg,
									WPARAM wParam,
									LPARAM lParam	)
{
	return CApplication::MainWindowProc( hwnd, uMsg, wParam, lParam );
}

LRESULT CApplication::MainWindowProc(	HWND hwnd,
										UINT uMsg,
										WPARAM wParam,
										LPARAM lParam	)
{
	switch (uMsg)
	{

		////////////////////////////////////////////////////////////////
		// window maintenance

		case WM_CREATE :
		{
			HDC hdc = GetDC( hwnd );
			ReleaseDC( hwnd, hdc );

			CreateClientWindow( hwnd );
//			CreateOverviewWindow( hwnd );
			CreateToolbars( hwnd );
			CreateStatusbar( hwnd );

			return 0;
		}

		case WM_DESTROY :
		{
			::WinHelp( hwnd, "Bach2.hlp", HELP_QUIT, NULL );
			
			PostQuitMessage( 0 );
			
			return 0;
		}

		case WM_CLOSE :
		{
			if (OnFileExit())
			{
				DestroyWindow( hwnd );
			}

			return 0;
		}

		////////////////////////////////////////////////////////////////
		// sizing, moving, scrolling

		case WM_SIZE :
		{
			RECT rcToolbar1;
			RECT rcStatusbar;

			// save new dimensions
			m_iMainWindowWidth = LOWORD( lParam );
			m_iMainWindowHeight = HIWORD( lParam );

			// resize toolbar
			::SendMessage( m_hwndToolbar1, TB_AUTOSIZE, 0, 0L );
			::GetWindowRect( m_hwndToolbar1, &rcToolbar1 );
			m_iToolbar1Height = rcToolbar1.bottom - rcToolbar1.top;
			m_iToolbar1Width = rcToolbar1.right - rcToolbar1.left;

			// resize status bar
			::GetWindowRect( m_hwndStatusbar, &rcStatusbar );
			m_iStatusbarHeight = rcStatusbar.bottom - rcStatusbar.top;
			m_iStatusbarWidth = rcStatusbar.right - rcStatusbar.left;
			::MoveWindow(	m_hwndStatusbar,
							0,
							m_iMainWindowHeight - m_iStatusbarHeight,
							m_iMainWindowWidth,
							m_iStatusbarHeight,
							TRUE	);

			// resize status bar panes
			::GetWindowRect( m_hwndStatusbar, &rcStatusbar );
			m_iStatusbarWidth = rcStatusbar.right - rcStatusbar.left;
			m_iStatusbarHeight= rcStatusbar.bottom - rcStatusbar.top;

			int iPaneWidths[] = 
			{
				m_iStatusbarWidth - (	APPLICATION_SBPANE1WIDTH + 
										APPLICATION_SBPANE2WIDTH +
										APPLICATION_SBPANE3WIDTH +
										APPLICATION_SBPANE4WIDTH	),
				m_iStatusbarWidth - (	APPLICATION_SBPANE2WIDTH + 
										APPLICATION_SBPANE3WIDTH +
										APPLICATION_SBPANE4WIDTH	),
				m_iStatusbarWidth - (	APPLICATION_SBPANE3WIDTH + 
										APPLICATION_SBPANE4WIDTH	),
				m_iStatusbarWidth - APPLICATION_SBPANE4WIDTH,
				-1
			};

			::SendMessage(	m_hwndStatusbar, 
							SB_SETPARTS,
							APPLICATION_SBPANES,
							(LPARAM)iPaneWidths	);

			
			int iClientAreaTotalHeight = m_iMainWindowHeight - (m_iStatusbarHeight + m_iToolbar1Height);
			
			// resize client window
			m_iClientWindowWidth = m_iMainWindowWidth;
//			m_iClientWindowHeight = iClientAreaTotalHeight - iClientAreaTotalHeight / 3;
			m_iClientWindowHeight = iClientAreaTotalHeight;

			::MoveWindow(	m_hwndClientWindow,
							0,
							m_iToolbar1Height,
							m_iClientWindowWidth,
							m_iClientWindowHeight,
							TRUE	);
/*
			// resize overview window
			m_iOverviewWindowWidth = m_iMainWindowWidth;
			m_iOverviewWindowHeight = iClientAreaTotalHeight - m_iClientWindowHeight;

			::MoveWindow(	m_hwndOverviewWindow,
							0,
							m_iToolbar1Height + m_iClientWindowHeight,
							m_iOverviewWindowWidth,
							m_iOverviewWindowHeight,
							TRUE	);
*/
			return 0;
		}

		case WM_NOTIFY :
		{
			LPNMHDR lpnmhdr = (LPNMHDR)lParam;
			LPSTR   lpReply;

			if (lpnmhdr->code == TTN_NEEDTEXT)
			{
				LPTOOLTIPTEXT lpttt = (LPTOOLTIPTEXT)lParam;
				switch (lpttt->hdr.idFrom)
				{
					case ID_FILE_NEW :
						lpReply = "New File";
						break;
					case ID_FILE_OPEN :
						lpReply = "Open File";
						break;
					case ID_FILE_SAVE :
						lpReply = "Save File";
						break;
					case ID_NOTE_CHORD :
						lpReply = "Chord";
						break;
					default :
						lpReply = "Unknown";
						break;
				}
				lstrcpy(lpttt->szText, lpReply);
			}
			
			return 0;
		}

		// wave device messages

		case MM_WOM_DONE :
		{
			m_blPlayingWave = FALSE;
			OnPlayStop( FALSE );

			return 0;
		}

		case MM_WIM_DATA :
		{
//			OnRecordStop();

			return 0;
		}

		////////////////////////////////////////////////////////////////
		// keyboard

		case WM_KEYDOWN :	// process wParam as virtual key code
		{
			// pass message on to client window
			return ::SendMessage( m_hwndClientWindow, uMsg, wParam, lParam );
		}

		case WM_CHAR :		// process wParam as ASCII code
		{
			return 0;
		}

		case WM_KEYUP :	// process wParam as virtual key code
		{
			return 0;
		}

		////////////////////////////////////////////////////////////////
		// mouse

		case WM_MOUSEMOVE :
		{
			if (!m_blCursorIsShowing)
			{
//				::ShowCursor( TRUE );
				m_blCursorIsShowing = TRUE;
			}

			// pass message on to client window
			return ::SendMessage( m_hwndClientWindow, uMsg, wParam, lParam );
		}

		case WM_LBUTTONDOWN :
		{
			if (!m_blCursorIsShowing)
			{
//				::ShowCursor( TRUE );
				m_blCursorIsShowing = TRUE;
			}

			// pass message on to client window
			return ::SendMessage( m_hwndClientWindow, uMsg, wParam, lParam );
		}

		case WM_LBUTTONUP :
		{
			if (!m_blCursorIsShowing)
			{
//				::ShowCursor( TRUE );
				m_blCursorIsShowing = TRUE;
			}

			// pass message on to client window
			return ::SendMessage( m_hwndClientWindow, uMsg, wParam, lParam );
		}

		case WM_LBUTTONDBLCLK :
		{
			if (!m_blCursorIsShowing)
			{
//				::ShowCursor( TRUE );
				m_blCursorIsShowing = TRUE;
			}

			// pass message on to client window
			return ::SendMessage( m_hwndClientWindow, uMsg, wParam, lParam );
		}

		////////////////////////////////////////////////////////////////
		// focus

		case WM_SETFOCUS :
		{
			// give focus to client window
			return ::SendMessage( m_hwndClientWindow, WM_SETFOCUS, wParam, lParam );
		}

		case WM_KILLFOCUS :
		{
			DestroyCaret();

			return 0;
		}

		
		////////////////////////////////////////////////////////////////
		// menu commands

		case WM_CONTEXTMENU :
		{
			// pass message on to client window
			return ::SendMessage( m_hwndClientWindow, uMsg, wParam, lParam );
		}

		case WM_INITMENU :
		{
			if (!m_blCursorIsShowing)
			{
//				::ShowCursor( TRUE );
				m_blCursorIsShowing = TRUE;
			}

			HMENU hMenu = GetMenu( hwnd );

			if (!TheCursor.m_Selection.IsEnabled())
			{
				::EnableMenuItem( hMenu, ID_CHANGE_TRANSPOSE_NOTES, MF_GRAYED );
				::EnableMenuItem( hMenu, ID_CHANGE_TRANSPOSE_DIATONIC, MF_GRAYED );
			}
			else
			{
				::EnableMenuItem( hMenu, ID_CHANGE_TRANSPOSE_NOTES, MF_ENABLED );
				::EnableMenuItem( hMenu, ID_CHANGE_TRANSPOSE_DIATONIC, MF_ENABLED );
			}
			
			if (TheSong.GetChannel( TheCursor.GetVoice() ) == CHANNEL_DRUM)
			{
				::EnableMenuItem( hMenu, ID_NOTE_PERCUSSION, MF_ENABLED );
			}
			else
			{
				::EnableMenuItem( hMenu, ID_NOTE_PERCUSSION, MF_GRAYED );
			}

			if (m_blGrandStaff)
			{
				::CheckMenuItem( hMenu, ID_VIEW_GRANDSTAFF, MF_CHECKED );
			}
			else
			{
				::CheckMenuItem( hMenu, ID_VIEW_GRANDSTAFF, MF_UNCHECKED );
			}

			if (TheCursor.GetVoice() >= 7)
			{
				::EnableMenuItem( hMenu, ID_COMPOSE_GUITAR_CHORD, MF_ENABLED );
				::EnableMenuItem( hMenu, ID_COMPOSE_CHORDPROGRESSION, MF_ENABLED );
			}
			else
			{
				::EnableMenuItem( hMenu, ID_COMPOSE_GUITAR_CHORD, MF_GRAYED );
				::EnableMenuItem( hMenu, ID_COMPOSE_CHORDPROGRESSION, MF_GRAYED );
			}

			if (m_blSwingIt)
			{
				::CheckMenuItem( hMenu, ID_PLAY_SWINGIT, MF_CHECKED );
			}
			else
			{
				::CheckMenuItem( hMenu, ID_PLAY_SWINGIT, MF_UNCHECKED );
			}

			return 0;
		}

		case WM_MENUSELECT :
		{	

			return 0;
		}

		case WM_COMMAND :
		{
			HMENU hMenu = GetMenu( hwnd );

			switch(LOWORD( wParam ))
			{
				// File menu

				case ID_FILE_NEW :
				{
					if (!OKToCloseFile()) return FALSE;

					::HideCaret( ::GetParent( hwnd ) );

					OnPlayStop( TRUE );
					TheSong.Reset();

					lstrcpy( m_szFilePath, DEFAULT_FILEPATH );
					SetMainWindowTitle();

					::InvalidateRect( m_hwndClientWindow, NULL, TRUE );

					::SetCaretPos(	TheCursor.GetXpos() - m_iHScrollPos,
									TheCursor.GetYpos() - m_iVScrollPos  );
					::ShowCaret( ::GetParent( hwnd ) );

					break;
				}

				case ID_FILE_OPEN :
				{
					char szFileName[ MAX_PATH ] = "";
					char szFileTitle[ _MAX_FNAME + _MAX_EXT ] = "";

					OPENFILENAME ofn;
					ZeroMemory( &ofn, sizeof OPENFILENAME );
					
					ofn.lStructSize			= sizeof OPENFILENAME;
					ofn.hwndOwner			= m_hwndMainWindow;
					ofn.hInstance			= m_hInstance;
					ofn.lpstrFilter			= APPLICATION_OPENFILEFILTER;
					ofn.lpstrCustomFilter	= NULL;
					ofn.nMaxCustFilter		= 0;
					ofn.nFilterIndex		= 0;
					ofn.lpstrFile			= szFileName;
					ofn.nMaxFile			= MAX_PATH;
					ofn.lpstrFileTitle		= NULL;
					ofn.nMaxFileTitle		= _MAX_FNAME + _MAX_EXT;
					ofn.lpstrInitialDir		= NULL;
					ofn.lpstrTitle			= NULL;
					ofn.Flags				= OFN_CREATEPROMPT | OFN_EXPLORER;
					ofn.nFileOffset			= 0;
					ofn.nFileExtension		= 0;
					ofn.lpstrDefExt			= APPLICATION_DEFAULTEXTENSION;
					ofn.lCustData			= 0L;
					ofn.lpfnHook			= NULL;
					ofn.lpTemplateName		= NULL;

					if (GetOpenFileName( &ofn ) && OKToCloseFile())
					{
						::HideCaret( ::GetParent( hwnd ) );

						OnPlayStop( TRUE );
						TheSong.Reset();

						lstrcpy( m_szFilePath, ofn.lpstrFile );

						// read the file
						FILE* pFile = fopen( ofn.lpstrFile, "rb" );
						TheStructuredSong.Input( pFile );
						fclose( pFile );

						::SetCaretPos(	TheCursor.GetXpos() - m_iHScrollPos,
										TheCursor.GetYpos() - m_iVScrollPos  );
						::ShowCaret( ::GetParent( hwnd ) );
					}

					::InvalidateRect( m_hwndClientWindow, NULL, TRUE );

					break;
				}

				case ID_FILE_SAVE :
				{
					if (TheSong.HasNeverBeenSaved())
					{
						return ::SendMessage( hwnd, WM_COMMAND, ID_FILE_SAVE_AS, 0 );
					}
					else
					{
						FILE* pFile = fopen( m_szFilePath, "wb" );
						TheStructuredSong.Output( pFile );
						fclose( pFile );

						return TRUE;
					}

					break;
				}

				case ID_FILE_SAVE_AS :
				{
					OPENFILENAME ofn;
					ZeroMemory( &ofn, sizeof OPENFILENAME );
					
					ofn.lStructSize			= sizeof OPENFILENAME;
					ofn.hwndOwner			= m_hwndMainWindow;
					ofn.hInstance			= m_hInstance;
					ofn.lpstrFilter			= APPLICATION_OPENFILEFILTER;
					ofn.lpstrCustomFilter	= NULL;
					ofn.nMaxCustFilter		= 0;
					ofn.nFilterIndex		= 0;
					ofn.lpstrFile			= m_szFilePath;
					ofn.nMaxFile			= MAX_PATH;
					ofn.lpstrFileTitle		= NULL;
					ofn.nMaxFileTitle		= _MAX_FNAME + _MAX_EXT;
					ofn.lpstrInitialDir		= NULL;
					ofn.lpstrTitle			= NULL;
					ofn.Flags				= OFN_OVERWRITEPROMPT;
					ofn.nFileOffset			= 0;
					ofn.nFileExtension		= 0;
					ofn.lpstrDefExt			= APPLICATION_DEFAULTEXTENSION;
					ofn.lCustData			= 0L;
					ofn.lpfnHook			= NULL;
					ofn.lpTemplateName		= NULL;

					if (GetSaveFileName( &ofn ))
					{
						lstrcpy( m_szFilePath, ofn.lpstrFile );

						FILE* pFile = fopen( ofn.lpstrFile, "wb" );
						TheStructuredSong.Output( pFile );
						fclose( pFile );

						return TRUE;
					}

					break;
				}

				case ID_FILE_IMPORT :
				{
					char szFileName[ MAX_PATH ] = "";
					char szFileTitle[ _MAX_FNAME + _MAX_EXT ] = "";

					OPENFILENAME ofn;
					ZeroMemory( &ofn, sizeof OPENFILENAME );
					
					ofn.lStructSize			= sizeof OPENFILENAME;
					ofn.hwndOwner			= m_hwndMainWindow;
					ofn.hInstance			= m_hInstance;
					ofn.lpstrFilter			= APPLICATION_IMPORTFILEFILTER;
					ofn.lpstrCustomFilter	= NULL;
					ofn.nMaxCustFilter		= 0;
					ofn.nFilterIndex		= 0;
					ofn.lpstrFile			= szFileName;
					ofn.nMaxFile			= MAX_PATH;
					ofn.lpstrFileTitle		= NULL;
					ofn.nMaxFileTitle		= _MAX_FNAME + _MAX_EXT;
					ofn.lpstrInitialDir		= NULL;
					ofn.lpstrTitle			= NULL;
					ofn.Flags				= OFN_CREATEPROMPT | OFN_EXPLORER;
					ofn.nFileOffset			= 0;
					ofn.nFileExtension		= 0;
					ofn.lpstrDefExt			= NULL;
					ofn.lCustData			= 0L;
					ofn.lpfnHook			= NULL;
					ofn.lpTemplateName		= NULL;

					if (GetOpenFileName( &ofn ) && OKToCloseFile())
					{
						::HideCaret( ::GetParent( hwnd ) );

						TheSong.Reset();

//						lstrcpy( m_szFilePath, ofn.lpstrFile );
						lstrcpy( m_szFilePath, "MidiImport.bch" );

						// read the file
						TheSong.ImportMidiFile( ofn.lpstrFile );

						::SetCaretPos(	TheCursor.GetXpos() - m_iHScrollPos,
										TheCursor.GetYpos() - m_iVScrollPos  );
						::ShowCaret( ::GetParent( hwnd ) );
					}

					InvalidateRect( m_hwndClientWindow, NULL, TRUE );

					break;
				}

				case ID_FILE_LOADWAVEFILE :
				{
					char szFileName[ MAX_PATH ] = "";
					char szFileTitle[ _MAX_FNAME + _MAX_EXT ] = "";

					OPENFILENAME ofn;
					ZeroMemory( &ofn, sizeof OPENFILENAME );
					
					ofn.lStructSize			= sizeof OPENFILENAME;
					ofn.hwndOwner			= m_hwndMainWindow;
					ofn.hInstance			= m_hInstance;
					ofn.lpstrFilter			= APPLICATION_LOADWAVEFILTER;
					ofn.lpstrCustomFilter	= NULL;
					ofn.nMaxCustFilter		= 0;
					ofn.nFilterIndex		= 0;
					ofn.lpstrFile			= szFileName;
					ofn.nMaxFile			= MAX_PATH;
					ofn.lpstrFileTitle		= NULL;
					ofn.nMaxFileTitle		= _MAX_FNAME + _MAX_EXT;
					ofn.lpstrInitialDir		= NULL;
					ofn.lpstrTitle			= NULL;
					ofn.Flags				= OFN_CREATEPROMPT | OFN_EXPLORER;
					ofn.nFileOffset			= 0;
					ofn.nFileExtension		= 0;
					ofn.lpstrDefExt			= NULL;
					ofn.lCustData			= 0L;
					ofn.lpfnHook			= NULL;
					ofn.lpTemplateName		= NULL;

					if (::GetOpenFileName( &ofn ))
					{
						::HideCaret( ::GetParent( hwnd ) );

						// read the file
						TheSong.LoadWaveFile( ofn.lpstrFile, TheCursor.GetVoice() );

						::SetCaretPos(	TheCursor.GetXpos() - m_iHScrollPos,
										TheCursor.GetYpos() - m_iVScrollPos  );
						::ShowCaret( ::GetParent( hwnd ) );
					}

					InvalidateRect( m_hwndClientWindow, NULL, TRUE );

					break;
				}

				case ID_FILE_PRINT :
				{

					break;
				}

				case ID_FILE_PRINT_PREVIEW :
				{

					break;
				}

				case ID_FILE_PRINT_SETUP :
				{

					break;
				}

				case ID_FILE_SEND :
				{

					break;
				}

				case ID_FILE_MRU_FILE1 :
				{

					break;
				}

				case ID_FILE_EXIT :
				{
					OnFileExit();

					break;
				}


				// Edit menu

				case ID_EDIT_UNDO :
				{

					break;
				}

				case ID_EDIT_CUT :
				{
					TheCursor.OnEditCut();

					TheSong.Normalize();
					Redraw();

					break;
				}

				case ID_EDIT_COPY :
				{
					TheCursor.OnEditCopy();

					break;
				}

				case ID_EDIT_PASTE :
				{
					TheCursor.OnEditPaste();

					TheSong.Normalize();
					Redraw();

					break;
				}

				case ID_EDIT_DELETE :
				{

					break;
				}

				case ID_EDIT_SELECTALL :
				{

					break;
				}


				// View menu

				case ID_VIEW_TOOLBAR :
				{

					break;
				}

				case ID_VIEW_STATUSBAR :
				{

					break;
				}

				case ID_VIEW_ZOOMIN:
				{
					switch (m_iZoomDivisor)
					{
						case 1: m_iZoomDivisor = 2; Redraw(); MakeCaret();break;
						case 2: m_iZoomDivisor = 4; Redraw(); MakeCaret();break;
						case 4: m_iZoomDivisor = 4; break;
					}

					break;
				}

				case ID_VIEW_ZOOMOUT:
				{
					switch (m_iZoomDivisor)
					{
						case 1: m_iZoomDivisor = 1; break;
						case 2: m_iZoomDivisor = 1; Redraw(); MakeCaret(); break;
						case 4: m_iZoomDivisor = 2; Redraw(); MakeCaret(); break;
					}

					break;
				}

				case ID_VIEW_GRANDSTAFF :
				{
					m_blGrandStaff = !m_blGrandStaff;
					Redraw();
					break;
				}


				// Settings menu

				case ID_SETTINGS_CURSOR :
				{
					if (TheCursor.OnSettings( m_hInstance, hwnd ))
					{
						Redraw();
					}

					break;
				}

				case ID_SETTINGS_CHANNEL :
				{
					if (TheCursor.OnSettingsChannel( m_hInstance, hwnd ))
					{
						Redraw();
					}

					break;
				}

				case ID_SETTINGS_VOICE :
				{
					TheSong.OnSettingsVoice( m_hInstance, hwnd, TheCursor.GetVoice() );

					MakeCaret();

					break;
				}

				case ID_SETTINGS_SONG :
				{
					TheSong.OnSettingsSong( m_hInstance, hwnd );

					MakeCaret();

					break;
				}

				case ID_SETTINGS_CHORD :
				{
					CDialogChordSettings dcs;

					dcs.m_iVoices = TheCursor.m_iVoicesInChord;
					
					if (dcs.DoModal( m_hInstance, hwnd ) == IDOK)
					{
						TheCursor.m_iVoicesInChord = dcs.m_iVoices;
					}

					break;
				}

				case ID_SETTINGS_PATCH :
				{
					TheSong.OnSettingsPatch( m_hInstance, hwnd );

					break;
				}

				case ID_SETTINGS_KEY :
				{
					CDialogKeySettings dks;

					dks.m_iKey = TheCursor.m_pNote->GetKey();

					if (dks.DoModal( m_hInstance, hwnd ) == IDOK)
					{
						TheCursor.SetKey( dks.m_iKey );
						TheSong.SetKey( dks.m_iKey );
					}

					break;
				}

				case ID_SETTINGS_RECORDING :
				{
					TheSong.OnSettingsRecording( m_hInstance, hwnd );

					break;
				}


				// Play menu

				case ID_PLAY_GO :
				{
					OnPlayGo( &TheSong, FALSE, FALSE );

					break;
				}

				case ID_PLAY_GOFROMCURSOR :
				{
					OnPlayGo( &TheSong, TRUE, FALSE );

					break;
				}

				case ID_PLAY_STOP :
				{
					OnPlayStop( TRUE );

					break;
				}

				case ID_PLAY_LOOP :
				{
					OnPlayGo( &TheSong, TRUE, TRUE );

					break;
				}

				case ID_PLAY_SWINGIT :
				{
					m_blSwingIt = !m_blSwingIt;

					break;
				}

				case ID_PLAY_PLAYSTRUCTUREDSONG :
				{
					OnPlayGo( &TheStructuredSong, FALSE, FALSE );

					break;
				}


				// Record menu

				case ID_RECORD_GO :
				{
					OnRecordGo();

					break;
				}

				case ID_RECORD_STOP :
				{
					OnRecordStop();

					break;
				}



				// Note menu

				case ID_NOTE_THIRTYSECOND :
				{
					NoteMenuSetChecks( hMenu, NOTE_THIRTYSECOND, LOWORD( wParam ) );
					break;
				}

				case ID_NOTE_SIXTEENTH :
				{
					NoteMenuSetChecks( hMenu, NOTE_SIXTEENTH, LOWORD( wParam ) );
					break;
				}

				case ID_NOTE_SIXTEENTHD :
				{
					NoteMenuSetChecks( hMenu, NOTE_SIXTEENTHD, LOWORD( wParam ) );
					break;
				}

				case ID_NOTE_EIGHTH :
				{
					NoteMenuSetChecks( hMenu, NOTE_EIGHTH, LOWORD( wParam ) );
					break;
				}

				case ID_NOTE_EIGHTHD :
				{
					NoteMenuSetChecks( hMenu, NOTE_EIGHTHD, LOWORD( wParam ) );
					break;
				}

				case ID_NOTE_QUARTER :
				{
					NoteMenuSetChecks( hMenu, NOTE_QUARTER, LOWORD( wParam ) );
					break;
				}

				case ID_NOTE_QUARTERD :
				{
					NoteMenuSetChecks( hMenu, NOTE_QUARTERD, LOWORD( wParam ) );
					break;
				}

				case ID_NOTE_HALF :
				{
					NoteMenuSetChecks( hMenu, NOTE_HALF, LOWORD( wParam ) );
					break;
				}

				case ID_NOTE_HALFD :
				{
					NoteMenuSetChecks( hMenu, NOTE_HALFD, LOWORD( wParam ) );
					break;
				}

				case ID_NOTE_WHOLE :
				{
					NoteMenuSetChecks( hMenu, NOTE_WHOLE, LOWORD( wParam ) );
					break;
				}

				case ID_NOTE_CHORD :
				{
					switch (TheCursor.m_blChord)
					{
						case TRUE :
							TheCursor.m_blChord = FALSE;
							CheckMenuItem( hMenu, ID_NOTE_CHORD, MF_UNCHECKED );
							::SendMessage(	m_hwndToolbar1, 
											TB_PRESSBUTTON, 
											(WPARAM)ID_NOTE_CHORD, 
											(LPARAM)MAKELONG(FALSE, 0 )	);
							break;

						case FALSE :
							TheCursor.m_blChord = TRUE;
							CheckMenuItem( hMenu, ID_NOTE_CHORD, MF_CHECKED );
							::SendMessage(	m_hwndToolbar1, 
											TB_PRESSBUTTON, 
											(WPARAM)ID_NOTE_CHORD, 
											MAKELONG(TRUE, 0 )	);
							break;
					}

					break;
				}

				case ID_NOTE_PERCUSSION :
				{
					::HideCaret( hwnd );

					TheCursor.OnPercussion();

					// move caret
					::SetCaretPos(	TheCursor.GetXpos() - m_iHScrollPos, 
									TheCursor.GetYpos() - m_iVScrollPos	);
					
					::ShowCaret( hwnd );

					break;
				}


				// Change menu

				case ID_CHANGE_TRANSPOSE_NOTES :
				{
					if (TheCursor.TransposeNotes( m_hInstance, hwnd ))
					{
						TheSong.Normalize();
						Redraw();
						MakeCaret();
					}

					break;
				}

				case ID_CHANGE_TRANSPOSE_DIATONIC :
				{
					if (TheCursor.TransposeDiatonic( m_hInstance, hwnd ))
					{
						TheSong.Normalize();
						Redraw();
						MakeCaret();
					}

					break;
				}


				// Compose menu

				case ID_COMPOSE_MACRO :
				{
					if (TheCursor.ComposeMacro( m_hInstance, hwnd ))
					{
						Redraw();
						MakeCaret();
					}

					break;
				}

				case ID_COMPOSE_DRUM_PART :
				{
					if (TheCursor.ComposeDrumPart( m_hInstance, hwnd ))
					{
						TheSong.Normalize();
						Redraw();
						MakeCaret();
					}

					break;
				}

				case ID_COMPOSE_GUITAR_CHORD :
				{
					if (TheCursor.ComposeGuitarChord( m_hInstance, hwnd ))
					{
						// make sure scroll happens
						::SendMessage( m_hwndClientWindow, WM_KEYDOWN, 0, 0 );
						
						TheSong.Normalize();
						TheCursor.ResetPitch();

						Redraw();
						MakeCaret();
					}

					break;
				}

				case ID_COMPOSE_CHORDPROGRESSION :
				{
					if (TheCursor.ComposeChordProgression( m_hInstance, hwnd ))
					{
						TheSong.Normalize();
						TheCursor.ResetPitch();

						Redraw();
						MakeCaret();
					}

					break;
				}

				case ID_COMPOSE_MELODY :
				{
					if (TheCursor.ComposeMelody( m_hInstance, hwnd ))
					{
						TheSong.Normalize();
						TheCursor.ResetPitch();

						Redraw();
						MakeCaret();
					}

					break;
				}

				case ID_COMPOSE_HARMONIZE :
				{
					if (TheCursor.ComposeHarmonize( m_hInstance, hwnd ))
					{
						TheSong.Normalize();
						Redraw();
						MakeCaret();
					}

					break;
				}

				case ID_COMPOSE_SONGANDMELODY1 :
				{
					// show wait cursor
					HCURSOR hPreviousCursor = ::SetCursor( m_hWaitCursor );

					OnComposeSongAABA();

					// show previous cursor
					::SetCursor( hPreviousCursor );

					break;
				}

				case ID_COMPOSE_SONGANDMELODY2 :
				{
					// show wait cursor
					HCURSOR hPreviousCursor = ::SetCursor( m_hWaitCursor );

					OnComposeSong( TRUE );

					// show previous cursor
					::SetCursor( hPreviousCursor );

					break;
				}

				case ID_COMPOSE_SONGNOMELODY :
				{
					// show wait cursor
					HCURSOR hPreviousCursor = ::SetCursor( m_hWaitCursor );

					OnComposeSong( FALSE );

					// show previous cursor
					::SetCursor( hPreviousCursor );

					break;
				}

				case ID_COMPOSE_SONGS_ :
				{
					// show wait cursor
					HCURSOR hPreviousCursor = ::SetCursor( m_hWaitCursor );

					OnComposeSongs();

					// show previous cursor
					::SetCursor( hPreviousCursor );

					break;
				}

				case ID_COMPOSE_DEFINESECTION :
				{
					TheStructuredSong.DefineSection();

					break;
				}

				case ID_COMPOSE_SONGSTRUCTURE :
				{
					TheStructuredSong.EditStructure();

					break;
				}

				case ID_COMPOSE_CLICKTRACK :
				{
					if (TheCursor.ComposeClickTrack( m_hInstance, hwnd ))
					{
						TheSong.Normalize();
						Redraw();
						MakeCaret();
					}

					break;
				}


				// Help menu

				case ID_HELP_TOPICS :
				{
					::WinHelp( hwnd, "Bach2.hlp", HELP_FINDER, 0L );

					break;
				}

				case ID_HELP_ABOUT :
				{
					DialogBox( m_hInstance, "AboutBox", hwnd, (DLGPROC)DialogProcAboutBox );

					break;
				}


				// Debug menu

				case ID_DEBUG_NORMALIZE :
				{

					break;
				}

				case ID_DEBUG_RESET :
				{
					lstrcpy( m_szFilePath, DEFAULT_FILEPATH );

					TheSong.Reset();

					MakeCaret();

					break;
				}

				case ID_DEBUG_DUMPSONG :
				{
					DumpSong( &TheSong );

					break;
				}


				// Popup menu

				case ID_POPUP_MUTE :
				{
					TheSong.MuteVoice( TheCursor.GetVoice() );
					Redraw();

					break;
				}

				case ID_POPUP_CLEARALL :
				{

					break;
				}

				case ID_POPUP_CLEF_TREBLE :
				{

					break;
				}

				case ID_POPUP_CLEF_BASS :
				{

					break;
				}


				// Popup channel menu

				case ID_POPUP_CHANNEL_1 :
				{

					break;
				}

				case ID_POPUP_CHANNEL_2 :
				{

					break;
				}

				case ID_POPUP_CHANNEL_3 :
				{

					break;
				}

				case ID_POPUP_CHANNEL_4 :
				{

					break;
				}

				case ID_POPUP_CHANNEL_5 :
				{

					break;
				}

				case ID_POPUP_CHANNEL_6 :
				{

					break;
				}

				case ID_POPUP_CHANNEL_7 :
				{

					break;
				}

				case ID_POPUP_CHANNEL_8 :
				{

					break;
				}

				case ID_POPUP_CHANNEL_9 :
				{

					break;
				}

				case ID_POPUP_CHANNEL_10 :
				{

					break;
				}

				case ID_POPUP_CHANNEL_11 :
				{

					break;
				}

				case ID_POPUP_CHANNEL_12 :
				{

					break;
				}

				case ID_POPUP_CHANNEL_13 :
				{

					break;
				}

				case ID_POPUP_CHANNEL_14 :
				{

					break;
				}

				case ID_POPUP_CHANNEL_15 :
				{

					break;
				}

				case ID_POPUP_CHANNEL_16 :
				{

					break;
				}


				// Popup patch menu

				case ID_POPUP_PATCH_PIANO_ACOUSTICGRANDPIANO :
				{

					break;
				}

				case ID_POPUP_PATCH_PIANO_BRIGHTACOUSTICPIANO :
				{

					break;
				}

				case ID_POPUP_PATCH_PIANO_ELECTRICGRANDPIANO :
				{

					break;
				}

				case ID_POPUP_PATCH_PIANO_HONKYTONKPIANO :
				{

					break;
				}

				case ID_POPUP_PATCH_PIANO_RHODESPIANO :
				{

					break;
				}

				case ID_POPUP_PATCH_PIANO_CHORUSEDPIANO :
				{

					break;
				}

				case ID_POPUP_PATCH_PIANO_HARPSICHORD :
				{

					break;
				}

				case ID_POPUP_PATCH_PIANO_CLAVINET :
				{

					break;
				}

				case ID_POPUP_PATCH_CHROMATICPERCUSSION_CELESTA :
				{

					break;
				}

				case ID_POPUP_PATCH_CHROMATICPERCUSSION_GLOCKENSPIEL :
				{

					break;
				}

				case ID_POPUP_PATCH_CHROMATICPERCUSSION_MUSICBOX :
				{

					break;
				}

				case ID_POPUP_PATCH_CHROMATICPERCUSSION_VIBRAPHONE :
				{

					break;
				}

				case ID_POPUP_PATCH_CHROMATICPERCUSSION_MARIMBA :
				{

					break;
				}

				case ID_POPUP_PATCH_CHROMATICPERCUSSION_XYLOPHONE :
				{

					break;
				}

				case ID_POPUP_PATCH_CHROMATICPERCUSSION_TUBULARBELLS :
				{

					break;
				}

				case ID_POPUP_PATCH_CHROMATICPERCUSSION_DULCIMER :
				{

					break;
				}

				case ID_POPUP_PATCH_ORGAN_DRAWBARORGAN :
				{

					break;
				}

				case ID_POPUP_PATCH_ORGAN_PERCUSSIVEORGAN :
				{

					break;
				}

				case ID_POPUP_PATCH_ORGAN_ROCKORGAN :
				{

					break;
				}

				case ID_POPUP_PATCH_ORGAN_CHURCHORGAN :
				{

					break;
				}

				case ID_POPUP_PATCH_ORGAN_REEDORGAN :
				{

					break;
				}

				case ID_POPUP_PATCH_ORGAN_ACCORDION :
				{

					break;
				}

				case ID_POPUP_PATCH_ORGAN_HARMONICA :
				{

					break;
				}

				case ID_POPUP_PATCH_ORGAN_TANGOACCORDION :
				{

					break;
				}

				case ID_POPUP_PATCH_GUITAR_ACOUSTICGUITARNYLON :
				{

					break;
				}

				case ID_POPUP_PATCH_GUITAR_ACOUSTICGUITARSTEEL :
				{

					break;
				}

				case ID_POPUP_PATCH_GUITAR_ELECTRICGUITARJAZZ :
				{

					break;
				}

				case ID_POPUP_PATCH_GUITAR_ELECTRICGUITARCLEAN :
				{

					break;
				}

				case ID_POPUP_PATCH_GUITAR_ELECTRICGUITARMUTED :
				{

					break;
				}

				case ID_POPUP_PATCH_GUITAR_OVERDRIVENGUITAR :
				{

					break;
				}

				case ID_POPUP_PATCH_GUITAR_DISTORTIONGUITAR :
				{

					break;
				}

				case ID_POPUP_PATCH_GUITAR_GUITARHARMONICS :
				{

					break;
				}

				case ID_POPUP_PATCH_BASS_ACOUSTICBASS :
				{

					break;
				}

				case ID_POPUP_PATCH_BASS_ELECTRICBASSFINGER :
				{

					break;
				}

				case ID_POPUP_PATCH_BASS_ELECTRICBASSPICK :
				{

					break;
				}

				case ID_POPUP_PATCH_BASS_FRETLESSBASS :
				{

					break;
				}

				case ID_POPUP_PATCH_BASS_SLAPBASS1 :
				{

					break;
				}

				case ID_POPUP_PATCH_BASS_SLAPBASS2 :
				{

					break;
				}

				case ID_POPUP_PATCH_BASS_SYNTHBASS1 :
				{

					break;
				}

				case ID_POPUP_PATCH_BASS_SYNTHBASS2 :
				{

					break;
				}

				case ID_POPUP_PATCH_STRINGSORCHESTRA_VIOLIN :
				{

					break;
				}

				case ID_POPUP_PATCH_STRINGSORCHESTRA_VIOLA :
				{

					break;
				}

				case ID_POPUP_PATCH_STRINGSORCHESTRA_CELLO :
				{

					break;
				}

				case ID_POPUP_PATCH_STRINGSORCHESTRA_CONTRABASS :
				{

					break;
				}

				case ID_POPUP_PATCH_STRINGSORCHESTRA_TREMOLOSTRINGS :
				{

					break;
				}

				case ID_POPUP_PATCH_STRINGSORCHESTRA_PIZZICATOSTRINGS :
				{

					break;
				}

				case ID_POPUP_PATCH_STRINGSORCHESTRA_ORCHESTRALHARP :
				{

					break;
				}

				case ID_POPUP_PATCH_STRINGSORCHESTRA_TIMPANI :
				{

					break;
				}

				case ID_POPUP_PATCH_ENSEMBLE_STRINGENSEMBLE1 :
				{

					break;
				}

				case ID_POPUP_PATCH_ENSEMBLE_STRINGENSEMBLE2 :
				{

					break;
				}

				case ID_POPUP_PATCH_ENSEMBLE_SYNTHSTRINGS1 :
				{

					break;
				}

				case ID_POPUP_PATCH_ENSEMBLE_SYNTHSTRINGS2 :
				{

					break;
				}

				case ID_POPUP_PATCH_ENSEMBLE_CHOIRAAHS :
				{

					break;
				}

				case ID_POPUP_PATCH_ENSEMBLE_VOICEOOHS :
				{

					break;
				}

				case ID_POPUP_PATCH_ENSEMBLE_SYNTHVOICE :
				{

					break;
				}

				case ID_POPUP_PATCH_ENSEMBLE_ORCHESTRAHIT :
				{

					break;
				}

				case ID_POPUP_PATCH_BRASS_TRUMPET :
				{

					break;
				}

				case ID_POPUP_PATCH_BRASS_TROMBONE :
				{

					break;
				}

				case ID_POPUP_PATCH_BRASS_TUBA :
				{

					break;
				}

				case ID_POPUP_PATCH_BRASS_MUTEDTRUMPET :
				{

					break;
				}

				case ID_POPUP_PATCH_BRASS_FRENCHHORN :
				{

					break;
				}

				case ID_POPUP_PATCH_BRASS_BRASSSECTION :
				{

					break;
				}

				case ID_POPUP_PATCH_BRASS_SYNTHBRASS1 :
				{

					break;
				}

				case ID_POPUP_PATCH_BRASS_SYNTHBRASS2 :
				{

					break;
				}

				case ID_POPUP_PATCH_REED_SOPRANOSAX :
				{

					break;
				}

				case ID_POPUP_PATCH_REED_ALTOSAX :
				{

					break;
				}

				case ID_POPUP_PATCH_REED_TENORSAX :
				{

					break;
				}

				case ID_POPUP_PATCH_REED_BARITONESAX :
				{

					break;
				}

				case ID_POPUP_PATCH_REED_OBOE :
				{

					break;
				}

				case ID_POPUP_PATCH_REED_ENGLISHHORN :
				{

					break;
				}

				case ID_POPUP_PATCH_REED_BASSOON :
				{

					break;
				}

				case ID_POPUP_PATCH_REED_CLARINET :
				{

					break;
				}

				case ID_POPUP_PATCH_PIPE_PICCOLO :
				{

					break;
				}

				case ID_POPUP_PATCH_PIPE_FLUTE :
				{

					break;
				}

				case ID_POPUP_PATCH_PIPE_RECORDER :
				{

					break;
				}

				case ID_POPUP_PATCH_PIPE_PANFLUTE :
				{

					break;
				}

				case ID_POPUP_PATCH_PIPE_BLOWNBOTTLE :
				{

					break;
				}

				case ID_POPUP_PATCH_PIPE_SHAKUHACHI :
				{

					break;
				}

				case ID_POPUP_PATCH_PIPE_WHISTLE :
				{

					break;
				}

				case ID_POPUP_PATCH_PIPE_OCARINA :
				{

					break;
				}

				case ID_POPUP_PATCH_SYNTHLEAD_LEAD1SQUARE :
				{

					break;
				}

				case ID_POPUP_PATCH_SYNTHLEAD_LEAD2SAWTOOTH :
				{

					break;
				}

				case ID_POPUP_PATCH_SYNTHLEAD_LEAD3CALLIOPE :
				{

					break;
				}

				case ID_POPUP_PATCH_SYNTHLEAD_LEAD4CHIFF :
				{

					break;
				}

				case ID_POPUP_PATCH_SYNTHLEAD_LEAD5CHARANG :
				{

					break;
				}

				case ID_POPUP_PATCH_SYNTHLEAD_LEAD6VOICE :
				{

					break;
				}

				case ID_POPUP_PATCH_SYNTHLEAD_LEAD7FIFTHS :
				{

					break;
				}

				case ID_POPUP_PATCH_SYNTHLEAD_LEAD8BASSLEAD :
				{

					break;
				}

				case ID_POPUP_PATCH_SYNTHPAD_PAD1NEWAGE :
				{

					break;
				}

				case ID_POPUP_PATCH_SYNTHPAD_PAD2WARM :
				{

					break;
				}

				case ID_POPUP_PATCH_SYNTHPAD_PAD3POLYSYNTH :
				{

					break;
				}

				case ID_POPUP_PATCH_SYNTHPAD_PAD4CHOIR :
				{

					break;
				}

				case ID_POPUP_PATCH_SYNTHPAD_PAD5BOWED :
				{

					break;
				}

				case ID_POPUP_PATCH_SYNTHPAD_PAD6METALLIC :
				{

					break;
				}

				case ID_POPUP_PATCH_SYNTHPAD_PAD7HALO :
				{

					break;
				}

				case ID_POPUP_PATCH_SYNTHPAD_PAD8SWEEP :
				{

					break;
				}

				case ID_POPUP_PATCH_SYNTHSFX_RAIN :
				{

					break;
				}

				case ID_POPUP_PATCH_SYNTHSFX_SOUNDTRACK :
				{

					break;
				}

				case ID_POPUP_PATCH_SYNTHSFX_CRYSTAL :
				{

					break;
				}

				case ID_POPUP_PATCH_SYNTHSFX_ATMOSPHERE :
				{

					break;
				}

				case ID_POPUP_PATCH_SYNTHSFX_BRIGHTNESS :
				{

					break;
				}

				case ID_POPUP_PATCH_SYNTHSFX_GOBLINS :
				{

					break;
				}

				case ID_POPUP_PATCH_SYNTHSFX_ECHOES :
				{

					break;
				}

				case ID_POPUP_PATCH_SYNTHSFX_SCIFI :
				{

					break;
				}

				case ID_POPUP_PATCH_ETHNIC_SITAR :
				{

					break;
				}

				case ID_POPUP_PATCH_ETHNIC_BANJO :
				{

					break;
				}

				case ID_POPUP_PATCH_ETHNIC_SHAMISEN :
				{

					break;
				}

				case ID_POPUP_PATCH_ETHNIC_KOTO :
				{

					break;
				}

				case ID_POPUP_PATCH_ETHNIC_KALIMBA :
				{

					break;
				}

				case ID_POPUP_PATCH_ETHNIC_BAGPIPE :
				{

					break;
				}

				case ID_POPUP_PATCH_ETHNIC_FIDDLE :
				{

					break;
				}

				case ID_POPUP_PATCH_ETHNIC_SHANAI :
				{

					break;
				}

				case ID_POPUP_PATCH_PERCUSSIVE_TINKLEBELL :
				{

					break;
				}

				case ID_POPUP_PATCH_PERCUSSIVE_AGOGO :
				{

					break;
				}

				case ID_POPUP_PATCH_PERCUSSIVE_STEELDRUMS :
				{

					break;
				}

				case ID_POPUP_PATCH_PERCUSSIVE_WOODBLOCK :
				{

					break;
				}

				case ID_POPUP_PATCH_PERCUSSIVE_TAIKODRUM :
				{

					break;
				}

				case ID_POPUP_PATCH_PERCUSSIVE_MELODICDRUM :
				{

					break;
				}

				case ID_POPUP_PATCH_PERCUSSIVE_SYNTHDRUM :
				{

					break;
				}

				case ID_POPUP_PATCH_PERCUSSIVE_REVERSECYMBAL :
				{

					break;
				}

				case ID_POPUP_PATCH_SFX_GUITARFRETNOISE :
				{

					break;
				}

				case ID_POPUP_PATCH_SFX_BREATHNOISE :
				{

					break;
				}

				case ID_POPUP_PATCH_SFX_SEASHORE :
				{

					break;
				}

				case ID_POPUP_PATCH_SFX_BIRDTWEET :
				{

					break;
				}

				case ID_POPUP_PATCH_SFX_TELEPHONERING :
				{

					break;
				}

				case ID_POPUP_PATCH_SFX_HELICOPTER :
				{

					break;
				}

				case ID_POPUP_PATCH_SFX_APPLAUSE :
				{

					break;
				}

				case ID_POPUP_PATCH_SFX_GUNSHOT :
				{

					break;
				}

				
				default :
					return 0;
			}

			return 0;
		}

		default :

			return ::DefWindowProc( hwnd, uMsg, wParam, lParam );
	}

	return 0; 
}


//////////////////////////////////////////////////////////////////////////////
//	Client window proc

// stub proc
LRESULT CALLBACK ClientWindowProc(	HWND hwnd,
									UINT uMsg,
									WPARAM wParam,
									LPARAM lParam	)
{
	return CApplication::ClientWindowProc( hwnd, uMsg, wParam, lParam );
}

LRESULT CApplication::ClientWindowProc(	HWND hwnd,
											UINT uMsg,
											WPARAM wParam,
											LPARAM lParam	)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (uMsg)
	{
		case WM_PAINT :
		{
			HideCaret( ::GetParent( hwnd ) );

			hdc = ::BeginPaint( hwnd, &ps );

			// draw song
			TheSong.Draw( hdc );

			// draw selection, if any
			TheCursor.m_Selection.Draw( hdc );

			::EndPaint( hwnd, &ps );

			ShowCaret( ::GetParent( hwnd ) );

			return 0;
		}

		case WM_SIZE :
		{
			try
			{
				SCROLLINFO si;
				ZeroMemory( &si, sizeof SCROLLINFO );

				// set horizontal scroll info

				si.cbSize = sizeof SCROLLINFO;
				si.fMask = SIF_ALL;
				si.nMin = 0;
				si.nMax = TheSong.GetLayoutSize().cx + 20;
				si.nPage = NOTE_WIDTH * TheSong.GetTicksPerMeasure() + 2;
				si.nPos = m_iHScrollPos;

				SetScrollInfo(	hwnd,
								SB_HORZ,
								&si,
								TRUE	);

				// set vertical scroll info

				si.nMin = 0;
				si.nMax = TheSong.GetLayoutSize().cy + 20;
				si.nPage = m_iClientWindowHeight;
				si.nPos = m_iVScrollPos;

				SetScrollInfo(	hwnd,
								SB_VERT,
								&si,
								TRUE	);
			}
			catch (...)
			{
			}

			return 0;
		}

		case WM_SETFOCUS :
		{
			MakeCaret();

			return 0;
		}

		case WM_HSCROLL :
		{
			int iOldHScrollPos = m_iHScrollPos;

			switch (LOWORD( wParam ))
			{
				case SB_LINEUP :
					m_iHScrollPos -= NOTE_WIDTH * NOTE_TICKSPERQUARTER;
					break;
				case SB_LINEDOWN :
					m_iHScrollPos += NOTE_WIDTH * NOTE_TICKSPERQUARTER;
					break;
				case SB_PAGEUP :
					m_iHScrollPos -= NOTE_WIDTH * TheSong.GetTicksPerMeasure() + 2;
					break;
				case SB_PAGEDOWN :
					m_iHScrollPos += NOTE_WIDTH * TheSong.GetTicksPerMeasure() + 2;
					break;
				case SB_THUMBPOSITION :
//					m_iHScrollPos = HIWORD( wParam );
					m_iHScrollPos = ::GetScrollPos( hwnd, SB_HORZ );
					break;
				case SB_THUMBTRACK :
					m_iHScrollPos = HIWORD( wParam );
					break;
				case SB_TOP :
					m_iHScrollPos = 0;
					break;
				case SB_BOTTOM :
					m_iHScrollPos = 1000;
					break;
				case SB_ENDSCROLL :
					break;
			}

			m_iHScrollPos = max( 0, 
								 min( TheSong.GetLayoutSize().cx - m_iClientWindowWidth + 20,
									  m_iHScrollPos ) );

			if (m_iHScrollPos != iOldHScrollPos)
			{
				::HideCaret( ::GetParent( hwnd ) );

				::ScrollWindowEx(	hwnd, 
									iOldHScrollPos - m_iHScrollPos, 
									0, 
									(CONST RECT *) NULL, 
									(CONST RECT *) NULL, 
									(HRGN) NULL, 
									(LPRECT) NULL, 
									SW_INVALIDATE	); 

				SCROLLINFO si;
				::ZeroMemory( &si, sizeof SCROLLINFO );

				si.cbSize = sizeof SCROLLINFO;
				si.fMask = SIF_POS;
				si.nPos = m_iHScrollPos;

				::SetScrollInfo(	hwnd,
									SB_HORZ,
									&si,
									TRUE	);

				::InvalidateRect( hwnd, NULL, TRUE );

				TheCursor.SetXOrigin( m_iHScrollPos );

				::SetCaretPos(	TheCursor.GetXpos() - m_iHScrollPos,
								TheCursor.GetYpos() - m_iVScrollPos  );

				::ShowCaret( ::GetParent( hwnd ) );
			}

			return 0;
		}

		case WM_VSCROLL :
		{
			int iOldVScrollPos = m_iVScrollPos;

			switch (LOWORD( wParam ))
			{
				case SB_LINEUP :
					m_iVScrollPos -= VOICE_HEIGHT;
					break;
				case SB_LINEDOWN :
					m_iVScrollPos += VOICE_HEIGHT;
					break;
				case SB_PAGEUP :
					m_iVScrollPos -= m_iClientWindowHeight;
					break;
				case SB_PAGEDOWN :
					m_iVScrollPos += m_iClientWindowHeight;
					break;
				case SB_THUMBPOSITION :
//					m_iVScrollPos = HIWORD( wParam );
					m_iVScrollPos = ::GetScrollPos( hwnd, SB_VERT );
					break;
				case SB_THUMBTRACK :
					m_iVScrollPos = HIWORD( wParam );
					break;
				case SB_TOP :
					m_iVScrollPos = 0;
					break;
				case SB_BOTTOM :
					m_iVScrollPos = TheSong.GetLayoutSize().cx + 20;
					break;
				case SB_ENDSCROLL :
					break;
			}

			m_iVScrollPos = max( 0, 
								 min( TheSong.GetLayoutSize().cy - m_iClientWindowHeight + 20, 
									  m_iVScrollPos ) );

			if (m_iVScrollPos != iOldVScrollPos)
			{
				::HideCaret( ::GetParent( hwnd ) );

				::ScrollWindowEx(	hwnd, 
									0, 
									iOldVScrollPos - m_iVScrollPos, 
									(CONST RECT *) NULL, 
									(CONST RECT *) NULL, 
									(HRGN) NULL, 
									(LPRECT) NULL, 
									SW_INVALIDATE	); 

				SCROLLINFO si;
				::ZeroMemory( &si, sizeof SCROLLINFO );

				si.cbSize = sizeof SCROLLINFO;
				si.fMask = SIF_POS;
				si.nPos = m_iVScrollPos;

				::SetScrollInfo(	hwnd,
									SB_VERT,
									&si,
									TRUE	);

				::InvalidateRect( hwnd, NULL, TRUE );

				TheCursor.SetYOrigin( m_iToolbar1Height - m_iVScrollPos );

				::SetCaretPos(	TheCursor.GetXpos() - m_iHScrollPos,
								TheCursor.GetYpos() - m_iVScrollPos  );

				::ShowCaret( ::GetParent( hwnd ) );
			}

			return 0;
		}

		////////////////////////////////////////
		// mouse
		//
		//	LOWORD( lParam ) = x-coordinate
		//	HIWORD( lParam ) = y-coordinate
		//	wParam = state

		case WM_MOUSEMOVE :
		{
			if (!m_blCursorIsShowing)
			{
//				::ShowCursor( TRUE );
				m_blCursorIsShowing = TRUE;
			}

			return 0;
		}

		case WM_LBUTTONDOWN :
		{
			if (!m_blCursorIsShowing)
			{
//				::ShowCursor( TRUE );
				m_blCursorIsShowing = TRUE;
			}

			return 0;
		}

		case WM_LBUTTONUP :
		{
			if (!m_blCursorIsShowing)
			{
//				::ShowCursor( TRUE );
				m_blCursorIsShowing = TRUE;
			}

			POINT p;

			p.x = LOWORD( lParam ) + m_iHScrollPos;
			p.y = HIWORD( lParam ) + m_iVScrollPos;

			int iVoice = TheSong.GetVoiceAtPoint( p );
			int iTick = TheSong.GetTickAtPoint( p );

			if (iVoice >= 0 && iTick >= 0)
			{
				::HideCaret( hwnd );

				TheCursor.MoveToVoice( iVoice );
				TheCursor.MoveToAbsoluteTick( iTick );

				UpdateStatusBar();

				// move caret
				::SetCaretPos(	TheCursor.GetXpos() - m_iHScrollPos, 
								TheCursor.GetYpos() - m_iVScrollPos	);

				::ShowCaret( hwnd );
			}
			else
			{
				::MessageBeep( MB_OK );
			}

			return 0;
		}

		case WM_LBUTTONDBLCLK :
		{
			POINT p;

			p.x = LOWORD( lParam ) + m_iHScrollPos;
			p.y = HIWORD( lParam ) + m_iVScrollPos;

			int iVoice = TheSong.GetVoiceAtPoint( p );

			if (iVoice >= 0)
			{
				TheSong.OnSettingsVoice( m_hInstance, hwnd, TheCursor.GetVoice() );
			}
			else
			{
				::MessageBeep( MB_OK );
			}

			return 0;
		}

		case WM_CONTEXTMENU :
		{
			POINT p;

			p.x = LOWORD( lParam ) + m_iHScrollPos;
			p.y = HIWORD( lParam ) + m_iVScrollPos;

			::ScreenToClient( hwnd, &p );

			int iVoice = TheSong.GetVoiceAtPoint( p );

			if (iVoice >= 0)
			{
				::HideCaret( hwnd );

				TheCursor.MoveToVoice( iVoice );

				UpdateStatusBar();

				// move caret
				::SetCaretPos(	TheCursor.GetXpos() - m_iHScrollPos, 
								TheCursor.GetYpos() - m_iVScrollPos	);

				if (TheSong.IsMute( iVoice ))
				{
					::CheckMenuItem( m_hPopupMenu, ID_POPUP_MUTE, MF_CHECKED );
				}
				else
				{
					::CheckMenuItem( m_hPopupMenu, ID_POPUP_MUTE, MF_UNCHECKED );
				}

				::TrackPopupMenuEx(	m_hPopupMenu,
									TPM_LEFTALIGN | TPM_RIGHTBUTTON,
									LOWORD( lParam ),
									HIWORD( lParam ),
									m_hwndMainWindow,
									NULL	);

   				::ShowCaret( hwnd );
			}
			else
			{
				::MessageBeep( MB_OK );
			}

			return 0;
		}


		case WM_KEYDOWN :	// process wParam as virtual key code
		{
			HDC hdc = GetDC( hwnd );

			if (m_blCursorIsShowing)
			{
//				::ShowCursor( FALSE );
				m_blCursorIsShowing = FALSE;
			}

			::HideCaret( hwnd );

			BOOL blPassOnToCursor = TRUE;
			BOOL blAdjustScrollPosition = FALSE;

			// on Ctrl+arrow, scroll window
			if (::GetKeyState( VK_CONTROL ) & 0x80)
			{
				switch (wParam)
				{
					case VK_RIGHT :
					case 222 :	// '''

						::SendMessage(	hwnd,
										WM_HSCROLL,
										MAKELONG( (WORD)SB_LINEDOWN, 0 ),
										0L	);
						
						blPassOnToCursor = FALSE;
						blAdjustScrollPosition = TRUE;
									
						break;

					case 76 :	// 'l'
					case VK_LEFT :
						
						::SendMessage(	hwnd,
										WM_HSCROLL,
										MAKELONG( (WORD)SB_LINEUP, 0 ),
										0L	);
						
						blPassOnToCursor = FALSE;
						blAdjustScrollPosition = TRUE;
									
						break;

					default :

						blPassOnToCursor = TRUE;
						blAdjustScrollPosition = FALSE;
									
						break;

				}
			}

			// if cursor is off-screen, move it on-screen
			if (TheCursor.GetXpos() >= m_iHScrollPos + m_iClientWindowWidth - 20 ||
				TheCursor.GetXpos() <= m_iHScrollPos)
			{
				blPassOnToCursor = TRUE;
			}

			if (::GetKeyState( VK_SHIFT ) & 0x80)
			{
				blPassOnToCursor = TRUE;
			}

			if (blPassOnToCursor)
			{
				// let cursor process keystroke
				int iResult = TheCursor.OnKeyDown( wParam, hdc );
				
				if (iResult & UPDATE_VIEW)
				{
					InvalidateRect( hwnd, NULL, TRUE );
				}

				if (iResult & UPDATE_SCROLL)
				{
					blAdjustScrollPosition = TRUE;
				}
			}

			if (wParam == 0)
			{
				blAdjustScrollPosition = TRUE;
			}

			if (blAdjustScrollPosition)
			{
				// scroll to beginning if in measure 0
				if (TheCursor.GetMeasure() == 0)
				{
					::SendMessage(	hwnd, 
									WM_HSCROLL, 
									MAKELONG( (WORD)SB_TOP, 0 ), 
									0L	);
				}

				// cursor onscreen or close to it (horizontally)?
				if(
					(TheCursor.GetXpos() >= m_iHScrollPos - (NOTE_WIDTH * NOTE_TICKSPERQUARTER)) && 
					(TheCursor.GetXpos() <= m_iHScrollPos + m_iClientWindowWidth + 
						(NOTE_WIDTH * NOTE_TICKSPERQUARTER)))
				{
					// scroll right or left 
					// if cursor moves off screen
					while (TheCursor.GetXpos() >= m_iHScrollPos + m_iClientWindowWidth - 20)
					{
						::SendMessage(	hwnd, 
										WM_HSCROLL, 
										MAKELONG( SB_LINEDOWN, 0 ),
										0L	);
					}
					while (TheCursor.GetXpos() <= m_iHScrollPos)
					{
						::SendMessage(	hwnd, 
										WM_HSCROLL, 
										MAKELONG( SB_LINEUP, 0 ),
										0L	);
					}
				}
				else // center cursor if offscreen
				{
					::SetScrollPos(	hwnd, 
									SB_HORZ, 
									TheCursor.GetXpos() - (m_iClientWindowWidth / 2), 
									TRUE	);
					::SendMessage(	hwnd, 
									WM_HSCROLL, 
									MAKELONG(	SB_THUMBPOSITION, 
												TheCursor.GetXpos() - (m_iClientWindowWidth / 2)),
									0L	);
				}

				// scroll down if necessary
				while (TheCursor.GetYpos() + CURSOR_HEIGHT >= m_iVScrollPos + m_iClientWindowHeight)
				{
					// escape!
					if (m_iVScrollPos >= TheSong.GetLayoutSize().cy - m_iClientWindowHeight) break;

					::SendMessage(	hwnd, 
									WM_VSCROLL, 
									MAKELONG( SB_LINEDOWN, 0 ),
									0L	);
				}
				// or scroll up
				while (TheCursor.GetYpos() <= m_iVScrollPos)
				{
					// escape!
					if (m_iVScrollPos <= 0) break;

					::SendMessage(	hwnd, 
									WM_VSCROLL, 
									MAKELONG( SB_LINEUP, 0 ),
									0L	);
				}
			}

			UpdateStatusBar();

			// move caret
			::SetCaretPos(	TheCursor.GetXpos() - m_iHScrollPos, 
							TheCursor.GetYpos() - m_iVScrollPos	);
			
			::ReleaseDC( hwnd, hdc );

			::ShowCaret( hwnd );

			return 0;
		}

		default :

			return ::DefWindowProc( hwnd, uMsg, wParam, lParam );
	}

	return 0;
}


//////////////////////////////////////////////////////////////////////////////
//	Overview window proc

// stub proc
LRESULT CALLBACK OverviewWindowProc(	HWND hwnd,
										UINT uMsg,
										WPARAM wParam,
										LPARAM lParam	)
{
	return CApplication::OverviewWindowProc( hwnd, uMsg, wParam, lParam );
}

LRESULT CApplication::OverviewWindowProc(	HWND hwnd,
											UINT uMsg,
											WPARAM wParam,
											LPARAM lParam	)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (uMsg)
	{
		case WM_PAINT :
		{
			hdc = ::BeginPaint( hwnd, &ps );

			// draw song
			TheSong.Draw( hdc );

			// draw selection, if any
//			TheCursor.m_Selection.Draw( hdc );

			::EndPaint( hwnd, &ps );

			return 0;
		}

		case WM_SIZE :
		{
			try
			{
				SCROLLINFO si;
				::ZeroMemory( &si, sizeof SCROLLINFO );

				// set horizontal scroll info

				si.cbSize = sizeof SCROLLINFO;
				si.fMask = SIF_ALL;
				si.nMin = 0;
				si.nMax = TheSong.GetLayoutSize().cx + 20;
				si.nPage = NOTE_WIDTH * TheSong.GetTicksPerMeasure() + 2;
				si.nPos = m_iHScrollPos;
/*
				::SetScrollInfo(	hwnd,
									SB_HORZ,
									&si,
									TRUE	);

				// set vertical scroll info

				si.nMin = 0;
				si.nMax = TheSong.GetLayoutSize().cy + 20;
				si.nPage = m_iClientWindowHeight;
				si.nPos = m_iVScrollPos;

				::SetScrollInfo(	hwnd,
									SB_VERT,
									&si,
									TRUE	);
*/
			}
			catch (...)
			{
			}

			return 0;
		}

		case WM_SETFOCUS :
		{

			return 0;
		}

		case WM_HSCROLL :
		{
			int iOldHScrollPos = m_iOverviewHScrollPos;

			switch (LOWORD( wParam ))
			{
				case SB_LINEUP :
					m_iOverviewHScrollPos -= NOTE_WIDTH * NOTE_TICKSPERQUARTER;
					break;
				case SB_LINEDOWN :
					m_iOverviewHScrollPos += NOTE_WIDTH * NOTE_TICKSPERQUARTER;
					break;
				case SB_PAGEUP :
					m_iOverviewHScrollPos -= NOTE_WIDTH * TheSong.GetTicksPerMeasure() + 2;
					break;
				case SB_PAGEDOWN :
					m_iOverviewHScrollPos += NOTE_WIDTH * TheSong.GetTicksPerMeasure() + 2;
					break;
				case SB_THUMBPOSITION :
					m_iOverviewHScrollPos = ::GetScrollPos( hwnd, SB_HORZ );
					break;
				case SB_THUMBTRACK :
					m_iOverviewHScrollPos = HIWORD( wParam );
					break;
				case SB_TOP :
					m_iOverviewHScrollPos = 0;
					break;
				case SB_BOTTOM :
					m_iOverviewHScrollPos = 1000;
					break;
				case SB_ENDSCROLL :
					break;
			}

			m_iOverviewHScrollPos = max( 0, 
								 min( TheSong.GetLayoutSize().cx - m_iClientWindowWidth + 20,
									  m_iHScrollPos ) );

/*
			if (m_iOverviewHScrollPos != iOldHScrollPos)
			{
				::ScrollWindowEx(	hwnd, 
									iOldHScrollPos - m_iOverviewHScrollPos, 
									0, 
									(CONST RECT *) NULL, 
									(CONST RECT *) NULL, 
									(HRGN) NULL, 
									(LPRECT) NULL, 
									SW_INVALIDATE	); 

				SCROLLINFO si;
				::ZeroMemory( &si, sizeof SCROLLINFO );

				si.cbSize = sizeof SCROLLINFO;
				si.fMask = SIF_POS;
				si.nPos = m_iOverviewHScrollPos;

				::SetScrollInfo(	hwnd,
									SB_HORZ,
									&si,
									TRUE	);

				::InvalidateRect( hwnd, NULL, TRUE );
			}
*/
			return 0;
		}

		case WM_VSCROLL :
		{
			int iOldVScrollPos = m_iOverviewVScrollPos;

			switch (LOWORD( wParam ))
			{
				case SB_LINEUP :
					m_iOverviewVScrollPos -= VOICE_HEIGHT;
					break;
				case SB_LINEDOWN :
					m_iOverviewVScrollPos += VOICE_HEIGHT;
					break;
				case SB_PAGEUP :
					m_iOverviewVScrollPos -= m_iClientWindowHeight;
					break;
				case SB_PAGEDOWN :
					m_iOverviewVScrollPos += m_iClientWindowHeight;
					break;
				case SB_THUMBPOSITION :
					m_iOverviewVScrollPos = ::GetScrollPos( hwnd, SB_VERT );
					break;
				case SB_THUMBTRACK :
					m_iOverviewVScrollPos = HIWORD( wParam );
					break;
				case SB_TOP :
					m_iOverviewVScrollPos = 0;
					break;
				case SB_BOTTOM :
					m_iOverviewVScrollPos = TheSong.GetLayoutSize().cx + 20;
					break;
				case SB_ENDSCROLL :
					break;
			}

			m_iOverviewVScrollPos = max( 0, 
										min( TheSong.GetLayoutSize().cy - m_iClientWindowHeight + 20, 
										m_iOverviewVScrollPos ) );
/*
			if (m_iOverviewVScrollPos != iOldVScrollPos)
			{
				::ScrollWindowEx(	hwnd, 
									0, 
									iOldVScrollPos - m_iOverviewVScrollPos, 
									(CONST RECT *) NULL, 
									(CONST RECT *) NULL, 
									(HRGN) NULL, 
									(LPRECT) NULL, 
									SW_INVALIDATE	); 

				SCROLLINFO si;
				::ZeroMemory( &si, sizeof SCROLLINFO );

				si.cbSize = sizeof SCROLLINFO;
				si.fMask = SIF_POS;
				si.nPos = m_iVScrollPos;

				::SetScrollInfo(	hwnd,
									SB_VERT,
									&si,
									TRUE	);

				::InvalidateRect( hwnd, NULL, TRUE );
			}
*/
			return 0;
		}

		////////////////////////////////////////
		// mouse
		//
		//	LOWORD( lParam ) = x-coordinate
		//	HIWORD( lParam ) = y-coordinate
		//	wParam = state

		case WM_MOUSEMOVE :
		{

			return 0;
		}

		case WM_LBUTTONDOWN :
		{

			return 0;
		}

		case WM_LBUTTONUP :
		{

			return 0;
		}

		case WM_LBUTTONDBLCLK :
		{

			return 0;
		}


		case WM_KEYDOWN :	// process wParam as virtual key code
		{

			return 0;
		}

		default :

			return ::DefWindowProc( hwnd, uMsg, wParam, lParam );
	}

	return 0;
}









