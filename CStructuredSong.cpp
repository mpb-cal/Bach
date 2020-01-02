
// CStructuredSong.cpp

#include "stdafx.h"

#ifndef __CSTRUCTUREDSONG_H__
#include "CStructuredSong.h"
#endif

#ifndef __CSECTION_H__
#include "CSection.h"
#endif

#ifndef __CMIDIFILE_H__
#include "CMidiFile.h"
#endif

#ifndef __CVOICE_H__
#include "CVoice.h"
#endif



CStructuredSong::CStructuredSong() :
	CSong(),
	m_iSections( 0 )
{
}	

CStructuredSong::~CStructuredSong()
{
}

const CSong& CStructuredSong::operator=( const CSong& rvalue )
{
	CSong::operator=( rvalue );

	return *this;
}

BOOL CStructuredSong::Output( FILE* pFile )
{
	if (fwrite( &m_iSections, sizeof( m_iSections ), 1, pFile ) < 1 ) return FALSE;
	if (fwrite( m_cSectionOrder, sizeof( m_cSectionOrder ), 1, pFile ) < 1 ) return FALSE;

	int iDefinedSections = m_SectionsList.GetSize();
	if (fwrite( &iDefinedSections, sizeof( iDefinedSections ), 1, pFile ) < 1 ) return FALSE;

	m_SectionsList.Output( pFile );

	return TheSong.Output( pFile );
}

BOOL CStructuredSong::Input( FILE* pFile )
{
	if (fread( &m_iSections, sizeof( m_iSections ), 1, pFile ) < 1 ) return FALSE;
	if (fread( m_cSectionOrder, sizeof( m_cSectionOrder ), 1, pFile ) < 1 ) return FALSE;

	int iDefinedSections;
	if (fread( &iDefinedSections, sizeof( iDefinedSections ), 1, pFile ) < 1 ) return FALSE;
	for (int i = 0; i < iDefinedSections; i++)
	{
		m_SectionsList.AppendItem( new CSection( 1, 1, "" ) );
	}

	m_SectionsList.Input( pFile );

	return TheSong.Input( pFile );
}

void CStructuredSong::DefineSection()
{
	CDialogDefineSection dds;

	lstrcpy( dds.m_szName, "NewSection" );

	if (dds.DoModal( CApplication::GetInstance(), CApplication::GetMainWindowHandle() ) == IDOK)
	{
		m_SectionsList.AppendItem( new CSection( dds.m_iFirstMeasure, dds.m_iLastMeasure, dds.m_szName ) );
	}
}

void CStructuredSong::EditStructure()
{
	CDialogSongStructure dss;

	dss.m_pSectionsList = &m_SectionsList;
	::CopyMemory( dss.m_cSectionOrder, m_cSectionOrder, sizeof( dss.m_cSectionOrder ) );
	dss.m_iSections = m_iSections;

	if (dss.DoModal( CApplication::GetInstance(), CApplication::GetMainWindowHandle() ) == IDOK)
	{
		::CopyMemory( m_cSectionOrder, dss.m_cSectionOrder, sizeof( m_cSectionOrder ) );
		m_iSections = dss.m_iSections;
	}
}

void CStructuredSong::ExportMidiFile(	const char* szTitle,
										BOOL blSwingIt,
										CPosition& rposFrom, 
										CPosition& rposTo	)
{
	*this = TheSong;

	if (m_iSections)
	{
		for (int i = 0; i < m_iVoices; i++)
		{
			ClearVoice( i );
		}

		CSelection sel;
		int iCurrentMeasure = 0;
		for (i = 0; i < m_iSections; i++)
		{
			sel.Enable(	0, 
						((CSection*)(m_SectionsList[ m_cSectionOrder[ i ] ]))->m_iFirstMeasure - 1,
						0,
						0,
						0,
						0,
						m_iTicksPerMeasure	);

			sel.SetX(	m_iVoices - 1,
						((CSection*)(m_SectionsList[ m_cSectionOrder[ i ] ]))->m_iLastMeasure - 1,
						m_iTicksPerMeasure,
						0,
						m_iTicksPerMeasure	);

			sel.LoadTicks( &TheSong );

			CPosition posFrom;
			posFrom.Initialize( 0, iCurrentMeasure, 0, m_iTicksPerMeasure );
			sel.PutTicks( this, posFrom, m_iTicksPerMeasure, m_iKey );

			sel.Disable();

			iCurrentMeasure +=
				((CSection*)(m_SectionsList[ m_cSectionOrder[ i ] ]))->m_iLastMeasure -
				((CSection*)(m_SectionsList[ m_cSectionOrder[ i ] ]))->m_iFirstMeasure +
				1;
		}

		rposFrom.SetMeasure( 0 );
		rposTo.SetMeasure( iCurrentMeasure );
	}

	CSong::ExportMidiFile( szTitle, blSwingIt, rposFrom, rposTo );
}


