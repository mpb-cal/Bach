/*
	CMidiFile.cpp
*/

#include "stdafx.h"

#ifndef __CAPPLICATION_H__
#include "CApplication.h"
#endif

#ifndef __CMIDIFILE_H__
#include "CMidiFile.h"
#endif

#ifndef __CMIDITRACK_H__
#include "CMidiTrack.h"
#endif

#ifndef __CMIDIEVENT_H__
#include "CMidiEvent.h"
#endif

#ifndef __CONSTANT_H__
#include "Constant.h"
#endif

// use this ctor when reading file off disk
CMidiFile::CMidiFile() :
	m_pucHeader( NULL ),
	m_iFileFormat( 0 ),
	m_iTracks( 0 ),
	m_iDeltasPerQuarter( 0 ),
	m_iTempo( 0 ),
	m_pbData( NULL ),
	m_pmevntEvents( NULL ),
	m_iNumberOfEvents( 0 ),
	m_iFileSize( 0 ),
	m_iFilePtr( 0 ),
	m_ppTracks( NULL ),
	m_iKeySignature( 0 ),
	m_iMode( 0 ),
	m_iTimesigN( 0 ),
	m_iTimesigD( 0 ),
	m_blStop( FALSE ),
	m_blPlaying( FALSE ),
	m_blLooping( FALSE )
{
	::memset( &m_mhdrMidiHeader, 0, sizeof MIDIHDR );
	m_pucHeader = new unsigned char[ MF_HEADERSIZE ];
}

// use this ctor when creating new file
CMidiFile::CMidiFile(	int iTracks, 
						double dDpq, 
						int iKey, 
						int iMode, 
						int iTimen, 
						int iTimed	) :
	m_pucHeader( NULL ),
	m_iFileFormat( 1 ),
	m_iTracks( iTracks ),
	m_iDeltasPerQuarter( (int)dDpq ),
	m_iTempo( (int)dDpq ),
	m_pbData( NULL ),
	m_pmevntEvents( NULL ),
	m_iNumberOfEvents( 0 ),
	m_iFileSize( 0 ),
	m_iFilePtr( 0 ),
	m_ppTracks( NULL ),
	m_iKeySignature( iKey ),
	m_iMode( iMode ),
	m_iTimesigN( iTimen ),
	m_iTimesigD( iTimed ),
	m_blStop( FALSE ),
	m_blPlaying( FALSE ),
	m_blLooping( FALSE )
{
	::memset( &m_mhdrMidiHeader, 0, sizeof MIDIHDR );

	m_pucHeader = new unsigned char[ MF_HEADERSIZE ];

	m_pucHeader[ 0 ] = (unsigned char)'M';
	m_pucHeader[ 1 ] = (unsigned char)'T';
	m_pucHeader[ 2 ] = (unsigned char)'h';
	m_pucHeader[ 3 ] = (unsigned char)'d';

	m_pucHeader[ 4 ] = (unsigned char)0;
	m_pucHeader[ 5 ] = (unsigned char)0;
	m_pucHeader[ 6 ] = (unsigned char)0;
	m_pucHeader[ 7 ] = (unsigned char)6;

	m_pucHeader[ 8 ] = (unsigned char)0;
	m_pucHeader[ 9 ] = (unsigned char)m_iFileFormat;

	m_pucHeader[ 10 ] = (unsigned char)0;
	m_pucHeader[ 11 ] = (unsigned char)m_iTracks;

	m_pucHeader[ 12 ] = (unsigned char)0;
	m_pucHeader[ 13 ] = (unsigned char)m_iDeltasPerQuarter;

	m_ppTracks = new CMidiTrack*[ m_iTracks ];
	for (int i = 0; i < m_iTracks; i++)
	{
		m_ppTracks[ i ] = NULL;
	}
}

CMidiFile::~CMidiFile()
{
	if (m_pbData) delete[] m_pbData;
	if (m_pmevntEvents) delete[] m_pmevntEvents;

	if (m_ppTracks)
	{
		for (int i = 0; i < m_iTracks; i++)
		{
			if (m_ppTracks[ i ])
			{
				delete m_ppTracks[ i ];
				m_ppTracks[ i ] = NULL;
			}
		}
		m_ppTracks = NULL;
	}
}

void CMidiFile::Write( ostream& fout )
{
	fout.write( m_pucHeader, MF_HEADERSIZE );

	for (int i = 0; i < m_iTracks; i++)
	{
		m_ppTracks[ i ]->Write( fout );
	}
}

