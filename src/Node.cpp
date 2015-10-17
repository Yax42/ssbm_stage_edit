#include "Node.h"
#include "Window.h"
#include "MyMath.h"

std::vector<ANode *>		ANode::NodesList;
int							ANode::SelectCount = 0;
float						ANode::Scale = 1;
		
ANode::ANode(int *ptr, const std::vector<std::string> &labels, const std::string &name, int id, int deep, sf::Shape *shape, bool ptrIsOk)
	: Object(ptr, labels, name, id, deep, ptrIsOk), m_shape(shape)
{
	NodesList.push_back(this);
	m_selected = false;
}

ANode::~ANode()
{
	unselect();
	for (auto i = NodesList.begin(); i != NodesList.end() ;++i)
		if (*i == this)
		{
			NodesList.erase(i);
			break;
		}
}

void		ANode::globalDisplay()
{
	for (auto i = NodesList.begin(); i != NodesList.end() ;++i)
		if (!(*i)->hide())
			(*i)->display();
}

void		ANode::switchSelect()
{
	if (m_selected)
		unselect();
	else
		select();
}

void			ANode::getMouseTarget(bool isSwitch)
{
	sf::Vector2f	mouse = g_window.mousePos();

	for (auto i = NodesList.begin(); i != NodesList.end();++i)
	{
		sf::Vector2f	ss = (*i)->scaledSize();
		if (mouse.x > (*i)->x() && mouse.x < (*i)->x() + ss.x
			&& mouse.y >(*i)->y() && mouse.y < (*i)->y() + ss.y)
		{
			{
				if (isSwitch)
					(*i)->switchSelect();
				else
					(*i)->select();
				break;
			}
		}
	}
}

void			ANode::selectArea(const sf::Vector2f &from, const sf::Vector2f &to, bool isSwitch, bool onlyOne)
{
	for (auto i = NodesList.begin(); i != NodesList.end(); ++i)
		if (!(*i)->hide())
		{
			sf::Vector2f p = (*i)->center();
			if (from.x < p.x && to.x > p.x &&
				from.y < p.y && to.y > p.y)
			{
				if (isSwitch)
					(*i)->switchSelect();
				else
					(*i)->select();
				if (onlyOne)
					break;
			}
		}
	if (SelectCount == 1)
		for (auto i = NodesList.begin(); i != NodesList.end(); ++i)
			if ((*i)->m_selected)
				(*i)->printOnly();

}

void			ANode::clearSelections()
{
	for (auto i = NodesList.begin(); i != NodesList.end(); ++i)
		(*i)->unselect();
}

void			ANode::moveSelect(const sf::Vector2f &delta)
{
	for (auto i = NodesList.begin(); i != NodesList.end(); ++i)
		if (!(*i)->hide())
		if ((*i)->m_selected)
		{
			if ((*i)->m_type == NodeType::ELEM || (*i)->m_type == NodeType::TEST_POS)
			{
				(*i)->x(delta.x);
				(*i)->y(delta.y);
			}
			else
			{
				(*i)->x((*i)->x() + delta.x);
				(*i)->y((*i)->y() + delta.y);
			}
			(*i)->updatePos();
		}
}

void			ANode::globalAct(int *data)
{
	if (SelectCount > 1 && !g_window.m_keyPressed[sf::Keyboard::LAlt])
	{
		std::cout << "More than one selection, that could be dangerous to modifie, LAlt to force" << std::endl;
		return ;
	}
	for (auto i = NodesList.begin(); i != NodesList.end(); ++i)
		if (!(*i)->hide())
			if ((*i)->m_selected)
				(*i)->act(data);
}

void			ANode::select()
{
	if (m_selected == true)
		return ;
	setThickness(3);
	m_selected = true;
	ANode::SelectCount++;
}

bool			ANode::hide()
{
	return (g_window.m_tmp[3] > 0 && !(g_window.m_tmp[3] & m_type));
}

void			ANode::unselect()
{
	if (m_selected == false)
		return ;
	setThickness(0);
	m_selected = false;
	ANode::SelectCount--;
}

void		ANode::setScale(float s)
{
	if (m_shape != NULL)
	{
		m_shape->setScale(s, s);
		updatePos();
	}
}

void		ANode::globalSetScale(float s)
{
	Scale = s;
	for (auto i = NodesList.begin(); i != NodesList.end(); ++i)
		(*i)->setScale(s);
}

void	ANode::updatePos()
{
	if (m_shape != NULL)
	{
		m_shape->setPosition(pos() - scaledSize());
		onUpdatePos();
	}
}

void	ANode::globalUpdatePos()
{
	for (auto i = NodesList.begin(); i != NodesList.end(); ++i)
		(*i)->updatePos();
}

ANode			*ANode::getOneSelected()
{
	for (auto i = NodesList.begin(); i != NodesList.end(); ++i)
		if ((*i)->m_selected)
			return (*i);
	return NULL;
}

sf::Vector2f			ANode::center()
 {
	sf::Vector2f ss = scaledSize();
	return pos() - sf::Vector2f(ss.x * 0.5f, ss.y * 0.5f);
}
