

#ifndef __CDIALOGCHANNELSETTINGS_H__
#define __CDIALOGCHANNELSETTINGS_H__

#include "CDialogBox.h"

class CDialogChannelSettings : public CDialogBox
{

	public :

				CDialogChannelSettings();

	protected :
							
		BOOL	OnInitDialog( HWND hDlg );
		BOOL	OnOk( HWND hDlg );
		BOOL	CommandHandler( HWND hDlg, WPARAM wParam, LPARAM lParam );

		void	InitArrowComboboxes( HWND hDlg, int iValue );

	public :

		int		m_iPan[ 16 ];

};



#endif

