
#ifndef __CAPPLICATION_H__
#define __CAPPLICATION_H__

#include "stdafx.h"
#include "time.h"

#ifndef __CCURSOR_H__
#include "CCursor.h"
#endif


const char DEFAULT_FILEPATH[]		= "Bach1.bch";


class CSong;
class CStructuredSong;

class CApplication
{

public :

	friend DWORD		PlaySongsThread( LPVOID pParam );

	static void			Run( HINSTANCE hInstance, char* szCommandLine, int nCmdShow );
	
	static int			GetReturnValue() { return m_iReturnValue; }

	static LRESULT		MainWindowProc(	HWND hwnd,
										UINT uMsg,
										WPARAM wParam,
										LPARAM lParam	);
	static LRESULT		ClientWindowProc(	HWND hwnd,
											UINT uMsg,
											WPARAM wParam,
											LPARAM lParam	);
	static LRESULT		OverviewWindowProc(	HWND hwnd,
											UINT uMsg,
											WPARAM wParam,
											LPARAM lParam	);

	static void		SetModified( BOOL blModified );
	static void		UpdateDocumentSize();
	static void		Redraw();

	static HWND		GetMainWindowHandle() { return m_hwndMainWindow; }

	static int			GetXOrigin() { return -m_iHScrollPos; }
	static int			GetYOrigin() { return -m_iVScrollPos; }
	static HINSTANCE	GetInstance() { return m_hInstance; }
	static void			GetFilePath( char* szFilePath ) { lstrcpy( szFilePath, m_szFilePath ); }
	static int			GetMainWindowHeight() { return m_iMainWindowHeight; }
	static int			GetMainWindowWidth() { return m_iMainWindowWidth; }
	static int			GetClientWindowHeight() { return m_iClientWindowHeight; }
	static int			GetClientWindowWidth() { return m_iClientWindowWidth; }
	static int			GetToolbar1Height() { return m_iToolbar1Height; }
	static int			GetToolbar1Width() { return m_iToolbar1Width; }
	static int			GetStatusbarHeight() { return m_iStatusbarHeight; }
	static int			GetStatusbarWidth() { return m_iStatusbarWidth; }
	static int			GetZoomDivisor() { return m_iZoomDivisor; }

	static BOOL			IsGrandStaff() { return m_blGrandStaff; }

	static void	OnPlayGo( CSong* pSong, BOOL blFromCursor = FALSE, BOOL blLoop = FALSE );
	static void	OnPlayStop( BOOL blAlways );

	static void	OnRecordGo();
	static void	OnRecordStop( BOOL success = TRUE );

protected :

	static void	MessageLoop();

	static BOOL	CreateMainWindow();
	static BOOL	CreateClientWindow( HWND hwndParent );
	static BOOL	CreateOverviewWindow( HWND hwndParent );
	static BOOL	CreateToolbars( HWND hwndParent );
	static BOOL	CreateStatusbar( HWND hwndParent );

	static BOOL	OnFileExit();
	static BOOL	OKToCloseFile();

	static void	SetMainWindowTitle();

	static void	NoteMenuSetChecks( HMENU hMenu, int iSelection, WORD wParam );
	static void	MakeCaret();

	static void	OnComposeSong( BOOL blCreateMelody );
	static void	OnComposeSongAABA();
	static void	OnComposeSongs();

	static void ExportMidiFile( CSong* pSong, BOOL blFromCursor );

	static void DumpSong( CSong* pSong );
	static void UpdateStatusBar();

	static HINSTANCE	m_hInstance;
	static int			m_iCmdShow;
	static char			m_szCommandLine[ 1000 ];
	static int			m_iReturnValue;

	static HWND			m_hwndMainWindow;
	static HWND			m_hwndClientWindow;
	static HWND			m_hwndOverviewWindow;
	static HWND			m_hwndToolbar1;
	static HWND			m_hwndStatusbar;

	static HACCEL		m_hAccel;
	static HCURSOR		m_hWaitCursor;
	static HMENU		m_hPopupMenu;

	static int			m_iMainWindowHeight;
	static int			m_iMainWindowWidth;
	static int			m_iClientWindowHeight;
	static int			m_iClientWindowWidth;
	static int			m_iOverviewWindowHeight;
	static int			m_iOverviewWindowWidth;
	static int			m_iToolbar1Height;
	static int			m_iToolbar1Width;
	static int			m_iStatusbarHeight;
	static int			m_iStatusbarWidth;
	static int			m_iZoomDivisor;

	static int			m_iVScrollPos;
	static int			m_iVScrollMax;
	static int			m_iHScrollPos;
	static int			m_iHScrollMax;

	static int			m_iOverviewVScrollPos;
	static int			m_iOverviewVScrollMax;
	static int			m_iOverviewHScrollPos;
	static int			m_iOverviewHScrollMax;

	static BOOL			m_blSwingIt;
	static BOOL			m_blCursorIsShowing;
	static BOOL			m_blGrandStaff;

	static char			m_szFilePath[ MAX_PATH ];
	static char			m_szDumpString[ 0x10000 ];
	static BOOL			m_blPlayingSongs;

	static BOOL			m_blPlayingWave;
};


extern CSong			TheSong;
extern CStructuredSong	TheStructuredSong;
extern CCursor			TheCursor;


#endif