BOOL CMidiFile::Read( HANDLE hFile )
{
    m_iFileSize = ::GetFileSize( hFile, NULL );
    if (!(m_pbData = new BYTE[ m_iFileSize ]))
    {
		::MessageBox( NULL, "Unable to allocate memory", NULL, MB_OK );
		return FALSE;
    }

    DWORD dwBytesRead;

    if (!::ReadFile( hFile, m_pbData, m_iFileSize, &dwBytesRead, NULL )) return FALSE;

//	fin.read( m_pucHeader, MF_HEADERSIZE );

	::CopyMemory( m_pucHeader, m_pbData, MF_HEADERSIZE );

	// check for magic
	if (m_pucHeader[ 0 ] != 'M') return FALSE;
	if (m_pucHeader[ 1 ] != 'T') return FALSE;
	if (m_pucHeader[ 2 ] != 'h') return FALSE;
	if (m_pucHeader[ 3 ] != 'd') return FALSE;

	// check for header size
	if (m_pucHeader[ 4 ] != 0) return FALSE;
	if (m_pucHeader[ 5 ] != 0) return FALSE;
	if (m_pucHeader[ 6 ] != 0) return FALSE;
	if (m_pucHeader[ 7 ] != 6) return FALSE;

	m_iFileFormat = (int)ntohs( *(WORD*)(&m_pucHeader[ 8 ]) );
	m_iTracks = (int)ntohs( *(WORD*)(&m_pucHeader[ 10 ]) );
	m_iDeltasPerQuarter = (int)ntohs( *(WORD*)(&m_pucHeader[ 12 ]) );
	m_iTempo = m_iDeltasPerQuarter;

	m_ppTracks = new CMidiTrack*[ m_iTracks ];

	for (int i = 0; i < m_iTracks; i++)
	{
		m_ppTracks[ i ] = NULL;
	}

	int iIndex = MF_HEADERSIZE;
	for (i = 0; i < m_iTracks; i++)
	{
		m_ppTracks[ i ] = new CMidiTrack;
		if (!m_ppTracks[ i ]->Read( m_pbData, iIndex ))
		{
			return FALSE;
		}
	}

	// process MIDI events

	CountTracks();
	if (m_iTracks == 0) return FALSE;

    // count number of events
    if (!CountMidiEvents()) return FALSE;
	if (m_iNumberOfEvents == 0) return FALSE;

	// allocate events
	m_pmevntEvents = new CMidiEvent[ m_iNumberOfEvents ];
	if (m_pmevntEvents == NULL) return FALSE;

	// make events
	if (!MakeMidiEvents()) return FALSE;

	// sort events by time
	SortMidiEvents();

	return TRUE;
}

DWORD PlayThread( LPVOID pParam )
{
	CMidiFile* pMidiFile = (CMidiFile*)pParam;

	MMRESULT mmresult;
	HMIDIOUT hMidiout;
	DWORD dwAbsoluteDeltaTime = 0;
	int i;
	const int BUFFER_SIZE = 1024;
	char szErrorText[ BUFFER_SIZE ] = "";

	pMidiFile->m_blPlaying = TRUE;

	UINT uiNumDevs = ::midiOutGetNumDevs();


	try
	{
		mmresult = ::midiOutOpen( &hMidiout, 0, NULL, (ULONG)CApplication::GetInstance(), CALLBACK_NULL);
		if (mmresult != MMSYSERR_NOERROR) throw mmresult;

		mmresult = ::midiOutReset( hMidiout );
		if (mmresult != MMSYSERR_NOERROR) throw mmresult;

		do
		{
			for (i = 0; i < pMidiFile->m_iNumberOfEvents; i++)
			{
				if (pMidiFile->m_blStop) break;

				::Sleep( pMidiFile->m_pmevntEvents[ i ].GetMIDIEVENT().dwDeltaTime * 500 / pMidiFile->m_iTempo );

				dwAbsoluteDeltaTime += pMidiFile->m_pmevntEvents[ i ].GetMIDIEVENT().dwDeltaTime;
//				CApplication::MidiMessage(dwAbsoluteDeltaTime / pMidiFile->m_iDeltasPerQuarter);

				mmresult = ::midiOutShortMsg( hMidiout, pMidiFile->m_pmevntEvents[ i ].GetMIDIEVENT().dwEvent);
				if (mmresult != MMSYSERR_NOERROR) throw mmresult;
			}
		} while (pMidiFile->m_blLooping);
	}
	catch( MMRESULT m )
	{
		::midiOutGetErrorText( m, szErrorText, BUFFER_SIZE ); 
		::MessageBox( CApplication::GetMainWindowHandle(), szErrorText, NULL, MB_OK | MB_TASKMODAL );
	}

	// close midi stream
	mmresult = ::midiOutClose( hMidiout );
	if (mmresult != MMSYSERR_NOERROR)
	{
		::midiOutGetErrorText( mmresult, szErrorText, BUFFER_SIZE ); 
		::MessageBox( CApplication::GetMainWindowHandle(), szErrorText, NULL, MB_OK | MB_TASKMODAL );
	}

	pMidiFile->m_blPlaying = FALSE;

	// tell main window we're done
	CApplication::OnPlayStop( FALSE );

	return 0;
}

