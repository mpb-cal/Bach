/* cmidinote.h	
*/

#ifndef __CMIDINOTE_H__
#define __CMIDINOTE_H__

#include <fstream.h>



int get_ticks(int note_value);

class CMidiNote
{
public:
			CMidiNote()	{ }
			CMidiNote( int iPitch, int iLength, int iChannel, int iVelocity );
			~CMidiNote() { }
	void	Write( ostream& fout, int& riDelay );
	
protected:
	int			m_iPitch;
	int			m_iLength;
	int			m_iChannel;
	int			m_iVelocity;

};


#endif __MIDINOTE_H__


