#include "ByteButton.h"

float	ByteButton::FloatValue = 0;
int		ByteButton::IntValue = 0;

ByteButton::ByteButton(Ptr *ptrObject, int *ptr, const std::string &name, float size, sf::Vector2f &pos, const sf::Color &color, const sf::Color &outLineColor, bool empty)
	: m_ptrObject(ptrObject), m_buttons {
	  Button(std::to_string(*((float *) ptr)), pos,											pos + sf::Vector2f(1,		0.2f) * size, color, size * 0.01f, outLineColor)
	, Button(std::to_string(*ptr),				pos + sf::Vector2f(0,		0.2f) * size,	pos + sf::Vector2f(1,		0.4f) * size, color, size * 0.01f, outLineColor)
	, Button(std::to_string(((short*)ptr)[0]),	pos + sf::Vector2f(0.5f,	0.4f) * size,	pos + sf::Vector2f(1.f,	0.6f) * size, color, size * 0.01f, outLineColor)
	, Button(std::to_string(((short*)ptr)[1]),	pos + sf::Vector2f(0,	 	0.4f) * size,	pos + sf::Vector2f(0.5f,	0.6f) * size, color, size * 0.01f, outLineColor)
	, Button(std::to_string(((Byte*)ptr)[0]),	pos + sf::Vector2f(0.75f,	0.6f) * size,	pos + sf::Vector2f(1.f,		0.8f) * size, color, size * 0.01f, outLineColor)
	, Button(std::to_string(((Byte*)ptr)[1]),	pos + sf::Vector2f(0.5f,	0.6f) * size,	pos + sf::Vector2f(0.75f,	0.8f) * size, color, size * 0.01f, outLineColor)
	, Button(std::to_string(((Byte*)ptr)[2]),	pos + sf::Vector2f(0.25f,	0.6f) * size,	pos + sf::Vector2f(0.5f,	0.8f) * size, color, size * 0.01f, outLineColor)
	, Button(std::to_string(((Byte*)ptr)[3]),	pos + sf::Vector2f(0.f,		0.6f) * size,	pos + sf::Vector2f(0.25f,	0.8f) * size, color, size * 0.01f, outLineColor)
	, Button(name,								pos + sf::Vector2f(0,		0.8f) * size,	pos + sf::Vector2f(1,		1) * size, color, size *	0.01f, outLineColor)
	}
{
	Jump = false;
	m_color = color;
	Empty = empty;
	if (Empty)
		m_buttons[NAME].setOrigin(pos, pos + sf::Vector2f(size, size));
	m_ptr = ptr;
	updateJump();
}

void	ByteButton::updateJump()
{
	bool isInside = Data::isInside(*m_ptr + 4) && *m_ptr > 100;
	m_buttons[NAME].Enabled = isInside;
	m_buttons[NAME].setColor(isInside ? sf::Color::Green : m_color);

}

void	ByteButton::displayTexts()
{
	if (!Empty)
		for (int i = 0; i < COUNT; i++)
			m_buttons[i].displayText();
}

void	ByteButton::displayBoxes()
{
	if (Empty)
	{
		m_buttons[NAME].displayBox();
	}
	else
	{
		for (int i = 0; i < COUNT; i++)
			m_buttons[i].displayBox();
	}
}

bool	ByteButton::process(sf::Vector2f &mouse, bool leftMouse, bool rightMouse, bool ctrl)
{
	if (Empty)
		return false;
	bool modif = false;
	bool isOn = false;
	if (m_buttons[FLOAT].coloredContains(mouse))
	{
		isOn = true;
		if (rightMouse)
		{
			*((float *)m_ptr) = FloatValue;
			modif = true;
		}
	}
	if (m_buttons[INT].coloredContains(mouse))
	{
		isOn = true;
		if (rightMouse)
		{
			*m_ptr = IntValue;
			modif = true;
		}
	}
	if (m_buttons[SHORT0].coloredContains(mouse))
	{
		isOn = true;
		if (rightMouse)
		{
			((short *)m_ptr)[0] = IntValue;
			modif = true;

		}
	}
	if (m_buttons[SHORT1].coloredContains(mouse))
	{
		isOn = true;
		if (rightMouse)
		{
			((short *)m_ptr)[1] = IntValue;
			modif = true;
		}
	}
	for (int i = 0; i < 4; i++)
	{
		if (m_buttons[BYTE0 + i].coloredContains(mouse))
		{
			isOn = true;
			if (rightMouse)
			{
				((Byte *)m_ptr)[i] = IntValue;
				modif = true;
			}
		}
	}
	if (m_buttons[NAME].coloredContains(mouse))
	{
		isOn = true;
		if (leftMouse && ctrl)
		{
			Jump = true;
		}
	}
	if (modif)
	{
		m_buttons[SHORT0].setText(std::to_string(((short*)m_ptr)[0]));
		m_buttons[SHORT1].setText(std::to_string(((short*)m_ptr)[1]));
		m_buttons[INT].setText(std::to_string(*m_ptr));
		m_buttons[FLOAT].setText(std::to_string(*((float *)m_ptr)));
		for (int i = 0; i < 4; i++)
		{
			m_buttons[BYTE0 + i].setText(std::to_string(((Byte *)m_ptr)[i]));
		}
		updateJump();
	}
	return isOn;
}
