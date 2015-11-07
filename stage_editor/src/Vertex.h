#pragma once
#include "Node.h"
#include "Mesh.h"

class Vertex : Ptr
{
public:
	enum
	{
		IDENT,
		USAGE,
		FORMATX,
		TYPE,
		SCALE,
		UNKNOWN,
		STRIDE,
		DATA,
	};
	static const std::vector<std::string> m_labels;
	Vertex(Mesh *father, int *ptr);
	~Vertex();

#if false
	virtual void			print();
	virtual void			display();

	virtual void			updatePos();
	virtual void			setThickness(int v);
	virtual void			act(int *data);
	sf::Vector2f			getMyPos(int idx, int id);
#endif
};