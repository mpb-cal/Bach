
#ifndef _TABLE_H_
#define _TABLE_H_

#ifndef _WINDOWS_H
#include<windows.h>
#define _WINDOWS_H
#endif


#define MAX_LEN         28
#define MAX_VALUES      64
#define MAX_RGVNAME     1024

struct SRegValue
{
    char    szString[ MAX_LEN ];
    DWORD   dwValue;

    SRegValue( void ) : dwValue( 0 ) { strcpy( szString, "" ); };

    void Init( void )
    {
        dwValue = 0;
        strcpy( szString, "" );
    }

};


class CTable
{
    private:
        char        m_szRegPath[ MAX_PATH ];
        SRegValue   m_prgvTable[ MAX_VALUES ];

        void        InitializeTable( void );

    public:
                    CTable( void ) {};
                    ~CTable( void ) {};

        HRESULT     Initialize( LPCSTR szSubKey );
        HRESULT     Cleanup( void );
        HRESULT     Reload( void );
        HRESULT     GetValue( const char* szKey, DWORD& rdwValue );

};


#endif _TABLE_H_
