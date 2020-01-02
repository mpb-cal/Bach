// constant.h

#ifndef __CONSTANT_H__
#define __CONSTANT_H__

#include <CRTDBG.H>

#define ASSERT	_ASSERTE

// note
const int NOTE_THIRTYSECOND =		1;
const int NOTE_SIXTEENTH =			2;
const int NOTE_SIXTEENTHD =			3;
const int NOTE_EIGHTH =				4;
const int NOTE_EIGHTHD =			6;
const int NOTE_QUARTER =			8;
const int NOTE_QUARTERD =			12;
const int NOTE_HALF =				16;
const int NOTE_HALFD =				24;
const int NOTE_WHOLE =				32;
const int NOTE_VALUE =				33;

const int NOTE_WIDTH =				12;
const int NOTE_HEIGHT =				7;
const int NOTE_SPACEABOVE =			25;
const int NOTE_ACCWIDTH =			10;
const int NOTE_ACCHEIGHT =			16;
const int NOTE_TICKSPERQUARTER =	8;

const int NOTE_TOPPITCH =			128;
const int NOTE_BOTTOMPITCH =		0;
const int NOTE_TOPOCTAVE =			10;
const int NOTE_BOTTOMOCTAVE =		0;

const int NOTE_REST =				NOTE_TOPPITCH;

// measure
const int MEASURE_FIRSTX =			200;

// song
const int SONG_NUMVOICES =			14;
const int SONG_DPQ =				120;
const int SONG_TEMPO =				120;
const int SONG_TIMESIG_N =			4;
const int SONG_TIMESIG_D =			4;

// staff
const int STAFF_SPACEWIDTH =		8;
const int STAFF_TOP =				NOTE_ACCHEIGHT;
const int STAFF_END =				1000;
const int STAFF_LINES =				5;
const int STAFF_HEIGHT =			(STAFF_LINES - 1) * STAFF_SPACEWIDTH; // = 32

// voice
const int VOICE_VELOCITY =			0x70;
const int VOICE_HEIGHT =			STAFF_TOP * 2 + STAFF_HEIGHT;
const int VOICE_TOTALPATCHES =		128;
const int VELOCITY_MIN	=			0;
const int VELOCITY_MAX	=			127;

// percussion
const int PERCUSSION_TOTALPITCHES =	61;
const int PERCUSSION_FIRSTPITCH =	27;

// cursor
const int CURSOR_HEIGHT =			STAFF_HEIGHT + 6;
const int CURSOR_WIDTH =			1;
const int CURSOR_SHORTJUMP =		1;
const int CURSOR_TBARWIDTH =		10;
const int CURSOR_SPACEABOVE =		20;
const int CURSOR_XOFFSET =			0;
const int CURSOR_YOFFSET =			1;
const int CURSOR_STARTXPOS =		MEASURE_FIRSTX + NOTE_WIDTH + CURSOR_XOFFSET;
const int CURSOR_STARTYPOS =		(STAFF_TOP + STAFF_SPACEWIDTH + CURSOR_YOFFSET) - CURSOR_SPACEABOVE;
const LPARAM CURSOR_UPDATE =		0x01;

// bitmapstorage
const int BMS_WIDTH =			20;
const int BMS_HEIGHT =			64;
const int BMS_NUMBER =			34;
const int BMS_STOREWIDTH =		BMS_NUMBER * BMS_HEIGHT;
const int BMS_STOREHEIGHT =		BMS_HEIGHT;

// spread bitmaps BMS_WIDTH pixels apart:
// remember to update BMS_NUMBER and CBMStorage::Initialize()!
const int BM_TREBLECLEF		= 0;
const int BM_BASSCLEF		= BM_TREBLECLEF		+ BMS_WIDTH;

