

#ifndef __CDIALOGBOX_H__
#define __CDIALOGBOX_H__

#include <windows.h>
#include <stdio.h>
#include "resource.h"


class CDialogBox
{

	public :
		
						CDialogBox();
		virtual			~CDialogBox();

// call this method to display the dialog box:
		BOOL			DoModal( HINSTANCE hInstance, HWND hwndParent );

	protected :

// override the following 3 methods:
		virtual	BOOL	OnInitDialog( HWND hDlg ) = 0;
		virtual	BOOL	OnOk( HWND hDlg ) = 0;
		virtual	BOOL	CommandHandler( HWND hDlg, WPARAM wParam, LPARAM lParam ) = 0;

		BOOL					DialogProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam );
		friend BOOL CALLBACK	DialogProcStub( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam );

// set this string in the constructor, then call the base constructor:
		char					m_szDialogResourceName[ 512 ];
};


// to validate dialog control values, declare minimum and maximum values:
//	const int DIALOG_XXX_MIN = 1;
//	const int DIALOG_XXX_MAX = 10;
// check bounds in the OnOk() implementation:
//	CHECK_BOUNDS( m_xxx, XXX );

#define CHECK_BOUNDS( x, y )	if (x < DIALOG_##y##_MIN || x > DIALOG_##y##_MAX)	\
	{	\
		char _szOutOfBounds[ 512 ];	\
		sprintf(	_szOutOfBounds,										\
					"Please enter an integer between %d and %d.",		\
					DIALOG_##y##_MIN,									\
					DIALOG_##y##_MAX	);								\
		MessageBox( hDlg, _szOutOfBounds, "Bach", MB_OK | MB_ICONEXCLAMATION );	\
		SendDlgItemMessage(	hDlg,												\
							IDC_##y##,										\
							EM_SETSEL,										\
							(WPARAM)0,											\
							(LPARAM)-1	);										\
																				\
		return FALSE;															\
	}


#endif

