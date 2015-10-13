#include "Coll_LinkNode.h"
#include "Window.h"
#include "CollData.h"

namespace Coll
{
	LinkNode::LinkNode(int *ptr, int id) : ANode(0, NULL)
	{
		m_type = NodeType::LINK;
		m_ptr = ptr;
		m_shortPtr = (short *) ptr;
		m_id = id;
		m_sizeFactor = 4;
		m_flag[0] = &((Byte*) &m_ptr[I_FLAG])[1];
		m_flag[1] = &((Byte*) &m_ptr[I_FLAG])[2];
		m_flag[2] = &((Byte*) &m_ptr[I_FLAG])[0];
		m_flag[3] = &((Byte*) &m_ptr[I_FLAG])[3];

		m_correct =	CollData::m_count > m_shortPtr[LINK1_F]
					&& CollData::m_count > m_shortPtr[LINK1_T]
					&& ((CollData::m_count > m_shortPtr[LINK2_F]
					&& CollData::m_count > m_shortPtr[LINK2_T])
					|| (m_shortPtr[LINK2_F] == -1 && m_shortPtr[LINK2_T] == -1));
		if (!m_correct)
		{
			std::cout << "Fake link" << CollData::m_count
				<< " " << m_shortPtr[LINK1_F]
				<< " " << m_shortPtr[LINK1_T]
				<< " " << m_shortPtr[LINK2_F]
				<< " " << m_shortPtr[LINK2_T] << std::endl;
		}
	//	std::cout << id << "\t" << m_shortPtr[LINK1_F] << "\t" << m_shortPtr[LINK1_T] << "\t"
	//	 << m_shortPtr[LINK2_F] << "\t" << m_shortPtr[LINK2_T] << std::endl;
	}

	LinkNode::~LinkNode()
	{

	}

	void			LinkNode::print()
	{
			std::cout << m_id << "\tfrom: " << m_shortPtr[LINK1_F] << "\tto: " << m_shortPtr[LINK1_T] << std::endl;
			std::cout << "\tfrom: " << m_shortPtr[LINK2_F] << "\tto: " << m_shortPtr[LINK2_T] << std::endl;
			std::cout << "\t" << Math::binStr(*m_flag[0]) << " " << (int)*m_flag[0] << std::endl;
			std::cout << "\t" << Math::binStr(*m_flag[1]) << " " << (int)*m_flag[1] << std::endl;
			if (*m_flag[2] || *m_flag[3])
			{
				std::cout << "WUTFLAG2 " << Math::binStr(*m_flag[2]) << " " << (int)*m_flag[2] << std::endl;
				std::cout << "WUTFLAG3 " << Math::binStr(*m_flag[3]) << " " << (int)*m_flag[3] << std::endl;
			}
			//std::cout << " " << Math::binStr(m_shortPtr[FLAG1]) << " " << m_shortPtr[FLAG1] << std::endl;
			//std::cout << " " << Math::binStr(m_shortPtr[FLAG2]) << " " << m_shortPtr[FLAG2] << std::endl;
//			Math::toStr<unsigned short>(m_flags[idx], 1) << std::endl;
	}

	void			LinkNode::display()
	{
		if (!m_correct)
			return;

		m_line[0] = sf::Vertex(getNodePos(LINK1_F)),
		m_line[1] = sf::Vertex(getNodePos(LINK1_T));
		if (m_selected)
		{
			m_line[0].color = sf::Color::Green;
			m_line[1].color = sf::Color::Green;
		}
		else
		{
			m_line[1].color = sf::Color::Cyan;
			m_line[0].color = sf::Color::Cyan;
		}
		g_window.m_window.draw(m_line, 2, sf::Lines);

		if (m_shortPtr[LINK2_F] != -1)
		{
			m_line[0] = sf::Vertex(pos());
			m_line[1] = sf::Vertex(getLinkPos(LINK2_F));
			m_line[0].color = sf::Color(0,128,0, 255);
			m_line[1].color = sf::Color(0,128,0, 255);
			g_window.m_window.draw(m_line, 2, sf::Lines);
		}

		if (m_shortPtr[LINK2_T] != -1)
		{
			m_line[0] = sf::Vertex(pos());
			m_line[1] = sf::Vertex(getLinkPos(LINK2_T));
			m_line[0].color = sf::Color(0,128,0, 255);
			m_line[1].color = sf::Color(0,128,0, 255);
			g_window.m_window.draw(m_line, 2, sf::Lines);
		}
	}

	sf::Vector2f	LinkNode::getNodePos(int id)
	{
		return CollData::m_nodes[m_shortPtr[id]]->pos();
	}

	sf::Vector2f	LinkNode::getLinkPos(int id)
	{
		return CollData::m_links[m_shortPtr[id]]->pos();
	}

	float			LinkNode::x() const
	{
		return (CollData::m_nodes[m_shortPtr[LINK1_F]]->x() +
		CollData::m_nodes[m_shortPtr[LINK1_T]]->x()) / 2;
	}
	float			LinkNode::y() const
	{
		return (CollData::m_nodes[m_shortPtr[LINK1_F]]->y() +
		CollData::m_nodes[m_shortPtr[LINK1_T]]->y()) / 2;
	}
	void			LinkNode::x(float x){}
	void			LinkNode::y(float y){}
	void			LinkNode::updatePos(){}
	void			LinkNode::setThickness(int v)
	{
		if (v > 0)
			m_line[1].color = sf::Color::White;
		else
			m_line[1].color = sf::Color::Blue;
	}

	void			LinkNode::act(int *data)
	{
		if (data[2] == 0)
			*m_flag[data[0]] = data[1];
		else
			m_shortPtr[data[0]] = data[1];
	}
}