const int BM_THIRTYSECOND	= BM_BASSCLEF		+ BMS_WIDTH;
const int BM_SIXTEENTH		= BM_THIRTYSECOND	+ BMS_WIDTH;
const int BM_SIXTEENTHD		= BM_SIXTEENTH		+ BMS_WIDTH;
const int BM_EIGHTH			= BM_SIXTEENTHD		+ BMS_WIDTH;
const int BM_EIGHTHD		= BM_EIGHTH			+ BMS_WIDTH;
const int BM_QUARTER		= BM_EIGHTHD		+ BMS_WIDTH;
const int BM_QUARTERD		= BM_QUARTER		+ BMS_WIDTH;
const int BM_HALF			= BM_QUARTERD		+ BMS_WIDTH;
const int BM_HALFD			= BM_HALF			+ BMS_WIDTH;
const int BM_WHOLE			= BM_HALFD			+ BMS_WIDTH;

const int BM_THIRTYSECONDU	= BM_WHOLE			+ BMS_WIDTH;
const int BM_SIXTEENTHU		= BM_THIRTYSECONDU	+ BMS_WIDTH;
const int BM_SIXTEENTHDU	= BM_SIXTEENTHU		+ BMS_WIDTH;
const int BM_EIGHTHU		= BM_SIXTEENTHDU	+ BMS_WIDTH;
const int BM_EIGHTHDU		= BM_EIGHTHU		+ BMS_WIDTH;
const int BM_QUARTERU		= BM_EIGHTHDU		+ BMS_WIDTH;
const int BM_QUARTERDU		= BM_QUARTERU		+ BMS_WIDTH;
const int BM_HALFU			= BM_QUARTERDU		+ BMS_WIDTH;
const int BM_HALFDU			= BM_HALFU			+ BMS_WIDTH;
const int BM_WHOLEU			= BM_HALFDU			+ BMS_WIDTH;

const int BM_THIRTYSECONDR	= BM_WHOLEU			+ BMS_WIDTH;
const int BM_SIXTEENTHR		= BM_THIRTYSECONDR	+ BMS_WIDTH;
const int BM_SIXTEENTHDR	= BM_SIXTEENTHR		+ BMS_WIDTH;
const int BM_EIGHTHR		= BM_SIXTEENTHDR	+ BMS_WIDTH;
const int BM_EIGHTHDR		= BM_EIGHTHR		+ BMS_WIDTH;
const int BM_QUARTERR		= BM_EIGHTHDR		+ BMS_WIDTH;
const int BM_QUARTERDR		= BM_QUARTERR		+ BMS_WIDTH;
const int BM_HALFR			= BM_QUARTERDR		+ BMS_WIDTH;
const int BM_HALFDR			= BM_HALFR			+ BMS_WIDTH;
const int BM_WHOLER			= BM_HALFDR			+ BMS_WIDTH;

const int BM_SHARP			= BM_WHOLER			+ BMS_WIDTH;
const int BM_NOACC			= BM_SHARP			+ BMS_WIDTH;

// key signature
enum EKeySignature
{
	CMAJOR,
	GMAJOR,
	DMAJOR,
	AMAJOR,
	EMAJOR,
	BMAJOR,
	LastKeySignature,
	CSMAJOR,
//	DMAJOR,
	DSMAJOR,
//	EMAJOR,
	FMAJOR,
	FSMAJOR,
//	GMAJOR,
	GSMAJOR,
//	AMAJOR,
	ASMAJOR,
//	BMAJOR,

	CMINOR,
	CSMINOR,
	DMINOR,
	DSMINOR,
	EMINOR,
	FMINOR,
	FSMINOR,
	GMINOR,
	GSMINOR,
	AMINOR,
	ASMINOR,
	BMINOR
};

// mode
enum EMode
{
	MODE_DIATONIC,
	MODE_MAJOR,
	MODE_MINOR,

	LastMode
};

// clefs
enum EClef
{
	CLEF_TREBLE,
	CLEF_BASS,

	LastClef
};

// channels
const int CHANNEL_MIN	=	1;
const int CHANNEL_MAX	=	16;
const int CHANNEL_DRUM	=	10;

