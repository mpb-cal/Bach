

#ifndef __CDIALOGHARMONIZE_H__
#define __CDIALOGHARMONIZE_H__

#include "CDialogBox.h"

class CDialogHarmonize : public CDialogBox
{

	public :

				CDialogHarmonize();

	protected :
							
		BOOL	OnInitDialog( HWND hDlg );
		BOOL	OnOk( HWND hDlg );
		BOOL	CommandHandler( HWND hDlg, WPARAM wParam, LPARAM lParam );


	public :

		int		m_iVoice;
		int		m_iMeasures;
		int		m_iGranularity;
		char	m_szGranularity[ 256 ];

};



#endif

