

#ifndef __CDIALOGGUITARCHORD_H__
#define __CDIALOGGUITARCHORD_H__

#include "CDialogBox.h"

class CDialogGuitarChord : public CDialogBox
{

	public :

				CDialogGuitarChord();

	protected :
							
		BOOL	OnInitDialog( HWND hDlg );
		BOOL	OnOk( HWND hDlg );
		BOOL	CommandHandler( HWND hDlg, WPARAM wParam, LPARAM lParam );


	public :

		int		m_iTicks;
		int		m_iChord;
		BOOL	m_blChord;
		BOOL	m_blBassNote;
		BOOL	m_blMelody;
};



#endif

