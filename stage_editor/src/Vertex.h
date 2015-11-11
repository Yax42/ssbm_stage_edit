#pragma once
#include "Node.h"
#include "Mesh.h"

class VertexNode;

class Vertex : Ptr
{
public:
	enum
	{
		ATTRIBUTE,
		USAGE,
		FORMAT,
		TYPE,
		SCALE_STRIDE,
		DATA,
	};
	static const std::vector<std::string> m_labels;
	Vertex(Mesh *father, int *ptr);
	~Vertex();
public:
	Mesh				*m_father;
	int					m_attribute;
	int					m_usage;
	int					m_format;
	int					m_type;
	float				m_scale;
	int					m_count;
	VertexNode			*m_child;

#if false
	virtual void			print();
	virtual void			display();

	virtual void			updatePos();
	virtual void			setThickness(int v);
	virtual void			act(int *data);
	sf::Vector2f			getMyPos(int idx, int id);
#endif
public:
	enum Attribute
	{
		PNMtxIdx,
		Tex0MtxIdx,
		Tex1MtxIdx,
		Tex2MtxIdx,
		Tex3MtxIdx,
		Tex4MtxIdx,
		Tex5MtxIdx,
		Tex6MtxIdx,
		Tex7MtxIdx,
		Position,
		Normal,
		Color0,
		Color1,
		Tex0,
		Tex1,
		Tex2,
		Tex3,
		Tex4,
		Tex5,
		Tex6,
		Tex7,
		PosMtxArray,
		NrmMtxArray,
		TexMtxArray,
		LightArray,
		NBT,
		MaxAttr,
		Null = 0xff,
	};
	enum Usage
	{
		None,
		Direct,
		Index8,
		Index16
	};
	enum Format
	{
		Default,
		Pos_XY,
		Pos_XYZ,
		Normal_XYZ,
		Normal_NBT,
		Normal_NBT3,
		Color_RGB,
		Color_RGBA,
		Tex_S,
		Tex_ST,
	};
	enum Type
	{
		U8,
		S8,
		U16,
		S16,
		F32,
		RGB565,
		RGB8,
		RGBX8,
		RGBA4,
		RGBA6,
		RGBA8,
	};
};