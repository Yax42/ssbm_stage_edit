#pragma once
#include "SFML\Graphics.hpp"
#include "Object.h"

class NodeData;
class Vertex;

class Mesh : Object
{
public:
	enum
	{
		UNKNOWN,
		NEXT,
		VERTEX,
		FLAGS,
		DISPLAY,
		WEIGHT,
	};
	static const std::vector<std::string> m_labels;
	Mesh(NodeData *father, int *ptr);
	~Mesh();
	Mesh			*m_next;
	Vertex			*m_vertex;
	NodeData		*m_father;

	virtual void			print() {}
	virtual void			display() {}

#if false
	virtual void			updatePos();
	virtual void			setThickness(int v);
	virtual void			act(int *data);
	sf::Vector2f			getMyPos(int idx, int id);
#endif
};
