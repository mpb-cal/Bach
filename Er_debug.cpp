/****************************************************************************\
 *
 *        File: ER_DEBUG.CPP
 *
 *      Author: JPB
 *
 *
 *    Abstract:
 *
 *      Comments go here...
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
 *      JPB 95.12.??    Created
 *      JCM 96.08.27    Merge VC4.2 and VC2.2 versions into common code base.
 *      JCM 96.09.05    Remove uninitialize of critical section in destructor
 *                      of ERDebugStreamBuf. This needs to be revisited, but
 *                      for now this allows shutdown to complete successfully.
 *      JCM 96.10.08    Cleaned up includes
 *      $$Changes
 *
\****************************************************************************/
#include "stdafx.h"
#define DEBUG

//#define _er_debug_c
#include "er_debug.h"

#ifdef ERD_DEBUG_MAPI
#include "mapidbg.h"
#include "mapidefs.h"
#endif

#ifdef STD_STREAM

#ifndef _FSTREAM_V
#include <fstream>
#define _FSTREAM_V
#endif

#ifndef _STRSTREAM_V
#include <strstream>
#define _STRSTREAM_V
#endif

#else

#ifndef _FSTREAM_H
#include <fstream.h>
#define _FSTREAM_H
#endif

#ifndef _STRSTREA_H
#include <strstrea.h>
#define _STRSTREA_H
#endif

#endif

#ifndef  _REGTOOL_H
#include "regtool.h"
#endif

//#ifndef _GETVER_H
//#include "getver.h"
//#endif


char m_szModuleName[2048];
char m_szApplicationName[2048];
int ERDebugStream::sm_iDDTLevel = 0;
LPCSTR ERDebugStream::sm_pszEnterString = "Enter ====> ";
LPCSTR ERDebugStream::sm_pszLeaveString = "Leave ====> ";
ERDebugStreamBuf ERDebugStream::sm_GlobalBuffer;
DWORD ERDebugStreamBuf::sm_dwIndentTlsIndex = 0xFFFFFFFF;
LPCSTR g_szSubkey = "SOFTWARE\\Marc\\ER_Debug";

CTable ERDebugStream::sm_KeyTable;

const LONG MAX_PATH_STRINGLENGTH = 1024L;

struct SErrorLog
{
    public:
        SErrorLog( void );
        ~SErrorLog( void );
        void OutputString( LPCSTR pString );

        fstream m_logfile;
        DWORD m_dwFlags;

        enum
        {
            tokenDebugApp = 1,
            tokenDebugLog = 2,
            tokenFlashMode_Paranoid = 4,
            tokenFlushMode = 8
        };

        unsigned char m_szLogName[MAX_PATH_STRINGLENGTH];
} g_SErrorLog;

