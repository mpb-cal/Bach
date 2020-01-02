
#ifndef __GLOBALDEFS_H__
#include "GlobalDefs.h"
#endif


void NormalizeRectangle( RECT* pRect )
{
	if (pRect->left > pRect->right)
	{
		long temp = pRect->left;
		pRect->left = pRect->right;
		pRect->right = temp;
	}
	if (pRect->top > pRect->bottom)
	{
		long temp = pRect->top;
		pRect->top = pRect->bottom;
		pRect->bottom = temp;
	}
}


char g_szOpenFileFilter[]	=	"Bach Files (*.bch)\0*.bch\0";


char* g_pszPatches[] = 
{
	"Acoustic Grand Piano",
	"Bright Acoustic Piano",
	"Electric Grand Piano",
	"Honky-tonk Piano",
	"Rhodes Piano",
	"Chorused Piano",
	"Harpsichord",
	"Clavinet",
	"Celesta",
	"Glockenspiel",
	"Music Box",
	"Vibraphone",
	"Marimba",
	"Xylophone",
	"Tubular Bells",
	"Dulcimer",
	"Hammond Organ",
	"Percussive Organ",
	"Rock Organ",
	"Church Organ",
	"Reed Organ",
	"Accordion",
	"Harmonica",
	"Tango Accordion",
	"Acoustic Guitar (nylon)",
	"Acoustic Guitar (steel)",
	"Electric Guitar (jazz)",
	"Electric Guitar (clean)",
	"Electric Guitar (muted)",
	"Overdriven Guitar",
	"Distortion Guitar",
	"Guitar Harmonics",
	"Acoustic Bass",
	"Electric Bass (finger)",
	"Electric Bass (pick)",
	"Fretless Bass",
	"Slap Bass 1",
	"Slap Bass 2",
	"Synth Bass 1",
	"Synth Bass 2",
	"Violin",
	"Viola",
	"Cello",
	"Contrabass",
	"Tremolo Strings",
	"Pizzicato Strings",
	"Orchestral Harp",
	"Timpani",
	"String Ensemble 1",
	"String Ensemble 2",
	"SynthStrings 1",
	"SynthStrings 2",
	"Choir Aahs",
	"Voice Oohs",
	"Synth Voice",
	"Orchestra Hit",
	"Trumpet",
	"Trombone",
	"Tuba",
	"Muted Trumpet",
	"French Horn",
	"Brass Section",
	"Synth Brass 1",
	"Synth Brass 2",
	"Soprano Sax",
	"Alto Sax",
	"Tenor Sax",
	"Baritone Sax",
	"Oboe",
	"English Horn",
	"Bassoon",
	"Clarinet",
	"Piccolo",
	"Flute",
	"Recorder",
	"Pan Flute",
	"Bottle Blow",
	"Shakuhachi",
	"Whistle",
	"Ocarina",
	"Lead 1 (square)",
	"Lead 2 (sawtooth)",
	"Lead 3 (caliope lead)",
	"Lead 4 (chiff lead)",
	"Lead 5 (charang)",
	"Lead 6 (voice)",
	"Lead 7 (fifths)",
	"Lead 8 (brass + lead)",
	"Pad 1 (new age)",
	"Pad 2 (warm)",
	"Pad 3 (polysynth)",
	"Pad 4 (choir)",
	"Pad 5 (bowed)",
	"Pad 6 (metallic)",
	"Pad 7 (halo)",
	"Pad 8 (sweep)",
	"FX 1 (rain)",
	"FX 2 (soundtrack)",
	"FX 3 (crystal)",
	"FX 4 (atmosphere)",
	"FX 5 (brightness)",
	"FX 6 (goblins)",
	"FX 7 (echoes)",
	"FX 8 (sci-fi)",
	"Sitar",
	"Banjo",
	"Shamisen",
	"Koto",
	"Kalimba",
	"Bagpipe",
	"Fiddle",
	"Shanai",
	"Tinkle Bell",
	"Agogo",
	"Steel Drums",
	"Woodblock",
	"Taiko Drum",
	"Melodic Tom",
	"Synth Drum",
	"Reverse Cymbal",
	"Guitar Fret Noise",
	"Breath Noise",
	"Seashore",
	"Bird Tweet",
	"Telephone Ring",
	"Helicopter",
	"Applause",
	"Gunshot",
};

char* g_pszPercussionPitches[] = 
{
	"High Q",
	"Slap",
	"Scratch Push",
	"Scratch Pull",
	"Sticks",
	"Square Click",
	"Metronome Click",
	"Metronome Bell",
	"Kick Drum 2",
	"Kick Drum 1",
	"Side Stick",
	"Snare Drum 1",
	"Hand Clap",
	"Snare Drum 2",
	"Low Tom 2",
	"Closed Hi-Hat",
	"Low Tom 1",
	"Pedal Hi-Hat",
	"Mid Tom 2",
	"Open Hi-Hat",
	"Mid Tom 1",
	"High Tom 2",
	"Crash Cymbal 1",
	"High Tom 1",
	"Ride Cymbal 1",
	"Chinese Cymbal",
	"Ride Bell",
	"Tambourine",
	"Splash Cymbal",
	"Cowbell",
	"Crash Cymbal 2",
	"Vibra-slap",
	"Ride Cymbal 2",
	"High Bongo",
	"Low Bongo",
	"Mute High Conga",
	"Open High Conga",
	"Low Conga",
	"High Timbale",
	"Low Timbale",
	"High Agogo",
	"Low Agogo",
	"Cabasa",
	"Maracas",
	"Short Hi Whistle",
	"Long Low Whistle",
	"Short Guiro",
	"Long Guiro",
	"Claves",
	"High Wood Block",
	"Low Wood Block",
	"Muted Cuica",
	"Open Cuica",
	"Mute Triangle",
	"Open Triangle",
	"Shaker",
	"Jingle Bell",
	"Belltree",
	"Castanets",
	"Mute Surdo",
	"Open Surdo",
};


