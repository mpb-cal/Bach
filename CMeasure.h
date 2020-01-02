
// cmeasure.h

#ifndef __CMEASURE_H__
#define __CMEASURE_H__

#ifndef __CMUSICALTHING_H__
#include "CMusicalThing.h"
#endif

#ifndef __CNOTE_H__
#include "CNote.h"
#endif

#include "CLinkedList.h"

class CTick;

class CMeasure : public CMusicalThing, public CLinkedListItem
{

public:

					CMeasure();
					CMeasure(	int iKey, 
								int iYpos, 
								int iXpos, 
								int iNumber, 
								int iClef,
								int iTicksPerMeasure	);
					~CMeasure();

	const CMeasure&	operator=( const CMeasure& rvalue );
	void			MakeCopy( CLinkedListItem*& rpNewCopy );

	void			Dump( char* szString ) const;

	BOOL			Output( FILE* pFile ) const;
	BOOL			Input( FILE* pFile );

	void			Draw( HDC hdc ) const;
	void			Make( int iTop );
	void			Normalize( int iTop );
	
	void			AddNote(	CNote* pNote, 
								int iTick, 
								BOOL blRest, 
								BOOL blSharp, 
								BOOL blRememberAttack	);
	void			ClearNotes();

	BOOL			SetTicksPerMeasure( int iTicksPerMeasure );

	int				GetNoteCount() const;
	void			GetPitches( int*& rpiPitches ) const;
	void			GetValues( int*& rpiValues ) const;
	RECT			GetRect() const;
	int				GetTickAtPoint( POINT p ) const;
	int 			GetXpos() const { return m_iXpos; }
	int 			GetYpos() const { return m_iYpos; }

	void			GetTicks( CTick* pTicks, int& rnIndex, int iFrom, int iTo ) const;
	void			PutTicks( CTick* pTicks, int& rnIndex, int iFrom, int iTo );

	int				GetMaxLinesAbove() const;
	int				GetMaxLinesBelow() const;

	void			TransposeNotes(	int iHalfSteps, 
									ENoteDirection Direction, 
									int iFrom, 
									int iTo	);
	void			TransposeDiatonic(	int iPitches, 
										ENoteDirection Direction, 
										int iFrom, 
										int iTo	);

	BOOL			HasNotes() const;
	void			RemoveAttacks();

protected:

	CTick*			m_pTicks;
	CNote**			m_ppNotes;
	int				m_iNotes;
		
	char			m_szNumber[ 6 ];
	int				m_iXpos;
	int				m_iYpos;	//same as staff top

	void			MakeNotes();

private:

};



#endif __MEASURE_H__
