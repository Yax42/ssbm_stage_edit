#pragma once
#include "Object.h"
#include "Button.h"

class ByteButton
{
private:
	enum
	{
		FLOAT,
		INT,
		SHORT0,
		SHORT1,
		BYTE0,
		BYTE1,
		BYTE2,
		BYTE3,
		NAME,
		COUNT,
	};
public:
	static float			FloatValue;
	static int				IntValue;
public:
	ByteButton(Ptr *ptrObject, int *ptr, const std::string &name, float size, sf::Vector2f &pos, const sf::Color &color, const sf::Color &outLineColor, bool empty = false);
	~ByteButton() {}
	bool process(sf::Vector2f &mouse, bool leftMouse, bool rightMouse, bool ctrl);
	void displayBoxes();
	void displayTexts();
private:
	void		updateJump();
	bool		containsAndClick();
private:
	Button		m_buttons[COUNT];
	sf::Color	m_color;
	bool		m_modif;
	bool		m_isOn;
	bool		m_rightclick;
public:
	bool					Jump;
	int						*m_ptr;
	bool					Empty;
	Ptr						*m_ptrObject;
};
