#include <assert.h>

#include "MyMath.h"
#include "Window.h"
#include "MapHead.h"
#include "Worker.h"
#include "Coll_Node.h"
#include "GrGroundData.h"
#include "BytesMatrix.h"
#include "DatFile.h"

bool			DynamicVar::m_dummy = false;
bool			*DynamicVar::m_shift = &DynamicVar::m_dummy;
bool			*DynamicVar::m_ctrl = &DynamicVar::m_dummy;
bool			*DynamicVar::m_alt = &DynamicVar::m_dummy;
Window	*Window::Instance = NULL;
const int		Window::height = 768;
const int		Window::width = 1024;

void		Window::init()
{
	if (Instance == NULL)
		Instance = new Window();
}

Window::Window() : m_window(sf::VideoMode(width, height), "Melee mapping"), m_view()
{
	m_search = NULL;
	m_editing = true;
	m_zoomTotal = 1;
	m_focus = true;
	m_isSelecting = false;
	m_selectShape.setFillColor(sf::Color::Transparent);
	m_selectShape.setOutlineColor(sf::Color::Red);
	m_selectShape.setOutlineThickness(2);
	m_alive = true;
	assert(m_window.isOpen());
	m_window.setFramerateLimit(60);
	m_window.setView(m_view);
	for (int i = 0; i < sf::Keyboard::KeyCount; i++)
	{
		m_keyPressed[i] = false;
		m_justPressed[i] = false;
	}
	for (int i = 0; i < sf::Mouse::ButtonCount; i++)
	{
		m_mouse[i] = false;
		m_mouseJustPressed[i] = false;
	}
	m_zoom = 1;
	for (int i = 0; i < 10; i++)
		m_tmp[i] = 0;
	DynamicVar::m_shift = &m_keyPressed[sf::Keyboard::LShift];
	DynamicVar::m_ctrl = &m_keyPressed[sf::Keyboard::LControl];
	DynamicVar::m_alt = &m_keyPressed[sf::Keyboard::LAlt];
	m_vars[0] = DynamicVar("id", m_tmp[0], m_justPressed[sf::Keyboard::W], m_justPressed[sf::Keyboard::S]);
	m_vars[1] = DynamicVar("val", m_tmp[1], m_justPressed[sf::Keyboard::Q], m_justPressed[sf::Keyboard::A]);
	m_vars[2] = DynamicVar("type", m_tmp[2], m_justPressed[sf::Keyboard::E], m_justPressed[sf::Keyboard::D]);
	m_vars[3] = DynamicVar("focus", m_tmp[3], m_justPressed[sf::Keyboard::R], m_justPressed[sf::Keyboard::F]);
	m_vars[4] = DynamicVar("focusTestFloat", m_tmp[4], m_justPressed[sf::Keyboard::T], m_justPressed[sf::Keyboard::G]);
}

void	Window::draw(const sf::Drawable &drawable)
{
	m_window.draw(drawable);
}

sf::Vector2f			Window::mousePos()
{
	return m_mousePosFloat;
}

