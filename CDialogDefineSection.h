

#ifndef __CDIALOGDEFINESECTION_H__
#define __CDIALOGDEFINESECTION_H__

#include "CDialogBox.h"

class CDialogDefineSection : public CDialogBox
{

	public :

				CDialogDefineSection();

	protected :
							
		BOOL	OnInitDialog( HWND hDlg );
		BOOL	OnOk( HWND hDlg );
		BOOL	CommandHandler( HWND hDlg, WPARAM wParam, LPARAM lParam );


	public :

		int		m_iFirstMeasure;
		int		m_iLastMeasure;
		char	m_szName[ 512 ];

};



#endif

