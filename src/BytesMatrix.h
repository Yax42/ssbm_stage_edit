#pragma once
#include "ByteButton.h"

class BytesMatrix
{
public:
	BytesMatrix(int *firstPtr, float buttonSize);
	BytesMatrix(int pos, float buttonSize);
	void		generateMatrix(int *ptr);
	void		display();
	void		process(sf::Vector2f &mouse, bool leftMouse, bool rightMouse, bool ctrl);
	void		numberPressed(int v);
	~BytesMatrix() {}
public:
	std::vector<ByteButton>		m_bytesButtons;
	std::vector<Button>			m_buttons;
	int							m_position;
	float						m_buttonSize;
	int							*Next;
	std::string					m_number;

public:
	static BytesMatrix			*Instance;
};