
#include "CDialogChannelSettings.h"
#include "Constant.h"
#include "GlobalDefs.h"
#include "CNote.h"


const int DIALOG_EDIT1_MIN = 0;
const int DIALOG_EDIT1_MAX = 127;
const int DIALOG_EDIT2_MIN = 0;
const int DIALOG_EDIT2_MAX = 127;
const int DIALOG_EDIT3_MIN = 0;
const int DIALOG_EDIT3_MAX = 127;
const int DIALOG_EDIT4_MIN = 0;
const int DIALOG_EDIT4_MAX = 127;
const int DIALOG_EDIT5_MIN = 0;
const int DIALOG_EDIT5_MAX = 127;
const int DIALOG_EDIT6_MIN = 0;
const int DIALOG_EDIT6_MAX = 127;
const int DIALOG_EDIT7_MIN = 0;
const int DIALOG_EDIT7_MAX = 127;
const int DIALOG_EDIT8_MIN = 0;
const int DIALOG_EDIT8_MAX = 127;
const int DIALOG_EDIT9_MIN = 0;
const int DIALOG_EDIT9_MAX = 127;
const int DIALOG_EDIT10_MIN = 0;
const int DIALOG_EDIT10_MAX = 127;
const int DIALOG_EDIT11_MIN = 0;
const int DIALOG_EDIT11_MAX = 127;
const int DIALOG_EDIT12_MIN = 0;
const int DIALOG_EDIT12_MAX = 127;
const int DIALOG_EDIT13_MIN = 0;
const int DIALOG_EDIT13_MAX = 127;
const int DIALOG_EDIT14_MIN = 0;
const int DIALOG_EDIT14_MAX = 127;
const int DIALOG_EDIT15_MIN = 0;
const int DIALOG_EDIT15_MAX = 127;
const int DIALOG_EDIT16_MIN = 0;
const int DIALOG_EDIT16_MAX = 127;

CDialogChannelSettings::CDialogChannelSettings()
{
	lstrcpy( m_szDialogResourceName, "ChannelSettings" );
}


BOOL CDialogChannelSettings::OnInitDialog( HWND hDlg )
{
	char szPan[ 16 ] = "";

	sprintf( szPan, "%d", m_iPan[ 0 ] );
	::SetDlgItemText(	hDlg, 
						IDC_EDIT1, 
						szPan	);
	sprintf( szPan, "%d", m_iPan[ 1 ] );
	::SetDlgItemText(	hDlg, 
						IDC_EDIT2, 
						szPan	);
	sprintf( szPan, "%d", m_iPan[ 2 ] );
	::SetDlgItemText(	hDlg, 
						IDC_EDIT3, 
						szPan	);
	sprintf( szPan, "%d", m_iPan[ 3 ] );
	::SetDlgItemText(	hDlg, 
						IDC_EDIT4, 
						szPan	);
	sprintf( szPan, "%d", m_iPan[ 4 ] );
	::SetDlgItemText(	hDlg, 
						IDC_EDIT5, 
						szPan	);
	sprintf( szPan, "%d", m_iPan[ 5 ] );
	::SetDlgItemText(	hDlg, 
						IDC_EDIT6, 
						szPan	);
	sprintf( szPan, "%d", m_iPan[ 6 ] );
	::SetDlgItemText(	hDlg, 
						IDC_EDIT7, 
						szPan	);
	sprintf( szPan, "%d", m_iPan[ 7 ] );
	::SetDlgItemText(	hDlg, 
						IDC_EDIT8, 
						szPan	);
	sprintf( szPan, "%d", m_iPan[ 8 ] );
	::SetDlgItemText(	hDlg, 
						IDC_EDIT9, 
						szPan	);
	sprintf( szPan, "%d", m_iPan[ 9 ] );
	::SetDlgItemText(	hDlg, 
						IDC_EDIT10, 
						szPan	);
	sprintf( szPan, "%d", m_iPan[ 10 ] );
	::SetDlgItemText(	hDlg, 
						IDC_EDIT11, 
						szPan	);
	sprintf( szPan, "%d", m_iPan[ 11 ] );
	::SetDlgItemText(	hDlg, 
						IDC_EDIT12, 
						szPan	);
	sprintf( szPan, "%d", m_iPan[ 12 ] );
	::SetDlgItemText(	hDlg, 
						IDC_EDIT13, 
						szPan	);
	sprintf( szPan, "%d", m_iPan[ 13 ] );
	::SetDlgItemText(	hDlg, 
						IDC_EDIT14, 
						szPan	);
	sprintf( szPan, "%d", m_iPan[ 14 ] );
	::SetDlgItemText(	hDlg, 
						IDC_EDIT15, 
						szPan	);
	sprintf( szPan, "%d", m_iPan[ 15 ] );
	::SetDlgItemText(	hDlg, 
						IDC_EDIT16, 
						szPan	);

	return TRUE;
}

