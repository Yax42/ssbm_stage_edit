#pragma once

#include <string>
#include "Object.h"
#include "MapHead.h"
#include "GrGroundData.h"
#include "CollData.h"

class Image: public Object
{
public:
	Image(int pos, int strPos, int deep = 0) : Object(deep)
	{
		m_deepPtr = Data::get<int>(pos);
		m_ptr = Data::get<int>(*m_deepPtr);
		m_strOffset = Data::get<int>(pos, 1);
		m_str = Data::get<char>(strPos, *m_strOffset);

		if (isEqual("map_head"))
			MapHead::load(m_deepPtr);
		if (isEqual("coll_data"))
			CollData::load(m_deepPtr);
		if (isEqual("grGroundData"))
			GrGroundData::load(m_deepPtr);
	}
	~Image(){}
	virtual void	print()
	{
		printTab();
		std::cout << *m_deepPtr << "\t " << std::string(m_str) << std::endl;
	}

	virtual void	deepPrint()
	{
		//print();
		if (isEqual("map_head"))
		{
			Data::print(m_ptr, 100);

			//for (int i = 0; i < 4; i++) m_ptr[i] = 0;

			float * ptr = (float *) m_ptr;

			//for (int i = 2; i < 10; i++) ptr[i] = 1;
			//float *ptr = Data::get<float>(m_ptr[0]);
			//for (int i = 0; i < 16; i++) ptr[i] = 1;
			//Data::print(m_ptr[0], 20);

		}
		return;
		if (isEqual("map_head"))
			MapHead::print();
		if (isEqual("coll_data"))
			CollData::print();
		//Tested:
		//			-map_head
		//			-coll_data
		//			-grGroundData
		//			-map_plit			X_X
		//			-map_ptcl			X_X
		//			-itemdata			X_X
		//			-quake_model_set	X_X des valeurs int qui font plant�s si je les changes
		//			-map_texg			X_X textures
		//			-ALDYakuAll			X_X des floats cancereux et des HUGES
		//			-yakumono_param		X_X 4 pointeurs et des valeurs cheloux
	}

	virtual void	display() {}
public:
	int		*m_strOffset;
	char	*m_str;
	int		*m_ptr;
	int		*m_deepPtr;

	bool	isEqual(char *str)
	{
		return (std::string(m_str).compare(str) == 0);
	}
};