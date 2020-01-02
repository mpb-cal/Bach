

#ifndef __CDIALOGKEYSETTINGS_H__
#define __CDIALOGKEYSETTINGS_H__

#include "CDialogBox.h"

class CDialogKeySettings : public CDialogBox
{

	public :

				CDialogKeySettings();

	protected :
							
		BOOL	OnInitDialog( HWND hDlg );
		BOOL	OnOk( HWND hDlg );
		BOOL	CommandHandler( HWND hDlg, WPARAM wParam, LPARAM lParam );


	public :

		int		m_iKey;

};



#endif

