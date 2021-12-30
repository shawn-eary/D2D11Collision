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
//    Implementation for a Collision Object (CO). COs contain the 
//    velocity, position and geometry information for the objects
//    that bounce around in this demo
//
// References (Places I got Help From):
//       Note: I'm not real sure how to cite some of these places
//       (I find citing Stackoverflow [in particular] quite confuing since
//        it tends to be a moving target and the fact that the comments are 
//        "sometimes" more useful than the answers) 
// [1] - Microsoft
//       Math Constants 
//       MSDN 
//       http://msdn.microsoft.com/en-us/library/4hwaceh6(v=VS.71).aspx
//       (Last Accessed: 14-JUN-2012) 
// 
// [2] - Various Authors
//       C++11 (Tuple) 
//       Wikipedia 
//       http://en.wikipedia.org/w/index.php?title=C%2B%2B11&oldid=496341468#Tuple_types
//       (Last Accessed: 14-JUN-2012) 
// 
// [3] - Ferruccio; vitus; Jessop, Steve; 
//       Shadow2531; Dibling, John; janm; 
//       mstrobl; Peter; tjohns20; opal
//       how-to initialize 'const std::vector<T>' like a c array
//       StackOverflow
//       http://stackoverflow.com/questions/231491/how-to-initialize-const-stdvectort-like-a-c-array
//       (Last Accessed: 9-JUN-2012) 
// 
// [4] - Tsirunyan, Armen; user3262424; Nobody; 
//       Pirau, Ioan Paul; mihsathe; Velthuis, Rudy; 
//       phoxis; user411313
//       C — number of elements in an array?
//       StackOverflow
//       http://stackoverflow.com/questions/6862682/c-number-of-elements-in-an-array
//       (Last Accessed: 9-JUN-2012) 
//  
// [5] - ildjarn; paulsm4; Gregory, Carey and possibly others
//       Where's GetTickCount() in Windows 8 Consumer Preview?
//       StackOverflow
//       http://stackoverflow.com/questions/10442191/wheres-gettickcount-in-windows-8-consumer-preview   
//       (Last Accessed: 9-JUN-2012) 
//
// [6] - Microsoft
//       ID2D1Geometry::Outline methods
//       Windows | Dev Center - Desktop
//       http://msdn.microsoft.com/en-us/library/windows/desktop/dd742769(v=vs.85).aspx
//       (Last Accessed: 10-NOV-2013) 
//
// [7] - Microsoft
//       ID2D1Factory::CreateTransformedGeometry methods
//       Windows | Dev Center - Desktop
//       http://msdn.microsoft.com/en-us/library/windows/desktop/dd742730(v=vs.85).aspx
//       (Last Accessed: 10-NOV-2013) 
// 
// [8] - Various Authors
//       Ellastic Collisions
//       Wikipedia 
//       http://en.wikipedia.org/wiki/Conservation_of_momentum#Elastic_collisions
//       (Last Accessed: 14-JUN-2012) 
// 
// [9] - Microsoft
//       map::insert
//       Classic MSDN
//       http://msdn.microsoft.com/en-us/library/81ac0zkz
//       (Last Accessed: 14-JUN-2012) 
//
// [10] - Microsoft
//        map::find
//        Classic MSDN
//        http://msdn.microsoft.com/en-us/library/92cwhskb.aspx
//        (Last Accessed: 14-JUN-2012) 
//
// [11] - Various Authors
//        Hexagon
//        Wikipedia 
//        http://en.wikipedia.org/wiki/Hexagon
//        (Last Accessed: 9-JUN-2012) 
//
// [12] - Microsoft
//        Direct2D animation sample
//        Windows | Dev Center - Windows Store apps
//        http://code.msdn.microsoft.com/windowsapps/Direct2D-Basic-Animation-d0a5a4d1
//        (Last Accessed: 10-NOV-2013) 
//
// [13] - Microsoft
//       ID2D1GradientStopCollection interface
//       Windows | Dev Center - Desktop 
//       http://msdn.microsoft.com/en-us/library/windows/desktop/dd316783(v=vs.85).aspx
//       (Last Accessed: 9-JUN-2012) 
//
// [14] - MSDN User treckle
//        http://social.msdn.microsoft.com/profile/treckle/?ws=usercard-hover
#include "CollisionObject.h"
#include "CoordinateOrientation.h"
#include "config.h"
#include <typeinfo>
#include <math.h>
#include <wrl.h>
#include <Windows.h>
#include <WinBase.h>
#include <vector>
#include <tuple>

