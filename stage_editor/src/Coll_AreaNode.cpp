#include "Coll_AreaNode.h"
#include "Window.h"
#include "CollData.h"


namespace Coll
{
	const std::vector<std::string> AreaNode::m_labels =
	{
		"Flag Top",
		"Flag Bot",
		"Flag Right",
		"Flag Left",
		"EMPTY",
		"X1",
		"Y1",
		"X2",
		"Y2",
		"Flags",
	};
	AreaNode::AreaNode(int *ptr, int id) : ANode(ptr, m_labels, "AreaNode", id, 0, &m_centerShape, true)
	{
		m_type = NodeType::ELEM;
		m_shortPtr = (short *) m_ptr;
		m_floatPtr = (float *) &m_ptr[X1];
		m_sizeFactor = 3;

		m_shape.setFillColor(sf::Color::Green);

		m_centerShape.setFillColor(sf::Color::Red);
		m_centerShape.setSize(size());
	}

	AreaNode::~AreaNode()
	{

	}

	void			AreaNode::print()
	{
			std::cout << "AreaNode\t" << m_id << "   ------------" << std::endl;

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

	void			AreaNode::display()
	{
#if 0
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
#endif

		for (int j = 0; j < 4; j++)
		{
			line[0] = sf::Vertex(sf::Vector2f(m_floatPtr[(j / 2) * 2], m_floatPtr[(j % 2) * 2 + 1]));
			line[1] = sf::Vertex(sf::Vector2f(m_floatPtr[(j % 2) * 2], m_floatPtr[(1 - (j / 2)) * 2 + 1]));

			if (m_selected)
			{
				line[0].color = sf::Color::Red;
				line[1].color = sf::Color::Red;
			}
			else
			{
				line[0].color = sf::Color::Magenta;
				line[1].color = sf::Color::Magenta;
			}

			g_window.m_window.draw(line, 2, sf::Lines);
		}
		g_window.draw(m_centerShape);
	}

	float			AreaNode::x() const
	{
		return (m_floatPtr[0] + m_floatPtr[2]) / 2;
	}
	float			AreaNode::y() const
	{
		return (m_floatPtr[1] + m_floatPtr[3]) / 2;
	}
	void			AreaNode::x(float x)
	{
		if (g_window.m_tmp[0] != 1)
			m_floatPtr[0] += x;
		if (g_window.m_tmp[0] == 1 || g_window.m_tmp[0] == 0)
			m_floatPtr[2] += x;
	}

	void			AreaNode::y(float y)
	{
		if (g_window.m_tmp[0] != 1)
			m_floatPtr[1] += y;
		if (g_window.m_tmp[0] == 1 || g_window.m_tmp[0] == 0)
			m_floatPtr[3] += y;
	}

	void			AreaNode::updatePos(){}

	void			AreaNode::setThickness(int v)
	{
		m_centerShape.setOutlineThickness(v);
	}

	void			AreaNode::act(int *data)
	{
		m_shortPtr[data[0] * 2 + (data[2] == 0)] = data[1];
	}
}