
#include "resource.h"

#include "CBachComponent.h"

#include <initguid.h>

// {588E26C0-550A-11d1-91C6-CC7002C10000}
DEFINE_GUID(IID_IBachSong, 
0x588e26c0, 0x550a, 0x11d1, 0x91, 0xc6, 0xcc, 0x70, 0x2, 0xc1, 0x0, 0x0);



EXPORT IBachSong* CreateBachComponent()
{
	CBachComponent* pComponent = new CBachComponent;

	if (pComponent)
	{
		pComponent->AddRef();
	}

	return (IBachSong*)pComponent; 
}


CBachComponent::CBachComponent() :
	CApplication(),
	m_ulRefCount( 0 )
{
	g_uiClipboardFormat = ::RegisterClipboardFormat( g_szClipboardFormat );

	if (!CreateMainWindow()) m_iReturnValue = -1;

	if (m_iReturnValue == 0)
	{
		::SendMessage( m_hwndMainWindow, WM_COMMAND, ID_FILE_NEW, 0 );
	}
}

CBachComponent::~CBachComponent()
{
}

STDMETHODIMP
CBachComponent::QueryInterface(REFIID riid, void FAR* FAR* ppv)
{
	// riid identifies an interface type.  Do we support it?

	if (IsEqualIID( riid, IID_IUnknown ))
	{
		*ppv = (LPUNKNOWN)this;
	}
	else if (IsEqualIID( riid, IID_IBachSong ))
	{
		*ppv = (IBachSong*)this;
	}
	else
	{
		*ppv = NULL;
		return ResultFromScode(E_NOINTERFACE);
	}

	AddRef();
	return NOERROR;
}

STDMETHODIMP_(ULONG)
CBachComponent::AddRef()
{
	return ++m_ulRefCount;
}

STDMETHODIMP_(ULONG)
CBachComponent::Release()
{
	if (--m_ulRefCount == 0)
	{
		delete this;
		return 0;
	}

	return m_ulRefCount;
}

STDMETHODIMP
CBachComponent::ComposeSong()
{
	HRESULT hResult = S_OK;

	OnComposeSong();

	return hResult;
}

STDMETHODIMP
CBachComponent::PlaySong()
{
	HRESULT hResult = S_OK;

	OnPlayGo();

	return hResult;
}

STDMETHODIMP
CBachComponent::StopSong()
{
	HRESULT hResult = S_OK;

	OnPlayStop();

	return hResult;
}

