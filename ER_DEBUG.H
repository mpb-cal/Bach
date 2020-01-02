/****************************************************************************\
 *
 *        File: ER_DEBUG.H
 *
 *      Author: JPB
 *
 *
 *    Abstract:
 *
 *      Debugging support class header.
 *
 *
 *   Copyright:
 *
 *      Copyright (c) 1995, 1996 by Eagle Research, Inc.
 *      Copyright (c) 1995, 1996 by Digital Sound Corporation
 *
 *
 *     Changes:
 *
 *      Who --Date--    -----------------------Comment-----------------------
        JPB 95.12.??    Created
 *      JCM 96.08.13    Modified to work with new (as of vc4.x) stream library.
 *      JCM 96.08.27    Merged vc4.2 and vc2.2 code versions.
 *      JCM 96.10.08    Cleaned up includes
 *      $$Changes
 *
\****************************************************************************/

#ifndef _ER_DEBUG_H
#define _ER_DEBUG_H

#ifndef _WINDOWS_H
#include<windows.h>
#define _WINDOWS_H
#endif

#ifndef _WINNLS_H
#include<winnls.h>
#define _WINNLS_H
#endif

#ifndef _TABLE_H_
#include "table.h"
#endif 

//
// Choose new ansi c++ <template> based stream or old style stream
//
#ifdef STD_STREAM

#ifndef _OSTREAM_V
#include<ostream>
#define _OSTREAM_V
#endif

#ifndef _IOMANIP_V
#include<iomanip>
#define _IOMANIP_V
#endif

#ifndef _STRSTREAM_V
#include<strstream>
#define _STRSTREAM_V
#endif

#else /* ifdef STD_STREAM */

#ifndef _OSTREAM_H
#include<ostream.h>
#define _OSTREAM_V
#endif

#ifndef _IOMANIP_H
#include<iomanip.h>
#define _IOMANIP_H
#endif

#endif /* ifdef STD_STREAM */

#define EAG_DEBUG
#ifdef EAG_DEBUG

#pragma warning( disable: 4270 )

#define DDT_MAX_INDENT (511)

#define DDT_BEGIN_BLOCK DDT_dfl.Lock();
#define DDT_END_BLOCK DDT_dfl.Unlock();

#ifdef ERD_DEBUG_MAPI
#define DDT_MAPI_VALIDATE_RETURN( funcname, hr ) DDT_MAPI_Validate_Return( funcname, hr )
#endif

#define DDTQ

#define DDTL( x ) if( DDT_dfl.IsEnabled( x )) DDT_dfl

#define DDT_( x ) DDTL(x) << beginline
#define DDTC_( x ) DDTL(x)

#define DDTE DDT_( 0 )
#define DDTEC DDTC_( 0 )

#define DDTW DDT_( 1 )
#define DDTWC DDTC_( 1 )

#define DDT DDT_( 4 )
#define DDTC DDTC_( 4 )

#define DDT_INIT( x, y ) ERDebugStream DDT_dfl( x, y );
#define DDT_INIT_QUIET( y ) ERDebugStream DDT_dfl( NULL, y );

//#define DDT_OUTPUT_DATA( len, ptr )                                 \
//    {                                                               \
//        for( ULONG ddt_i = 0; ddt_i < (ULONG)(len); ddt_i++ )       \
//        {                                                           \
//            DDT << int(((unsigned char *)(ptr))[ddt_i]);            \
//            if( !((ddt_i+1) % 16 ) && ddt_i != (ULONG)(len) - 1)    \
//            {                                                       \
//                DDT << endl;                                        \
//            }                                                       \
//            if( !(ddt_i % 2) )                                      \
//            {                                                       \
//                DDT << ' ';                                         \
//            }                                                       \
//        }                                                           \
//        DDT << endl;                                                \
//    }

HRESULT DDT_MAPI_Validate_Return( LPCSTR pFunctionName, HRESULT hResult );

class ERDebugStream;

inline
ostream&
operator << ( ostream& ros, RECT &rrc )
{
    ros << '(' << rrc.left << ',' << rrc.top << ',' << rrc.right << ',' << rrc.bottom << ')';
    return ros;
}

//
// New template based
//

ostream& formatsysmessage_fp( ostream& os, long hr );
//ostream& __cdecl formatsysmessage_fp( ostream& os, unsigned long hr );
//ostream& __cdecl formatsysmessage_fp( ostream& os, long hr );

