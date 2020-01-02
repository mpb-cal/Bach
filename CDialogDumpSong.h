

#ifndef __CDIALOGDUMPSONG_H__
#define __CDIALOGDUMPSONG_H__

#include "CDialogBox.h"

class CDialogDumpSong : public CDialogBox
{

	public :

				CDialogDumpSong();

	protected :
							
		BOOL	OnInitDialog( HWND hDlg );
		BOOL	OnOk( HWND hDlg );
		BOOL	CommandHandler( HWND hDlg, WPARAM wParam, LPARAM lParam );


	public :

		char	m_szText[ 0x10000 ];

};



#endif

