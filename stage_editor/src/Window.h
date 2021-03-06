#pragma once
#include "SFML/Graphics.hpp"
#include "DynamicVar.h"
#include "Search.h"

#define g_window (*Window::Instance)

namespace Map
{
	class Node;
}
namespace Coll
{
	class Node;
}

namespace Var
{
	enum
	{
		ID,
		VAL,
		TYPE,
		FOCUS,
	};
}

class Window
{
//----SINGLETON
public:
	static int		height;
	static int		width;
public:
	static void		init(int w, int h);
	static Window	*Instance;
private:
	Window();
public:
	~Window() {};
//----------------------------------------

	sf::Vector2f			mousePos();
	//sf::Vector2i			mousePos();
	void					procEvent();
	void					cleanEvent();
	void					act();
	void					draw(const sf::Drawable &drawable);
	void					loop();
	void					select();
	void					display();

public:
	sf::RenderWindow		m_window;
	sf::View				m_view;
	sf::View				m_neutralView;
	bool					m_mouse[sf::Mouse::ButtonCount];
	bool					m_mouseJustPressed[sf::Mouse::ButtonCount];
	sf::Vector2i			m_mousePos;
	sf::Vector2i			m_deltaMousePos;
	int						m_wheelDelta;
	sf::Vector2f			m_mousePosFloat;
	sf::Vector2f			m_deltaMouseFloat;
	bool					m_alive;
	bool					m_isSelecting;
	sf::RectangleShape		m_selectShape;
	bool					m_keyPressed[sf::Keyboard::KeyCount];
	bool					m_justPressed[sf::Keyboard::KeyCount];
	float					m_fromx;
	float					m_fromy;
	float					m_sizex;
	float					m_sizey;
	float					m_zoom;
	float					m_zoomTotal;
	int						m_tmp[10];
	DynamicVar				m_vars[10];
	Search					m_search;

	bool					m_focus;
	bool					m_editing;
};