//class basic_ostream<char,struct char_traits<char>>
//&( class basic_ostream<char,struct char_traits<char>> &,unsigned long)' (new behavior; please see help)

#ifdef STD_STREAM

//
// Following should be defined by the vc4.2 libraries, but is not!
//
#ifdef STD_OMANIP_NOT_DEFINED

template<class _T>
struct omanip
{
	omanip( ostream& (*_F)( ostream&, _T), _T _A)
		: _Pf(_F), _Manarg(_A) {}
	ostream& (*_Pf)(ostream&, _T);
	_T _Manarg;
};

template<class _T> inline
ostream& operator<<( ostream& _O, const omanip<_T>& _M)
{
	(*_M._Pf)(_O,_M._Manarg);
	return( _O );
}

#endif /* _STD_OMANIP_DEFINED */

inline
omanip<long> formatsysmessage( long A )
{
    return omanip<long>(&formatsysmessage_fp, A);
}

#else
IOMANIPdeclare( HRESULT );
OMANIP(HRESULT) formatsysmessage( HRESULT hr );
#endif


#ifdef ERD_DEBUG_MAPI

ostream& propertytag_fp( ostream& os, ULONG ulTag );

#ifdef STD_STREAM

inline
omanip<ULONG> propertytag( ULONG A )
{
    return omanip<ULONG>(&propertytag_fp,A);
}

#else
IOMANIPdeclare( ULONG );
OMANIP(ULONG) propertytag( ULONG ulTag );
#endif

ostream& propertyid_fp( ostream& os, WORD wType );

#ifdef STD_STREAM

inline
omanip<WORD> propertyid( WORD A )
{
    return omanip<WORD>(&propertyid_fp,A);
}

#else
IOMANIPdeclare( WORD );
OMANIP(WORD) propertyid( WORD wType );
#endif

ostream& propertytype_fp( ostream& os, WORD wType );

#ifdef STD_STREAM

inline
omanip<WORD> propertytype( WORD A )
{
    return omanip<WORD>(&propertytype_fp,A);
}

#else
OMANIP(WORD) propertytype( WORD wType );
#endif

#endif /* #ifdef ERD_DEBUG_MAPI */


typedef const GUID * LPCGUID;

ostream& guidstring_fp( ostream& os, LPCGUID pGUID );

#ifdef STD_STREAM

inline
omanip<const GUID *> guidstring( const GUID * A )
{
    return omanip<const GUID *>(&guidstring_fp,A);
}
#else
IOMANIPdeclare( LPCGUID );
OMANIP( LPCGUID ) guidstring( LPCGUID pGUID );
#endif

struct pointer_and_count_params
{
    void   *pData;
    DWORD   dwBytes;
};

#ifndef STD_STREAM
IOMANIPdeclare( pointer_and_count_params );
#endif

ostream& hexdump_fp( ostream& os, pointer_and_count_params params );
#ifdef STD_STREAM

//C'_Smanip<struct pointer_and_count_params>::_Smanip<struct pointer_and_count_params>'
// : none of the 2 overloads can convert parameter 1 from type
// 'class basic_ostream<char,struct char_traits<char>> &
//    (__cdecl *)( class basic_ostream<char,struct char_traits<char>> &,
//                   struct pointer_and_count_params)' (new behavior; please see help)

inline
omanip<pointer_and_count_params> hexdump( void * pData, DWORD dwBytes )
{
    pointer_and_count_params _A;
    _A.pData = pData;
    _A.dwBytes = dwBytes;
    return omanip<pointer_and_count_params>(&hexdump_fp, _A);
}

#else
OMANIP(pointer_and_count_params) hexdump( void *pData, DWORD dwBytes );
#endif

ostream& hexstring_fp( ostream& os, pointer_and_count_params params );
#ifdef STD_STREAM

inline
omanip<pointer_and_count_params> hexstring( void * pData, DWORD dwBytes )
{
    pointer_and_count_params _A;
    _A.pData = pData;
    _A.dwBytes = dwBytes;
    return omanip<pointer_and_count_params>(&hexstring_fp, _A);
}
#else
OMANIP(pointer_and_count_params) hexstring( void *pData, DWORD dwBytes );
#endif

ostream& b32dump_fp( ostream& os, pointer_and_count_params params );
#ifdef STD_STREAM

inline
omanip<pointer_and_count_params> __cdecl b32dump( void * pData, DWORD dwBytes )
{
    pointer_and_count_params _A;
    _A.pData = pData;
    _A.dwBytes = dwBytes;
    return omanip<pointer_and_count_params>(&b32dump_fp, _A);
}
#else
OMANIP(pointer_and_count_params) b32dump( void *pData, DWORD dwBytes );
#endif