// Just a variable I used to keep from reseeding s_rand every time I 
// turn around 
static bool s_randSeeded = false; 

// _USE_MATH_DEFINES isn't working for me so I will define the constant
// myself using the MSDN definition of [1]  
float const M_PI = 3.14159265358979323846f; 

using namespace Microsoft::WRL;

// A set of Direct2D Colors that are used at the start and end color
// of Artifact Gradients.  I'm not sure but I think tuple may be 
// farily new to C++ [2] 
typedef std::tuple<D2D1::ColorF::Enum, D2D1::ColorF::Enum> colorTuple; 
colorTuple artifactColorsArray[] = {
	colorTuple(D2D1::ColorF::Red, D2D1::ColorF::RosyBrown), 
	colorTuple(D2D1::ColorF::Blue, D2D1::ColorF::Azure), 
	colorTuple(D2D1::ColorF::Purple, D2D1::ColorF::Lavender), 
	colorTuple(D2D1::ColorF::OrangeRed, D2D1::ColorF::Orange), 
	colorTuple(D2D1::ColorF::Green, D2D1::ColorF::GreenYellow),
	colorTuple(D2D1::ColorF::Yellow, D2D1::ColorF::LawnGreen)}; 

// Put the ColorTuple pairs for the gradients into an immutable vector 
// for easier access [3][4].  Immutability has obvious obvious advantages, but 
// the one advantage of the vector is the fact that I don't have to lug
// the collection length around everywhere
size_t const artifactColorsArrayNumElements = sizeof(artifactColorsArray) / sizeof(colorTuple); 
const std::vector<const colorTuple> colorTupleArray(artifactColorsArray, artifactColorsArray+artifactColorsArrayNumElements); 

// See header file for description of this method
CollisionObject::CollisionObject(D2D1_SIZE_F const renderSize) {
	this->m_p.x = 0.0f;
	this->m_p.y = 0.0f;
	this->m_p.theta = 0.0f; 	
	this->m_oldV = NULL; 
	
	if (!s_randSeeded) {
		// Use high resolution QueryPerformanceCounter to seed the random number 
		// generator [5]
		LARGE_INTEGER li;
	    QueryPerformanceCounter(&li);
		unsigned int seed =  li.QuadPart % UINT_MAX; 		
		srand(seed); 
		s_randSeeded = true; 
	}

	// Pick a pseudo-random orientation for the Collision Object (CO)
	float const xLoc = (((float)rand() / (float)RAND_MAX) * (float)renderSize.width) - ((float)renderSize.width / 2.0f);
	this->m_p.x = xLoc; 	
	float const yLoc = (((float)rand() / (float)RAND_MAX) * (float)renderSize.height) - ((float)renderSize.height / 2.0f);
	this->m_p.y = yLoc;
	float const thetaLoc = ((float)rand() / (float)RAND_MAX) * 2.0f * M_PI;
	this->m_p.theta = thetaLoc; 
	
	// Pseudo-randomly determine the initial x and y components of 
	// each CO's linear velocity.  
	float const speed = 
		((float)rand() / (float)RAND_MAX) * sc_maxInitialLinearSpeed;
	this->m_v.x = speed * cos(thetaLoc);
	this->m_v.y = speed * sin(thetaLoc); 

	// Pseudo-randomly determine the each CO's angular velocity.  	
	float const angularSpeed = 
		((float)rand() / (float)RAND_MAX) * sc_maxInitialAngularSpeed; 	
	this->m_v.theta = 
		angularSpeed - (sc_maxInitialAngularSpeed / 2.0f); 

	// Get a number between 0.5 and 4.5 for the mass
	float const massSpread = sc_maxMass - sc_minMass; 
	float const massOffset = 
		((float)rand() / (float)RAND_MAX) * massSpread; 
	this->m_mass = massOffset + sc_minMass; 
	
	// Pseudo randomly determine the type of the collision object
	// (In this demo, it can only be square or octagon) 
	this->m_type = 
		(collisionObjectType)((int)rand() % 
		(int)(collisionObjectTypeMax+1)); 
}
	
