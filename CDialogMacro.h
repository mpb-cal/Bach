

#ifndef __CDIALOGMACRO_H__
#define __CDIALOGMACRO_H__

#include "CDialogBox.h"

const int MACRO_MAXLENGTH = 1024;

class CDialogMacro : public CDialogBox
{

	public :

				CDialogMacro();

	protected :
							
		BOOL	OnInitDialog( HWND hDlg );
		BOOL	OnOk( HWND hDlg );
		BOOL	CommandHandler( HWND hDlg, WPARAM wParam, LPARAM lParam );


	public :

		char	m_szMacro[ MACRO_MAXLENGTH ];
		int		m_iRepetitions;

};



#endif

