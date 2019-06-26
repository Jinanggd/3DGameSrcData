#include "extra/bass.h"

#ifndef SOUND_H
#define SOUND_H



enum class sound_types {

	background,
	death

};

class Sound
{


public:


	Sound();
	void playSound(sound_types);
	HSAMPLE loadSample(char * filename);
	void playSound(const char *filename);
	~Sound();


	HSAMPLE *samples;

};


#endif