// Helper function to get the "remainder" of a floating point
// division.  Used by getWrappedResult to help objects wraps
// around the 2D world
//
// numerator - The numer to divide into to compute the "remainder"
// divisor - The number to divide by to compute the "reaminder" 
float const floatMod(float const numerator, float const divisor) {
	if (numerator < divisor) {
		return numerator; 
	} else {
		float const numberOfDivides = numerator / divisor; 
		float const multipleOfArg = divisor * numberOfDivides; 
		float const remainder = numerator - multipleOfArg; 
		return remainder; 
	}
}

// Helper function to ensure that a coordinate wraps around the 
// "span" of the world
//
// coord - The x, y or theta coord that should be wrapped around
// span - The span of valid values that coord can have
//        (For x and y this could be the width and height of the 
//         screen respectively.  For theta this would most likely
//         be 2*PI)
float const getWrappedResult(float const coord, float const span) {
	float const halfSpan = span / 2.0f; 
	if (coord >= 0.0f) {
		float const remainder = floatMod(coord, span); 
		if (remainder <= halfSpan) {
			return remainder; 
		} else {
			float const edge = 0.0f - halfSpan; 
			return edge + (remainder - halfSpan);
		}
	} else {
		float remainderX = floatMod((coord * -1.0f), span);
		if (remainderX <= halfSpan) {
			return remainderX * -1.0f; 
		} else {
			float const rightEdge = halfSpan; 
			return rightEdge - (remainderX - halfSpan);
		}
	}  
}

// A helper function to take an input geometry 
// ipGeometryToTransform and returns a new geometry 
// oopTransformedGeometry according to the transformation 
// specified by "transform".  I don't completely understand 
// all of the AddRef and Release code going on here, but 
// there are some good examples at [6] and [7] that I used
// to create this implementation
//
// ipGeometryToTransform - The geometry to transform
// oppTranformedGeometry - The transformed geometry as 
//    and output parameter
// transform - The tranformation to apply to ipGeometryToTransform
HRESULT TransformGeometry(
    ID2D1Geometry *ipGeometryToTransform,
    ID2D1Geometry **oppTranformedGeometry, 
	D2D1_MATRIX_3X2_F const & transform
    )
{
    HRESULT hr;
    ID2D1Factory *pFactory = NULL;
    ipGeometryToTransform->GetFactory(&pFactory);

	ID2D1TransformedGeometry *pTransformedGeometry = NULL;
	hr = pFactory->CreateTransformedGeometry(
		ipGeometryToTransform,
		transform,
		&pTransformedGeometry
     );

	// User treckle [14] showed me that I need to release the 
	// pFactory here in order to avoid a memory leak
	pFactory->Release(); 
	
	*oppTranformedGeometry = pTransformedGeometry;
    (*oppTranformedGeometry)->AddRef();
	pTransformedGeometry->Release();

    return hr;
}



// Helper function to compute the new velocity of object A due to
// an elastic collision with B.  The content and formulas at [8] were 
// uses in determining the correct logic for this helper method
//
// NOTE: This function can be called again with the parameters reversed
// to compute the new vecocity of object B due to an elastic collision 
// with A
float const getNewVelocityFromElasticColision(float const iVa, float const iMa, float const iVb, float const iMb) {
	float const massFactor1 = (iMa - iMb) / (iMa + iMb); 
	float const massFactor2 = (2 * iMb) / (iMa + iMb); 
	float const retValue = (massFactor1 * iVa) + (massFactor2 * iVb); 
	return retValue; 
}

