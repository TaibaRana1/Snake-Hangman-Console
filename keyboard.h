#pragma once

#include "window.h"       //draw on window
#include "tiles.h"        //draw tiles   


#include <iostream>
#include <string>

#include <SFML/Graphics.hpp>
#include"tiles.h"

using namespace std;
using namespace sf;

class keyboard : public Wnd
{
private:
	tile tiles_list[28]; //array to store tiles

	int tile_count;
	void initialise_tiles()
	{

		const string tiles_arr[] =
    {
        "Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P",
        "A", "S", "D", "F", "G", "H", "J", "K", "L", "Enter",
        "Z", "X", "C", "V", "B", "N", "M", "Clear"
    };

    int tile_width = 60;
    int tile_height = 60;
    int btn_width = 150;
    int btn_height = 60;

    int curr_x = 50, curr_y = 50;
    int space = 5;

    for (int i = 0; i < 28; ++i)
    {
        if (i < 10)
        {
            tiles_list[i] = tile(IntRect(curr_x, curr_y, tile_width, tile_height), tiles_arr[i]);
            curr_x += tile_width + space;
        }
        else if (i >= 10 && i < 20)
        {
			if (i == 19)
				tiles_list[i] = tile(IntRect(curr_x, curr_y, btn_width, btn_height), tiles_arr[i]);
			else
            tiles_list[i] = tile(IntRect(curr_x, curr_y, tile_width, tile_height), tiles_arr[i]);
            curr_x += tile_width + space;
        }
        else if (i >= 20)
        {
			if (i == 27)
				tiles_list[i] = tile(IntRect(curr_x, curr_y, btn_width, btn_height), tiles_arr[i]);
			else
            tiles_list[i] = tile(IntRect(curr_x, curr_y, tile_width, tile_height), tiles_arr[i]);
            curr_x += tile_width + space;
        }
    }
	}
	string input_word;
	bool _exit;

public:

	//initialize the abstarct (base) class "window" aswell
	keyboard() 
		: Wnd() , tile_count(0) , _exit(0)
	{ 
		input_word = "";
		initialise_tiles();
		cout << "\nTiles initiazlized";
	}

	void draw( RenderWindow& r_window) override
	{
		cout << "Keyboard draw start..." << endl;
		for (int i = 0; i < 28; i++)
		{
			cout << "Attempting to draw tile index: " << i << endl;
			tiles_list[i].draw(r_window);
		}
		cout << "Keyboard draw finished." << endl;
	}

	void handle_mouse_input(const Vector2f& m_pos) override //listener
	{
		for (int i = 0; i < 28; i++)
		{
			if (tiles_list[i].is_tile_clicked(m_pos))
			{
				if (i != 19 && i != 27)
					input_word.append(tiles_list[i].get_tile_text());
				else if (i == 27) //clear
				{
					if (!input_word.empty())
						input_word.pop_back();
				}
				else if (i == 19) //enter
				{
					break;
				}
			}
		}
	}

	void handle_keyboard_input(Keyboard::Key key) override
	{
		if (key == sf::Keyboard::Escape) 
		{
			cout << "\nExiting the game";
			_exit = 1;
		}
	}

};
//load_font("arial.ttf");
//1280x720