// patchess
const int PATCH_MIN	=	0;
const int PATCH_MAX	=	127;

// midinote note values
const int NOTE2MN_FACTOR		= 15;	// e.g., NOTE_WHOLE(=32) * 15 = MN_WHOLE(=480)

const int MN_WHOLE				= 480;
const int MN_HALF				= MN_WHOLE / 2;							// = 240
const int MN_DOTTEDHALF			= MN_HALF + MN_HALF / 2;				// = 360
const int MN_QUARTER			= MN_WHOLE / 4;							// = 120
const int MN_DOTTEDQUARTER		= MN_QUARTER + MN_QUARTER / 2;			// = 180
const int MN_EIGHTH				= MN_WHOLE / 8;							// = 60
const int MN_DOTTEDEIGHTH		= MN_EIGHTH + MN_EIGHTH / 2;			// = 90
const int MN_SIXTEENTH			= MN_WHOLE / 16;						// = 30
const int MN_DOTTEDSIXTEENTH	= MN_SIXTEENTH + MN_SIXTEENTH / 2;		// = 45
const int MN_THIRTYSECOND		= MN_WHOLE / 32;						// = 15
const int MN_STARTVELOCITY		= 64;
const int MN_STOPVELOCITY		= 64;
const int MN_REST				= 0;

// midinote midi events
const BYTE MN_NOTEOFF	=			0x80;
const BYTE MN_NOTEON	=			0x90;
const BYTE MN_KEYAFTERTOUCH	=		0xA0;
const BYTE MN_CONTROLCHANGE	=		0xB0;
const BYTE MN_PATCHCHANGE =			0xC0;
const BYTE MN_CHANNELAFTERTOUCH	=	0xD0;
const BYTE MN_PITCHWHEELCHANGE	=	0xE0;
const BYTE MN_METAEVENT	=			0xFF;

// miditrack
const int MT_HEADERSIZE =			8;
const int MT_FOOTERSIZE =			4;

// midifile
const int MF_HEADERSIZE = 14;


// update messages
const int UPDATE_NONE =			0x00;	// no update
const int UPDATE_MEASURE =		0x01;	// update current measure in current voice
const int UPDATE_MEASURES =		0x02;	// update current measure in all voices
const int UPDATE_DOC =			0x04;	// set modified flag
const int UPDATE_VIEW =			0x08;	// update view
const int UPDATE_VOICE =		0x10;	// update current voice
const int UPDATE_SONG =			0x20;	// update entire song
const int UPDATE_SCROLL =		0x40;	// adjust scroll position

// drum part
const int DRUMVOICE_KICK =			19;
const int DRUMVOICE_SNARE =			18;
const int DRUMVOICE_HIHAT =			17;
const int DRUMVOICE_TOM1 =			16;
const int DRUMVOICE_TOM2 =			15;

const int DRUM_KICK1 =				36;
const int DRUM_SIDESTICK =			36; //sharpen
const int DRUM_SNARE1 =				38;
const int DRUM_CLOSEDHIHAT =		41;	//sharpen
const int DRUM_OPENHIHAT =			45;	//sharpen

const int DRUM_SQUARECLICK =		33;	//sharpen
const int DRUM_METRONOMECLICK  =	34;

// guitar chords
const int CHORD_TYPES =				19;

enum EDrumStyle
{
	DRUMSTYLE_ROCK1,
	DRUMSTYLE_ROCK2,
	DRUMSTYLE_TECHNO1,
	DRUMSTYLE_SKA1,
	DRUMSTYLE_HIPHOP1,
	DRUMSTYLE_SWING1,

	LastDrumStyle
};


#define NOT_USED( x )			x
#define ARE_EQUAL( x, y )		lstrcmp( x, y ) == 0

extern const char* g_szClipboardFormat;
extern unsigned int g_uiClipboardFormat;



#endif __CONSTANT_H__
