#include "Map_Node.h"
#include "Map_Group.h"

namespace Map
{
	Node::Node(int *ptr, int deep) : Object(deep)
	{
		ObjectGroup::tmp++;
		m_ptr = ptr;

		m_intData = Data::get<int>(*ptr);
		m_floatData = Data::get<float>(*ptr);

		m_rect.setSize(size());
		m_rect.setPosition(pos());
		m_rect.setFillColor(sf::Color(128, 128, 0, 128));

		if (m_intData[CHILD_PTR] != 0)
			m_child = new Node(&m_intData[CHILD_PTR], deep + 1);
		else
			m_child = NULL;
		if (m_intData[NEXT_PTR] != 0)
			m_next = new Node(&m_intData[NEXT_PTR], deep);
		else
			m_next = NULL;
	}

	Node::~Node()
	{
		if (m_next != NULL)
			delete m_next;
		if (m_child != NULL)
			delete m_child;
	}

	void		Node::printOnly()
	{
		std::cout.precision(4);
		printTab();
		std::cout << "Node Z" << m_intData[EMPTY1]
				<< " Flags:" << m_intData[FLAGS]
				<< " Child:" << m_intData[CHILD_PTR]
				<< " Next:" << m_intData[NEXT_PTR]
				<< " Data:" << m_intData[DATA_PTR]
				<< " Inverse:" << m_intData[INVERSE]
				<< " Z" << m_intData[EMPTY2] << std::endl;
		printTab();
		std::cout << "\tRotate:\t\tX" << m_floatData[ROTATEX] << "\tY" << m_floatData[ROTATEY] << "\tZ" << m_floatData[ROTATEZ] << std::endl;
		printTab();
		std::cout << "\tSize:\t\tX" << m_floatData[SIZEX] << "\tY" << m_floatData[SIZEY] << "\tZ" << m_floatData[SIZEZ] << std::endl;
		printTab();
		std::cout << "\tPosition:\tX" << m_floatData[LOCATIONX] << "\tY" << m_floatData[LOCATIONY] << "\tZ" << m_floatData[LOCATIONZ] << std::endl;
	}

	void		Node::print()
	{
		printOnly();
		if (m_child != NULL)
			m_child->print();
		if (m_next != NULL)
			m_next->print();
	}

	void		Node::display()
	{
		g_window.draw(m_rect);
		if (m_child != NULL)
			m_child->display();
		if (m_next != NULL)
			m_next->display();
	}

	Map::Node			*Node::getMouseTarget()
	{
		sf::Vector2f	mouse = g_window.mousePos();

		if (mouse.x > x() && mouse.x < x() + size().x
			&& mouse.y > y() && mouse.y < y() + size().y)
				return this;
		Node	*result = NULL;
		if (m_child != NULL)
			result = m_child->getMouseTarget();
		if (m_next != NULL && result == NULL)
			result = m_next->getMouseTarget();
		return result;
	}

	void			Node::x(float x)
	{
		m_floatData[LOCATIONX] = x;
		m_rect.setPosition(x, m_rect.getPosition().y);
	}

	void			Node::y(float y)
	{
		m_floatData[LOCATIONY] = y;
		m_rect.setPosition(m_rect.getPosition().x, y);
	}
}