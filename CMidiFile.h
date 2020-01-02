/* cmidifile.h, cmiditrack.h, cmidinote.h
	
	Contain class definitions for midi files:
	(MidiEvent: data for /dev/sequencer)
	CMidiNote: pitch, length
	CMidiTrack: array of Midinotes, patch number(0-127), 
		channel number(1-16), velocity, keysig, timesig
	CMidiFile: array of Miditracks, tempo, keysig, timesig

	Interface is thru class CMidiFile:
	1) CMidiFile(# of tracks, tempo in bpm, 
		keysig(-7 = 7 flats,0 = key of C, 7 = 7 sharps), 
		mode(0 = major, 1 = minor),
		timesig numerator, timesig denominator);
	2) For each track, do:
		 CMidiFile.edit_track(
			# of notes,array of pitches(0-127),
			array of lengths(1-32:see below),
			patch #, channel #, velocity);

*/

#ifndef __CMIDIFILE_H__
#define __CMIDIFILE_H__

#include <fstream.h>

#ifndef __CBASEOBJECT_H__
#include "CBaseObject.h"
#endif

class CMidiTrack;
class CMidiEvent;

class CMidiFile
{
friend DWORD PlayThread( LPVOID pParam );

public:
			CMidiFile();
			CMidiFile(	int iTracks, 
						double dDpq, 
						int iKey, 
						int iMode, 
						int iTimen, 
						int iTimed	);
			~CMidiFile();
	void	Write( ostream& fout );
	BOOL	Read( HANDLE hFile );
	BOOL	Play( int iTempo, BOOL blLoop );
	void	Stop();
	BOOL	IsPlaying() { return m_blPlaying; }

//	void	play();	//call save() and load() first!!
	void	edit_track(	int iTrack, 
						int iSize, 
						int* piPitches, 
						int* piLengths,
						int iPatch, 
						int iPan,
						int iChannel, 
						int iVelocity	);

	int		GetFileFormat() { return m_iFileFormat; }
	int		GetTracks() { return m_iTracks; }
	int		GetTempo() { return m_iDeltasPerQuarter; }

protected:

	unsigned char*	m_pucHeader;
	int				m_iFileFormat;			//0,1 or 2
	int				m_iTracks;
	int				m_iDeltasPerQuarter;

	int				m_iTempo;
	BYTE*			m_pbData;
	CMidiEvent*		m_pmevntEvents;
    MIDIHDR			m_mhdrMidiHeader;
	int				m_iNumberOfEvents;

	int				m_iFileSize;
	int				m_iFilePtr;
	CMidiTrack**	m_ppTracks;
	int				m_iKeySignature;			//-7 to 7 (# of flats/sharps)
	int				m_iMode;				//0=major, 1=minor
	int				m_iTimesigN;
	int				m_iTimesigD;

	BOOL			m_blStop;
	BOOL			m_blPlaying;
	BOOL			m_blLooping;

	void			CountTracks();
	BOOL			CountMidiEvents();
	BOOL			MakeMidiEvents();
	void			SortMidiEvents();
};

#endif __MIDIFILE_H__


