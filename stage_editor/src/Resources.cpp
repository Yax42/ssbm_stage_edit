#include "Resources.h"

sf::Font	Resources::Font;

void		Resources::init()
{
	if (!Font.loadFromFile("../font.ttf"))
		throw("Font not found");
}