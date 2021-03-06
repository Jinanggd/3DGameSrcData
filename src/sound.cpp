#include "sound.h"
#include <fstream>
#include <iostream>
#include "game.h"




Sound::Sound()
{
	if (BASS_Init(-1, 44100.0f, 0, 0, NULL) == false)
	{
		//error abriendo la tarjeta de sonido...

		std::cout << "error loading sound";

	}


	samples = new HSAMPLE[10];
	chs = new HCHANNEL[10];
	initTimes = new float[10];


	samples[0] = loadSample("data/sounds/background.wav");
	samples[1] = loadSample("data/sounds/death.wav");
	samples[2] = BASS_SampleLoad(FALSE, "data/sounds/ambience.mp3", 0, 0, 3, BASS_SAMPLE_LOOP);
	samples[3] = loadSample("data/sounds/canon.mp3");
	samples[4] = loadSample("data/sounds/footstep.wav");
	samples[5] = loadSample("data/sounds/pick.wav");
	samples[6] = loadSample("data/sounds/titan.mp3");
	samples[7] = loadSample("data/sounds/win.wav");
	samples[8] = BASS_SampleLoad(FALSE, "data/sounds/init.wav", 0, 0, 3, BASS_SAMPLE_LOOP);
	samples[9] = loadSample("data/sounds/explosion.wav");
	samples[10] = loadSample("data/sounds/upgrade.wav");



}


void Sound::playSound (sound_types type, bool loop)
{



	if ((int)type == (int)sound_types::ambience) {

	
		BASS_SAMPLE info;
		BASS_SampleGetInfo(samples[(int)type], &info); // get the sample's current info
		info.volume = 0.5;
		BASS_SampleSetInfo(samples[(int)type], &info);

	}

	if ((int)type == (int)sound_types::footstep) {

	
		BASS_SAMPLE info;
		BASS_SampleGetInfo(samples[(int)type], &info); // get the sample's current info
		info.volume = 0.2;
		BASS_SampleSetInfo(samples[(int)type], &info);

	}


	if ((int)type == (int)sound_types::init) {


		BASS_SAMPLE info;
		BASS_SampleGetInfo(samples[(int)type], &info); // get the sample's current info
		info.volume = 0.5;
		BASS_SampleSetInfo(samples[(int)type], &info);

	}


	if ((int)type == (int)sound_types::cannon) {


		BASS_SAMPLE info;
		BASS_SampleGetInfo(samples[(int)type], &info); // get the sample's current info
		info.volume = 0.4;
		BASS_SampleSetInfo(samples[(int)type], &info);
		BASS_ChannelStop(chs[(int)type]);

	}

	if ((int)type == (int)sound_types::win) {


		BASS_SAMPLE info;
		BASS_SampleGetInfo(samples[(int)type], &info); // get the sample's current info
		info.volume = 0.5;
		BASS_SampleSetInfo(samples[(int)type], &info);

	}
	

	if ((int)type == (int)sound_types::explosion) {


		BASS_SAMPLE info;
		BASS_SampleGetInfo(samples[(int)type], &info); // get the sample's current info
		info.volume = 0.6;
		BASS_SampleSetInfo(samples[(int)type], &info);

		BASS_ChannelStop(chs[(int)type]);

	}



	if (!(BASS_ChannelIsActive(chs[(int)type]) == BASS_ACTIVE_PLAYING)) {


	chs[(int)type] = BASS_SampleGetChannel(samples[(int)type], FALSE);


	BASS_ChannelPlay(chs[(int)type], loop);

	}

}



void Sound::playSound(sound_types type, bool loop, float distance)
{

	BASS_SAMPLE info;
	BASS_SampleGetInfo(samples[(int)type], &info); // get the sample's current info

	info.volume = clamp((1.0f / (distance*0.3)), 0, 1);
	BASS_SampleSetInfo(samples[(int)type], &info);

	if (!(BASS_ChannelIsActive(chs[(int)type]) == BASS_ACTIVE_PLAYING)) {


		chs[(int)type] = BASS_SampleGetChannel(samples[(int)type], FALSE);


		BASS_ChannelPlay(chs[(int)type], loop);

	}

}



void Sound::PauseSound(sound_types type)
{

	BASS_ChannelPause(chs[(int)type]);



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






void Sound::pause(sound_types type)
{
	BASS_ChannelStop(samples[int(type)]);
}

Sound::~Sound()
{
}

