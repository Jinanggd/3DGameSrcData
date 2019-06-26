#include "sound.h"
#include <fstream>
#include <iostream>


//World* World::Instance() {
//	return instance;
//}
Sound::Sound()
{
	if (BASS_Init(-1, 44100, 0, 0, NULL) == false)
	{
		//error abriendo la tarjeta de sonido...

		std::cout << "error loading sound";

	}


	samples = new HSAMPLE[10];

	samples[0] = loadSample("data/sounds/background.wav");
	samples[1] = loadSample("data/sounds/death.wav");


}


void Sound::playSound (sound_types type)
{
	HCHANNEL ch;

	switch (type){
	
	case sound_types::background:

		ch = BASS_SampleGetChannel(samples[0], FALSE);

		if (!BASS_ChannelPlay(ch, true))

		break;

	case sound_types::death:

		ch = BASS_SampleGetChannel(samples[1], FALSE);
	
		if (!BASS_ChannelPlay(ch, true))

		break;
	
	
	}

}


HSAMPLE Sound::loadSample(char * filename)
{
	HSAMPLE sam;
	if (sam = BASS_SampleLoad(FALSE, filename, 0, 0, 3, BASS_SAMPLE_OVER_POS))
		std::cout << "sample " << filename << " loaded!" << std::endl;
	else
	{
		std::cout << "Can't load sample";
		exit(0);
	}
	return sam;
}




Sound::~Sound()
{
}