// See header file for description of this method
void CollisionObject::updateFromElasticCollision(CollisionObject const * const iSO) {
	this->m_oldV = new CoordinateOrientation();
	this->m_oldV->x = this->m_v.x;
	this->m_oldV->y = this->m_v.y;
	this->m_oldV->theta = this->m_v.theta;

	float isoVX, isoVY, isoVTheta;
	if (iSO->m_oldV != NULL) {
		isoVX = iSO->m_oldV->x; 
		isoVY = iSO->m_oldV->y;
		isoVTheta = iSO->m_oldV->theta;
	} else {
		isoVX = iSO->m_v.x;
		isoVY = iSO->m_v.y;
		isoVTheta = iSO->m_v.theta;
	}

	float const newVX = getNewVelocityFromElasticColision( 
		this->m_v.x, this->m_mass, isoVX, iSO->m_mass
	); 
	float const newVY = getNewVelocityFromElasticColision( 
		this->m_v.y, this->m_mass, isoVY, iSO->m_mass
	); 
	float const newVTheta = getNewVelocityFromElasticColision( 
		this->m_v.theta, this->m_mass, isoVTheta, iSO->m_mass
	); 
	this->m_v.x = newVX; 
	this->m_v.y = newVY; 
	this->m_v.theta = newVTheta; 
}

// See header file for description of this method
void CollisionObject::updateDynamics(
	Microsoft::WRL::ComPtr<ID2D1DeviceContext> const i_d2dContext)
{
	D2D1_SIZE_F renderTargetSize = i_d2dContext->GetSize();
	float const worldWidth = renderTargetSize.width / 2.0f; 
	float const halfWorldWidth = worldWidth / 2.0f; 
	float const worldHeight = renderTargetSize.height / 2.0f; 
	float const halfWorldHieght = worldHeight / 2.0f; 

	// Update the position in ideal coordinates
	float idealX = m_p.x + m_v.x;
	float idealY = m_p.y + m_v.y; 

	// Account for wrap around	
	m_p.x = getWrappedResult(idealX, worldWidth); 
	m_p.y = getWrappedResult(idealY, worldHeight); 
	m_p.theta += m_v.theta; 		

	// Compute results of elastic collisions
	for (list<CollisionObject *>::iterator it = m_myObjectWorldP->begin(); it != m_myObjectWorldP->end(); it++) {
		CollisionObject * const COP = (*it);
		
		CollisionObject * thisCOP = this; 
		CollisionObject * currentCOP = COP; 
		
		// Don't compare this CollisionObject with itself
		if (thisCOP != currentCOP) {
			D2D1_MATRIX_3X2_F thisTransform = thisCOP->gTransformation();
			D2D1_MATRIX_3X2_F currentTransform = currentCOP->gTransformation();
			ID2D1PathGeometry * const thisGP = thisCOP->gGeoP(); 
			ID2D1PathGeometry * const curGP = currentCOP->gGeoP();

			D2D1_GEOMETRY_RELATION theRelation; 
			ID2D1Geometry * thisTGP; 
			TransformGeometry(thisGP, &thisTGP, thisTransform); 
			ID2D1Geometry * currentTGP; 
			TransformGeometry(curGP, &currentTGP, currentTransform); 
			thisTGP->CompareWithGeometry(
				(ID2D1Geometry *)currentTGP, 
				NULL, 
				&theRelation
			);			
			thisTGP->Release(); 
			currentTGP->Release(); 
			if (theRelation != D2D1_GEOMETRY_RELATION_DISJOINT) {
				// A Collision has occured.
				// 
				// Check to see if the this Collision Object is already colliding with 
				// the current Collision Object (CO)
				map<CollisionObject *, boolean>::iterator someCOI = 
					m_recentCollisions.find(COP);
				if (someCOI == m_recentCollisions.end()) {
					// This Collision Object is NOT already colliding with the current 
					// Collision Object defined by COP.  Update this Collision Object's
					// velocites					
					this->updateFromElasticCollision(COP); 

					// Add an entry for the current Collision Object Pointer (COP) into
					// m_recentCollisions.  You can see an example of how to insert into 
					// a map at [9]
   				    typedef pair <CollisionObject *, boolean> SPO_Pair;
					m_recentCollisions.insert(SPO_Pair (COP, true)); 
				}				
			} else {
				// This Collision Object Pointer is not colliding
				// with the current Collision Object Pointer.  If applicable, 
				// remove the current Collision Object Pointer from from this 
				// Collision Object pointer's dictionary of recent collisions
				// if apliciable.  You can see an example of how to find objects
				// in a map at [10]
				map<CollisionObject *, boolean>::iterator someCOI = 
					m_recentCollisions.find(COP);
				if (someCOI != m_recentCollisions.end()) {
					// As previously mentioned.  This Collision Object (CO) is 
					// not colliding with the current  CO; however, the current
					// CO Pointer is listed in the recent collisions dictionary 
					// so it needs to be removed
					m_recentCollisions.erase(someCOI); 
				}
			}
		}
	}	
}

