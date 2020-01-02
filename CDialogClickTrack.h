

#ifndef __CDIALOGCLICKTRACK_H__
#define __CDIALOGCLICKTRACK_H__

#include "CDialogBox.h"

class CDialogClickTrack : public CDialogBox
{

	public :

				CDialogClickTrack();

	protected :
							
		BOOL	OnInitDialog( HWND hDlg );
		BOOL	OnOk( HWND hDlg );
		BOOL	CommandHandler( HWND hDlg, WPARAM wParam, LPARAM lParam );


	public :

		int	m_iMeasures;
		int	m_iVoice;

};



#endif

