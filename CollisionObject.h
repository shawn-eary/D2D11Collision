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
//    Specification for a Collision Object (CO). COs contain the 
//    velocity, position and geometry information for the objects
//    that bounce around in this demo
#pragma once

#include "CoordinateOrientation.h"
#include <wrl.h>
#include <d2d1_1.h>
#include <list>
#include <map>

using namespace std;

// This demo allows for Collision Objects to be of two types Square and 
// Hexagon. The values in this enumeration should be contiguous 
// and start at zero.  Also the collisionObjectTypeMax value that follows
// should be set to the largest value in this enumeration
enum collisionObjectType {
	square = 0, 
	hexagon = 1
}; 
unsigned int const collisionObjectTypeMax = 1; 

class CollisionObject
{
public:
	// Creates a collision object using the paramenter RenderSize
	//
	// renderSize - Used to determine where in rendered screen space
	//    The CollisionObject (CO) will appear.  The renderSize tells the 
	//    height and width that the CO can occupy
	CollisionObject::CollisionObject(D2D1_SIZE_F const renderSize);

	// Don't think I need a destructor because I don't remember
	// The CollisionObject (CO) class explicitly calling new
	// ~CollisionObject(void) {;}

	// Initializes the Direct2D geometry used by the collision Object using ideas in [6]. 
    //
    // theContext - The "canvas" on which to draw
    // the_d2dFactory - Needed to initialize this artifact's 
    //    Object Geometry to a path geometry
    // iTheObjectWorldP - 
    //    Each Collision Object needs to know about its buddies.  
    //    The iTheObjectWorldP is a way for each Collision Object to keep tabs
    //    on all the other Collision Objects in the demo "world"
	void create( 
	   Microsoft::WRL::ComPtr<ID2D1DeviceContext> const & theContext, 
	   Microsoft::WRL::ComPtr<ID2D1Factory1> const & the_d2dFactory, 
	   list<CollisionObject *> * iTheObjectWorldP
	); 

	// Attempts to update the position and velocity vectors using 
	// conservation of momentum 
	// 
	// i_d2dContext - Used to ensure proper wrap around is performed
	//    after the dynamics are updated 
	void updateDynamics(
		Microsoft::WRL::ComPtr<ID2D1DeviceContext> const i_d2dContext);

	// All Collision Objects must be able to take a 2D DirectX
	// Context and render themselves onto that context  
	HRESULT renderCO( 
		Microsoft::WRL::ComPtr<ID2D1DeviceContext> const & DC, 
		D2D1::Matrix3x2F const & scaleMatrix, 
		D2D1::Matrix3x2F const & translationMatrix
	); 
	
public: 	
	// Takes a pointer from another collision object "iCO" and tries to 
	// update this object's velocity based upon and elastic collision with 
	// iCO
	// 
	// iCO - A pointer of the other collision object that should collide
	//    with this Collision Object
	void updateFromElasticCollision(CollisionObject const * const iCO); 

	// The velocity vector of the Collsion Object
	// Note that acelleration vectors are not demonstrated in this sample
	CoordinateOrientation m_v; 

	// The old velocity vector of the Collision Object.
	// Used temporarily when computing momentums 
	CoordinateOrientation * m_oldV; 

	// The current position of the Collision Object
	CoordinateOrientation m_p; 

	// The inititial position of the Collision Object
	CoordinateOrientation const m_initialPosition; 

	// The mass of the Collision Object.  
	float m_mass; 

	// A pointer to the collection of all the objects in this Collision Object's world
	list<CollisionObject *> * m_myObjectWorldP; 

	// A dictionary of Collision Objects that this Collision Object recently 
	// collided with.  Using a dictionary here theoretically allows for an 
	// O(1) look up time
	map<CollisionObject *, boolean> m_recentCollisions; 

	// Gets the position of a Collision Object without changing anything
	// because m_p is currently public this isn't actually necessary, but
	// it is an artifact of when I was "trying" to following good design
	// principles.  At some point, however, I was wasting too much time
	// on this potential vaporware and started getting a bit sloppy so
	// this function isn't as important as it once was.
	CoordinateOrientation getPosition(void) const;	

	// Returns the object geometry associated with this Collision Object (CO)
	// without affecting this CO	
	ID2D1PathGeometry * const gGeoP(void) const; 

	// Returns the transformation last applied to this Collision Object (CO)
	// without affecting this CO
	D2D1_MATRIX_3X2_F const gTransformation(void) const; 

protected: 
	// This demo allows Square and Octagon Artifacts
	// The type of the Artifact determins how it is rendered
	collisionObjectType m_type; 

	// The D2DGeometry that is used to define the unpained shape of 
	// this Collision Object
	Microsoft::WRL::ComPtr<ID2D1PathGeometry> m_objectGeometry;

	// The pattern used to fill in this interior of this 
	// Collision Object (CO) 
	Microsoft::WRL::ComPtr<ID2D1LinearGradientBrush> m_brush;

	// The transformation that was last applied to this 
	// Collision Object (CO)
	D2D1_MATRIX_3X2_F m_myTransformation; 
};

