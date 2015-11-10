#include "Mesh.h"
#include "Vertex.h"

const std::vector<std::string> Mesh::m_labels =
{
	"Unknown",
	"Next",
	"Vertex",
	"Flags",
	"Display",
	"Weight",
};

Mesh::Mesh(NodeData *father, int *ptr) : Object(ptr, m_labels, "Mesh", 0, 0, false), m_father(father), m_next(NULL), m_vertex(NULL)
{
	if (Data::ValidOffset(m_ptr[NEXT]))
		m_next = new Mesh(m_father, &m_ptr[NEXT]);
	if (Data::ValidOffset(m_ptr[VERTEX]))
		m_vertex = new Vertex(this, &m_ptr[VERTEX]);
}

Mesh::~Mesh()
{
	if (m_next != NULL)
		delete m_next;
	if (m_vertex != NULL)
		delete m_vertex;
}
