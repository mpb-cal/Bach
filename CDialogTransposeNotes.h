

#ifndef __CDIALOGTRANSPOSENOTES_H__
#define __CDIALOGTRANSPOSENOTES_H__

#include "CDialogBox.h"

class CDialogTransposeNotes : public CDialogBox
{

	public :

				CDialogTransposeNotes();

	protected :
							
		BOOL	OnInitDialog( HWND hDlg );
		BOOL	OnOk( HWND hDlg );
		BOOL	CommandHandler( HWND hDlg, WPARAM wParam, LPARAM lParam );


	public :

		int	m_iHalfSteps;
		int	m_iDirection;

};



#endif

