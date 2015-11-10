#include "NodeData.h"
#include "Map_Node.h"
#include "Mesh.h"
#include "Data.h"

const std::vector<std::string> NodeData::m_labels =
{
	"Unknwon",
	"Next",
	"Material",
	"Mesh",
};

NodeData::NodeData(Map::Node *father, int *ptr)
	: Ptr(ptr, m_labels, "NodeData", 0, false), m_father(father)
{
	if (Data::ValidOffset(m_ptr[NEXT]))
		m_next = new NodeData(father, &m_ptr[NEXT]);
	else
		m_next = NULL;
	if (Data::ValidOffset(m_ptr[MESH]))
		m_mesh = new Mesh(this, &m_ptr[MESH]);
	else
		m_mesh = NULL;
}

NodeData::~NodeData()
{
	if (m_next != NULL)
		delete m_next;
	if (m_mesh != NULL)
		delete m_mesh;
}
