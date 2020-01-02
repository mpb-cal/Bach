
// CSection.h

#ifndef __CSECTION_H__
#define __CSECTION_H__

#include "CPosition.h"
#include "CLinkedList.h"


class CStructuredSong;


class CSection : public CLinkedListItem
{

	public:
			
			CSection( int iFirstMeasure, int iLastMeasure, char* szName ) :
				m_iFirstMeasure( iFirstMeasure ),
				m_iLastMeasure( iLastMeasure )
				{ lstrcpy( m_szName, szName ); }
//			~CSection();

		void			MakeCopy( CLinkedListItem*& rpNewCopy );

		BOOL			Output( FILE* pFile ) const;
		BOOL			Input( FILE* pFile );

	protected:

		int		m_iFirstMeasure;
		int		m_iLastMeasure;
		char	m_szName[ 512 ];

friend class CStructuredSong;
friend class CDialogSongStructure;

};


#endif
