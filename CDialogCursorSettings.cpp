
#include "CDialogCursorSettings.h"
#include "Constant.h"
#include "GlobalDefs.h"
#include "CNote.h"


const int DIALOG_CHANNEL_MIN = CHANNEL_MIN;
const int DIALOG_CHANNEL_MAX = CHANNEL_MAX;

CDialogCursorSettings::CDialogCursorSettings()
{
	lstrcpy( m_szDialogResourceName, "CursorSettings" );
}


BOOL CDialogCursorSettings::OnInitDialog( HWND hDlg )
{
	// fill in arrow and ctl-arrow combo box and set current selection
	m_iArrowIndex = 0;
	m_iCtlArrowIndex = 0;

	InitArrowComboboxes( hDlg, NOTE_THIRTYSECOND );
	InitArrowComboboxes( hDlg, NOTE_SIXTEENTH );
	InitArrowComboboxes( hDlg, NOTE_SIXTEENTHD );
	InitArrowComboboxes( hDlg, NOTE_EIGHTH );
	InitArrowComboboxes( hDlg, NOTE_EIGHTHD );
	InitArrowComboboxes( hDlg, NOTE_QUARTER );
	InitArrowComboboxes( hDlg, NOTE_QUARTERD );
	InitArrowComboboxes( hDlg, NOTE_HALF );
	InitArrowComboboxes( hDlg, NOTE_HALFD );
	InitArrowComboboxes( hDlg, NOTE_WHOLE );

	// set RememberAttack checkbox
	CheckDlgButton( hDlg, IDC_REMEMBERATTACK, m_blRememberAttack );

	// set InsertMode checkbox
	CheckDlgButton( hDlg, IDC_INSERTIONMODE, m_blInsertMode );

	// set RetainPitch checkbox
	CheckDlgButton( hDlg, IDC_RETAINPITCH, m_blRetainPitch );

	// set AutoAdvance checkbox
	CheckDlgButton( hDlg, IDC_AUTOADVANCE, m_blAutoAdvance );


	return TRUE;
}

BOOL CDialogCursorSettings::OnOk( HWND hDlg )
{
	// retrieve arrow
	int iCurrentSelection = ::SendDlgItemMessage(	hDlg, 
													IDC_CRSARROW, 
													CB_GETCURSEL, 
													(WPARAM)0, 
													(LPARAM)0	);
	switch (iCurrentSelection)
	{
		case 0 : m_iArrow = NOTE_THIRTYSECOND; break;
		case 1 : m_iArrow = NOTE_SIXTEENTH; break;
		case 2 : m_iArrow = NOTE_SIXTEENTHD; break;
		case 3 : m_iArrow = NOTE_EIGHTH; break;
		case 4 : m_iArrow = NOTE_EIGHTHD; break;
		case 5 : m_iArrow = NOTE_QUARTER; break;
		case 6 : m_iArrow = NOTE_QUARTERD; break;
		case 7 : m_iArrow = NOTE_HALF; break;
		case 8 : m_iArrow = NOTE_HALFD; break;
		case 9 : m_iArrow = NOTE_WHOLE; break;
	}

	// retrieve ctl-arrow
	iCurrentSelection = ::SendDlgItemMessage(	hDlg, 
												IDC_CTLARROW, 
												CB_GETCURSEL, 
												(WPARAM)0, 
												(LPARAM)0	);
	switch (iCurrentSelection)
	{
		case 0 : m_iCtlArrow = NOTE_THIRTYSECOND; break;
		case 1 : m_iCtlArrow = NOTE_SIXTEENTH; break;
		case 2 : m_iCtlArrow = NOTE_SIXTEENTHD; break;
		case 3 : m_iCtlArrow = NOTE_EIGHTH; break;
		case 4 : m_iCtlArrow = NOTE_EIGHTHD; break;
		case 5 : m_iCtlArrow = NOTE_QUARTER; break;
		case 6 : m_iCtlArrow = NOTE_QUARTERD; break;
		case 7 : m_iCtlArrow = NOTE_HALF; break;
		case 8 : m_iCtlArrow = NOTE_HALFD; break;
		case 9 : m_iCtlArrow = NOTE_WHOLE; break;
	}

	// retrieve RememberAttack
	m_blRememberAttack = IsDlgButtonChecked( hDlg, IDC_REMEMBERATTACK );

	// retrieve InsertMode
	m_blInsertMode = IsDlgButtonChecked( hDlg, IDC_INSERTIONMODE );

	// retrieve RetainPitch
	m_blRetainPitch = IsDlgButtonChecked( hDlg, IDC_RETAINPITCH );

	// retrieve AutoAdvance
	m_blAutoAdvance = IsDlgButtonChecked( hDlg, IDC_AUTOADVANCE );

	return TRUE;
}

BOOL CDialogCursorSettings::CommandHandler( HWND hDlg, WPARAM wParam, LPARAM lParam )
{

	return FALSE;
}

void CDialogCursorSettings::InitArrowComboboxes( HWND hDlg, int iValue )
{
	char szArrow[ 64 ];

	CNote::TicksToString( iValue, szArrow );

	::SendDlgItemMessage(	hDlg, 
							IDC_CRSARROW, 
							CB_ADDSTRING, 
							(WPARAM)0, 
							(LPARAM)szArrow	);
	::SendDlgItemMessage(	hDlg, 
							IDC_CTLARROW, 
							CB_ADDSTRING, 
							(WPARAM)0, 
							(LPARAM)szArrow	);


	if (iValue == m_iArrow)
	{
		::SendDlgItemMessage(	hDlg, 
								IDC_CRSARROW, 
								CB_SETCURSEL, 
								(WPARAM)m_iArrowIndex, 
								(LPARAM)0	);
	}

	if (iValue == m_iCtlArrow)
	{
		::SendDlgItemMessage(	hDlg, 
								IDC_CTLARROW, 
								CB_SETCURSEL, 
								(WPARAM)m_iCtlArrowIndex, 
								(LPARAM)0	);
	}

	m_iArrowIndex++;
	m_iCtlArrowIndex++;
}