SErrorLog::SErrorLog( void ):
    m_dwFlags( tokenDebugApp )
{
    DDT_INIT_QUIET( "" )

    RegEntry regEntry;

    lstrcpy( m_szModuleName, "[Unknown]" );
    GetModuleFileName( NULL, m_szApplicationName, sizeof( m_szApplicationName ) );
/*    char szSubkey[_MAX_PATH];
    HRESULT hResult = GetRegistryVersionKey( &szSubkey[0], sizeof( szSubkey ) );
    if( hResult != S_OK )
    {
        DDT_BEGIN_BLOCK;
        DDTW << "GetReigstryVersionKey failed " << formatsysmessage( hResult );
        DDT_END_BLOCK;
        szSubkey[0] = '\0';
    }


*/
    ULONG lStringLength = MAX_PATH_STRINGLENGTH;
    DWORD dwFlags = 0;
    DWORD dwType = REG_SZ;
    HKEY hkLogKey = 0;

    HRESULT hResult = regEntry.Open(    HKEY_LOCAL_MACHINE,
                                        g_szSubkey,
                                        NULL,
                                        KEY_QUERY_VALUE     );

    if( hResult != ERROR_SUCCESS )
    {
        DDT_BEGIN_BLOCK
        DDTW << "RegOpenKeyEx failed " << formatsysmessage( hResult );
        DDT_END_BLOCK
    }
    try
    {
        hResult = regEntry.GetString(   "DebugLog",
                                        (LPSTR)&m_szLogName[0],
                                        &lStringLength  );

        if( hResult != ERROR_SUCCESS )
        {
            DDT_BEGIN_BLOCK
            DDTW << "RegQueryValueEx for DebugLog failed " << formatsysmessage( hResult );
            DDT_END_BLOCK
        }

        hResult = regEntry.GetDWORD(    "DebugOpt",
                                        dwFlags     );
        if( hResult != ERROR_SUCCESS )
        {
            DDT_BEGIN_BLOCK
            DDTW << "RegQueryValueEx for DebugOpt failed " << formatsysmessage( hResult );
            DDT_END_BLOCK
        }
    }
    catch(...)
    {
        DDT_BEGIN_BLOCK
        DDTE << "Exception caught while accessing debug flags in registry." << endl;
        DDT_END_BLOCK
    }

    hResult = regEntry.Close();

    if( hResult != ERROR_SUCCESS )
    {
        DDT_BEGIN_BLOCK
        DDTW << "RegCloseKey failed " << formatsysmessage( hResult );
        DDT_END_BLOCK
    }

    if( (!(dwFlags & tokenFlashMode_Paranoid)) && (dwFlags & tokenDebugLog) )
    {
#ifdef STD_STREAM
        m_logfile.open( (const char *)&m_szLogName[0], ios::out | ios::app );
#else
        m_logfile.open( (const char *)&m_szLogName[0], ios::out | ios::app, filebuf::sh_write | filebuf::sh_read );
#endif
    }


    m_dwFlags = dwFlags & 0x0000FFFF;
    ERDebugStream::sm_iDDTLevel = dwFlags >> 16;
    OutputString( "### Startup\n" );
}

SErrorLog::~SErrorLog( void )
{
    OutputString( "### Shutdown\n" );
    m_logfile.close();
}

void
SErrorLog::OutputString( LPCSTR pString )
{
    if( m_dwFlags & tokenDebugLog )
    {
        if( m_dwFlags & tokenFlashMode_Paranoid )
        {
#ifdef STD_STREAM
            m_logfile.open( (const char *)&m_szLogName[0], ios::out | ios::app );
#else
            m_logfile.open( (const char *)&m_szLogName[0], ios::out | ios::app, filebuf::sh_write | filebuf::sh_read );
#endif
        }
        m_logfile << pString;
        if( m_dwFlags & tokenFlashMode_Paranoid )
        {
            m_logfile.close();
        }
        else if( m_dwFlags & tokenFlushMode )
        {
            m_logfile.flush();
        }
    }

    if( m_dwFlags & tokenDebugApp )
    {
        OutputDebugString( pString );
    }
}


ostream& formatsysmessage_fp( ostream& os, HRESULT hr )
{
    long savedFlags = os.flags();
    char savedFill  = os.fill('0');
    os << '(' << hex << setw(8) << DWORD(hr) << ')';
    LPTSTR pBuffer = NULL;
    FormatMessage(  FORMAT_MESSAGE_ALLOCATE_BUFFER |
                    FORMAT_MESSAGE_FROM_SYSTEM,
                    NULL,
                    (DWORD)hr,
                    LOCALE_SYSTEM_DEFAULT,
                    (LPTSTR)&pBuffer,
                    0,
                    NULL                                );
    if( pBuffer )
    {
        os << ' ' << pBuffer;
        LocalFree( pBuffer );
    }
    os.fill( savedFill );
    os.flags( savedFlags );
    return os;
}

#ifndef STD_STREAM
OMANIP(HRESULT) formatsysmessage( HRESULT hr )
{
    return OMANIP(HRESULT) ( formatsysmessage_fp, hr );
}
#endif


