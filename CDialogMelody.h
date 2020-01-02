

#ifndef __CDIALOGMELODY_H__
#define __CDIALOGMELODY_H__

#include "CDialogBox.h"

class CDialogMelody : public CDialogBox
{

	public :

				CDialogMelody();

	protected :
							
		BOOL	OnInitDialog( HWND hDlg );
		BOOL	OnOk( HWND hDlg );
		BOOL	CommandHandler( HWND hDlg, WPARAM wParam, LPARAM lParam );


	public :

		int		m_iMeasures;

};



#endif

