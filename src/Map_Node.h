#pragma once

#include "Node.h"

namespace Map
{
	class ObjectGroup;
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
	private:
		static const std::vector<std::string> m_labels;
	public:
		Node(int *ptr, int deep, Node *mother, ObjectGroup &father, int idx);
		~Node();
		virtual void	print();
		virtual void	display();
		virtual void			act(int *data);
		float			x() const;
		float			y() const;
		void			x(float x);
		void			y(float y);
		void			updateFamilyPos();
		virtual void	onUpdatePos();
		void			printOnly();
		virtual void	setThickness(int v);

	public:
		int					*m_intData;
		float				*m_floatData;
		Node				*m_child;
		Node				*m_next;
		Node				*m_mother;
		sf::RectangleShape	m_rect;
		static int			MotherCount;
		static char			*Names[17];
		std::string			m_name;
		int					m_count;
	};
}