ostream& guidstring_fp( ostream& os, LPCGUID pGUID )
{
    long savedFlags = os.flags();
    char savedFill  = os.fill('0');
    if( pGUID )
    {
        os  << hex
            << '{' << setw(8) << pGUID->Data1
            << '-' << setw(4) << pGUID->Data2
            << '-' << setw(4) << pGUID->Data3
            << '-';
        for( int i = 0; i < 8 ; i++ )
        {
            os << (pGUID->Data4[i] & 0xFF);
        }
        os << '}';
    }
    else
    {
        os << "[NULL]";
    }
    os.fill( savedFill );
    os.flags( savedFlags );
    return os;
}

#ifndef STD_STREAM
OMANIP( LPCGUID ) guidstring( LPCGUID pGUID )
{
    return OMANIP( LPCGUID ) ( guidstring_fp, pGUID );
}
#endif


ostream& hexdump_fp( ostream& os, pointer_and_count_params params )
{
    char *pChar = (char *)(params.pData);

    long savedFlags = os.flags();
    char savedFill  = os.fill();

    if( pChar )
    {
        DWORD dwOffset = 0;

        os << hex << setfill('0');

        while( dwOffset < params.dwBytes )
        {
            char *pBytes = pChar + dwOffset;

            if( dwOffset )
            {
                os << beginline;
            }

            for( DWORD dwIndex = 0 ; dwIndex < 16 ; ++dwIndex )
            {
                if( (dwIndex % 16) == 8 )
                {
                    os << ' ' << '-' << ' ';
                }
                else
                {
                    os << ' ';
                }
                if( dwOffset + dwIndex < params.dwBytes )
                {
                    os << setw( 2 ) << (*((int *)(pBytes+dwIndex)) & 0xff);
                }
                else
                {
                    os << "XX";
                }
            }

            os << ' ' << ' ';

            for( dwIndex = 0 ; dwIndex < 16 ; ++dwIndex )
            {
                if( dwOffset + dwIndex < params.dwBytes )
                {
                    char ch = *(pBytes+dwIndex);
                    if( (ch < 32) || (ch>127) )
                    {
                        ch = '.';
                    }
                    os << ch;
                }
            }

            dwOffset += 16;

            if( dwOffset < params.dwBytes )
            {
                os << endl;
            }
        }
    }
    else
    {
        os << "[NULL]";
    }
    os.fill(savedFill);
    os.flags( savedFlags );
    return os;
}

#ifndef STD_STREAM
OMANIP(pointer_and_count_params) hexdump( void *pData, DWORD dwBytes )
{
    pointer_and_count_params params;

    params.pData = pData;
    params.dwBytes  = dwBytes;
    return OMANIP (pointer_and_count_params)( hexdump_fp, params );
}
#endif

ostream& hexstring_fp( ostream& os, pointer_and_count_params params )
{
    char *pChar = (char *)(params.pData);

    long savedFlags = os.flags();
    char savedFill  = os.fill();

    if( pChar )
    {
        os << hex << setfill('0');

        for ( DWORD c = 0; c < params.dwBytes; c++ ) {
            os << setw( 2 ) << (*((int *)(pChar++)) & 0xff);
        }
    }
    else
    {
        os << "[NULL]";
    }
    os.fill(savedFill);
    os.flags( savedFlags );
    return os;
}

#ifndef STD_STREAM
OMANIP(pointer_and_count_params) hexstring( void *pData, DWORD dwBytes )
{
    pointer_and_count_params params;

    params.pData = pData;
    params.dwBytes  = dwBytes;
    return OMANIP (pointer_and_count_params)( hexstring_fp, params );
}
#endif

ostream& b32dump_fp( ostream& os, pointer_and_count_params params )
{
    static char *pCode =
    "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz@#";
    DWORD dwBuffer;
    char *pChar = (char *)(params.pData);

    if( pChar )
    {
        while( params.dwBytes > 0 )
        {
            dwBuffer = 0;
            for( DWORD i = 0; i < 3; i++ )
            {
                char ch = 0;
                if( params.dwBytes )
                {
                    --params.dwBytes;
                    ch = *pChar;
                    ++pChar;
                }
                *(((char *)(&dwBuffer))+i) = ch;
            }
            for( DWORD j = 0; j < 4; j++ )
            {
                DWORD index = dwBuffer & 0x3f;
                dwBuffer >>= 6;
                os << *(pCode+index);
            }
        }
    }
    else
    {
        os << "[NULL]";
    }

    return os;
}

