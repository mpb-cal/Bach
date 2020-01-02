

#ifndef __CDIALOGCURSORSETTINGS_H__
#define __CDIALOGCURSORSETTINGS_H__

#include "CDialogBox.h"

class CDialogCursorSettings : public CDialogBox
{

	public :

				CDialogCursorSettings();

	protected :
							
		BOOL	OnInitDialog( HWND hDlg );
		BOOL	OnOk( HWND hDlg );
		BOOL	CommandHandler( HWND hDlg, WPARAM wParam, LPARAM lParam );

		void	InitArrowComboboxes( HWND hDlg, int iValue );

	public :

		int		m_iArrow;
		int		m_iArrowIndex;
		int		m_iCtlArrow;
		int		m_iCtlArrowIndex;
		BOOL	m_blRememberAttack;
		BOOL	m_blInsertMode;
		BOOL	m_blRetainPitch;
		BOOL	m_blAutoAdvance;

};



#endif

