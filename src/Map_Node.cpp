#include "Map_Node.h"
#include "Map_Group.h"

namespace Map
{
	int			Node::MotherCount;
	char		*Node::Names[17] =
	{
		"",
		"",
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

	Node::Node(int *ptr, int deep, Node *mother) : ANode(deep)
	{
		m_type = NodeType::MAP;
		if (ObjectGroup::tmp < 17)
			m_name = Names[ObjectGroup::tmp];
		else
			m_name = "";
		ObjectGroup::tmp++;
		m_ptr = ptr;

		if (*ptr >= Data::m_fileSize || *ptr < 0)
		{	
			ptr = Data::Dummy;
			m_intData = Data::Dummy;
			m_floatData = (float *)Data::Dummy;
			m_mother = mother;
			m_child = NULL;
			m_sizeFactor = 0;
			m_rect.setSize(size());
			m_next = NULL;
			std::cout << "Dummy Map_Node" << std::endl;
			ObjectGroup::Dummy = true;
		}
		else
		{
			m_intData = Data::get<int>(*ptr);
			m_floatData = Data::get<float>(*ptr);
			m_mother = mother;

			m_rect.setPosition(pos());
			m_rect.setFillColor(sf::Color((MotherCount % 2)* 127, ((MotherCount / 2) % 2) * 63 + 128, (m_intData[DATA_PTR] != 0) * 255, 128));

			//std::cout << m_intData[Node::EMPTY1] << std::endl;
			if (m_intData[CHILD_PTR] != 0)
			{
				m_child = new Node(&m_intData[CHILD_PTR], deep + 1, this);
				m_sizeFactor = 2;
				MotherCount++;
			}
			else
			{
				m_child = NULL;
				m_sizeFactor = 4;
			}
			m_rect.setSize(size());
			if (m_intData[NEXT_PTR] != 0)
				m_next = new Node(&m_intData[NEXT_PTR], deep, m_mother);
			else
				m_next = NULL;
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
			std::cout << "Node " << m_name << std::endl;
		printTab();
		std::cout <<"Z" << m_intData[EMPTY1]
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
		if (false && m_intData[DATA_PTR] != 0)
		{
			Data::strongPrint(m_intData[DATA_PTR]);
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
		updatePos();
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

	void			Node::updatePos()
	{
		m_rect.setPosition(x(), y());
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
		updatePos();
	}

	void			Node::y(float y)
	{
		if (m_mother != NULL)
			y -= m_mother->y();
		m_floatData[LOCATIONY] = y;
		updatePos();
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
}