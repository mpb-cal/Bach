

#ifndef __CDIALOGPERCUSSION_H__
#define __CDIALOGPERCUSSION_H__

#include "CDialogBox.h"

class CDialogPercussion : public CDialogBox
{

	public :

				CDialogPercussion();

	protected :
							
		BOOL	OnInitDialog( HWND hDlg );
		BOOL	OnOk( HWND hDlg );
		BOOL	CommandHandler( HWND hDlg, WPARAM wParam, LPARAM lParam );


	public :

		int		m_iPitch;

};



#endif

