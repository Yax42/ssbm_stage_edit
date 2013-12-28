#pragma once

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
		void		print();

	public:
		int			*m_ptr;
		int			*m_intData;
		float			*m_floatData;
		Node		*m_child;
		Node		*m_next;
	};

	class ObjectGroup : public Object
	{
	public:
		ObjectGroup(int *ptr, int *id, int deep = 0);
		~ObjectGroup();
		void	print();
		
	public:
		int		*m_ptr;
		int		*m_id;
		int		*m_ptrToHeadNode;
		int		*m_unknown1;
		int		*m_unknown2;
		Node	*m_node;
	};
};

class MapHead
{
private:
	MapHead();
	~MapHead();
public:
	static void		load(int *ptr);
	static void		print();
	static void		clean();
public:
	static int									*m_ptr;
	static std::vector<Map::ObjectGroup *>		m_groups;
};