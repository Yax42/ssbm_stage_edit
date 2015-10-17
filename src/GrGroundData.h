#pragma once
#include "Ptr.h"

class GrGroundData : public Ptr
{
public:
/*
000	3F 80 00 00	Entire stage size
004	00 C0 00 00
008	00 0A 00 00
00C	00 00 03 20
010	00 00 04 4C
014	FF FF FF F6	?Minimum tilt & tilt scaling
018	00 00 00 00	?Horizontal rotation factor
01C	00 00 00 00	?Vertical rotation factor
020	40 00 00 00	"Fixed"-ness of camera - higher is more fixed
024	3F 80 00 00	?Character bubble buffer multiplier OR character bubble front mult.
028	3F E6 66 66	Camera speed smoothness - higher is tighter
02C	00 00 00 0A	?Unknown 1
030	00 00 05 78	?Pause minimum Z position
034	00 00 05 DC	?Pause initial Z position
038	00 00 05 DC	?Pause max Z position
03C	00 00 00 00
040	00 00 00 00	?Pause max angle up
044	00 00 00 00	?Pause max angle left
048	00 00 00 00	?Pause max angle right
04C	00 00 00 00	?Pause max angle down
050	C1 70 00 00	?Unknown 3
054	42 F0 00 00	?Unknown 4
058	43 F5 00 00	?Unknown 5
05C	41 F0 00 00	?Unknown 6
060	C1 00 00 00
064	00 00 00 00
068	00 00 00 00
06C	00 00 00 00
070	00 00 00 00
074	00 00 00 00
078	00 00 00 00
07C	00 00 00 00
080	00 00 00 00
084	00 00 00 00
088	00 00 00 00
08C	00 00 00 00
090	00 00 00 00
094	00 00 00 00
098	00 00 00 00
09C	00 00 00 00
0A0	00 00 00 00
0A4	00 00 00 00
0A8	00 00 00 00
0AC	00 00 00 00
0B0	00 01 3C 48	Pointer to ? table
0B4	00 00 00 01	Number of entries in ? table
0B8	07 00 2A FF	Bubble color - top left
0BC	07 00 2A FF	Bubble color - top middle
0C0	07 00 2A FF	Bubble color - top right
0C4	00 07 36 FF	Bubble color - sides top
0C8	00 07 36 FF	Bubble color - sides middle
0CC	00 07 36 FF	Bubble color - sides bottom
0D0	00 12 33 FF	Bubble color - bottom left
0D4	00 12 33 FF	Bubble color - bottom middle
0D8	00 12 33 FF	Bubble color - bottom right
*/
	enum
	{
		FLOAT1,
		FLAG1,
		FLAG2,
		INT1,
		INT2,
		INT3,
		ROTATION_FACTORX,
		ROTATION_FACTORY,
		FIXNESS,
		FLOAT_,
		SMOOTH,
		INT4,
		INT5,
		INT6,
		INT7,
		Z1,
		FLOAT3,
		FLOAT4,
		FLOAT5,
		Z2,
		Z3,
		FLOAT6,
		FLOAT7,
		FLOAT8,
		FLOAT9,
		Z4,
		COUNT
	};


	enum
	{
		CAM_ROTATION_FACTORX = 6,
		CAM_ROTATION_FACTORY,
		CAM_FIXNESS,
		CAM_FLOAT,
		CAM_SMOOTH,
		CAM_COUNT
	};

private:
	GrGroundData();
	~GrGroundData();
public:
	static void			load(int *ptr);
	static void			print();
	static void			display();
	static void			clean();
	static void			act();
public:
	static int									*m_ptr;
	static float								*m_floatPtr;
	static int									*m_deepPtr;
	//static std::vector<Map::ObjectGroup *>		m_groups;
};