#include "Vertex.h"
#include "Mesh.h"

const std::vector<std::string> Vertex::m_labels =
{
	"Ident",
	"Usage",
	"Formatx",
	"Type",
	"Scale",
	"Unknown",
	"Stride",
	"Data",
};

Vertex::Vertex(Mesh *father, int *ptr)
	: Ptr(ptr, m_labels, "Vertex", 0, false)
{
}

Vertex::~Vertex()
{
}
