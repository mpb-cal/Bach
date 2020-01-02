
#ifndef __IBACHSONG_H__
#define __IBACHSONG_H__


#include <windows.h>


// {588E26C0-550A-11d1-91C6-CC7002C10000}
DEFINE_GUID(IID_IBachSong, 
0x588e26c0, 0x550a, 0x11d1, 0x91, 0xc6, 0xcc, 0x70, 0x2, 0xc1, 0x0, 0x0);

#undef INTERFACE
#define INTERFACE IBachSong

#define EXPORT extern "C" __declspec (dllexport)


DECLARE_INTERFACE_(IBachSong, IUnknown)
{
    /* IUnknown methods */
    STDMETHOD(QueryInterface)(THIS_ REFIID riid, LPVOID FAR* ppvObj) PURE;
    STDMETHOD_(ULONG, AddRef)(THIS) PURE;
    STDMETHOD_(ULONG, Release)(THIS) PURE;

    /* IBachSong methods */
    STDMETHOD(ComposeSong)(THIS) PURE;
    STDMETHOD(PlaySong)(THIS) PURE;
    STDMETHOD(StopSong)(THIS) PURE;
};



EXPORT IBachSong* CreateBachComponent();



#endif
