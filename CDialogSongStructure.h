

#ifndef __CDIALOGSONGSTRUCTURE_H__
#define __CDIALOGSONGSTRUCTURE_H__


#include "CLinkedList.h"
#include "CDialogBox.h"

class CDialogSongStructure : public CDialogBox
{

	public :

				CDialogSongStructure();

	protected :
							
		BOOL	OnInitDialog( HWND hDlg );
		BOOL	OnOk( HWND hDlg );
		BOOL	CommandHandler( HWND hDlg, WPARAM wParam, LPARAM lParam );

		void	PopulateSectionOrderBox( HWND hDlg );

	public :

		CLinkedList*	m_pSectionsList;
		char			m_cSectionOrder[ 512 ];
		int				m_iSections;

};



#endif

