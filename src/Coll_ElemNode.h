#pragma once

#include "Node.h"

namespace Coll
{
	class ElemNode : public ANode
	{
	public:
		enum
		{
			//les flags ont sont deux short : le nombre de ligne orientee dans le sens donne, et l'id du link ligne "maitresse" (?) de l'orientation.
			// Il faut faire attention de mettre les ligne maitre a cote je crois. (un ttruc comme ca)
			FLAG1, //TOP
			FLAG2, // BOT
			FLAG3, // RIGHT
			FLAG4, //LEFT
			EMPTY,
			X1,
			Y1,
			X2,
			Y2,
			FLAG5,
		};
	private:
		static const std::vector<std::string> m_labels;
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

		short				*m_shortPtr;
		float				*m_floatPtr;
		sf::Vertex			line[2];
		sf::RectangleShape	m_centerShape;
		sf::RectangleShape	m_shape;
	};



};