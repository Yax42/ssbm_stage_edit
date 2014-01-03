#pragma once

#include "Node.h"

namespace Coll 
{
	class Node : public ANode
	{
	public:
		enum
		{
			LOCATIONX,
			LOCATIONY,
			COUNT
		};
	public:
		Node(int *ptr, int deep = 0);
		~Node();
		virtual void	print();
		virtual void	printOnly() { print(); };
		virtual void	display();
		float			x() const;
		float			y() const;
		void			x(float x);
		void			y(float y);
		void			updatePos();
		virtual void	setThickness(int v);
	public:
		int					*m_ptr;
		sf::CircleShape		m_rect;
		float				*m_floatData;
	};
}