ostream& timestamp( ostream &os );
ostream& timetoken( ostream &os );
ostream& timeshort( ostream &os );

ostream& threadstamp( ostream &os );
ostream& threadtoken( ostream &os );

ostream& beginline( ostream &os );
//ERDebugStream&  operator << (   ERDebugStream& debugstream,
//                                ERDebugStream& (*_f)(ERDebugStream&)    );

class ERDebugStreamBuf: public streambuf
{
    public:
        ~ERDebugStreamBuf( void );

        static DWORD GetThreadIndent( void );
        static void  SetThreadIndent( DWORD dwIndent );
        static DWORD sm_dwIndentTlsIndex;

    protected:
        friend class ERDebugStream;

        ERDebugStreamBuf( void );

        void Lock( void );
        void Unlock( void );

        virtual int underflow( void );
        virtual int overflow( int nCh = EOF );
        virtual int sync( void );

        CRITICAL_SECTION m_cs;
        char m_OutputBuffer[1024];
};

class ERDebugStream: public ostream
{
    public:
        ERDebugStream( LPCSTR pszHeading, LPCSTR pGroupId ):
            ostream( &sm_GlobalBuffer ),
            m_pszHeading( pszHeading ),
            m_pGroupId( pGroupId ),
            m_ulLockCount( 0 )
        {
            if( ERDebugStream::sm_iDDTLevel > 2 )
            {
                Lock();
                DWORD dwIndent = ERDebugStreamBuf::GetThreadIndent();
                if( m_pszHeading )
                {
                    *this << beginline << sm_pszEnterString << m_pszHeading << endl;
                }
                if( dwIndent < DDT_MAX_INDENT )
                {
                    ++dwIndent;
                    ERDebugStreamBuf::SetThreadIndent( dwIndent );
                }
                Unlock();
            }
        };
        ~ERDebugStream( void )
        {
            if( ERDebugStream::sm_iDDTLevel > 2 )
            {
                Lock();
                DWORD dwIndent = ERDebugStreamBuf::GetThreadIndent();
                if( dwIndent )
                {
                    --dwIndent;
                    ERDebugStreamBuf::SetThreadIndent( dwIndent );
                }
                if( m_pszHeading )
                {
                    *this << beginline << sm_pszLeaveString << m_pszHeading << endl;
                }
            }
            // 96.07.10 JCM moved from inside of error level test.
            // This will now correctly unlock even if we are not
            // running with debug output turned on...
            if( m_ulLockCount )
            {
                sm_GlobalBuffer.Unlock();
            }
        };
        ERDebugStream &Lock( void )
        {
            if( ! m_ulLockCount++ )
            {
                sm_GlobalBuffer.Lock();
            }
            return *this;
        };
        ERDebugStream &Unlock( void )
        {
            if( ! --m_ulLockCount )
            {
                sm_GlobalBuffer.Unlock();
            }
            return *this;
        };
        BOOL IsEnabled( DWORD dwLevel )
        {
            DWORD dwMinLevel = ERDebugStream::sm_iDDTLevel;
            sm_KeyTable.GetValue( m_pGroupId, dwMinLevel );

            return dwLevel & dwMinLevel;
        };
        static void InitKeyTable( LPCSTR szSubKey )
        {
            sm_KeyTable.Initialize( szSubKey );
        }

        static void InitDebugLevel( HINSTANCE hinst );

        static int sm_iDDTLevel;

    protected:
        friend ostream &beginline( ostream &debugstream );

        LPCSTR m_pszHeading;
        LPCSTR m_pGroupId;
        ULONG  m_ulLockCount;
        static LPCSTR sm_pszEnterString;
        static LPCSTR sm_pszLeaveString;
        static ERDebugStreamBuf sm_GlobalBuffer;
        static CTable sm_KeyTable;

};


#else /* EAG_DEBUG */

#define DDT_BEGIN_BLOCK ///*
#define DDT_END_BLOCK //*/

#define DDT_MAPI_VALIDATE_RETURN( funcname, hr ) (hr)

#define DDTQ //

#define DDT_( x ) //

#define DDT //
#define DDTC //

#define DDT_INIT( x ) //
#define DDT_INIT_QUIET //
#endif /* EAG_DEBUG */

#endif /* _ER_DEBUG_H */