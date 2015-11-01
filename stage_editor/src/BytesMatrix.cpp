#include "BytesMatrix.h"
#include "Worker.h"
#include "Data.h"
#include "Window.h"
#include "Ptr.h"
#include "SFML\Graphics.hpp"

BytesMatrix			*BytesMatrix::Instance = NULL;

void	BytesMatrix::init(int *firstPtr, float buttonSize, Search &search)
{
	if (Instance != NULL)
		delete Instance;
	Instance = new BytesMatrix(firstPtr, buttonSize, search);
}

void	BytesMatrix::init(int pos, float buttonSize, Search &search)
{
	if (Instance != NULL)
		delete Instance;
	Instance = new BytesMatrix(pos, buttonSize, search);
}

sf::Vector2f			BytesMatrix::ButtonsRects[COUNT][2] =
{
	{ sf::Vector2f(0, 0), sf::Vector2f(0, 0)},
	{ sf::Vector2f(0, 940), sf::Vector2f(333, 970)},
	{ sf::Vector2f(0, 970), sf::Vector2f(333, 1000)},
	{ sf::Vector2f(333, 970), sf::Vector2f(666, 1000)},
	{ sf::Vector2f(666, 970), sf::Vector2f(1000, 1000)},
	{ sf::Vector2f(333, 940), sf::Vector2f(666, 970)},
	{ sf::Vector2f(666, 940), sf::Vector2f(1000, 970)}
};

BytesMatrix::BytesMatrix(int *firstPtr, float buttonSize, Search &search)
{
	int firstIdx = 0;
	m_position = Data::getId(firstPtr);
	m_buttonSize = buttonSize;
	m_ptr = firstPtr;
	privateInit(search);
}

BytesMatrix::BytesMatrix(int pos, float buttonSize, Search &search)
{
	if (pos + 20 > Data::m_fileSize)
		pos = Data::m_fileSize - 20;
	if (pos < 0) pos = 0;
	m_buttonSize = buttonSize;
	m_position = pos;
	int firstId = pos;//(intPos) / (Window::width / (buttonSize + 1));
	firstId = (firstId / 4) * 4;
	m_ptr = Data::get<int>(firstId);
	privateInit(search);
}

void		BytesMatrix::privateInit(Search &search)
{
	m_current = m_ptr;
	Next = NULL;
	m_number = "0";
	int *ptr = m_ptr;
	int idx = 0;
	for (int i = 0; i < Ptr::List.size(); i++)
	{
		if (Ptr::List[i]->m_ptr >= ptr)
		{
			break;
		}
		idx = i;
	}
	float x = 0;
	float y = 0;
	int idxIdx;
	float cursor = ((float) Data::getId(ptr)) / ((float) Data::m_fileSize);
	if (idx < Ptr::List.size())
	{
		idxIdx = ptr - Ptr::List[idx]->m_ptr;
		assert(Ptr::List[idx]->m_ptr + idxIdx == ptr);
	}
	int imageOrigin = 0;

	bool black = false;

	int *imagePtr = Data::get<int>(*Worker::m_images[imageOrigin]->m_ptr);
	while (y < 1000 && Data::getId(ptr) < Data::m_fileSize)
	{
		std::string name = "";
		while (Worker::m_images.size() - 1 > imageOrigin && imagePtr <= ptr)
		{
			std::string str = Worker::m_images[imageOrigin]->m_str;
			if (str.length() >= 5)
			{
					black = (0 == str.compare(str.length() - 5, 5, "image"));
			}
			else
			{
				black = false;
			}
			imageOrigin++;
			imagePtr = Data::get<int>(*Worker::m_images[imageOrigin]->m_ptr);
		}
		bool isKnown = false;
		if (idx < Ptr::List.size())
		{
			while (idxIdx >= 0 && idxIdx >= Ptr::List[idx]->Labels.size())
			{
				idx++;
				if (idx >= Ptr::List.size())
				{
					idxIdx = -1;
					break;
				}

				idxIdx = ptr - Ptr::List[idx]->m_ptr;
			}
			if (idxIdx >= 0)
			{
				name = Ptr::List[idx]->Labels[idxIdx];
				assert(Ptr::List[idx]->m_ptr + idxIdx == ptr);
				isKnown = true;
			}
		}

		m_bytesButtons.emplace_back(
			isKnown ? Ptr::List[idx] : NULL
			, ptr
			, name
			, m_buttonSize
			, sf::Vector2f(x, y)
			, black ? sf::Color::Black
				: isKnown ? idxIdx == 0 ?
				sf::Color(255, 128, 128)
				: sf::Color(128, 255, 255)
				: sf::Color(255, 255, 128)
			, sf::Color(255 * (imageOrigin % 3 == 0), 255 * (imageOrigin % 3 == 1), 255 * (imageOrigin % 3 == 2))
			, m_buttonSize < 20
			);

		idxIdx++;
		ptr++;
		x += m_buttonSize + 1;
		if (x + m_buttonSize > 1000)
		{
			x = 0;
			y += m_buttonSize + 10;
		}
	}
	{
		sf::Vector2f ref = sf::Vector2f(1000, cursor * 1000);
		sf::Vector2f from = sf::Vector2f(-20, -60) + ref;
		sf::Vector2f to = sf::Vector2f(0, 60) + ref;
		m_buttons.emplace_back(
			""
			, from
			, to
			, sf::Color::White
			, 2
			, sf::Color::Black
			);
	}

	for (int i = 1; i < COUNT; i++)
	{
		m_buttons.emplace_back(
			""
			, ButtonsRects[i][0]
			, ButtonsRects[i][1]
			, sf::Color::White
			, 2
			, sf::Color::Black
			);
	}
	m_buttons[VALUE].setText("0");
	m_buttons[SEARCH].setText(search.Found ? std::to_string(search.m_i + 1) + " / " + std::to_string(search.Count) : "-");
}


