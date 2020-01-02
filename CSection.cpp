

#include "CSection.h"


void CSection::MakeCopy( CLinkedListItem*& rpNewCopy )
{
	rpNewCopy = new CSection( m_iFirstMeasure, m_iLastMeasure, m_szName );
}

BOOL CSection::Output( FILE* pFile ) const
{
	if (fwrite( &m_iFirstMeasure, sizeof( m_iFirstMeasure ), 1, pFile ) < 1 ) return FALSE;
	if (fwrite( &m_iLastMeasure, sizeof( m_iLastMeasure ), 1, pFile ) < 1 ) return FALSE;
	if (fwrite( m_szName, sizeof( m_szName ), 1, pFile ) < 1 ) return FALSE;

	return TRUE;
}

BOOL CSection::Input( FILE* pFile )
{
	if (fread( &m_iFirstMeasure, sizeof( m_iFirstMeasure ), 1, pFile ) < 1 ) return FALSE;
	if (fread( &m_iLastMeasure, sizeof( m_iLastMeasure ), 1, pFile ) < 1 ) return FALSE;
	if (fread( m_szName, sizeof( m_szName ), 1, pFile ) < 1 ) return FALSE;

	return TRUE;
}


