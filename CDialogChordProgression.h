

#ifndef __CDIALOGCHORDPROGRESSION_H__
#define __CDIALOGCHORDPROGRESSION_H__

#include "CDialogBox.h"
#include "CLinkedList.h"


class CPlacedChord : public CLinkedListItem
{
	public :

		CPlacedChord() : 
			CLinkedListItem(),
			m_iChord( 0 ),
			m_iTicks( 0 )
		{
			lstrcpy( m_szChord, "-" );
		}
		CPlacedChord( char* szChord, int iChord, int iTicks ) : 
			CLinkedListItem(),
			m_iChord( iChord ),
			m_iTicks( iTicks )
		{
			lstrcpy( m_szChord, szChord );
		}
		~CPlacedChord()
		{}

		void	MakeCopy( CLinkedListItem*& rpNewCopy );

		BOOL	Output( FILE* pFile ) const;
		BOOL	Input( FILE* pFile );

		char	m_szChord[ 16 ];
		int		m_iChord;
		int		m_iTicks;
};


class CDialogChordProgression : public CDialogBox
{

	public :

				CDialogChordProgression();

	protected :
							
		BOOL	OnInitDialog( HWND hDlg );
		BOOL	OnOk( HWND hDlg );
		BOOL	CommandHandler( HWND hDlg, WPARAM wParam, LPARAM lParam );

		void	PopulateProgressionBox( HWND hDlg );
		void	PopulateHistoryBox( HWND hDlg );

	public :

		CLinkedList	m_PlacedChordList;
		CLinkedList	m_HistoryList;
		int			m_iStartMeasure;
		BOOL		m_blChord;
		BOOL		m_blBassNote;
		BOOL		m_blMelody;
};



#endif

