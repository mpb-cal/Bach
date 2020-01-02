
BOOL CCursor::ComposeGuitarChord( HINSTANCE hInstance, HWND hwndParent )
{
	CDialogGuitarChord dgc;

	dgc.m_iTicks = m_iPreviousGuitarChordTicks;
	dgc.m_iChord = m_iPreviousGuitarChord;
	dgc.m_blBassNote = m_blPreviousGuitarChordBassNote;

	if (dgc.DoModal( hInstance, hwndParent ) == IDOK)
	{
		char szMacro[ 256 ] = "";
		char szChordMacro[ 256 ] = "";
		sprintf( szMacro, "v%d ", dgc.m_iTicks );

		int iChord = dgc.m_iChord % 14;
		int iRoot = dgc.m_iChord / 14;
		int iTranspose;

		switch (iRoot)
		{
			case 0 :		// roots on A string
			case 1 :
			case 2 :
			case 3 :
			case 4 :
			case 10 :
			case 11 :
			{
				switch (iChord)
				{
					case 0 :	// M
						MoveToPitch( 48 );
						lstrcat( szMacro, "t U1 n U1 u4 n U1 u3 n U1 u2 n U1 u2 n D5" );
						break;

					case 1 :	// M7
						MoveToPitch( 48 );
						lstrcat( szMacro, "t U1 n U1 u4 n U1 u2 n U1 u3 n U1 u2 n D5" );
						break;

					case 2 :	// M9
						MoveToPitch( 48 );
						lstrcat( szMacro, "t U1 n U1 u4 n U1 u2 n U1 u2 n U1 u3 n D5" );
						break;

					case 3 :	// m
						MoveToPitch( 48 );
						lstrcat( szMacro, "t U1 n U1 u4 n U1 u3 n U1 u1 3 U1 u3 n D5" );
						break;

					case 4 :	// m7
						MoveToPitch( 48 );
						lstrcat( szMacro, "t U1 n U1 u4 n U1 u1 3 U1 u3 3 U1 u3 n D5" );
						break;

					case 5 :	// m9
						MoveToPitch( 48 );
						lstrcat( szMacro, "t U1 n U1 u1 3 U1 u4 3 U1 u3 n U1 u3 n D5" );
						break;

					case 6 :	// 7
						MoveToPitch( 48 );
						lstrcat( szMacro, "t U1 n U1 u4 n U1 u1 3 U1 u4 n U1 u2 n D5" );
						break;

					case 7 :	// 9
						MoveToPitch( 48 );
						lstrcat( szMacro, "t U1 n U1 u2 n U1 u3 3 U1 u3 n U1 u3 n D5" );
						break;

					case 8 :	// 11
						MoveToPitch( 48 );
						lstrcat( szMacro, "t U1 n U1 u2 n U1 u3 3 U1 u2 n U1 u3 n D5" );
						break;

					case 9 :	// m7b5
						MoveToPitch( 48 );
						lstrcat( szMacro, "t U1 n U1 u3 3 U1 u2 3 U1 u3 3 U1 t D5" );
						break;

					case 10 :	// dim7
						MoveToPitch( 48 );
						lstrcat( szMacro, "t U1 u1 3 U1 u4 n U1 u2 n U1 u3 3 U1 t D5" );
						break;

					case 11 :	// 6
						MoveToPitch( 48 );
						lstrcat( szMacro, "t U1 n U1 u4 n U1 u3 n U1 u2 n U1 u3 n D5" );
						break;

					case 12 :	// +
						MoveToPitch( 48 );
						lstrcat( szMacro, "t U1 u7 n U1 u2 n U1 u2 3 U1 u3 n U1 t D5" );
						break;

					case 13 :	// +7
						MoveToPitch( 48 );
						lstrcat( szMacro, "t U1 u5 3 U1 u4 n U1 u2 3 U1 u3 n U1 t D5" );
						break;

				}

				break;
			}

			case 5 :		// roots on E string
			case 6 :
			case 7 :
			case 8 :
			case 9 :
			{
				switch (iChord)
				{
					case 0 :	// M
						MoveToPitch( 41 );
						lstrcat( szMacro, "n U1 u4 n U1 u3 n U1 u2 n U1 u2 n U1 u3 n D5" );
						break;

					case 1 :	// M7
						MoveToPitch( 41 );
						lstrcat( szMacro, "t U1 t U1 u7 n U1 u2 n U1 u2 n U1 u2 n D5" );
						break;

					case 2 :	// M9
						MoveToPitch( 41 );
						lstrcat( szMacro, "t U1 t U1 u6 n U1 u3 n U1 u2 n U1 u4 n D5" );
						break;

					case 3 :	// m
						MoveToPitch( 41 );
						lstrcat( szMacro, "n U1 u4 n U1 u3 n U1 u1 3 U1 u3 n U1 u3 n D5" );
						break;

					case 4 :	// m7
						MoveToPitch( 41 );
						lstrcat( szMacro, "n U1 u4 n U1 u1 3 U1 u3 3 U1 u3 n U1 u3 n D5" );
						break;

					case 5 :	// m9
						MoveToPitch( 41 );
						lstrcat( szMacro, "n U1 u4 n U1 u1 3 U1 u3 3 U1 u3 n U1 u4 n D5" );
						break;

					case 6 :	// 7
						MoveToPitch( 41 );
						lstrcat( szMacro, "n U1 u4 n U1 u1 3 U1 u4 n U1 u2 n U1 u3 n D5" );
						break;

					case 7 :	// 9
						MoveToPitch( 41 );
						lstrcat( szMacro, "n U1 u4 n U1 u1 3 U1 u4 n U1 u2 n U1 u4 n D5" );
						break;

					case 8 :	// 11
						MoveToPitch( 41 );
						lstrcat( szMacro, "n U1 u2 3 U1 u3 3 U1 u4 3 U1 u2 n U1 u4 n D5" );
						break;

					case 9 :	// m7b5
						MoveToPitch( 41 );
						lstrcat( szMacro, "t U1 t U1 u7 n U1 u3 n U1 u2 3 U1 u3 3 D5" );
						break;

					case 10 :	// dim7
						MoveToPitch( 41 );
						lstrcat( szMacro, "t U1 t U1 u7 n U1 u3 n U1 u2 n U1 u3 3 D5" );
						break;

					case 11 :	// 6
						MoveToPitch( 41 );
						lstrcat( szMacro, "n U1 u4 n U1 t U1 u5 n U1 u3 n U1 u2 n D5" );
						break;

					case 12 :	// +
						MoveToPitch( 41 );
						lstrcat( szMacro, "t U1 t U1 u7 n U1 u2 n U1 u2 3 U1 u3 n D5" );
						break;

					case 13 :	// +7
						MoveToPitch( 41 );
						lstrcat( szMacro, "t U1 t U1 u5 3 U1 u4 n U1 u2 3 U1 u3 n D5" );
						break;

				}

				break;
			}
		}

		BOOL blOldRetainPitch = m_blRetainPitch;
		m_blRetainPitch = TRUE;
		BOOL blOldAutoAdvance = m_blAutoAdvance;
		m_blAutoAdvance = FALSE;
		Macro( szMacro, 1 );
		m_blRetainPitch = blOldRetainPitch;
		m_blAutoAdvance = blOldAutoAdvance;
		

		switch (iRoot)
		{
			case 0 : iTranspose = 0; break;
			case 1 : iTranspose = 1; break;
			case 2 : iTranspose = 2; break;
			case 3 : iTranspose = 3; break;
			case 4 : iTranspose = 4; break;
			
			case 5 : iTranspose = 0; break;
			case 6 : iTranspose = 1; break;
			case 7 : iTranspose = 2; break;
			case 8 : iTranspose = 3; break;
			case 9 : iTranspose = 4; break;

			case 10 : iTranspose = -2; break;
			case 11 : iTranspose = -1; break;
		}

		char szTransposeMacro[ 256 ] = "";
		sprintf( szTransposeMacro, "U5 bs r%d D5", dgc.m_iTicks );
		Macro( szTransposeMacro, 1 );
		m_Selection.TransposeNotes( iTranspose, NOTE_UP );
		sprintf( szTransposeMacro, "es l%d", dgc.m_iTicks );
		Macro( szTransposeMacro, 1 );

		// enter bass note
		if (dgc.m_blBassNote)
		{		
			switch (iRoot)
			{
				case 0 :
					MoveToPitch( 36 );
					sprintf( szMacro, "v%d D1 n U1", dgc.m_iTicks );
					break;

				case 1 :
					MoveToPitch( 36 );
					sprintf( szMacro, "v%d D1 3 U1", dgc.m_iTicks );
					break;

				case 2 :
					MoveToPitch( 38 );
					sprintf( szMacro, "v%d D1 n U1", dgc.m_iTicks );
					break;

				case 3 :
					MoveToPitch( 38 );
					sprintf( szMacro, "v%d D1 3 U1", dgc.m_iTicks );
					break;

				case 4 :
					MoveToPitch( 40 );
					sprintf( szMacro, "v%d D1 n U1", dgc.m_iTicks );
					break;

				case 5 :
					MoveToPitch( 41 );
					sprintf( szMacro, "v%d D1 n U1", dgc.m_iTicks );
					break;

				case 6 :
					MoveToPitch( 41 );
					sprintf( szMacro, "v%d D1 3 U1", dgc.m_iTicks );
					break;

				case 7 :
					MoveToPitch( 43 );
					sprintf( szMacro, "v%d D1 n U1", dgc.m_iTicks );
					break;

				case 8 :
					MoveToPitch( 43 );
					sprintf( szMacro, "v%d D1 3 U1", dgc.m_iTicks );
					break;

				case 9 :
					MoveToPitch( 45 );
					sprintf( szMacro, "v%d D1 n U1", dgc.m_iTicks );
					break;

				case 10 :
					MoveToPitch( 45 );
					sprintf( szMacro, "v%d D1 3 U1", dgc.m_iTicks );
					break;

				case 11 :
					MoveToPitch( 47 );
					sprintf( szMacro, "v%d D1 n U1", dgc.m_iTicks );
					break;

			}

			blOldRetainPitch = m_blRetainPitch;
			m_blRetainPitch = TRUE;
			blOldAutoAdvance = m_blAutoAdvance;
			m_blAutoAdvance = FALSE;
			Macro( szMacro, 1 );
			m_blRetainPitch = blOldRetainPitch;
			m_blAutoAdvance = blOldAutoAdvance;
		}

		if (m_blAutoAdvance)
		{
			Move( MOVE_RIGHT, FALSE, NULL );
		}
		
		m_iPreviousGuitarChordTicks = dgc.m_iTicks;
		m_iPreviousGuitarChord = dgc.m_iChord;
		m_blPreviousGuitarChordBassNote = dgc.m_blBassNote;

		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
