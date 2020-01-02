
// CStructuredSong.h

#ifndef __CSTRUCTUREDSONG_H__
#define __CSTRUCTUREDSONG_H__

#include "CSong.h"
#include "CDialogDefineSection.h"
#include "CDialogSongStructure.h"



class CStructuredSong : public CSong
{

	public:

					CStructuredSong();
					~CStructuredSong();

	const CSong&	operator=( const CSong& rvalue );

	// input and output methods
	BOOL				Output( FILE* pFile );
	BOOL				Input( FILE* pFile );

	public:

		void			DefineSection();
		void			EditStructure();

		void			ExportMidiFile(	const char* szTitle, 
										BOOL blSwingIt,
										CPosition& rposFrom, 
										CPosition& rposTo	);

	protected:

		CLinkedList		m_SectionsList;
		char			m_cSectionOrder[ 512 ];
		int				m_iSections;

};



#endif __CSTRUCTUREDSONG_H__
