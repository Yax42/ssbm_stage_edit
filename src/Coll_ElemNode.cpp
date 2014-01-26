#include "Coll_ElemNode.h"
#include "Window.h"
#include "CollData.h"

namespace Coll
{
	ElemNode::ElemNode(int *ptr, int id) : ANode(0)
	{
		m_type = NodeType::ELEM;
		m_ptr = ptr;
		m_shortPtr = (short *) m_ptr;
		m_floatPtr = (float *) &m_ptr[X1];
		m_id = id;
		m_sizeFactor = 1;

		m_shape.setFillColor(sf::Color::Green);

		m_centerShape.setFillColor(sf::Color::Transparent);
		m_centerShape.setOutlineColor(sf::Color::Red);
		m_centerShape.setOutlineThickness(1);
		m_centerShape.setSize(sf::Vector2f(1, 1));
	}

	ElemNode::~ElemNode()
	{

	}

	void			ElemNode::print()
	{
			std::cout << "ElemNode\t" << m_id << "   ------------" << std::endl;

			for (int j = 0; j < 10; j++)
				std::cout << m_shortPtr[j] << " ";
			std::cout << std::endl;
			for (int j = 0; j < 4; j++)
				std::cout << m_floatPtr[j] << " ";

			std::cout << std::endl;
			std::cout << m_shortPtr[18] << " " << m_shortPtr[19] << " " << std::endl;


			//std::cout << "\t" << Math::binStr(*m_flag[0]) << " " << (int)*m_flag[0] << std::endl;
			//std::cout << "\t" << Math::binStr(*m_flag[1]) << " " << (int)*m_flag[1] << std::endl;

			//std::cout << " " << Math::binStr(m_shortPtr[FLAG1]) << " " << m_shortPtr[FLAG1] << std::endl;
			//std::cout << " " << Math::binStr(m_shortPtr[FLAG2]) << " " << m_shortPtr[FLAG2] << std::endl;
//			Math::toStr<unsigned short>(m_flags[idx], 1) << std::endl;
	}

	void			ElemNode::display()
	{
		for (int j = 0; j < 5; j++)
		{
			if (m_shortPtr[2 * j])
			{
				m_shape.setSize(sf::Vector2f(0.5, j + 1));
				m_shape.setPosition(CollData::m_links[m_shortPtr[2 * j + 1]]->x(),
									CollData::m_links[m_shortPtr[2 * j + 1]]->y());
				g_window.draw(m_shape);
			}
		}

		for (int j = 0; j < 4; j++)
		{
			line[0] = sf::Vertex(sf::Vector2f(m_floatPtr[(j / 2) * 2], m_floatPtr[(j % 2) * 2 + 1]));
			line[1] = sf::Vertex(sf::Vector2f(m_floatPtr[(j % 2) * 2], m_floatPtr[(1 - (j / 2)) * 2 + 1]));

			if (m_selected)
				line->color = sf::Color::Red;
			else
				line->color = sf::Color::Magenta;

			g_window.m_window.draw(line, 2, sf::Lines);
		}
		m_centerShape.setPosition(x(), y());
		g_window.draw(m_centerShape);
	}

	float			ElemNode::x() const
	{
		return (m_floatPtr[0] + m_floatPtr[2]) / 2;
	}
	float			ElemNode::y() const
	{
		return (m_floatPtr[1] + m_floatPtr[3]) / 2;
	}
	void			ElemNode::x(float x)
	{
		if (g_window.m_tmp[0] != 1)
			m_floatPtr[0] += x;
		if (g_window.m_tmp[0] == 1 || g_window.m_tmp[0] == 0)
			m_floatPtr[2] += x;
	}

	void			ElemNode::y(float y)
	{
		if (g_window.m_tmp[0] != 1)
			m_floatPtr[1] += y;
		if (g_window.m_tmp[0] == 1 || g_window.m_tmp[0] == 0)
			m_floatPtr[3] += y;
	}

	void			ElemNode::updatePos(){}

	void			ElemNode::setThickness(int v)
	{
	}

	void			ElemNode::act(int *data)
	{
		m_shortPtr[data[0] * 2 + (data[2] == 0)] = data[1];
	}
}