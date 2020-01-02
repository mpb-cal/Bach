
// bmstorage.cpp

#include "stdafx.h"

#ifndef __CBMSTORAGE_H__
#include "CBMStorage.h"
#endif

#ifndef __CONSTANT_H__
#include "Constant.h"
#endif

// used in CBMStorage constructor
#define STOREBITMAP( IDB, X )				\
	hbmCurrent = ::LoadBitmap( hInstance, MAKEINTRESOURCE( IDB ) );			\
	::SelectObject( hdcCurrent, hbmCurrent );	\
	::BitBlt( m_hdcBMStorage, X, 0, BMS_WIDTH, BMS_HEIGHT, hdcCurrent, 0, 0, SRCCOPY );	\
	::DeleteObject( hbmCurrent );

// static member initialization

HDC CBMStorage::m_hdcBMStorage;
 
void CBMStorage::Initialize( HINSTANCE hInstance )
{
	// create bitmap storage
	// prepare DC for permanent bitmap storage
	m_hdcBMStorage = ::CreateCompatibleDC( NULL );

	short* psiInitialValues = new short[ BMS_STOREWIDTH * BMS_STOREHEIGHT ];
	::FillMemory( psiInitialValues, BMS_STOREWIDTH * BMS_STOREHEIGHT, 0xFF );
	HBITMAP hbmStorage = ::CreateBitmap( BMS_STOREWIDTH, BMS_STOREHEIGHT, 1, 1, psiInitialValues );
	delete[] psiInitialValues;

	::SelectObject( m_hdcBMStorage, hbmStorage );

	// prepare DC for temporary bitmap storage
	HDC hdcCurrent = ::CreateCompatibleDC( NULL );

	// prepare Bitmap for temporary bitmap storage
	HBITMAP hbmCurrent;

	// store clef bitmaps
	STOREBITMAP( IDB_TREBLECLEF, BM_TREBLECLEF );
	STOREBITMAP( IDB_BASSCLEF, BM_BASSCLEF );

	// store note bitmaps
	STOREBITMAP( IDB_THIRTYSECOND, BM_THIRTYSECOND );
	STOREBITMAP( IDB_SIXTEENTH, BM_SIXTEENTH );
	STOREBITMAP( IDB_SIXTEENTHD, BM_SIXTEENTHD );
	STOREBITMAP( IDB_EIGHTH, BM_EIGHTH );
	STOREBITMAP( IDB_EIGHTHD, BM_EIGHTHD );
	STOREBITMAP( IDB_QUARTER, BM_QUARTER );
	STOREBITMAP( IDB_QUARTERD, BM_QUARTERD );
	STOREBITMAP( IDB_HALF, BM_HALF );
	STOREBITMAP( IDB_HALFD, BM_HALFD );
	STOREBITMAP( IDB_WHOLE, BM_WHOLE );

	// store stem-up note bitmaps
	STOREBITMAP( IDB_THIRTYSECONDU, BM_THIRTYSECONDU );
	STOREBITMAP( IDB_SIXTEENTHU, BM_SIXTEENTHU );
	STOREBITMAP( IDB_SIXTEENTHDU, BM_SIXTEENTHDU );
	STOREBITMAP( IDB_EIGHTHU, BM_EIGHTHU );
	STOREBITMAP( IDB_EIGHTHDU, BM_EIGHTHDU );
	STOREBITMAP( IDB_QUARTERU, BM_QUARTERU );
	STOREBITMAP( IDB_QUARTERDU, BM_QUARTERDU );
	STOREBITMAP( IDB_HALFU, BM_HALFU );
	STOREBITMAP( IDB_HALFDU, BM_HALFDU );
	STOREBITMAP( IDB_WHOLEU, BM_WHOLEU );

	// store rest bitmaps
	STOREBITMAP( IDB_THIRTYSECONDR, BM_THIRTYSECONDR );
	STOREBITMAP( IDB_SIXTEENTHR, BM_SIXTEENTHR );
	STOREBITMAP( IDB_SIXTEENTHDR, BM_SIXTEENTHDR );
	STOREBITMAP( IDB_EIGHTHR, BM_EIGHTHR );
	STOREBITMAP( IDB_EIGHTHDR, BM_EIGHTHDR );
	STOREBITMAP( IDB_QUARTERR, BM_QUARTERR );
	STOREBITMAP( IDB_QUARTERDR, BM_QUARTERDR );
	STOREBITMAP( IDB_HALFR, BM_HALFR );
	STOREBITMAP( IDB_HALFDR, BM_HALFDR );
	STOREBITMAP( IDB_WHOLER, BM_WHOLER );

	// store accidental bitmaps
	STOREBITMAP( IDB_SHARP, BM_SHARP );
	STOREBITMAP( IDB_NOACC, BM_NOACC );
}

// iBitmap is BM_BITMAP
void CBMStorage::DrawBitmap( HDC hdc, int iX, int iY, int iZoomDivisor, int iBitmap )
{
	::StretchBlt(	hdc, 
					iX, 
					iY, 
					BMS_WIDTH / iZoomDivisor, 
					BMS_HEIGHT / iZoomDivisor, 
					m_hdcBMStorage, 
					iBitmap, 
					0, 
					BMS_WIDTH, 
					BMS_HEIGHT, 
					SRCAND	);
}

void CBMStorage::DrawEntireBitmap( HDC hdc, int iX, int iY )
{
	::BitBlt(	hdc, 
				iX, 
				iY, 
				BMS_WIDTH * BMS_NUMBER, 
				BMS_HEIGHT, 
				m_hdcBMStorage, 
				0, 
				0, 
				SRCAND	);
}
