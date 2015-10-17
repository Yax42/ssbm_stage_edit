#pragma once
#include "Node.h"
#include "Window.h"

class TestPosition : public ANode
{
public:
	enum
	{
		ANGLEX,
		ANGLEY,
		ANGLEZ,
		SCALEX,
		SCALEY,
		SCALEZ,
		LOCATIONX,
		LOCATIONY,
		LOCATIONZ,
		COUNT
	};
	const std::vector<std::string>		TestPosition::m_labels =
	{
		"rX",
		"rY",
		"rZ",
		"sX",
		"sY",
		"sZ",
		"X",
		"Y",
		"Z",
	};

public:
	TestPosition(int *ptr, const std::string &name, int id)
		: ANode(ptr, m_labels, "Transformation", id, 0, &m_rect, true)
	{
		m_name = name;
		m_type = NodeType::TEST_POS;
		m_position = (float *)ptr;

		m_rect.setPosition(pos());
		m_rect.setFillColor(sf::Color(m_id * 32, (m_id % 3) * 80, 255, 255));

		m_sizeFactor = 1;
		m_rect.setRadius(size().x);
	}
	~TestPosition() {}
	virtual void	print()
	{
		std::cout.precision(4);
		printTab();
		std::cout << m_id << " " << Data::getId(m_position) << "\t" << m_name <<
					" Angle: " << m_position[ANGLEX] << ", " << m_position[ANGLEY] << ", " << m_position[ANGLEZ] <<
					" Scale: " << m_position[SCALEX] << ", " << m_position[SCALEY] << ", " << m_position[SCALEZ] <<
					" Pos: " << m_position[LOCATIONX] << ", " << m_position[LOCATIONY] << ", " << m_position[LOCATIONZ] << std::endl;
	}
	virtual void	display()
	{
		g_window.draw(m_rect);
	}
	virtual void			x(float x)
	{
		m_position[LOCATIONX] += x;
	}
	virtual void			y(float y)
	{
		m_position[LOCATIONY] += y;
	}
	virtual void			act(int *data)
	{
		if (data[0] < 0 || data[0] > COUNT)
			return;
		m_position[data[0]] = data[1] / 10.0;
	}
	virtual float			x() const { return m_position[LOCATIONX]; }
	virtual float			y() const { return m_position[LOCATIONY]; }
	virtual void			setThickness(int v) { m_rect.setOutlineThickness(v); }
	virtual bool			hide() { return (ANode::hide() || (g_window.m_tmp[4] >= 0 && g_window.m_tmp[4] != m_id)); }

public:
	int					*m_ptr;
	sf::CircleShape		m_rect;
	float				*m_position;
	std::string			m_name;
	//int					*m_flags;
};