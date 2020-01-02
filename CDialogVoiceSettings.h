

#ifndef __CDIALOGVOICESETTINGS_H__
#define __CDIALOGVOICESETTINGS_H__

#include "CDialogBox.h"

class CDialogVoiceSettings : public CDialogBox
{

	public :

				CDialogVoiceSettings();

	protected :
							
		BOOL	OnInitDialog( HWND hDlg );
		BOOL	OnOk( HWND hDlg );
		BOOL	CommandHandler( HWND hDlg, WPARAM wParam, LPARAM lParam );


	public :

		int			m_iPatch;
		int			m_iClef;
		int			m_iChannel;
		int			m_iVelocity;
		int			m_iVoice;
		BOOL		m_blMute;
		COLORREF	m_crColor;
};



#endif

