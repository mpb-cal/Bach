
#include "stdafx.h"

#ifndef __CAPPLICATION_H__
#include "CApplication.h"
#endif


int WINAPI WinMain( HINSTANCE hInstance,
					HINSTANCE hPrevInstance,
					LPSTR lpszCmdLine,
					int nCmdShow	)
{
	CApplication::Run( hInstance, lpszCmdLine, nCmdShow );

	return CApplication::GetReturnValue();
}


