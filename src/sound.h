#include "extra/bass.h"

#ifndef SOUND_H
#define SOUND_H



enum class sound_types {

	background,
	death,
	ambience,
	cannon,
	footstep,
	pick,
	titan,
	win,
	init,
	explosion,
	upgrade


};





class Sound
{


public:


	Sound();
	void playSound(sound_types type, bool loop);
	void playSound(sound_types type, bool loop, float distance);
	void PauseSound(sound_types type);
	HSAMPLE loadSample(char * filename);
	void playSound(const char *filename);
	void pause(sound_types type);
	~Sound();


	HSAMPLE *samples;
	HCHANNEL *chs;
	float *initTimes;
	
	

};


#endif
