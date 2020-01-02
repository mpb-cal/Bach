

#ifndef __CDIALOGCHORDSETTINGS_H__
#define __CDIALOGCHORDSETTINGS_H__

#include "CDialogBox.h"

class CDialogChordSettings : public CDialogBox
{

	public :

				CDialogChordSettings();

	protected :
							
		BOOL	OnInitDialog( HWND hDlg );
		BOOL	OnOk( HWND hDlg );
		BOOL	CommandHandler( HWND hDlg, WPARAM wParam, LPARAM lParam );


	public :

		int		m_iVoices;

};



#endif

