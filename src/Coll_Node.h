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
	private:
		static const std::vector<std::string> m_labels;
	public:
		Node(int *ptr, int id, int deep = 0);
		~Node();
		virtual void	print();
		virtual void	display();
		float			x() const;
		float			y() const;
		void			x(float x);
		void			y(float y);
		void			printFlag(int idx);
		virtual void	setThickness(int v);
		virtual void	act(int *data);

	public:
		sf::CircleShape		m_rect;
		float				*m_position;
		//int					*m_flags;
	};
}