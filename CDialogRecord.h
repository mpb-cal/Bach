

#ifndef __CDIALOGRECORD_H__
#define __CDIALOGRECORD_H__

#include "CDialogBox.h"

class CDialogRecord : public CDialogBox
{

	public :

				CDialogRecord();

	protected :
							
		BOOL	OnInitDialog( HWND hDlg );
		BOOL	OnOk( HWND hDlg );
		BOOL	CommandHandler( HWND hDlg, WPARAM wParam, LPARAM lParam );


	public :

		int		m_iVoice;
		int		m_iMaxTime;
		int		m_iSampleRate;
		int		m_iBitsPerSample;
		int		m_iMode;

};



#endif

