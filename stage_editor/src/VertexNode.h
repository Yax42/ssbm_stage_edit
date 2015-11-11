#pragma once
#include "Node.h"

class Vertex;

class VertexNode : ANode
{
public:
	static const std::vector<std::string> m_labels;
	VertexNode(Vertex *father, int *ptr, int id, int offset = 0);
	~VertexNode();

	virtual void	print();
	virtual void	display();
	float			x() const;
	float			y() const;
	void			x(float x);
	void			y(float y);
	void			printFlag(int idx);
	virtual void	setThickness(int v);
	virtual void	act(int *data);

	bool				m_brotherMustBeSaved;
	Vertex				*m_father;
	VertexNode			*m_brother;
	sf::CircleShape		m_rect;
	int					m_offset;
	static VertexNode	*FirstBuilt;
};