#ifndef STD_STREAM
OMANIP(pointer_and_count_params) b32dump( void *pData, DWORD dwBytes )
{
    pointer_and_count_params params;

    params.pData = pData;
    params.dwBytes  = dwBytes;
    return OMANIP (pointer_and_count_params)( b32dump_fp, params );
}
#endif

ostream& timestamp( ostream &os )
{
    SYSTEMTIME systemTime;
    char buffer[16];
    long savedFlags = os.flags();
    char savedFill  = os.fill();

    GetSystemTime( &systemTime );
    GetDateFormat(  LOCALE_SYSTEM_DEFAULT,
                    0,
                    &systemTime,
                    "ddd MMM dd yyyy",
                    buffer,
                    sizeof( buffer )        );
    os << buffer;

    GetTimeFormat(  LOCALE_SYSTEM_DEFAULT,
                    0,
                    &systemTime,
                    " HH':'mm':'ss'.'",
                    buffer,
                    sizeof( buffer )        );
    os  << buffer << dec << setfill('0') << setw( 3 )
        << systemTime.wMilliseconds << ' ';

    os.fill(savedFill);
    os.flags( savedFlags );
    return os;
}

ostream& timetoken( ostream &os )
{
    SYSTEMTIME systemTime;
    char buffer[16];
    long savedFlags = os.flags();
    char savedFill  = os.fill();

    GetSystemTime( &systemTime );
    GetDateFormat(  LOCALE_SYSTEM_DEFAULT,
                    0,
                    &systemTime,
                    "yyyy'.'MM'.'dd",
                    buffer,
                    sizeof( buffer )        );
    os << buffer;

    GetTimeFormat(  LOCALE_SYSTEM_DEFAULT,
                    0,
                    &systemTime,
                    "'-'HH':'mm':'ss'.'",
                    buffer,
                    sizeof( buffer )        );
    os  << buffer << dec << setfill('0') << setw( 3 )
        << systemTime.wMilliseconds;

    os.fill(savedFill);
    os.flags( savedFlags );
    return os;
// 96.06.20 JPB: This was a great idea, but...
//    SYSTEMTIME systemTime;
//    GetSystemTime( &systemTime );
//    os << b32dump( &systemTime, sizeof( systemTime ));
//    return os;
}

ostream& timeshort( ostream &os )
{
    SYSTEMTIME systemTime;
    GetSystemTime( &systemTime );
    os << hexdump( &systemTime, sizeof( systemTime ));
    return os;
}

ostream& threadstamp( ostream &os )
{
    DWORD dwThreadId = GetCurrentThreadId();
    os << hexdump( &dwThreadId, sizeof( dwThreadId ) );
    return os;
}

ostream& threadtoken( ostream &os )
{
    DWORD dwThreadId = GetCurrentThreadId();
    long savedFlags = os.flags();
    char savedFill  = os.fill();
    os  << hex << setfill('0') << setw( 8 )
        << dwThreadId;
    os.fill(savedFill);
    os.flags( savedFlags );
    return os;
// 96.06.20 JPB: This was a great idea, but...
//    DWORD dwThreadId = GetCurrentThreadId();
//    os << b32dump( &dwThreadId, sizeof( dwThreadId ) );
//    return os;
}

