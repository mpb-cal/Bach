/*******************************************************************

CLinkedList and CLinkedListItem:

Declare a class that inherits CLinkedListItem and overrides
copy method and input/output methods:

	class CMyThing : public CLinkedListItem
	{
		public :
			CMyThing() : 
				CLinkedListItem()
			{}
			~CMyThing()
			{}

			void	MakeCopy( CLinkedListItem*& rpNewCopy );

			BOOL	Output( FILE* pFile ) const;
			BOOL	Input( FILE* pFile );

		protected :
			...
	};

Define MakeCopy() like this (make sure to define an assignment operator
if necessary):

	void CMyThing::MakeCopy( CLinkedListItem*& rpNewCopy )
	{
		CMyThing* pMyThing = new CMyThing;
		*pMyThing = *this;
		rpNewCopy = pMyThing;
	}

Declare a CLinkedList and use its methods:

	copy assignment:
		const CLinkedList&	operator=( const CLinkedList& rvalue );
	input and output:
		BOOL				Output( FILE* pFile ) const;
		BOOL				Input( FILE* pFile );
	append an item to the list:
		void				AppendItem( CLinkedListItem* pNewItem );
	insert an item after a specified item:
		void				InsertItem( CLinkedListItem* pNewItem, int iPrevious );
	remove and delete specified item:
		void				RemoveItem( int i );
	chop off and delete the last item:
		void				ChopItem();
	remove and delete all items:
		void				RemoveAll();
	retrieve a pointer to a specified item:
		CLinkedListItem*	operator[]( int i ) const;
	get the number of items in the list:
		int					GetSize() const;


*******************************************************************/



#ifndef __CLINKEDLIST_H__
#define __CLINKEDLIST_H__


#include <windows.h>

#include "CBaseObject.h"

class CLinkedListItem : public CBaseObject
{

	public :

		CLinkedListItem() :
			m_pNext( NULL ),
			m_pPrevious( NULL )
		{}

		virtual	~CLinkedListItem() {}

		virtual const CLinkedListItem&	operator=( const CLinkedListItem& rvalue ) { return *this; }
		virtual void					MakeCopy( CLinkedListItem*& rpNewCopy ) {}

		virtual void		Dump( char* szString ) const {}

		// input and output methods
		virtual BOOL		Output( FILE* pFile ) const { return FALSE; }
		virtual BOOL		Input( FILE* pFile ) { return FALSE; }

		void				SetNext( CLinkedListItem* pNext ) { m_pNext = pNext; }
		void				SetPrevious( CLinkedListItem* pPrevious ) { m_pPrevious = pPrevious; }
		CLinkedListItem*	GetNext() { return m_pNext; }
		CLinkedListItem*	GetPrevious() { return m_pPrevious; }

	protected :

		CLinkedListItem*	m_pNext;
		CLinkedListItem*	m_pPrevious;

};


class CLinkedList : public CBaseObject
{

	public :

		CLinkedList();
		~CLinkedList();

		const CLinkedList&	operator=( const CLinkedList& rvalue );

		void				Dump( char* szString ) const;

		// input and output methods
		BOOL				Output( FILE* pFile ) const;
		BOOL				Input( FILE* pFile );

		void				AppendItem( CLinkedListItem* pNewItem );
		void				InsertItem( CLinkedListItem* pNewItem, int iPrevious );
		void				RemoveItem( int i );
		void				ChopItem();
		void				RemoveAll();
		CLinkedListItem*	operator[]( int i ) const;

		int					GetSize() const;

	protected :

		CLinkedListItem*	m_pHead;
		int					m_iSize;

};



#endif
