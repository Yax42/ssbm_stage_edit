#pragma once
#include "SFML/Graphics.hpp"

#define g_window (*Window::Instance)

namespace Map
{
	class Node;
}

class Window
{
//----SINGLETON
public:
	static const int		height;
	static const int		width;
public:
	static void		init();
	static Window	*Instance;
private:
	Window();
public:
	~Window() {};
//----------------------------------------

	sf::Vector2f			mousePos();
	//sf::Vector2i			mousePos();
	void					procEvent();
	void					act();
	void					draw(const sf::Drawable &drawable);
	void					loop();
	void					select();
public:
	sf::RenderWindow		m_window;
	sf::View				m_view;
	Map::Node				*m_selectedNode;
	bool					m_mouse[sf::Mouse::ButtonCount];
	sf::Vector2i			m_mousePos;
	sf::Vector2i			m_deltaMousePos;
	int						m_wheelDelta;
	sf::Vector2f			m_mousePosFloat;
	bool					m_alive;
	int						m_zoom;
};