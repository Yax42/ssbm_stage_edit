#pragma once

#include "SFML\Graphics.hpp"
#include "Object.h"
#include "Window.h"
#include "Map_Node.h"

namespace Map
{
	class ObjectGroup : public Object
	{
	private:
		static const std::vector<std::string> m_labels;
	public:
		static bool		Dummy;
	public:
		ObjectGroup(int *ptr, int *id, int deep = 0);
		~ObjectGroup();
		virtual void	print();
		virtual void	display();
		
	public:
		int		*m_ptrToHeadNode;
		int		*m_unknown1;
		int		*m_unknown2;
		Node	*m_node;
		int		m_count;
	};
};
