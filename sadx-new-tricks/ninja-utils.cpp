#include "pch.h"
#include "SADXModLoader.h"
#include "ninja-utils.h"

void njTranslateX(float f)
{
	njTranslate(nullptr, f, 0, 0);
}

void njTranslateY(float f)
{
	njTranslate(nullptr, 0, f, 0);
}

void njTranslateZ(float f)
{
	njTranslate(nullptr, 0, 0, f);
}

void njRotateZXY(Rotation3* rot)
{
	njRotateZ(0, rot->z);
	njRotateX(0, rot->x);
	njRotateY(0, rot->y);
}

void njRotateYXZ(Rotation3* rot)
{
	njRotateY(0, rot->y);
	njRotateX(0, rot->x);
	njRotateZ(0, rot->z);
}

void njScaleX(float f)
{
	njScale(nullptr, f, 1.0f, 1.0f);
}

void njScaleY(float f)
{
	njScale(nullptr, 1.0f, f, 1.0f);
}

void njScaleZ(float f)
{
	njScale(nullptr, 1.0f, 1.0f, f);
}

void njScalef(float f)
{
	njScale(nullptr, f, f, f);
}

void njRotateX_(Angle x)
{
	if (x)
	{
		njRotateX(_nj_current_matrix_ptr_, x);
	}
}

void njRotateY_(Angle y)
{
	if (y)
	{
		njRotateY(_nj_current_matrix_ptr_, y);
	}
}

void njRotateZ_(Angle z)
{
	if (z)
	{
		njRotateZ(_nj_current_matrix_ptr_, z);
	}
}

void njLookAt(NJS_VECTOR* from, NJS_VECTOR* to, Angle* outx, Angle* outy)
{
	NJS_VECTOR unit = *to;

	njSubVector(&unit, from);

	if (outy)
	{
		*outy = static_cast<Angle>(atan2f(unit.x, unit.z) * 65536.0f * 0.1591549762031479f);
	}

	if (outx)
	{
		if (from->y == to->y)
		{
			*outx = 0;
		}
		else
		{
			Float len = 1.0f / squareroot(unit.z * unit.z + unit.x * unit.x + unit.y * unit.y);

			*outx = static_cast<Angle>((acos(len * 3.3499999f) * 65536.0f * 0.1591549762031479f)
				- (acos(-(len * unit.y)) * 65536.0f * 0.1591549762031479f));
		}
	}
}