BOOL CMidiFile::Play( int iTempo, BOOL blLoop )
{
	while (m_blPlaying)
	{
		Stop();
	}

	m_iTempo = iTempo;
	m_blStop = FALSE;
	m_blLooping = blLoop;
	DWORD dwThreadId;
	::CreateThread(	NULL, 
					0, 
					(LPTHREAD_START_ROUTINE)PlayThread, 
					this, 
					0, 
					&dwThreadId	);

	return TRUE;
}

void CMidiFile::Stop()
{
	m_blStop = TRUE;
	m_blLooping = FALSE;
}

void CMidiFile::edit_track(	INT nTrack, 
							INT nSize, 
							INT* pnPitch, 
							INT* pnLength, 
							INT nPatch,
							INT nPan,
							INT nChannel, 
							INT nVelocity	)
{
	m_ppTracks[ nTrack ] = new CMidiTrack(	nSize, 
											nPatch, 
											nPan,
											nChannel, 
											nVelocity, 
											m_iKeySignature, 
											m_iMode, 
											m_iTimesigN, 
											m_iTimesigD	);
	
	for(INT i = 0; i < nSize; i++) 
	{
		m_ppTracks[ nTrack ]->edit_note( i, pnPitch[i], pnLength[i]);
	}
}

void CMidiFile::CountTracks()
{
	int iDataOffset = 0;
	m_iTracks = 0;

	while (iDataOffset < m_iFileSize)
	{
		while  (::memcmp( &(m_pbData[ iDataOffset ]), "MTrk", 4 ) != 0 && 
			iDataOffset < m_iFileSize)
		{
			iDataOffset++;
			if (iDataOffset >= m_iFileSize - 4) goto ENDOFFILE;
		}

		iDataOffset++;
		m_iTracks++;
	}

	ENDOFFILE :;
}

BOOL CMidiFile::CountMidiEvents()
{
	m_iNumberOfEvents = 0;
	int iDataOffset = 0;

	// find first track
//	while  (memcmp( &(m_pbData[ dwDataOffset ]), "MTrk", 4 ) != 0 && 
//		dwDataOffset < m_dwFilesize)
//	{
//		dwDataOffset++;
//	}
	iDataOffset = MF_HEADERSIZE;	// this is much easier: move past 14-byte header chunk

	try
	{	
		for (int i = 0; i < m_iTracks; i++)
		{
			iDataOffset += MT_HEADERSIZE;	// move past "MTrk" and DWORD track size info
			
			BOOL blEndOfTrack = FALSE;

			while (!blEndOfTrack)
			{
				// move past delta time
				while (m_pbData[ iDataOffset++ ] & 0x80); // loop til MSB == 0

				// meta-events
				if (m_pbData[ iDataOffset ] == 0xff) 
				{
					iDataOffset++;
					int iCommand = m_pbData[ iDataOffset++ ];
					int iCommandLength = m_pbData[ iDataOffset++ ];

					switch ( iCommand ) 
					{
						case 0x2f: blEndOfTrack = TRUE; iDataOffset += iCommandLength; break;
						default: iDataOffset += iCommandLength;
					}	
				}
				// midi commands (only these increment m_iNumberOfEvents)
				else 
				{
					int iCommand = m_pbData[ iDataOffset++ ];
					int iCommandType = iCommand / 0x10;	

					if (iCommandType == 0xC || iCommandType == 0xD) // commands with 1 byte data
					{
						iDataOffset++;
					}	
					else  // commands with 2 bytes data
					{
						iDataOffset++;
						iDataOffset++;
					}		

					m_iNumberOfEvents++;
				}
			}
		}
	}
	catch (...)
	{
		::MessageBox( NULL, "An error occured when counting midi events.", NULL, MB_OK );
		return FALSE;
	}

	return TRUE;
}

