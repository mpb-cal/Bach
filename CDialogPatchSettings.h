

#ifndef __CDIALOGPATCHSETTINGS_H__
#define __CDIALOGPATCHSETTINGS_H__

#include "CDialogBox.h"

class CDialogPatchSettings : public CDialogBox
{

	public :

				CDialogPatchSettings();

	protected :
							
		BOOL	OnInitDialog( HWND hDlg );
		BOOL	OnOk( HWND hDlg );
		BOOL	CommandHandler( HWND hDlg, WPARAM wParam, LPARAM lParam );


	public :

		int		m_iPatch;

};



#endif

