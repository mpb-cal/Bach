

#ifndef __CDIALOGTRANSPOSEDIATONIC_H__
#define __CDIALOGTRANSPOSEDIATONIC_H__

#include "CDialogBox.h"

class CDialogTransposeDiatonic : public CDialogBox
{

	public :

				CDialogTransposeDiatonic();

	protected :
							
		BOOL	OnInitDialog( HWND hDlg );
		BOOL	OnOk( HWND hDlg );
		BOOL	CommandHandler( HWND hDlg, WPARAM wParam, LPARAM lParam );


	public :

		int	m_iPitches;
		int	m_iDirection;

};



#endif