DWORD ERDebugStreamBuf::GetThreadIndent( void )
{
    DWORD dwResult = 0;
    if( sm_dwIndentTlsIndex==0xFFFFFFFF )
    {
        sm_dwIndentTlsIndex = TlsAlloc();

    }



    if( sm_dwIndentTlsIndex!=0xFFFFFFFF )
    {
        dwResult = (DWORD)TlsGetValue( sm_dwIndentTlsIndex );
        if( !dwResult )
        {
            DWORD dwThreadId = GetCurrentThreadId();
            strstream strTemp;
            dwResult = 1;
            TlsSetValue( sm_dwIndentTlsIndex, (LPVOID)dwResult );
            strTemp << beginline
                << "New Thread Encountered in App '"
                << m_szApplicationName
                << "' in Module '"
                << m_szModuleName
                << "'"
                << endl
                << ends;
            LPSTR pStr = strTemp.str();
            g_SErrorLog.OutputString( pStr );
            // 96.04.08 JCM unfreeze instead of deleting buffer
            strTemp.rdbuf()->freeze( 0 );
            //delete pStr;
        }
        --dwResult;
    }
    return dwResult;
}

void ERDebugStreamBuf::SetThreadIndent( DWORD dwIndent )
{
    if( sm_dwIndentTlsIndex==0xFFFFFFFF )
    {
        sm_dwIndentTlsIndex = TlsAlloc();
    }

    if( sm_dwIndentTlsIndex!=0xFFFFFFFF )
    {
        TlsSetValue( sm_dwIndentTlsIndex, (LPVOID)++dwIndent );
    }
}


ostream &beginline( ostream &os )
{
    char szIndentBuffer[DDT_MAX_INDENT + 1];
    szIndentBuffer[ DDT_MAX_INDENT + 1 ];
    DWORD dwIndent = ERDebugStreamBuf::GetThreadIndent();

    os << timetoken << ';' << threadtoken << ' ';
    if( dwIndent )
    {
        for( DWORD i=0; i < dwIndent; i++ )
        {
            szIndentBuffer[i] = ' ';
        }
        szIndentBuffer[i] = 0;
        os << szIndentBuffer;
    }
    return os;
};

//ERDebugStream&  operator<< (   ERDebugStream& debugstream,
//                                ERDebugStream& (*_f)(ERDebugStream&)    )
//{
//    return (*_f)( debugstream );
//};

ERDebugStreamBuf::ERDebugStreamBuf( void )
{
	OutputDebugString( "ERDebugStreamBuf::ERDebugStreamBuf\n" );
    InitializeCriticalSection( &m_cs );
#ifndef STD_STREAM
    setb( m_OutputBuffer, m_OutputBuffer + sizeof(m_OutputBuffer), FALSE );
#endif
    setp( m_OutputBuffer, m_OutputBuffer + sizeof(m_OutputBuffer) );
}

ERDebugStreamBuf::~ERDebugStreamBuf( void )
{
	OutputDebugString( "ERDebugStreamBuf::~ERDebugStreamBuf\n" );

	// 96.09.05 JCM leave critical section around for now

	// TODO: rework shutdown for debug stuff so that module
	// shutdown order does not cause locking problems.

	//DeleteCriticalSection( &m_cs );
}

void
ERDebugStreamBuf::Lock( void )
{
    EnterCriticalSection( &m_cs );
}

void
ERDebugStreamBuf::Unlock( void )
{
    LeaveCriticalSection( &m_cs );
}

int
ERDebugStreamBuf::underflow( void )
{
    return EOF;
}

int
ERDebugStreamBuf::overflow( int nCh /* = EOF */ )
{
    int nResult = 0;
    if( nCh == EOF )
    {
        nResult = sync();
    }
    else
    {
        if( pptr() > pbase() )
        {
            pbump( -1 );
            char cHold = *pptr();
            *pptr() = 0;
            g_SErrorLog.OutputString( pbase() );
            setp( pbase(), epptr() );
            *pptr() = cHold;
            pbump( 1 );
        }
        *pptr() = nCh;
        pbump( 1 );
    }
    return nResult;
}

int
ERDebugStreamBuf::sync( void )
{
    if( pptr() > pbase() )
    {
        if( pptr() >= epptr() )
        {
            pbump( -1 );
            char cHold = *pptr();
            *pptr() = 0;
            g_SErrorLog.OutputString( pbase() );
            setp( pbase(), epptr() );
            *pptr() = cHold;
            pbump( 1 );
        }
        *pptr() = 0;
        g_SErrorLog.OutputString( pbase() );
        setp( pbase(), epptr() );
    }
    return 0;
}