BOOL CMidiFile::MakeMidiEvents()
{
	// find first track
	int iDataOffset = MF_HEADERSIZE;	// move past 14-byte header chunk

	// loop through each track
	int iEvent = 0;

	try
	{
		for (int i = 0; i < m_iTracks; i++)
		{
			iDataOffset += MT_HEADERSIZE;	// move past "MTrk" and DWORD track size info
			
			int iAbsoluteDeltaTime = 0;

			BOOL blEndOfTrack = FALSE;

			while (!blEndOfTrack) // get events from one track
			{
				// find delta time
				int i = 1;
				while (m_pbData[ iDataOffset ] & 0x80) // loop til MSB == 0
				{
					iAbsoluteDeltaTime += (m_pbData[ iDataOffset++ ] & 0x7f) 
						<< (8 * i) >> (1 * i);
					i++;
				}
				iAbsoluteDeltaTime += m_pbData[ iDataOffset++ ];

				// deal with meta-events
				if (m_pbData[ iDataOffset ] == 0xff) 
				{
					iDataOffset++;
					int iCommand = m_pbData[ iDataOffset++ ];
					int iCommandLength = m_pbData[ iDataOffset++ ];

					switch ( iCommand ) 
					{
						case 0x2f: blEndOfTrack = TRUE; iDataOffset += iCommandLength; break;
						default: iDataOffset += iCommandLength;
					}	
				}
				// deal with midi commands
				else 
				{
					// set event's absolute delta time
					m_pmevntEvents[ iEvent ].m_dwAbsoluteDeltaTime = 
						iAbsoluteDeltaTime * m_iTempo / m_iDeltasPerQuarter;
					
					// set event's stream ID (always 0)
					m_pmevntEvents[ iEvent ].m_midievent.dwStreamID = 0;

					// set event's dwEvent's high byte
					PBYTE pbEvent = (PBYTE)&m_pmevntEvents[ iEvent ].m_midievent.dwEvent;
					*pbEvent = MEVT_SHORTMSG;

					// read command
					BYTE bCommand = m_pbData[ iDataOffset++ ];
					BYTE bCommandType = bCommand / 0x10;	

					if (bCommandType == 0xC || bCommandType == 0xD) // commands with 1 byte data
					{
						// set event's dwEvent's low 2 bytes
						*(pbEvent) = bCommand;
						*(pbEvent + 1) = m_pbData[ iDataOffset++ ];
					}	
					else  // commands with 2 bytes data
					{
						// make sure NOTEOFF happens a butt-hair early
						if (bCommandType == 0x8 && 
							m_pmevntEvents[ iEvent ].m_dwAbsoluteDeltaTime != 0) 
								m_pmevntEvents[ iEvent ].m_dwAbsoluteDeltaTime--;

						// set event's dwEvent's low 3 bytes
						*(pbEvent) = bCommand;
						*(pbEvent + 1) = m_pbData[ iDataOffset++ ];
						*(pbEvent + 2) = m_pbData[ iDataOffset++ ];
					}
					
//					m_pmevntEvents[ nEvent ].m_midievent.dwParms[0] = 0;

					iEvent++; // go to next event
				}
			}
		}	
	}
	catch (...)
	{
		::MessageBox( NULL, "An error occured when compiling midi events.", NULL, MB_OK );
		return FALSE;
	}

	return TRUE;
}

void CMidiFile::SortMidiEvents()
{
	::qsort( m_pmevntEvents, m_iNumberOfEvents, sizeof CMidiEvent, 
		CMidiEvent::Compare );

	m_pmevntEvents[ 0 ].m_midievent.dwDeltaTime = m_pmevntEvents[ 0 ].m_dwAbsoluteDeltaTime;

	for (int i = 1; i < m_iNumberOfEvents; i++)
	{
		m_pmevntEvents[ i ].m_midievent.dwDeltaTime =
			m_pmevntEvents[ i ].m_dwAbsoluteDeltaTime - 
			m_pmevntEvents[ i - 1 ].m_dwAbsoluteDeltaTime;
	}
}


