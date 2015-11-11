#include "Vertex.h"
#include "Mesh.h"
#include "VertexNode.h"

const std::vector<std::string> Vertex::m_labels =
{
	"Attribute",
	"Usage",
	"Format",
	"Type",
	"Scale/Stride",
	"Data",
};

Vertex::Vertex(Mesh *father, int *ptr)
	: Ptr(ptr, m_labels, "Vertex", 0, false)
{
	m_father = father;
	m_attribute = m_ptr[ATTRIBUTE];
	m_usage = m_ptr[USAGE];
	m_format = m_ptr[FORMAT];
	m_type = m_ptr[TYPE];
	m_scale = 1.f / Math::pow(2.f, Data::GetMicro<Byte>(&m_ptr[SCALE_STRIDE], 0));
	m_count = Data::GetMicro<short>(&m_ptr[SCALE_STRIDE], 1);
	if (m_attribute == Attribute::Position
		&& m_count > 0)
	{
		m_child = new VertexNode(this, Data::get<int>(m_ptr[DATA]), 0);
	}
	else
	{
		m_child = NULL;
	}
}

Vertex::~Vertex()
{
	if (m_child != NULL)
		delete m_child;
}
