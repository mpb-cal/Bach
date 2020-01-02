/* cmiditrack.h
*/	

#ifndef __CMIDITRACK_H__
#define __CMIDITRACK_H__

#include <fstream.h>

#ifndef __CMIDINOTE_H__
#include "CMidiNote.h"
#endif

class CMidiTrack
{

public:
			CMidiTrack();
			CMidiTrack(	int iSize, 
						int iPatch, 
						int iPan, 
						int iChannel, 
						int iVelocity,
						int iKeySignature, 
						int iMode, 
						int iTimesigN, 
						int iTimesigD	);

			~CMidiTrack();

	void	Write( ostream& fout );
	BOOL	Read( PBYTE pbData, int& riIndex );

	void	edit_note(int nNumber, int nPitch, int nLength);

private:

	CMidiNote**		m_ppNotes;
	int				m_iNumberNotes;
	int				m_iPatch;
	int				m_iPan;
	int				m_iChannel;
	int				m_iVelocity;
	int				m_iKeySignature;		//-7 to 7 (# of flats/sharps)
	int				m_iMode;			//0=major, 1=minor
	int				m_iTimesigN;
	int				m_iTimesigD;
	int				m_iLengthInBytes;
	unsigned char*	m_pucHeader;
	unsigned char*	m_pucFooter;

};


#endif __MIDITRACK_H__