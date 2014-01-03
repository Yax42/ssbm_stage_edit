#include "Coll_Node.h"
#include "Window.h"

namespace Coll
{
	Node::Node(int *ptr, int deep) : ANode(deep)
	{
		m_ptr = ptr;
		m_floatData = (float *) ptr;

		m_rect.setPosition(pos());
		m_rect.setFillColor(sf::Color(255, 0, 255, 128));

		m_sizeFactor = 6;
		m_rect.setRadius(size().x/2);
	}

	Node::~Node()
	{
	}

	void		Node::print()
	{
		std::cout.precision(4);
		printTab();
		std::cout << "X:" << m_floatData[LOCATIONX]
				  << "\tY:" << m_floatData[LOCATIONY] << std::endl;
	}

	void		Node::display()
	{
		updatePos();
		g_window.draw(m_rect);
	}

	void			Node::updatePos()
	{
		m_rect.setPosition(x(), y());
	}

	void			Node::x(float x)
	{
		m_floatData[LOCATIONX] = x;
		updatePos();
	}

	void			Node::y(float y)
	{
		m_floatData[LOCATIONY] = y;
		updatePos();
	}

	float			Node::x() const
	{
		return m_floatData[LOCATIONX];
	}

	float			Node::y() const
	{
		return m_floatData[LOCATIONY];
	}

	void			Node::setThickness(int v)
	{
		m_rect.setOutlineThickness(v);
	}
}