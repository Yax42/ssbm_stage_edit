#pragma once
#include "SFML\Graphics.hpp"
#include <string>

class Button
{
public:
	Button(
		const std::string &text,
		sf::Vector2f &from,
		sf::Vector2f &to,
		const sf::Color &color,
		float outlineThickness,
		const sf::Color &outlineColor);
	bool contains(sf::Vector2f &mouse);
	bool coloredContains(sf::Vector2f &mouse);
	void displayBox();
	void displayText();
	void setColor2(bool);
	void setText(const std::string &str);
	void setOrigin(sf::Vector2f &from, sf::Vector2f &to);
	void setColor(sf::Color col);
	std::string getText();

public:
	bool					Enabled;
private:
	sf::RectangleShape		m_rect;
	sf::Color				m_color;
	sf::Color				m_color2;
	sf::Text				m_text;
	sf::Vector2f			m_from;
	sf::Vector2f			m_to;
};