void					Window::act()
{
	if (m_editing)
	{
		if (m_mouseJustPressed[sf::Mouse::Left])
		{
			m_fromx = mousePos().x;
			m_fromy = mousePos().y;
			m_isSelecting = true;
		}
		if (m_mouse[sf::Mouse::Left])
		{
			m_sizex = mousePos().x - m_fromx;
			m_sizey = mousePos().y - m_fromy;
			m_selectShape.setSize(sf::Vector2f(m_sizex, m_sizey));
			m_selectShape.setPosition(m_fromx, m_fromy);
		}
		else if (m_isSelecting)
		{
			m_isSelecting = false;
			if (!m_keyPressed[sf::Keyboard::LControl] && !m_keyPressed[sf::Keyboard::RControl])
				ANode::clearSelections();
			float x0 = m_fromx;
			float x1 = m_fromx + m_sizex;
			float y0 = m_fromy;
			float y1 = m_fromy + m_sizey;
			ANode::selectArea(
				sf::Vector2f(x0 > x1 ? x1 : x0, y0 > y1 ? y1 : y0),
				sf::Vector2f(x0 < x1 ? x1 : x0, y0 < y1 ? y1 : y0),
				true, m_keyPressed[sf::Keyboard::LShift]);
		}

		if (m_mouse[sf::Mouse::Right])
		{
			float factor = (m_keyPressed[sf::Keyboard::Space] ? 0.1 : 1);
			ANode::moveSelect(sf::Vector2f(m_deltaMouseFloat.x * !m_keyPressed[sf::Keyboard::LShift] * factor,
				m_deltaMouseFloat.y * !m_keyPressed[sf::Keyboard::LControl] * factor));
		}

		//Camera
		if (m_mouse[sf::Mouse::Middle])
		{
			//float			x = m_view.getCenter().x - m_deltaMousePos.x;
			//float			y = m_view.getCenter().y - m_deltaMousePos.y;
			float			x = m_view.getCenter().x - m_deltaMouseFloat.x;
			float			y = m_view.getCenter().y - m_deltaMouseFloat.y;
			m_view.setCenter(sf::Vector2f(x, y));
			m_window.setView(m_view);
		}

		//--------------------------
		//---User Actions-----------
		//--------------------------
		for (int i = 0; i < 10; i++)
		{
			m_vars[i].proc();
			if (m_justPressed[sf::Keyboard::V])
				m_vars[i].print();
		}
		if (m_justPressed[sf::Keyboard::X])
		{
			if (m_tmp[Var::TYPE] == -1)
			{
				if (m_tmp[Var::ID] == 0)
				{
					if (m_tmp[Var::VAL] < CollData::Instance->m_nodes.size())
						CollData::Instance->m_nodes[m_tmp[Var::VAL]]->select();
				}
				else if (m_tmp[Var::ID] == 1)
				{
					if (m_tmp[Var::VAL] < CollData::Instance->m_links.size())
						CollData::Instance->m_links[m_tmp[Var::VAL]]->select();
				}
				else if (m_tmp[Var::ID] == 2)
				{
					if (m_tmp[Var::VAL] < CollData::Instance->m_areas.size())
						CollData::Instance->m_areas[m_tmp[Var::VAL]]->select();
				}
			}
			else if (m_tmp[Var::TYPE] == -2)
				GrGroundData::Instance->act();
			else if (m_tmp[Var::TYPE] == -3)
				Data::m_globalFileSize = m_tmp[Var::VAL];
			else
				ANode::globalAct(m_tmp);
		}
		if (m_justPressed[sf::Keyboard::C])
		{
			CollData::Instance->autoResolve();
		}
		//--------------------------
		//--------------------------
		//--------------------------

		if (m_wheelDelta != 0 && m_keyPressed[sf::Keyboard::LControl])
		{
			float	deltaZoom = 1 - Math::sgn(m_wheelDelta) * 0.2;
			int		time = Math::abs(m_wheelDelta);
			m_zoom = Math::pow(deltaZoom, time);
			m_view.zoom(m_zoom);
			m_zoomTotal *= m_zoom;
			m_window.setView(m_view);
			ANode::globalSetScale(m_zoomTotal);
			m_selectShape.setOutlineThickness(m_zoomTotal * 2);
		}

		if (m_justPressed[sf::Keyboard::N])
		{
			auto node = ANode::getOneSelected();
			int *ptr = Data::get<int>(0);
			if (node != NULL)
				ptr = node->m_ptr;
			int btSize = BytesMatrix::Instance->m_buttonSize;
			BytesMatrix::init(ptr, btSize, m_search);
			m_editing = false;
			m_window.setView(m_neutralView);
		}
	}
	else // BytesMatrix
	{
		if (BytesMatrix::Instance->Next != NULL)
		{
			int btSize = BytesMatrix::Instance->m_buttonSize;
			auto next = BytesMatrix::Instance->Next;
			BytesMatrix::init(next, btSize, m_search);
		}
		else if (m_wheelDelta != 0)
		{
			int pos = BytesMatrix::Instance->m_position;
			int btSize = BytesMatrix::Instance->m_buttonSize;
			if (m_keyPressed[sf::Keyboard::LControl])
			{
				float	deltaZoom = 1 + Math::sgn(m_wheelDelta) * 0.2f;
				int		time = Math::abs(m_wheelDelta);
				float zoom = Math::pow(deltaZoom, time);
				btSize *= zoom;
			}
			else
			{
				int factor = 200 / btSize;
				factor = (factor < 1 ? 1 : factor);
				//pos -= m_wheelDelta * factor;
				int lineSize = (1000 / (btSize + 1));
				pos -= m_wheelDelta * factor * lineSize * (m_keyPressed[sf::Keyboard::LShift] ? 120 : 4);
			}
			BytesMatrix::init(pos, btSize, m_search);
		}
		else if (m_justPressed[sf::Keyboard::S])
		{
			m_search = Search(BytesMatrix::Instance->m_current);
			int pos = BytesMatrix::Instance->m_position;
			int btSize = BytesMatrix::Instance->m_buttonSize;
			BytesMatrix::init(pos, btSize, m_search);
		}
		else if (m_justPressed[sf::Keyboard::D] && !m_keyPressed[sf::Keyboard::LShift])
		{
			if (m_search.Found)
			{
				int btSize = BytesMatrix::Instance->m_buttonSize;
				BytesMatrix::init(m_search.next(), btSize, m_search);
			}
		}
		else if (m_justPressed[sf::Keyboard::D] && m_keyPressed[sf::Keyboard::LShift])
		{
			if (m_search.Found)
			{
				int btSize = BytesMatrix::Instance->m_buttonSize;
				BytesMatrix::init(m_search.m_origin, btSize, m_search);
			}
		}

		if (m_justPressed[sf::Keyboard::N])
		{
			m_window.setView(m_view);
			ANode::globalUpdatePos();
			m_editing = true;
		}

		if (m_justPressed[sf::Keyboard::Dash])
			BytesMatrix::Instance->numberPressed(-3);
		else if (m_justPressed[sf::Keyboard::Space])
			BytesMatrix::Instance->numberPressed(-2);
		else if (m_justPressed[sf::Keyboard::Period])
			BytesMatrix::Instance->numberPressed(-1);
		else
		{
			for (int i = 0; i < 10; i++)
				if (m_justPressed[sf::Keyboard::Num0 + i])
				{
					BytesMatrix::Instance->numberPressed(i);
					break;
				}
		}

		BytesMatrix::Instance->process(m_mousePosFloat, m_mouseJustPressed[sf::Mouse::Left], m_mouseJustPressed[sf::Mouse::Right], m_keyPressed[sf::Keyboard::LControl]);
	}
	//Save
	if (m_justPressed[sf::Keyboard::Delete] || m_justPressed[sf::Keyboard::Tilde])
	{
		DatFile::save();
	}

	if (m_justPressed[sf::Keyboard::F1])
		DatFile::applyPrev();
	else if (m_justPressed[sf::Keyboard::F2])
		DatFile::applyNext();
}
void					Window::cleanEvent()
{
	m_deltaMousePos.x = 0;
	m_deltaMousePos.y = 0;
	m_wheelDelta = 0;
	m_deltaMouseFloat = sf::Vector2f();
	for (int i = 0; i < sf::Keyboard::KeyCount; i++)
		m_justPressed[i] = false;
	for (int i = 0; i < sf::Mouse::ButtonCount; i++)
		m_mouseJustPressed[i] = false;
}

