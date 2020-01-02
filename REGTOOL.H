#ifndef _REGTOOL_H
#define _REGTOOL_H

#ifndef _WINDOWS_H
#include<windows.h>
#define _WINDOWS_H
#endif

#ifndef _WINREG_H
#include<winreg.h>
#define _WINREG_H
#endif

class RegEntry
{
    public:
        RegEntry( void ):
            m_hKey( NULL ){};

        HRESULT Close( void )
        {
            HRESULT hResult = S_OK;
            if( m_hKey )
            {
                hResult = RegCloseKey( m_hKey );
                m_hKey = NULL;
            }
            return hResult;
        }

        HRESULT Open(   HKEY hKey,
                        LPCSTR pSubKeyPath,
                        DWORD dwOptions,
                        REGSAM samDesired   )
        {
            if( m_hKey )
            {
                Close();
            }
            HRESULT hResult = RegOpenKeyEx( hKey,
                                            pSubKeyPath,
                                            dwOptions,
                                            samDesired,
                                            &m_hKey     );
            return hResult;
        };

        HRESULT GetSubkeyValue( LPSTR   pEntry,
                                LPDWORD pReserved,
                                LPDWORD pType,
                                LPBYTE  pData,
                                LPDWORD pSize       )
        {
            HRESULT hResult = E_FAIL;
            if( m_hKey )
            {
                hResult = RegQueryValueEx(  m_hKey,
                                            pEntry,
                                            pReserved,
                                            pType,
                                            pData,
                                            pSize       );
            }
            return hResult;
        };

        HRESULT SetSubkeyValue( LPSTR   pEntry,
                                DWORD pReserved,
                                DWORD Type,
                                LPBYTE  pData,
                                DWORD   cbData       )
        {
            HRESULT hResult = E_FAIL;
            if( m_hKey )
            {
                hResult = RegSetValueEx( m_hKey,
                                         pEntry,
                                         pReserved,
                                         Type,
                                         pData,
                                         cbData );

            }
            return hResult;
        };

        HRESULT DeleteValue( LPCTSTR pValueName )
        {
            HRESULT hResult = E_FAIL;
            if( m_hKey )
            {
                hResult = RegDeleteValue( m_hKey, pValueName );
            }
            return hResult;
        }

        HRESULT SetString(  LPSTR pEntry,
                            LPSTR pSrc,
                            DWORD dwSize )
        {
            return SetSubkeyValue( pEntry,
                                   0,
                                   REG_SZ,
                                   (unsigned char *)pSrc,
                                   dwSize+1 );
        }



        HRESULT GetString(  LPSTR pEntry,
                            LPSTR pDest,
                            LPDWORD pdwSize )
        {
            HRESULT hResult = E_FAIL;
            if( m_hKey )
            {
                DWORD dwReserved = 0;
                DWORD dwType = REG_SZ;
				// 96.09.13 JCM fix reserved value. Used to be address of a var, now zero.
                hResult = GetSubkeyValue(   pEntry,
                                            NULL, // 96.09.13 &dwReserved,
                                            &dwType,
                                            (unsigned char*)pDest,
                                            pdwSize       );
            }
            return hResult;
        }

        HRESULT GetStringExpand(    LPSTR pEntry,
                                    LPSTR pDest,
                                    LPDWORD pdwSize )
        {
            HRESULT hResult = E_FAIL;
            if( m_hKey )
            {
                DWORD dwReserved = 0;
                DWORD dwType = REG_EXPAND_SZ;
                hResult = GetSubkeyValue(   pEntry,
                                            NULL, // 96.09.13 &dwReserved,
                                            &dwType,
                                            (unsigned char*)pDest,
                                            pdwSize       );
            }
            return hResult;
        }

        HRESULT GetDWORD(   LPSTR pEntry,
                            DWORD &rdwData   )
        {
            HRESULT hResult = E_FAIL;
            if( m_hKey )
            {
                DWORD dwReserved = 0;
                DWORD dwType = REG_DWORD;
                DWORD dwSize = sizeof( DWORD );
                hResult = GetSubkeyValue(   pEntry,
                                            NULL, // 96.09.13 &dwReserved,
                                            &dwType,
                                            (unsigned char*)&rdwData,
                                            &dwSize       );
            }
            return hResult;
        };

		HRESULT SetDWORD( LPSTR pEntry,
				          DWORD dwData )
		{
            HRESULT hResult = E_FAIL;
            if( m_hKey )
            {
                DWORD dwReserved = 0;
                DWORD dwType = REG_DWORD;
                DWORD dwSize = sizeof( DWORD );
				hResult = RegSetValueEx( m_hKey,
										 pEntry,
										 0,
										 REG_DWORD,
										 (const unsigned char *)&dwData,
										 sizeof( DWORD ) );
			}
            return hResult;
		};

        HRESULT EnumValue( DWORD    dwIndex,
                           LPSTR    lpValueName,
                           LPDWORD  lpcbValueName,
                           LPDWORD  lpType,
                           LPBYTE   lpData,
                           LPDWORD  lpcbData       )
        {
            HRESULT hResult = E_FAIL;
            if( m_hKey )
            {
                hResult = RegEnumValue(  m_hKey,
                                         dwIndex,
                                         lpValueName,
                                         lpcbValueName,
                                         NULL,
                                         lpType,
                                         lpData,
                                         lpcbData       );
            }
            return hResult;
        };


        ~RegEntry( void )
        {
            Close();
        };

    protected:
        HKEY m_hKey;
};



#endif