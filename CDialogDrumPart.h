

#ifndef __CDIALOGDRUMPART_H__
#define __CDIALOGDRUMPART_H__

#include "CDialogBox.h"

class CDialogDrumPart : public CDialogBox
{

	public :

				CDialogDrumPart();

	protected :
							
		BOOL	OnInitDialog( HWND hDlg );
		BOOL	OnOk( HWND hDlg );
		BOOL	CommandHandler( HWND hDlg, WPARAM wParam, LPARAM lParam );


	public :

		int	m_iKick;
		int	m_iSnare;
		int	m_iHiHat;
		int	m_iTom1;
		int	m_iTom2;
		int	m_iStartMeasure;
		int	m_iMeasures;
		int	m_iStyle;

};



#endif