void					Window::procEvent()
{
	sf::Event			ev;
	cleanEvent();

	while (m_window.pollEvent(ev))
	{
		switch (ev.type)
		{
			case sf::Event::Closed:
				m_alive = false;
				break;
			case sf::Event::GainedFocus:
				m_focus = true;
				break;
			case sf::Event::LostFocus:
				m_focus = false;
				break;
			case sf::Event::MouseMoved:
				m_deltaMousePos.x = ev.mouseMove.x - m_mousePos.x;
				m_deltaMousePos.y = ev.mouseMove.y - m_mousePos.y;
				m_mousePos.x = ev.mouseMove.x;
				m_mousePos.y = ev.mouseMove.y;
				break;
			case sf::Event::MouseWheelScrolled:
				m_wheelDelta = ev.mouseWheelScroll.delta;
				break;
				/*
			case sf::Event::MouseButtonPressed:
				m_mouseJustPressed[ev.mouseButton.button] = !m_mouse[ev.mouseButton.button];
				m_mouse[ev.mouseButton.button] = true;
				break;
			case sf::Event::MouseButtonReleased:
				m_mouse[ev.mouseButton.button] = false;
				break;
			case sf::Event::KeyPressed:
				m_justPressed[ev.key.code] = !m_keyPressed[ev.key.code];
				m_keyPressed[ev.key.code] = true;
				break;
			case sf::Event::KeyReleased:
				m_keyPressed[ev.key.code] = false;
				break;
				*/
		}
	}

	for (int i = 0; i < sf::Keyboard::KeyCount; i++)
	{
		if (sf::Keyboard::isKeyPressed((sf::Keyboard::Key) i))
		{
			m_justPressed[i] = !m_keyPressed[i];
			m_keyPressed[i] = true;
		}
		else
			m_keyPressed[i] = false;
	}
	for (int i = 0; i < sf::Mouse::ButtonCount; i++)
	{
		if (sf::Mouse::isButtonPressed((sf::Mouse::Button) i))
		{
			m_mouseJustPressed[i] = !m_mouse[i];
			m_mouse[i] = true;
		}
		else
			m_mouse[i] = false;
	}
	m_mousePosFloat = m_window.mapPixelToCoords(m_mousePos);
	m_deltaMouseFloat = sf::Vector2f(m_deltaMousePos.x * m_zoomTotal * 2, m_deltaMousePos.y * m_zoomTotal * 2);
	if (m_focus == false)
		cleanEvent();
}

void					Window::display()
{
	if (m_editing)
	{
		Worker::Instance->display();
		if (m_isSelecting)
			draw(m_selectShape);
	}
	else
	{

		BytesMatrix::Instance->display();
	}
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
		display();
		m_window.display();
	}
}