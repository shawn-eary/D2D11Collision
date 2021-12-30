// Copyright 2012 - Shawn Eary
// http://www.conservativeart.net/Home/About
// Licenced via Microsoft Public License (MS-LPL) 
// http://msdn.microsoft.com/en-us/library/ff649456.aspx
// 
// Disclaimers: 
// 1) Shawn Eary does not work for Microsoft.  He simply chooses to 
//    use the MS-LPL to license this example. 
// 2) Warning!! - This is hobby code and it has not been throughly 
//    tested.  It could accidentally do things it should not.  While 
//    the author Shawn Eary is a professional *.NET programmer, you 
//    should be advised that at the time this program was released
//    he did not have any professional experience with Direct2D.
//    Furthermore, this example was slapped together in a big hurry
//    because of limited personal time. 
//
// File Purpose:
//    Implements the 2D cooridinates and rotation of objects in this 
//    demo's world.  Also serves as the base calls for the accelaration 
//    and velocity classes
//
#include "CoordinateOrientation.h"
#include <math.h>

// See header file for description of this method
CoordinateOrientation::CoordinateOrientation(void)
: x(0.0f)
, y(0.0f)
, theta(0.0f)
{
}

// See header file for description of this method
CoordinateOrientation::~CoordinateOrientation(void)
{
}

// See header file for description of this method
float CoordinateOrientation::magnitudeTo(CoordinateOrientation const otherCO) const
{
	float const distX = this->x - otherCO.x; 
	float const distY = this->y - otherCO.y; 
	return sqrt(distX * distX + distY * distY);
}

// See header file for description of this method
CoordinateOrientation const CoordinateOrientation::midpoint(CoordinateOrientation const & p2) const
{	
	CoordinateOrientation co; 
	co.x = (this->x + p2.x) / 2.0f;
	co.y = (this->y + p2.y) / 2.0f; 
	return co;
}
