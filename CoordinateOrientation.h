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
//    Describes the 2D cooridinates and rotation of objects in this 
//    demo's world.  Also serves as the base calls for the accelaration 
//    and velocity classes
//
#pragma once
class CoordinateOrientation
{
public:
	// Constructor to see all field values to 0.0
	CoordinateOrientation(void);

	// C++ formality (probably not needed for this demo) 
	~CoordinateOrientation(void);

	// The x, y and rotational components of the orientation
	// Theta is expressed in radians (not degrees)
	float x; float y; float theta;

	// Can be used for gravational field simulation, but I 
	// had to pull that out in the intrest of time
	// 
	// otherCO - the otherCO to compute the distance to 
	float magnitudeTo(CoordinateOrientation const otherCO) const;

	// I don't remember what I created this for but it was probably
	// for the gravatational field simulation that I removed
	//
	// Computes the location that is halfway between this coordinate
	// orientation and the next
	// 
	// p2 - The other coordination orientation location with which 
	// to compare this coordinate orientation
	CoordinateOrientation const midpoint(CoordinateOrientation const & p2) const;
};

