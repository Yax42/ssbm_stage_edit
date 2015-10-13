#pragma once

#include "Node.h"

namespace Coll
{
	class LinkNode : public ANode
	{
	public:
		enum
		{
			I_LINK1,
			I_LINK2,
			I_NEGONE,
			I_FLAG,
			I_COUNT
		};
		enum
		{
			LINK1_F,
			LINK1_T,
			LINK2_F,
			LINK2_T,
			NEGONE1,
			NEGONE2,
			FLAG1,
			FLAG2,
			COUNT
		};
		enum
		{
			F1_NONE = 0,
			F1_DOWN_THROUH = 1,
			F1_GRAB = 2,
		};
		enum
		{
			F2_NONE = 0,
			F2_BOT_HARD = 1,
			F2_TOP_HARD = 2,
			F2_RIGHT_HARD = 4,
			F2_LEFT_HARD = 8,
			F2_SLIDING = 17,
		};
	public:
		LinkNode(int *ptr, int id);
		~LinkNode();
		virtual void			print();
		virtual void			display();
		virtual float			x() const;
		virtual float			y() const;
		virtual void			x(float x);
		virtual void			y(float y);
		virtual void			updatePos();
		virtual void			setThickness(int v);
		virtual void			act(int *data);
		sf::Vector2f			getNodePos(int id);
		sf::Vector2f			getLinkPos(int id);

	public:
		int			*m_ptr;
		short		*m_shortPtr;
		int			m_id;
		sf::Vertex	m_line[2];
		bool		m_correct;
		Byte		*m_flag[4];
	};



};