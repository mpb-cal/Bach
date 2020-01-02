
// cbmstorage.h

#ifndef __CBMSTORAGE_H__
#define __CBMSTORAGE_H__

#include "resource.h"


class CBMStorage
{
public:
	static void		Initialize( HINSTANCE hInstance );
	static void		DrawBitmap( HDC hdc, int iX, int iY, int iZoomDivisor, int iBitmap );
	static void		DrawEntireBitmap( HDC hdc, int iX, int iY );

protected:
	static HDC		m_hdcBMStorage;

private:

};



#endif __BMSTORAGE_H__
