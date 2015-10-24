#include "Coll_Node.h"
#include "Window.h"
#include "MyMath.h"

namespace Coll
{
	const std::vector<std::string> Node::m_labels =
	{
		"X",
		"Y",
	};
	Node::Node(int *ptr, int id, int deep) : ANode(ptr, m_labels, "CollNode", id, deep, &m_rect, true)
	{
		m_type = NodeType::COLL;
		m_position = (float *)ptr;
//		m_flags = Data::get<int>(m_ptr[2]) + m_id * 4;

		m_rect.setPosition(pos());
		m_rect.setFillColor(sf::Color(255, 128, 255, 255));

		m_sizeFactor = 2;
		m_rect.setRadius(size().x);
	}

	Node::~Node()
	{
	}

	/*
	void	Node::printFlag(int idx)
	{
		std::cout	<< Math::binStr(m_flags[idx]) << "\t"
					<< Math::toStr<unsigned short>(m_flags[idx], 0) << "\t"
					<< Math::toStr<unsigned short>(m_flags[idx], 1) << std::endl;
					//<< m_flags[idx] << std::endl;
	}
	*/

	void		Node::print()
	{
		//2:2
		//5 6 7
		std::cout.precision(4);
		printTab();
		std::cout	<< "id:" << m_id
					<<"\tX:" << m_position[LOCATIONX]
					<< "\tY:" << m_position[LOCATIONY] << std::endl;
					/*
		printFlag(0);
		printFlag(1);
		if (m_flags[2] != -1)
			std::cout << "This should be -1 instead of: " << m_flags[2] << std::endl;
		printFlag(3);
		*/
		//m_position = Data::get<float>(ptr[0]) + m_id * 2;
	}

	void		Node::display()
	{
		g_window.draw(m_rect);
	}

	void			Node::x(float x)
	{
		m_position[LOCATIONX] = x;
	}

	void			Node::y(float y)
	{
		m_position[LOCATIONY] = y;
	}

	float			Node::x() const
	{
		return m_position[LOCATIONX];
	}

	float			Node::y() const
	{
		return m_position[LOCATIONY];
	}

	void			Node::setThickness(int v)
	{
		m_rect.setOutlineThickness(v);
	}

	void			Node::act(int *data)
	{
		m_position[data[0] != 0] = data[1];
	}
}