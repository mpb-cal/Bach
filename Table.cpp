
#include "stdafx.h"

#ifndef _TABLE_H
#include "Table.h"
#endif

#ifndef _REGTOOL_H
#include "regtool.h"
#endif


HRESULT CTable::Initialize( LPCSTR szSubKey )
{
    lstrcpy( m_szRegPath, szSubKey );

    return Reload();
}

void CTable::InitializeTable( void )
{
    for (INT i = 0; i < MAX_VALUES; i++)
    {
        m_prgvTable[ i ].Init();
    }
}

HRESULT CTable::Reload( void )
{
    HRESULT hResult = E_FAIL;

    // clear out the table
    InitializeTable();

    // create a RegEntry object to handle registry functions
    RegEntry regEntry;

    // open the required registry key for reading
    hResult = regEntry.Open(    (HKEY)HKEY_LOCAL_MACHINE,
                                m_szRegPath,
                                NULL,
                                KEY_READ                    );

    if (hResult == ERROR_SUCCESS)
    {
        // read values from the registry key and store them in the table
        char szValueName[ MAX_RGVNAME ] = "";
        DWORD cbValueName;
        DWORD dwType;
        DWORD dwData;
        DWORD cbData;

        DWORD dwRegIndex = 0;
        DWORD dwTableIndex = 0;

        // iterate through each value in the key
        BOOL bMoreItems = TRUE;
        while (bMoreItems)
        {
            // reset count variables
            cbData = sizeof DWORD;
            cbValueName = MAX_RGVNAME;

            hResult = regEntry.EnumValue(   dwRegIndex,
                                            szValueName,
                                            &cbValueName,
                                            &dwType,
                                            (LPBYTE)&dwData,
                                            &cbData             );

            if (hResult == ERROR_SUCCESS)
            {
                if (dwType == REG_DWORD)
                {
                    // truncate string to MAX_LEN chars
                    if (cbValueName > MAX_LEN)
                    {
                        szValueName[ MAX_LEN - 1 ] = '\0';
                    }
                    // place value into table
                    lstrcpy( m_prgvTable[ dwTableIndex ].szString, szValueName );
                    m_prgvTable[ dwTableIndex ].dwValue = dwData;

                    dwTableIndex++;
                }
            }
            else if (hResult == ERROR_NO_MORE_ITEMS)
            {
                hResult = ERROR_SUCCESS;
                bMoreItems = FALSE;
            }

            if (dwTableIndex >= MAX_VALUES)
            {
                // table has run out of space
                break;
            }

            ++dwRegIndex;
        }
    }

    return hResult;
}

HRESULT CTable::GetValue( const char* szKey, DWORD& rdwValue )
{
    HRESULT hResult = E_FAIL;

    //truncate szKey to MAX_LEN length
    char* szKeyCopy = new char[ lstrlen( szKey ) ];
    lstrcpy( szKeyCopy, szKey );
	if (lstrlen( szKeyCopy ) > MAX_LEN - 1)
	{
		szKeyCopy[ MAX_LEN - 1 ] = '\0';
	}

    DWORD dwIndex = 0;
    BOOL bStringNotFound = TRUE;

    while (bStringNotFound)
    {
        if (lstrcmp( szKeyCopy, m_prgvTable[ dwIndex ].szString ) == 0)
        {
            rdwValue = m_prgvTable[ dwIndex ].dwValue;
            bStringNotFound = FALSE;
            hResult = ERROR_SUCCESS;
        }

        if (++dwIndex >= MAX_VALUES)
        {
            // reached the end of the table
            break;
        }
    }

    return hResult;
}



