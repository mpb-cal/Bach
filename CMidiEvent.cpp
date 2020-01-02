
#include "CMidiEvent.h"
#include "Constant.h"

CMidiEvent::CMidiEvent() :
	m_dwAbsoluteDeltaTime( 0 )
{
	::memset( &m_midievent, 0, sizeof MIDIEVENT );
}

CMidiEvent::~CMidiEvent()
{
}

int CMidiEvent::Compare( const void* elem1, const void* elem2 )
{
	CMidiEvent* e1 = (CMidiEvent*)elem1;
	CMidiEvent* e2 = (CMidiEvent*)elem2;
	
	if (e1->m_dwAbsoluteDeltaTime < e2->m_dwAbsoluteDeltaTime) return -1;
	if (e1->m_dwAbsoluteDeltaTime > e2->m_dwAbsoluteDeltaTime) return 1;
	if (e1->m_dwAbsoluteDeltaTime == e2->m_dwAbsoluteDeltaTime)
	{
		BYTE cmd = (BYTE)e1->m_midievent.dwEvent & 0xF0;
		if ( cmd == MN_NOTEON)
		{
			return 1;
		}

		return 0;
	}

	return 0; // shouldn't get here
}

