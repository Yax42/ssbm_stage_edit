#include "Map_Node.h"
#include "Map_Group.h"
#include "NodeData.h"

namespace Map
{
	int			Node::MotherCount;
	char		*Node::Names[17] =
	{
		"0",
		"0",
		"Left/top camera boundary",
		"Right/bottom camera boundary",
		"Left/top blast zone",
		"Right/bottom blast zone",
		"Spawn item 1",
		"Spawn item 2",
		"Spawn item 3",
		"Spawn item 4",
		"Spawn item 5",
		"Spawn item 6",
		"Spawn item 7",
		"Spawn player 1",
		"Spawn player 2",
		"Spawn player 3",
		"Spawn player 4"
	};

	
	const std::vector<std::string> Node::m_labels =
	{
		"Empty1",
		"Flags",
		"Child_ptr",
		"Next_ptr",
		"Data_ptr",
		"Rotatex",
		"Rotatey",
		"Rotatez",
		"Sizex",
		"Sizey",
		"Sizez",
		"Locationx",
		"Locationy",
		"Locationz",
		"Inverse",
		"Empty2",
	};

	Node::Node(int *ptr, int deep, Node *mother, ObjectGroup &father, int idx)
		: ANode(ptr, m_labels, "MapNode", deep * 1000 + idx, deep, &m_rect, false)
	{
		m_type = NodeType::MAP;
		if (idx < 17 && father.m_id == 3 && deep == 2)
			//m_name = Names[idx];
		;//else
			m_name = std::to_string(idx);

		if (m_ptr == NULL)
		{	
			m_ptr = Data::Dummy;
			m_ptr = Data::Dummy;
			m_floatData = (float *)Data::Dummy;
			m_mother = mother;
			m_child = NULL;
			m_sizeFactor = 0;
			m_rect.setSize(size());
			m_next = NULL;
			//std::cout << "Dummy Map_Node" << std::endl;
			ObjectGroup::Dummy = true;
		}
		else
		{
			m_floatData = (float *) m_ptr;
			m_mother = mother;

			m_rect.setPosition(pos());
			m_rect.setFillColor(sf::Color((MotherCount % 2)* 127, ((MotherCount / 2) % 2) * 63 + 128, (m_ptr[DATA_PTR] != 0) * 255, 128));

			//std::cout << m_ptr[Node::EMPTY1] << std::endl;
			if (Data::ValidOffset(m_ptr[CHILD_PTR]))
			{
				m_child = new Node(&m_ptr[CHILD_PTR], deep + 1, this, father, 0);
				m_sizeFactor = 2.f;
				MotherCount++;
				m_count = m_child->m_count;
			}
			else
			{
				m_child = NULL;
				m_sizeFactor = 4.f;
			}
			m_rect.setSize(size());
			if (Data::ValidOffset(m_ptr[NEXT_PTR]))
			{
				m_next = new Node(&m_ptr[NEXT_PTR], deep, m_mother, father, idx + 1);
				m_count = m_next->m_count;
			}
			else
			{
				m_count = idx + 1;
				m_next = NULL;
			}

			if (Data::ValidOffset(m_ptr[DATA_PTR]))
			{
				m_data = new NodeData(this, &m_ptr[DATA_PTR]);
			}
			else
			{
				m_data = NULL;
			}
		}
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
		if (m_name.length() > 0)
			std::cout << "Node " << m_name;
		printTab();
		std::cout
			//		<<"Z" << m_ptr[EMPTY1]
				<< " Flag1:" << *((short*) &m_ptr[FLAGS])
				<< " Flag2:" << *(((short*) &m_ptr[FLAGS]) + 1)
				<< " Child:" << m_ptr[CHILD_PTR]
				<< " Next:" << m_ptr[NEXT_PTR]
				<< " Data:" << m_ptr[DATA_PTR]
				<< " Inverse:" << m_ptr[INVERSE]
		//		<< " Z" << m_ptr[EMPTY2]
		//		<< " Depth " << m_deep
				<< std::endl;
#if 1 // print transformation
		printTab();
		std::cout << "\tRotate:\t\tX" << m_floatData[ROTATEX] << "\tY" << m_floatData[ROTATEY] << "\tZ" << m_floatData[ROTATEZ] << std::endl;
		printTab();
		std::cout << "\tScale:\t\tX" << m_floatData[SIZEX] << "\tY" << m_floatData[SIZEY] << "\tZ" << m_floatData[SIZEZ] << std::endl;
		printTab();
		std::cout << "\tPosition:\tX" << m_floatData[LOCATIONX] << "\tY" << m_floatData[LOCATIONY] << "\tZ" << m_floatData[LOCATIONZ] << std::endl;
#endif
		if (false && m_ptr[DATA_PTR] != 0)
		{
			Data::strongPrint(m_ptr[DATA_PTR]);
		}
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

	void			Node::updateFamilyPos()
	{
		updatePos();
		if (m_next != NULL)
			m_next->updateFamilyPos();
	}

	void			Node::onUpdatePos()
	{
		if (m_child != NULL)
		{
			m_child->updateFamilyPos();
			if (ANode::SelectCount > 1 && m_selected)
				unselect();
		}
	}

	void			Node::x(float x)
	{
		if (m_mother != NULL)
			x -= m_mother->x();
		m_floatData[LOCATIONX] = x;
	}

	void			Node::y(float y)
	{
		if (m_mother != NULL)
			y -= m_mother->y();
		m_floatData[LOCATIONY] = y;
	}

	float			Node::x() const
	{
		float		result = m_floatData[LOCATIONX];
		if (m_mother != NULL)
			result += m_mother->x();
		return result;
	}

	float			Node::y() const
	{
		float		result = m_floatData[LOCATIONY];
		if (m_mother != NULL)
			result += m_mother->y();
		return result;
	}

	void			Node::setThickness(int v)
	{
		m_rect.setOutlineThickness(v);
	}


	void			Node::act(int *data)
	{
		if (data[0] < 0 || data[0] > COUNT)
			return;
		m_floatData[data[0]] = data[1] / 10.0;
	}
}