
#ifndef __CMIDIEVENT_H__
#define __CMIDIEVENT_H__

#include <windows.h>

class CMidiFile;

class CMidiEvent
{
friend class CMidiFile;

public:

				CMidiEvent();
				~CMidiEvent();

	static int	Compare( const void* elem1, const void* elem2 );
	MIDIEVENT	GetMIDIEVENT() { return m_midievent; }

private:

	MIDIEVENT	m_midievent;
	DWORD		m_dwAbsoluteDeltaTime;

};


#endif
