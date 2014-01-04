#pragma once

#include "Node.h"

namespace Coll
{
	class ElemNode : public ANode
	{
	public:
		enum
		{
			FLAG1,
			FLAG2,
			FLAG3,
			FLAG4,
			EMPTY,
			X1,
			Y1,
			X2,
			Y2,
			FLAG5,
		};
	public:
		ElemNode(int *ptr, int id);
		~ElemNode();
		virtual void			print();
		virtual void			display();
		virtual float			x() const;
		virtual float			y() const;
		virtual void			x(float x);
		virtual void			y(float y);
		virtual void			updatePos();
		virtual void			setThickness(int v);
		virtual void			act(int *data);
		sf::Vector2f			getMyPos(int idx, int id);

	public:

		int					*m_ptr;
		short				*m_shortPtr;
		float				*m_floatPtr;
		int					m_id;
		sf::Vertex			line[2];
		sf::RectangleShape	m_centerShape;
		sf::RectangleShape	m_shape;
	};



};