void
ERDebugStream::InitDebugLevel( HINSTANCE hinst )
{
    DDT_INIT_QUIET( "" )
    char szBuffer[2048];

    GetModuleFileName( hinst, m_szModuleName, sizeof( m_szModuleName ) );

    if( !lstrcmp( m_szModuleName, "[Unknown]" ) )
    {
        szBuffer[0] = 0;
    }
    else
    {
        lstrcpy( szBuffer, m_szModuleName );
    }

    char *pId = &szBuffer[lstrlen(szBuffer)];
    while( pId > &szBuffer[0] && *pId != '\\' )
    {
        --pId;
    }
    if( *pId == '\\' )
    {
        ++pId;
    }

    char szRegPath[2048] = "";

    lstrcpy( szRegPath, g_szSubkey );
    lstrcat( szRegPath, "\\" );
    lstrcat( szRegPath, pId );

    ERDebugStream::InitKeyTable( szRegPath );


/*
    RegEntry regEntry;

    char szSubkey[_MAX_PATH];
    HRESULT hResult = GetRegistryVersionKey( &szSubkey[0], sizeof( szSubkey ) );
    if( hResult != S_OK )
    {
        DDT_BEGIN_BLOCK;
        DDTW << "GetReigstryVersionKey failed " << formatsysmessage( hResult );
        DDT_END_BLOCK;
        szSubkey[0] = '\0';
    }
//    LPCSTR szSubkey = "SOFTWARE\\Digital Sound Corporation\\InfoMail\\1.0";

    ULONG lStringLength = MAX_PATH_STRINGLENGTH;
    DWORD dwFlags = sm_iDDTLevel;
    DWORD dwType = REG_SZ;
    HKEY hkLogKey = 0;

    HRESULT hResult = regEntry.Open(    HKEY_LOCAL_MACHINE,
                                        g_szSubkey,
                                        NULL,
                                        KEY_QUERY_VALUE     );

    if( hResult != ERROR_SUCCESS )
    {
        DDT_BEGIN_BLOCK
        DDTW << "RegOpenKeyEx failed " << formatsysmessage( hResult );
        DDT_END_BLOCK
    }
    try
    {
        hResult = regEntry.GetDWORD(    pId,
                                        dwFlags );
        if( hResult != ERROR_SUCCESS )
        {
            DDT_BEGIN_BLOCK
            DDTW << "RegQueryValueEx for LocalDebugOpt failed " << formatsysmessage( hResult );
            DDT_END_BLOCK
        }
    }
    catch(...)
    {
        DDT_BEGIN_BLOCK
        DDTE << "Exception caught while accessing debug flags in registry." << endl;
        DDT_END_BLOCK
    }

    hResult = regEntry.Close();

    if( hResult != ERROR_SUCCESS )
    {
        DDT_BEGIN_BLOCK
        DDTW << "RegCloseKey failed " << formatsysmessage( hResult );
        DDT_END_BLOCK
    }
    sm_iDDTLevel = dwFlags >> 16;
    g_SErrorLog.m_dwFlags = dwFlags & 0x0000FFFF;
*/
    DDT_BEGIN_BLOCK
    DDT << "### Startup of er_debug'ed module:" << endl;
    DDTC << "        Module Handle: 0x";
    DDTC << setw( 8 );
    DDTC << setfill( '0' );
    DDTC << hinst;
    DDTC << endl;
    DDTC << "          Module Name: " << szBuffer << endl;
    DDT_END_BLOCK
}

#ifdef ERD_DEBUG_MAPI
HRESULT DDT_MAPI_Validate_Return( LPCSTR pFunctionName, HRESULT hResult )
{
    DDT_INIT_QUIET( "" )
    DDT << pFunctionName << " returns: " << formatsysmessage( hResult ) << endl;
    return hResult;
}

