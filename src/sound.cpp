#include "sound.h"
#include <fstream>
#include <iostream>


//World* World::Instance() {
//	return instance;
//}
Sound::Sound()
{
	if (BASS_Init(-1, 44100.0f, 0, 0, NULL) == false)
	{
		//error abriendo la tarjeta de sonido...

		std::cout << "error loading sound";

	}


	samples = new HSAMPLE[10];

	//samples[0] = loadSample("data/sounds/background.wav");
	//samples[1] = loadSample("data/sounds/death.wav");
	//samples[2] = loadSample("data/sounds/ambience.mp3");
	//samples[3] = loadSample("data/sounds/canon.mp3");
	samples[4] = loadSample("data/sounds/footstep.wav");
	//samples[5] = loadSample("data/sounds/pick.wav");
	//samples[6] = loadSample("data/sounds/titan.mp3");
	//samples[7] = loadSample("data/sounds/win.wav");
	//samples[8] = loadSample("data/sounds/death.wav");
	//samples[9] = loadSample("data/sounds/death.wav");


}


void Sound::playSound (sound_types type)
{

	switch (type)
	{
	case sound_types::background:

		ch = BASS_SampleGetChannel(samples[0], FALSE);
		//ch = BASS_SampleGetChannel(samples[0], FALSE);

		if (!BASS_ChannelPlay(ch, true))

			break;
	case sound_types::death:

		ch = BASS_SampleGetChannel(samples[1], FALSE);
		//BASS_ChannelSetAttribute(ch, BASS_ATTRIB_FREQ,1.5f);
		//BASS_ChannelSetAttribute(ch, BASS_ATTRIB_MUSIC_SPEED, 0.0001f);



		if (!BASS_ChannelPlay(ch, true))

			break;
	case sound_types::ambience:
		ch = BASS_SampleGetChannel(samples[2], FALSE);
		////ch = BASS_SampleGetChannel(samples[0], FALSE);

		if (!BASS_ChannelPlay(ch, true))
		break;
	case sound_types::cannon:
		ch = BASS_SampleGetChannel(samples[3], FALSE);
		////ch = BASS_SampleGetChannel(samples[0], FALSE);

		if (!BASS_ChannelPlay(ch, true))
		break;
	case sound_types::footstep:
		ch = BASS_SampleGetChannel(samples[4], FALSE);
		//ch = BASS_SampleGetChannel(samples[0], FALSE);

		if (!BASS_ChannelPlay(ch, true))
		break;
	case sound_types::pick:
		ch = BASS_SampleGetChannel(samples[5], FALSE);
		//ch = BASS_SampleGetChannel(samples[0], FALSE);

		if (!BASS_ChannelPlay(ch, true))
		break;
	case sound_types::titan:
		ch = BASS_SampleGetChannel(samples[6], FALSE);
		//ch = BASS_SampleGetChannel(samples[0], FALSE);

		if (!BASS_ChannelPlay(ch, true))
		break;
	case sound_types::win:
		ch = BASS_SampleGetChannel(samples[7], FALSE);
		//ch = BASS_SampleGetChannel(samples[0], FALSE);

		if (!BASS_ChannelPlay(ch, true))
		break;
	default:
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

