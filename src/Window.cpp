#include <assert.h>
#include "Window.h"
#include "MapHead.h"
#include "Worker.h"

Window	*Window::Instance = NULL;
const int		Window::height = 600;
const int		Window::width = 800;

void		Window::init()
{
	if (Instance == NULL)
		Instance = new Window();
}

Window::Window() : m_window(sf::RenderWindow(sf::VideoMode(width, height), "M.StEdix")), m_view(), m_selectedNode(NULL)
{
	m_zoom = 50;
	m_alive = true;
	assert(m_window.isOpen());
	m_window.setFramerateLimit(60);
	m_window.setView(m_view);
}

void	Window::draw(const sf::Drawable &drawable)
{
	m_window.draw(drawable);
}

sf::Vector2f			Window::mousePos()
{
	return m_mousePosFloat;
/*	
	int		x = sf::Mouse::getPosition().x + m_view.getCenter().x - width / 2;
	int		y = sf::Mouse::getPosition().y + m_view.getCenter().y - height / 2;
	return sf::Vector2i(x, y);*/
}

/*
sf::Vector2i			Window::mousePos()
{
	int		x = sf::Mouse::getPosition().x + m_view.getCenter().x - width / 2;
	int		y = sf::Mouse::getPosition().y + m_view.getCenter().y - height / 2;
	return sf::Vector2i(x, y);
}
*/

void					Window::select()
{
	bool			prevNull = (m_selectedNode == NULL);
	if (m_selectedNode != NULL)
		m_selectedNode->m_rect.setOutlineThickness(0);
	m_selectedNode = MapHead::getMouseTarget();
	if (m_selectedNode != NULL)
		m_selectedNode->m_rect.setOutlineThickness(3);
	if (prevNull && m_selectedNode != NULL)
		m_selectedNode->printOnly();
}

void					Window::act()
{
	if (m_selectedNode == NULL || sf::Mouse::isButtonPressed(sf::Mouse::Left) == false)
		select();
	if (m_selectedNode != NULL && sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		m_selectedNode->x(mousePos().x);
		m_selectedNode->y(mousePos().y);
	}

	//Camera
	if (sf::Mouse::isButtonPressed(sf::Mouse::Middle))
	{
		float			x = m_view.getCenter().x - m_deltaMousePos.x;
		float			y = m_view.getCenter().y - m_deltaMousePos.y;
		m_view.setCenter(sf::Vector2f(x, y));
		m_window.setView(m_view);
	}

	if (m_wheelDelta != 0)
	{
		m_zoom += m_wheelDelta;
		if (m_zoom < 0)
			m_zoom = 0;
		else if (m_zoom > 100)
			m_zoom = 100;
		m_view.zoom(m_zoom / 100.0);
		m_window.setView(m_view);
	}
}

void					Window::procEvent()
{
	sf::Event			ev;

	m_deltaMousePos.x = 0;
	m_deltaMousePos.y = 0;
	m_wheelDelta = 0;
	while (m_window.pollEvent(ev));
	{
		switch (ev.type)
		{
			case sf::Event::Closed:
				m_alive = false;
				break;
			case sf::Event::MouseMoved:
				m_deltaMousePos.x = ev.mouseMove.x - m_mousePos.x;
				m_deltaMousePos.y = ev.mouseMove.y - m_mousePos.y;
				m_mousePos.x = ev.mouseMove.x;
				m_mousePos.y = ev.mouseMove.y;
				break;
			case sf::Event::MouseWheelMoved:
				m_wheelDelta = ev.mouseWheel.delta;
				break;
		}
	}

	m_mousePosFloat = m_window.mapPixelToCoords(m_mousePos);
}


void					Window::loop()
{
	sf::RectangleShape	rect(sf::Vector2f(10, 30));

	while (m_alive)
	{
		//rect.setPosition(mousePos().x, mousePos().y);
		//m_window.draw(rect);
		m_window.clear();
		procEvent();
		act();
		Worker::display();
		m_window.display();
	}
}