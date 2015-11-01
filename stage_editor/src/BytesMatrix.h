#pragma once
#include "Search.h"
#include "ByteButton.h"

class BytesMatrix
{
private:
	enum EButton
	{
		CUROSOR,
		VALUE,
		NAME,
		MOTHER,
		OFFSET,
		SEARCH,
		OTHER,
		COUNT,
	};
private:
	static sf::Vector2f			ButtonsRects[COUNT][2];
	void		privateInit(Search &search);
	BytesMatrix(int *firstPtr, float buttonSize, Search &search);
	BytesMatrix(int pos, float buttonSize, Search &search);
public:
	static void	init(int *firstPtr, float buttonSize, Search &search);
	static void	init(int pos, float buttonSize, Search &search);
	void		display();
	void		process(sf::Vector2f &mouse, bool leftMouse, bool rightMouse, bool ctrl);
	void		numberPressed(int v);
	~BytesMatrix() {}
public:
	std::vector<ByteButton>		m_bytesButtons;
	std::vector<Button>			m_buttons;
	int							m_position;
	float						m_buttonSize;
	int							*m_ptr;
	int							*m_current;
	int							*Next;
	std::string					m_number;

public:
	static BytesMatrix			*Instance;
};