// See header file for description of this method
CoordinateOrientation CollisionObject::getPosition(void) const
{
	return m_p;
}


// See header file for description of this method
void CollisionObject::create( 
	Microsoft::WRL::ComPtr<ID2D1DeviceContext> const & theContext, 
	Microsoft::WRL::ComPtr<ID2D1Factory1> const & the_d2dFactory, 
	list<CollisionObject *> * iTheObjectWorldP) 
{
	this->m_myObjectWorldP = iTheObjectWorldP; 

	// Setup the geometry sink
	the_d2dFactory->CreatePathGeometry(&m_objectGeometry);
    ComPtr<ID2D1GeometrySink> myGeoSink;
    m_objectGeometry->Open(&myGeoSink);    

	// In this demo artifacts can be of two types.  Square
	// and Octagon
	if (this->m_type == square) {
		// Determine the Mass Scale Factor
		float const MSF = sqrt(this->m_mass); 
		myGeoSink->BeginFigure(
			D2D1::Point2F(9.0f*MSF, 9.0f*MSF),
			D2D1_FIGURE_BEGIN_FILLED
		);		

		D2D1_POINT_2F const square[] = {
			{-9.0f*MSF, 9.0f*MSF}, 
			{-9.0f*MSF, -9.0f*MSF}, 
			{9.0f*MSF, -9.0f*MSF}
		};
		myGeoSink->AddLines(square, 3);
	} else {		
		// For this simple program, area is considered to 
		// be the same as the mass
		float const area = this->m_mass; 

		// There is a forumual for the area of a Hexagon in [11].  You can 
		// use that method to get the required side length of the hexagon
		// from the area
		float const sideLength = (float)sqrt(2.0 * area / 3.0 / sqrt(3.0)); 
		
		// We then use the sideLength as the mass scale factor
		// I'm not really sure where but it looks like a a float
		// in the length of a side more than quadruples the area
		// This is probably okay since hexagons should be able to 
		// store more area for the same side length
		float const MSF = sideLength; 

		float const bl = 15.0f;
		myGeoSink->BeginFigure(
			D2D1::Point2F(bl*MSF, 0.0f*MSF),
			D2D1_FIGURE_BEGIN_FILLED
		);		

		// By observation I guessed that the distance from the center
		// of a hexagon to each of its edge vertices is the same as its
		// hexagon side length.  I also used the statment in [7] that the 
		// each angle should be 120 degrees and elementary geometry to 
		// come up with the idea that each vertice should be 60 degrees
		// or PI / 3 radiuans appart.  To that effect, you can simply 
		// walk through the six vertices adding PI / 3 each time until 
		// you reason 2 * PI and convert from polar to rectagular 
		// coordinates at each step of the way
		D2D1_POINT_2F const hexagon[] = {
			{bl*cos(1.0f * M_PI / 3.0f)*MSF, bl*sin(1.0f * M_PI / 3.0f)*MSF}, 
			{bl*cos(2.0f * M_PI / 3.0f)*MSF, bl*sin(2.0f * M_PI / 3.0f)*MSF}, 
			{bl*cos(3.0f * M_PI / 3.0f)*MSF, bl*sin(3.0f * M_PI / 3.0f)*MSF}, 
			{bl*cos(4.0f * M_PI / 3.0f)*MSF, bl*sin(4.0f * M_PI / 3.0f)*MSF}, 
			{bl*cos(5.0f * M_PI / 3.0f)*MSF, bl*sin(5.0f * M_PI / 3.0f)*MSF}			
		};
		myGeoSink->AddLines(hexagon, 5);
	}

	myGeoSink->EndFigure(D2D1_FIGURE_END_OPEN);
	
	// Terminate the geometry sink 
    myGeoSink->Close();

	// Create an array of gradient stops to put in the gradient stop
	// collection that will be used in the gradient brush.
	ID2D1GradientStopCollection *pGradientStops = NULL;

	// Used example in [13] to create a linear gradient brush
	// much of the code that follows came from an MSDN example at [13]
	// 
	// Oops, a magic number...
    // (I don't have time to fix it right now.  You will have to live with it) 
	D2D1_GRADIENT_STOP gradientStops[2];
	
	// Tuples are described in [2] 
	unsigned int const colorPos = rand() % colorTupleArray.size(); 
	gradientStops[0].color = D2D1::ColorF(std::get<0>(colorTupleArray[colorPos]), 1);
	gradientStops[0].position = 0.0f;
	gradientStops[1].color = D2D1::ColorF(std::get<1>(colorTupleArray[colorPos]), 1);
	gradientStops[1].position = 1.0f;

	// Create the ID2D1GradientStopCollection from a previously
	// declared array of D2D1_GRADIENT_STOP structs.
	theContext->CreateGradientStopCollection(	
		gradientStops,
		2,
		D2D1_GAMMA_2_2,
		D2D1_EXTEND_MODE_CLAMP,
		&pGradientStops
	);
    
	theContext->CreateLinearGradientBrush(
		D2D1::LinearGradientBrushProperties(
			D2D1::Point2F(9.0f, 0.0f),
			D2D1::Point2F(-9.0, 0.0f)
		), 
		pGradientStops,
		&m_brush
	);	
}