BOOL CDialogChannelSettings::OnOk( HWND hDlg )
{
	char szPan[ 16 ] = "";

	::SendDlgItemMessage(	hDlg, 
							IDC_EDIT1, 
							WM_GETTEXT, 
							(WPARAM)sizeof( szPan ), 
							(LPARAM)szPan	);

	m_iPan[0] = atoi( szPan );
	CHECK_BOUNDS( m_iPan[0], EDIT1 );

	::SendDlgItemMessage(	hDlg, 
							IDC_EDIT2, 
							WM_GETTEXT, 
							(WPARAM)sizeof( szPan ), 
							(LPARAM)szPan	);

	m_iPan[1] = atoi( szPan );
	CHECK_BOUNDS( m_iPan[1], EDIT2 );

	::SendDlgItemMessage(	hDlg, 
							IDC_EDIT3, 
							WM_GETTEXT, 
							(WPARAM)sizeof( szPan ), 
							(LPARAM)szPan	);

	m_iPan[2] = atoi( szPan );
	CHECK_BOUNDS( m_iPan[2], EDIT3 );

	::SendDlgItemMessage(	hDlg, 
							IDC_EDIT4, 
							WM_GETTEXT, 
							(WPARAM)sizeof( szPan ), 
							(LPARAM)szPan	);

	m_iPan[3] = atoi( szPan );
	CHECK_BOUNDS( m_iPan[3], EDIT4 );

	::SendDlgItemMessage(	hDlg, 
							IDC_EDIT5, 
							WM_GETTEXT, 
							(WPARAM)sizeof( szPan ), 
							(LPARAM)szPan	);

	m_iPan[4] = atoi( szPan );
	CHECK_BOUNDS( m_iPan[4], EDIT5 );

	::SendDlgItemMessage(	hDlg, 
							IDC_EDIT6, 
							WM_GETTEXT, 
							(WPARAM)sizeof( szPan ), 
							(LPARAM)szPan	);

	m_iPan[5] = atoi( szPan );
	CHECK_BOUNDS( m_iPan[5], EDIT6 );

	::SendDlgItemMessage(	hDlg, 
							IDC_EDIT7, 
							WM_GETTEXT, 
							(WPARAM)sizeof( szPan ), 
							(LPARAM)szPan	);

	m_iPan[6] = atoi( szPan );
	CHECK_BOUNDS( m_iPan[6], EDIT7 );

	::SendDlgItemMessage(	hDlg, 
							IDC_EDIT8, 
							WM_GETTEXT, 
							(WPARAM)sizeof( szPan ), 
							(LPARAM)szPan	);

	m_iPan[7] = atoi( szPan );
	CHECK_BOUNDS( m_iPan[7], EDIT8 );

	::SendDlgItemMessage(	hDlg, 
							IDC_EDIT9, 
							WM_GETTEXT, 
							(WPARAM)sizeof( szPan ), 
							(LPARAM)szPan	);

	m_iPan[8] = atoi( szPan );
	CHECK_BOUNDS( m_iPan[8], EDIT9 );

	::SendDlgItemMessage(	hDlg, 
							IDC_EDIT10, 
							WM_GETTEXT, 
							(WPARAM)sizeof( szPan ), 
							(LPARAM)szPan	);

	m_iPan[9] = atoi( szPan );
	CHECK_BOUNDS( m_iPan[9], EDIT10 );

	::SendDlgItemMessage(	hDlg, 
							IDC_EDIT11, 
							WM_GETTEXT, 
							(WPARAM)sizeof( szPan ), 
							(LPARAM)szPan	);

	m_iPan[10] = atoi( szPan );
	CHECK_BOUNDS( m_iPan[10], EDIT11 );

	::SendDlgItemMessage(	hDlg, 
							IDC_EDIT12, 
							WM_GETTEXT, 
							(WPARAM)sizeof( szPan ), 
							(LPARAM)szPan	);

	m_iPan[11] = atoi( szPan );
	CHECK_BOUNDS( m_iPan[11], EDIT12 );

	::SendDlgItemMessage(	hDlg, 
							IDC_EDIT13, 
							WM_GETTEXT, 
							(WPARAM)sizeof( szPan ), 
							(LPARAM)szPan	);

	m_iPan[12] = atoi( szPan );
	CHECK_BOUNDS( m_iPan[12], EDIT13 );

	::SendDlgItemMessage(	hDlg, 
							IDC_EDIT14, 
							WM_GETTEXT, 
							(WPARAM)sizeof( szPan ), 
							(LPARAM)szPan	);

	m_iPan[13] = atoi( szPan );
	CHECK_BOUNDS( m_iPan[13], EDIT15 );

	::SendDlgItemMessage(	hDlg, 
							IDC_EDIT15, 
							WM_GETTEXT, 
							(WPARAM)sizeof( szPan ), 
							(LPARAM)szPan	);

	m_iPan[14] = atoi( szPan );
	CHECK_BOUNDS( m_iPan[14], EDIT16 );

	::SendDlgItemMessage(	hDlg, 
							IDC_EDIT16, 
							WM_GETTEXT, 
							(WPARAM)sizeof( szPan ), 
							(LPARAM)szPan	);

	m_iPan[15] = atoi( szPan );
	CHECK_BOUNDS( m_iPan[15], EDIT1 );

	return TRUE;
}

BOOL CDialogChannelSettings::CommandHandler( HWND hDlg, WPARAM wParam, LPARAM lParam )
{

	return FALSE;
}