ostream& propertytag_fp( ostream& os, ULONG ulTag )
{
    long savedFlags = os.flags();
    char savedFill  = os.fill();
    os << hex << setw(8) << setfill( '0' );
    os << ulTag << ':' << propertytype( WORD(ulTag & 0xFFFF) ) << ':' << SzDecodeUlPropTag( ulTag );
    os.fill(savedFill);
    os.flags( savedFlags );
    return os;
}

#ifndef STD_STREAM
OMANIP(ULONG) propertytag( ULONG ulTag )
{
    return OMANIP(ULONG) ( propertytag_fp, ulTag );
}
#endif

ostream& propertyid_fp( ostream& os, WORD wID )
{
    long savedFlags = os.flags();
    char savedFill  = os.fill();
    os << hex << setw(4) << setfill( '0' ) << wID;
    os.fill(savedFill);
    os.flags( savedFlags );
    return os;
}

#ifndef STD_STREAM
OMANIP(WORD) propertyid( WORD wType )
{
    return OMANIP(WORD) ( propertyid_fp, wType );
}
#endif

ostream& propertytype_fp( ostream& os, WORD wType )
{
    long savedFlags = os.flags();
    char savedFill  = os.fill();
    char *pType = ")=[Unknown] ";

    switch( wType )
    {
        case PT_APPTIME    : pType =  "PT_APPTIME        ";
            break;
        case PT_MV_APPTIME : pType =  "PT_MV_APPTIME     ";
            break;
        case PT_BINARY     : pType =  "PT_BINARY         ";
            break;
        case PT_MV_BINARY  : pType =  "PT_MV_BINARY      ";
            break;
        case PT_BOOLEAN    : pType =  "PT_BOOLEAN        ";
            break;
        case PT_CLSID      : pType =  "PT_CLSID          ";
            break;
        case PT_MV_CLSID   : pType =  "PT_MV_CLSID       ";
            break;
        case PT_CURRENCY   : pType =  "PT_CURRENCY       ";
            break;
        case PT_MV_CURRENCY: pType =  "PT_MV_CURRENCY    ";
            break;
        case PT_DOUBLE     : pType =  "PT_R8/DOUBLE      ";
            break;
        case PT_MV_DOUBLE  : pType =  "PT_MV_R8/DOUBLE   ";
            break;
        case PT_ERROR      : pType =  "PT_ERROR          ";
            break;
        case PT_FLOAT      : pType =  "PT_R4/FLOAT       ";
            break;
        case PT_MV_FLOAT   : pType =  "PT_MV_R4/FLOAT    ";
            break;
        case PT_I2         : pType =  "PT_I2/SHORT       ";
            break;
        case PT_MV_I2      : pType =  "PT_MV_I2/SHORT    ";
            break;
        case PT_I4         : pType =  "PT_I4/LONG        ";
            break;
        case PT_MV_I4      : pType =  "PT_MV_I4/LONG     ";
            break;
        case PT_I8         : pType =  "PT_I8/LONGLONG    ";
            break;
        case PT_MV_I8      : pType =  "PT_MV_I8/LONGLONG ";
            break;
        case PT_NULL       : pType =  "PT_NULL           ";
            break;
        case PT_OBJECT     : pType =  "PT_OBJECT         ";
            break;
        case PT_STRING8    : pType =  "PT_STRING8/TSTRING";
            break;
        case PT_MV_STRING8 : pType =  "PT_MV_STRING8     ";
            break;
        case PT_SYSTIME    : pType =  "PT_SYSTIME        ";
            break;
        case PT_MV_SYSTIME : pType =  "PT_MV_SYSTIME     ";
            break;
        case PT_UNICODE    : pType =  "PT_UNICODE        ";
            break;
        case PT_MV_UNICODE : pType =  "PT_MV_UNICODE     ";
            break;
        case PT_UNSPECIFIED: pType =  "PT_UNSPECIFIED    ";
            break;
        default:
            os << '(' << setw(4) << wType;
    }
    os << pType;

    os.fill(savedFill);
    os.flags( savedFlags );
    return os;
}

#ifndef STD_STREAM
OMANIP(WORD) propertytype( WORD wType )
{
    return OMANIP(WORD) ( propertytype_fp, wType );
}
#endif

#endif
