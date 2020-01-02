

#ifndef __CBACHCOMPONENT_H__
#define __CBACHCOMPONENT_H__


#include "IBachSong.h"

#include "CApplication.h"



class CBachComponent : public IBachSong, public CApplication
{
public:

	CBachComponent();
	~CBachComponent();

public:

	STDMETHOD(QueryInterface)(REFIID riid, void FAR* FAR* ppv);
	STDMETHOD_(ULONG, AddRef)(void);
	STDMETHOD_(ULONG, Release)(void);

	STDMETHOD(ComposeSong)(THIS);
	STDMETHOD(PlaySong)(THIS);
	STDMETHOD(StopSong)(THIS);

private:

	ULONG m_ulRefCount;

};



#endif

