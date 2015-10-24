#include "Button.h"
#include "Window.h"
#include "Resources.h"


Button::Button(
	const std::string &text,
	sf::Vector2f &from,
	sf::Vector2f &to,
	const sf::Color &color,
	float outlineThickness,
	const sf::Color &outlineColor)
{
	outlineThickness *= 2;

	Enabled = true;
	m_rect.setPosition(from);

	auto size = to - from;
	m_rect.setSize(size);
	m_rect.setFillColor(color);
	m_rect.setOutlineColor(outlineColor);
	m_rect.setOutlineThickness(outlineThickness);

	m_from = from;
	m_to = to;
	m_color = color;
	m_color2 = sf::Color(color.r * 0.7f, color.g*0.7f, color.b*0.7f, color.a);
	m_text.setFont(Resources::Font);
	m_text.setStyle(sf::Text::Bold);
	m_text.setString(text);
	m_text.setCharacterSize(size.y * 0.5f);
	m_text.setColor(sf::Color::Black);
	m_text.setPosition(m_from + sf::Vector2f(10.f, size.y * 0.2f));


}

bool	Button::contains(sf::Vector2f &mouse)
{

	return (Enabled &&
		m_from.x < mouse.x &&  m_to.x > mouse.x &&
		m_from.y < mouse.y &&  m_to.y > mouse.y);
}

void	Button::setColor(sf::Color col)
{
	m_color = col;
	m_rect.setFillColor(col);
}

void	Button::setColor2(bool b)
{
	m_rect.setFillColor(b ? m_color2 : m_color);
}

void	Button::displayText()
{
	g_window.m_window.draw(m_text);
}
void	Button::displayBox()
{
	g_window.m_window.draw(m_rect);
}

bool	Button::coloredContains(sf::Vector2f &mouse)
{
	bool c = contains(mouse);
	setColor2(c);
	return c;
}

void	Button::setText(const std::string &str)
{
	m_text.setString(str);
}

std::string		Button::getText()
{
	return m_text.getString();
}
	

void	Button::setOrigin(sf::Vector2f &from, sf::Vector2f &to)
{
	m_from = from;
	m_to = to;
	m_rect.setPosition(from);
	auto size = to - from;
	m_rect.setSize(size);
}
