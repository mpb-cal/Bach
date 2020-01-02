

#ifndef __CDIALOGSONGSETTINGS_H__
#define __CDIALOGSONGSETTINGS_H__

#include "CDialogBox.h"

class CDialogSongSettings : public CDialogBox
{

	public :

				CDialogSongSettings();

	protected :
							
		BOOL	OnInitDialog( HWND hDlg );
		BOOL	OnOk( HWND hDlg );
		BOOL	CommandHandler( HWND hDlg, WPARAM wParam, LPARAM lParam );


	public :

		int		m_iVoices ;
		int		m_iTempo;
		int		m_iTimeSigN;
		int		m_iTimeSigD;
		int		m_aiTimeSigN[ 15 ];
		int		m_aiTimeSigD[ 4 ];

};



#endif

