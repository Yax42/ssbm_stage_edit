#pragma once

#include "SFML\Graphics.hpp"
#include "Object.h"

namespace Map
{
	class Node : public Object
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
		Node(int *ptr, int deep);
		~Node();
		virtual void	print();
		virtual void	display();
		sf::Vector2f	pos() { return sf::Vector2f(x(), y()); }
		float			x() const { return m_floatData[LOCATIONX]; }
		float			y() const { return m_floatData[LOCATIONY]; }
		void			x(float x);
		void			y(float y);
		sf::Vector2f	size() { return sf::Vector2f(m_floatData[SIZEX] * 10, m_floatData[SIZEY] * 10); }
		Map::Node		*getMouseTarget();
		void			printOnly();

	public:
		int					*m_ptr;
		int					*m_intData;
		float				*m_floatData;
		Node				*m_child;
		Node				*m_next;
		sf::RectangleShape	m_rect;
	};
}