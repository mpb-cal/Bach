
#include "CLinkedList.h"


CLinkedList::CLinkedList() :
	m_pHead( NULL ),
	m_iSize( 0 )
{
	m_pHead = new CLinkedListItem;
}

CLinkedList::~CLinkedList()
{
	RemoveAll();

	delete m_pHead;
	m_pHead = NULL;
}

const CLinkedList&	CLinkedList::operator=( const CLinkedList& rvalue )
{
	RemoveAll();

	delete m_pHead;
	m_pHead = NULL;

	m_pHead = new CLinkedListItem;

	for (int i = 0; i < rvalue.m_iSize; i++)
	{
		CLinkedListItem* pItem;
		rvalue[ i ]->MakeCopy( pItem );
		AppendItem( pItem );
	}

	return *this;
}

void CLinkedList::Dump( char* szString ) const
{
	for (int i = 0; i < GetSize(); i++)
	{
		operator[]( i )->Dump( szString );
	}
}

BOOL CLinkedList::Output( FILE* pFile ) const
{
	for (int i = 0; i < m_iSize; i++)
	{
		if (!(operator[]( i )->Output( pFile ))) return FALSE;
	}

	return TRUE;
}

BOOL CLinkedList::Input( FILE* pFile )
{
	for (int i = 0; i < GetSize(); i++)
	{
		if (!(operator[]( i )->Input( pFile ))) return FALSE;
	}

	return TRUE;
}

void 
CLinkedList::AppendItem( CLinkedListItem* pNewItem )
{
	CLinkedListItem* pItem = m_pHead;

	while (pItem->GetNext())
	{
		pItem = pItem->GetNext();
	}

	pItem->SetNext( pNewItem );

	pNewItem->SetPrevious( pItem );

	m_iSize++;
}

void 
CLinkedList::InsertItem( CLinkedListItem* pNewItem, int iPrevious )
{
	CLinkedListItem* pPrevious = operator[]( iPrevious );
	
	if (pPrevious)
	{
		CLinkedListItem* pNext = pPrevious->GetNext();
	
		if (pNext)
		{
			pNext->SetPrevious( pNewItem );

			pNewItem->SetNext( pNext );
		}

		pPrevious->SetNext( pNewItem );

		pNewItem->SetPrevious( pNext );

		m_iSize++;
	}
}

void 
CLinkedList::RemoveItem( int iAt )
{
	CLinkedListItem* pItem = operator[]( iAt );
	
	if (pItem)
	{
		CLinkedListItem* pNext = pItem->GetNext();
		CLinkedListItem* pPrevious = pItem->GetPrevious();

		if (pNext)
		{
			pNext->SetPrevious( pPrevious );
		}

		if (pPrevious)
		{
			pPrevious->SetNext( pNext );
		}

		delete pItem;

		m_iSize--;
	}
}

void
CLinkedList::ChopItem()
{
	RemoveItem( GetSize() - 1 );
}

void 
CLinkedList::RemoveAll()
{
	int iSize = GetSize();
	
	for (int i = 0; i < iSize; i++)
	{
		ChopItem();
	}
}

CLinkedListItem*
CLinkedList::operator[]( int iAt ) const
{
	if (iAt < 0) return NULL;

	CLinkedListItem* pItem = m_pHead;
	
	for (int i = 0; i <= iAt; i++)
	{
		pItem = pItem->GetNext();

		if (pItem == NULL) return NULL;
	}

	return pItem;
}

int 
CLinkedList::GetSize() const
{
	return m_iSize;
}
