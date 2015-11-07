#pragma once
#include "Ptr.h"

class Mesh;
namespace Map
{
	class Node;
}

class NodeData : Ptr
{
public:
	static const std::vector<std::string> m_labels;
	NodeData(Map::Node *father, int *ptr);
	~NodeData();
	enum
	{
		UNKNWON,
		NEXT,
		MATERIAL,
		MESH,
	};

public:
	Mesh			*m_mesh;
	NodeData		*m_next;
	Map::Node		*m_father;
};