void		BytesMatrix::display()
{
	for (auto it = m_bytesButtons.begin(); it != m_bytesButtons.end(); ++it)
		(*it).displayBoxes();
	for (auto it = m_bytesButtons.begin(); it != m_bytesButtons.end(); ++it)
		(*it).displayTexts();
	for (auto it = m_buttons.begin(); it != m_buttons.end(); ++it)
		(*it).displayBox();
	for (auto it = m_buttons.begin(); it != m_buttons.end(); ++it)
		(*it).displayText();

}

void		BytesMatrix::process(sf::Vector2f &mouse, bool leftMouse, bool rightMouse, bool ctrl)
{
	for (auto it = m_bytesButtons.begin(); it != m_bytesButtons.end(); ++it)
	{
		if ((*it).process(mouse, leftMouse, rightMouse, ctrl))
		{
			m_current = (*it).m_ptr;
			std::string name = "Header";
			for (int i = 0; i < Worker::m_images.size(); i++)
			{
				if (Data::get<int>(*Worker::m_images[i]->m_ptr) <= (*it).m_ptr)
				{
					name = Worker::m_images[i]->m_str;
				}
				else
					break;
			}
			m_buttons[MOTHER].setText("[" + name + "]");
			m_buttons[OFFSET].setText("#" + std::to_string(Data::getId((*it).m_ptr)));
			m_buttons[NAME].setText((*it).m_ptrObject != NULL ? (*it).m_ptrObject->Name + " #" + std::to_string((*it).m_ptrObject->m_id) : "");
			if ((*it).Jump)
			{
				Next = Data::get<int>(*(*it).m_ptr);
				break;
			}
		}
	}
}
void		BytesMatrix::numberPressed(int v)
{
	if (v == -3)
	{
		if (m_number[0] == '-')
			m_number.erase(0, 1);
		else
			m_number = "-" + m_number;
	}
	else if (v == -2)
		m_number = "0";
	else if ((v == 0 && m_number == "0") || v == -1)
		m_number += ".";
	else
	{
		if (m_number == "0")
			m_number = "";
		m_number += std::to_string(v);
	}
	try
	{
		ByteButton::IntValue = std::stoi(m_number);
	}
	catch (...)
	{}
	try
	{
		ByteButton::FloatValue = std::stof(m_number);
	}
	catch (...)
	{}
	m_buttons[VALUE].setText(m_number);
}
