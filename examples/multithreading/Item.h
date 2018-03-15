#pragma once
#include "game.h"
#include "string.h"

Class game::Item{
	string icon;
	string title;
	Item(string, string);
	~Item();
}