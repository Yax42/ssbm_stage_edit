#pragma once
#include "VertexNode.h"
#include "Window.h"
#include "Vertex.h"
#include "Mesh.h"
#include "NodeData.h"
#include "Map_Node.h"

VertexNode	*VertexNode::FirstBuilt = NULL;
const std::vector<std::string> VertexNode::m_labels =
{
	"Value",
};

VertexNode::VertexNode(Vertex *father, int *ptr, int id, int offset)
	: ANode(ptr, m_labels, "VertexNode", id, 0, &m_rect, true), m_father(father)
{
	if (id == 0)
		FirstBuilt = this;
	m_offset = offset;
	m_sizeFactor = 0.7f;
	m_rect.setPosition(pos());
	m_rect.setFillColor(sf::Color(255, 255, 255, 255));
	m_rect.setRadius(size().x);
	if (id + 1 < m_father->m_count)
	{
		m_brotherMustBeSaved = false;
		m_brother = new VertexNode(father, m_ptr + 1, id + 1, !m_offset);
	}
	else
	{
		m_brotherMustBeSaved = true;
		m_brother = FirstBuilt;
	}
}

VertexNode::~VertexNode()
{
	if (!m_brotherMustBeSaved)
		delete m_brother;
}

void		VertexNode::print()
{
}

void		VertexNode::display()
{
	m_rect.setPosition(pos());
	g_window.draw(m_rect);
}

void			VertexNode::x(float x)
{
	float deltax = m_father->m_father->m_father->m_father->x();
	x -= deltax;
	if (m_father->m_scale != 0)
		x /= m_father->m_scale;
	Data::SetMicro<short>(m_ptr, m_offset, x);
}

void			VertexNode::y(float y)
{
	float deltay = m_father->m_father->m_father->m_father->y();
	y -= deltay;
	if (m_father->m_scale != 0)
		y /= m_father->m_scale;
	Data::SetMicro<short>(m_ptr + m_offset, !m_offset, y);
}

float			VertexNode::x() const
{
	float deltax = m_father->m_father->m_father->m_father->x();
	float x = Data::GetMicro<short>(m_ptr, m_offset);
	return x * m_father->m_scale + deltax;
}

float			VertexNode::y() const
{
	float deltay = m_father->m_father->m_father->m_father->y();
	float y = Data::GetMicro<short>(m_ptr + m_offset, !m_offset);
	return y * m_father->m_scale + deltay;
}

void			VertexNode::setThickness(int v)
{
	m_rect.setOutlineThickness(v);
}

void			VertexNode::act(int *data)
{
}
