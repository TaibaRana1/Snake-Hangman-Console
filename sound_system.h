//completed
#pragma once

#include <SFML/Audio.hpp>
#include <iostream>

using namespace std;
using namespace sf;


//sound system is a shell for it's 3 derrived classes so we make it Abstract 

class sound_system
{
protected:
	Sound sfx;

public:
	virtual void play_sfx(int sfx_id) = 0;
	virtual void play_bg_music() = 0;
	virtual void stop_bg_music() = 0;
	virtual ~sound_system() = default; // use default destructor as sfml will handle clean up of SoundBuffer and Music
};


//---------------------------
class snake_sound : public sound_system
{
	SoundBuffer eating;   //sfx id ->1
	SoundBuffer collision; //sfx id ->2
public:

	snake_sound()
	{
		if (!eating.loadFromFile("audio/sfx/snake eating.wav"))
		{
			cout << "\nfailuire to load eating sfx";
		}
		if (!collision.loadFromFile("audio/sfx/snake collision.wav"))
		{
			cout << "\nfailuire to load collision sfx";
		}
	}

	void play_sfx(int sfx_id) override
	{
		switch (sfx_id)
		{
		case 1:
			sfx.setBuffer(eating);
			sfx.play();
			break;
		
		case 2:
			sfx.setBuffer(collision);
			sfx.play();
			break;

		default:
			cout << "\ninvalid sfx id";
			break;
		}
	}

	void play_bg_music() override {}
	void stop_bg_music() override {}

};

class wordle_sound : public sound_system
{
	SoundBuffer win;      //sfx id ->1
	SoundBuffer loose;	  //sfx id ->2
	SoundBuffer tile_sfx; //sfx id ->3

public:

	wordle_sound()
	{
		if (!win.loadFromFile("audio/sfx/wordle win.wav"))
		{
			cout << "\nfailuire to load win sfx";
		}
		if (!loose.loadFromFile("audio/sfx/wordle loose.wav"))
		{
			cout << "\nfailuire to load loose sfx";
		}
		if (!tile_sfx.loadFromFile("audio/sfx/wordle tile select.wav"))
		{
			cout << "\nfailuire to load tile sfx";
		}
	}
	void play_sfx(int sfx_id) override
	{
		switch (sfx_id)
		{
		case 1:
			sfx.setBuffer(win);
			sfx.play();
			break;

		case 2:
			sfx.setBuffer(loose);
			sfx.play();
			break;

		case 3:
			sfx.setBuffer(tile_sfx);
			sfx.play();
			break;

		default:
			cout << "\ninvalid sfx id";
			break;
		}
	}
	
	void play_bg_music() override {}
	void stop_bg_music() override {}

};

class hangman_sound : public sound_system
{
	SoundBuffer win;      //sfx id ->1
	SoundBuffer loose;	  //sfx id ->2
	SoundBuffer right;	  //sfx id ->3
	SoundBuffer wrong;	  //sfx id ->4
	SoundBuffer tile_sfx; //sfx id ->5

public:

	hangman_sound()
	{
		if (!win.loadFromFile("audio/sfx/hm win.wav"))
		{
			cout << "\nfailuire to load win sfx";
		}
		if (!loose.loadFromFile("audio/sfx/hm loose.wav"))
		{
			cout << "\nfailuire to load loose sfx";
		}
		if (!right.loadFromFile("audio/sfx/hm right.wav"))
		{
			cout << "\nfailuire to load tile sfx";
		}
		if (!wrong.loadFromFile("audio/sfx/hm wrong.wav"))
		{
			cout << "\nfailuire to load tile sfx";
		}
		if (!tile_sfx.loadFromFile("audio/sfx/wordle tile select.wav"))
		{
			cout << "\nfailuire to load tile sfx";
		}
	}
	void play_sfx(int sfx_id) override
	{
		switch (sfx_id)
		{
		case 1:
			sfx.setBuffer(win);
			sfx.play();
			break;

		case 2:
			sfx.setBuffer(loose);
			sfx.play();
			break;

		case 3:
			sfx.setBuffer(right);
			sfx.play();
			break;
		case 4:
			sfx.setBuffer(wrong);
			sfx.play();
			break;
		case 5:
			sfx.setBuffer(tile_sfx);
			sfx.play();
			break;

		default:
			cout << "\ninvalid sfx id";
			break;
		}
	}

	void play_bg_music() override {}
	void stop_bg_music() override {}

};

class menu_sound : public sound_system
{
	SoundBuffer option_select; //sfx id -> 1
	Music bg_music;
public:

	menu_sound()
	{
		if (!option_select.loadFromFile("audio/sfx/select.wav"))
		{
			cout << "\nfailuire to load menu option selection sfx";
		}
		if (!bg_music.openFromFile("audio/bg music/menu bg music.wav")) 
		{	
			cout << "\nfailuire to load menu bg music";
		}
	}
	void play_sfx(int sfx_id) override
	{
		switch (sfx_id)
		{
		case 1:
			sfx.setBuffer(option_select);
			sfx.play();
			break;
		default:
			cout << "\ninvalid sfx id";
			break;
		}
	}
	void play_bg_music() override
	{
		bg_music.setVolume(30);
		bg_music.play();
	}
	void stop_bg_music() override
	{
		bg_music.stop();
	}

};
