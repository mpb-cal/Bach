// CStaff.h

#ifndef __CSTAFF_H__
#define __CSTAFF_H__

#ifndef __CONSTANT_H__
#include "Constant.h"
#endif

#ifndef __CBASEOBJECT_H__
#include "CBaseObject.h"
#endif

class CSong;

class CStaff : public CBaseObject
{
	friend class CSong;

public:
				CStaff();
				~CStaff();

	const CStaff& operator=( const CStaff& rvalue );

	void		Dump( char* szString ) const;

	// input and output methods
	BOOL		Output( FILE* pFile ) const;
	BOOL		Input( FILE* pFile );

	void		SetNumVoices( int iNumVoices );
	BOOL		Draw( HDC hdc ) const;

protected:

	int			m_iLength;
	int			m_iNumVoices;
	int			m_iYpos[ 256 ];

	HPEN		m_hMutePen;

};


#endif