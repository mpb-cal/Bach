

#ifndef __CDIALOGMIDIINFO_H__
#define __CDIALOGMIDIINFO_H__

#include "CDialogBox.h"

class CDialogMidiInfo : public CDialogBox
{

	public :

				CDialogMidiInfo();

	protected :
							
		BOOL	OnInitDialog( HWND hDlg );
		BOOL	OnOk( HWND hDlg );
		BOOL	CommandHandler( HWND hDlg, WPARAM wParam, LPARAM lParam );


	public :

		int		m_iFileFormat;
		int		m_iTracks;
		int		m_iTempo;

};



#endif