// See header file for description of this method
HRESULT CollisionObject::renderCO(
	Microsoft::WRL::ComPtr<ID2D1DeviceContext> const & DC, 
	D2D1::Matrix3x2F const & scaleMatrix, 
	D2D1::Matrix3x2F const & translationMatrix
	) {

	// If I remember correctly this is just a pattern I got from [12]
	// I don't think I did anything here other than take Microsoft's idea.
	CoordinateOrientation thePosition = this->getPosition(); 	
	float rotationVectorX = cos(thePosition.theta); 
	float rotationVectorY = sin(thePosition.theta); 
    D2D1_MATRIX_3X2_F SOMatrix;
    SOMatrix = D2D1::Matrix3x2F(
        (float)rotationVectorX, (float)rotationVectorY,
        (float)-rotationVectorY, (float)rotationVectorX,
        (float)thePosition.x, (float)thePosition.y
        );	
	this->m_myTransformation = 
		D2D1::Matrix3x2F::Identity() * SOMatrix * 
		scaleMatrix * translationMatrix;

	// Typical Direct2D pattern to apply the transmorm defined above
	// and then Fill the geometry.  Bascially taken from [12] 
	DC->SetTransform(m_myTransformation);    
	DC->BeginDraw();
    DC->FillGeometry(m_objectGeometry.Get(), m_brush.Get());	   
    HRESULT hr = DC->EndDraw();

	// I was returning NULL here because something from DC->EndDraw() 
	// was comming back with a value that broke my code, but 
	// user treckle [14] explained to me that S_OK should be fine.
	// After recieving treckle's comment, I looked at this code
	// again in the debugger and DC->EndDraw() was returning the 
	// S_OK that treckle advised.  After noting that I was getting
	// the recommended S_OK value from DC->EndDraw(), I opted to 
	// return that value for correctness.
	return hr; 
}

// See header file for description of this method
ID2D1PathGeometry * const CollisionObject::gGeoP(void) const {
	return m_objectGeometry.Get();
}

// See header file for description of this method
D2D1_MATRIX_3X2_F const CollisionObject::gTransformation(void) const {
	return m_myTransformation;
}
