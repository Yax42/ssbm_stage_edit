#pragma once

#include "Node.h"

namespace Map
{
	class Node : public ANode
	{
	public:
		enum
		{
			EMPTY1,
			FLAGS,
			CHILD_PTR,
			NEXT_PTR,
			DATA_PTR,
			ROTATEX,
			ROTATEY,
			ROTATEZ,
			SIZEX,
			SIZEY,
			SIZEZ,
			LOCATIONX,
			LOCATIONY,
			LOCATIONZ,
			INVERSE,
			EMPTY2,
			COUNT
		};
	public:
		Node(int *ptr, int deep, Node *mother);
		~Node();
		virtual void	print();
		virtual void	display();
		float			x() const;
		float			y() const;
		void			x(float x);
		void			y(float y);
		void			updateFamilyPos();
		void			updatePos();
		sf::Vector2f	size() { return sf::Vector2f(/*m_floatData[SIZEX]*/1 * m_sizeFactor, /*m_floatData[SIZEY]*/1 * m_sizeFactor); }
		void			printOnly();
		virtual void	setThickness(int v);

	public:
		int					*m_ptr;
		int					*m_intData;
		float				*m_floatData;
		Node				*m_child;
		Node				*m_next;
		Node				*m_mother;
		sf::RectangleShape	m_rect;
		static int			MotherCount;
		static char			*Names[17];
		std::string			m_name;
	};
}