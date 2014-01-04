#include "Node.h"
#include "Window.h"
#include "MyMath.h"

std::vector<ANode *>		ANode::NodesList;
int							ANode::SelectCount = 0;
		
ANode::ANode(int deep) : Object(deep)
{
	NodesList.push_back(this);
	m_selected = false;
}

ANode::~ANode()
{
	unselect();
	for (std::vector<ANode *>::iterator i = NodesList.begin(); i != NodesList.end() ;++i)
		if (*i == this)
		{
			NodesList.erase(i);
			break;
		}
}

void		ANode::globalDisplay()
{
	for (std::vector<ANode *>::iterator i = NodesList.begin(); i != NodesList.end() ;++i)
		if (g_window.m_tmp[3] < 1 || (g_window.m_tmp[3] & (*i)->m_type))
			(*i)->display();
}

void		ANode::switchSelect()
{
	if (m_selected)
		unselect();
	else
		select();
}

ANode			*ANode::getMouseTarget()
{
	sf::Vector2f	mouse = g_window.mousePos();

	for (std::vector<ANode *>::iterator i = NodesList.begin(); i != NodesList.end() ;++i)
		if (mouse.x > (*i)->x() && mouse.x < (*i)->x() + (*i)->size().x
			&& mouse.y > (*i)->y() && mouse.y < (*i)->y() + (*i)->size().y)
			return (*i);
	return NULL;
}

void			ANode::selectArea(const sf::Vector2f &from, const sf::Vector2f &to, bool isSwitch)
{
	sf::Vector2f	tl(Math::smallest(from.x, to.x), Math::smallest(from.y, to.y));
	sf::Vector2f	br(Math::biggest(from.x, to.x), Math::biggest(from.y, to.y));
	
	for (std::vector<ANode *>::iterator i = NodesList.begin(); i != NodesList.end(); ++i)
		if (g_window.m_tmp[3] < 1 || (g_window.m_tmp[3] & (*i)->m_type))
		if (tl.x < (*i)->x() + (*i)->size().x && br.x > (*i)->x()
			&& tl.y < (*i)->y() + (*i)->size().y && br.y > (*i)->y())
			{
				if (isSwitch)
					(*i)->switchSelect();
				else
					(*i)->select();
				if (from.x == to.x && from.y == to.y)
					break;
			}
	if (SelectCount == 1)
		for (std::vector<ANode *>::iterator i = NodesList.begin(); i != NodesList.end(); ++i)
			if ((*i)->m_selected)
				(*i)->printOnly();

}

void			ANode::clearSelections()
{
	for (std::vector<ANode *>::iterator i = NodesList.begin(); i != NodesList.end(); ++i)
		(*i)->unselect();
}

void			ANode::moveSelect(const sf::Vector2f &delta)
{
	for (std::vector<ANode *>::iterator i = NodesList.begin(); i != NodesList.end(); ++i)
		if (g_window.m_tmp[3] < 1 || (g_window.m_tmp[3] & (*i)->m_type))
		if ((*i)->m_selected)
		{
			if ((*i)->m_type == NodeType::ELEM)
			{
				(*i)->x(delta.x);
				(*i)->y(delta.y);
			}
			else
			{
				(*i)->x((*i)->x() + delta.x);
				(*i)->y((*i)->y() + delta.y);
			}
		}
}

void			ANode::globalAct(int *data)
{
	if (SelectCount > 1)
	{
		std::cout << "More than one selection, that could be dangerous to modifie" << std::endl;
		return ;
	}
	for (std::vector<ANode *>::iterator i = NodesList.begin(); i != NodesList.end(); ++i)
		if (g_window.m_tmp[3] < 1 || (g_window.m_tmp[3] & (*i)->m_type))
			if ((*i)->m_selected)
				(*i)->act(data);
}

void			ANode::select()
{
	if (m_selected == true)
		return ;
	setThickness(1);
	m_selected = true;
	ANode::SelectCount++;
}

void			ANode::unselect()
{
	if (m_selected == false)
		return ;
	setThickness(0);
	m_selected = false;
	ANode::SelectCount--;
}