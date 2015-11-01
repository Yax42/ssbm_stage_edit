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
		ANode(int *ptr, const std::vector<std::string> &labels, const std::string &name, int id, int deep, sf::Shape *shape, bool ptrIsOk);
		virtual ~ANode();
		virtual void			printOnly() { print(); };
		virtual void			print() = 0;
		virtual void			display() = 0;
		sf::Vector2f			pos() { return sf::Vector2f(x(), y()); }
		virtual float			x() const = 0;
		virtual float			y() const = 0;
		virtual void			x(float x) = 0;
		virtual void			y(float y) = 0;
		virtual void			onUpdatePos() {}
		virtual sf::Vector2f	size() { return sf::Vector2f(m_sizeFactor * 4, m_sizeFactor * 4); }
		sf::Vector2f			scaledSize() { return size() * Scale; }
		sf::Vector2f			center();
		void					switchSelect();
		void					select();
		void					unselect();
		virtual void			setThickness(int v) = 0;
		virtual void			act(int *data) { }
		void					updatePos();

		void					setScale(float s);
		virtual bool			hide();
//GLOBAL
		static void				getMouseTarget(bool isSwitch);
		static void				selectArea(const sf::Vector2f &from, const sf::Vector2f &to, bool isSwitch, bool onlyOne);
		static void				globalDisplay();
		static void				globalUpdatePos();
		static void				clearSelections();
		static void				globalAct(int *data);
		static void				globalSetScale(float s);
		static ANode			*getOneSelected();

	public:
		float							m_sizeFactor;
		bool							m_selected;
		int								m_type;
		static std::vector<ANode *>		NodesList;
		static void						moveSelect(const sf::Vector2f &delta);
		static int						SelectCount;
		static void						clearList();
	private:
		sf::Shape				*m_shape;
		static float					Scale;


};