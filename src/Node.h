#pragma once

#include "SFML\Graphics.hpp"
#include "Object.h"

namespace NodeType
{
	enum
	{
		MAP = 1,
		COLL = 2,
		LINK = 4,
		ELEM = 8,
		TEST_POS = 16,
		COUNT
	};
}

class ANode : public Object
{
	public:
		ANode(int deep);
		virtual ~ANode();
		virtual void			printOnly() { print(); };
		virtual void			print() = 0;
		virtual void			display() = 0;
		sf::Vector2f			pos() { return sf::Vector2f(x(), y()); }
		virtual float			x() const = 0;
		virtual float			y() const = 0;
		virtual void			x(float x) = 0;
		virtual void			y(float y) = 0;
		virtual void			updatePos() = 0;
		virtual sf::Vector2f	size() { return sf::Vector2f(m_sizeFactor, m_sizeFactor); }
		void					switchSelect();
		void					select();
		void					unselect();
		virtual void			setThickness(int v) = 0;
		virtual void			act(int *data) { }
		virtual bool			hide();
//GLOBAL
		static ANode			*getMouseTarget();
		static void				selectArea(const sf::Vector2f &from, const sf::Vector2f &to, bool isSwitch = true);
		static void				globalDisplay();
		static void				clearSelections();
		static void				globalAct(int *data);

	public:
		float							m_sizeFactor;
		bool							m_selected;
		int								m_type;
		static std::vector<ANode *>		NodesList;
		static void						moveSelect(const sf::Vector2f &delta);
		static int						SelectCount;
};