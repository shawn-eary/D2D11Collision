// Copyright 2013, 2012 - Shawn Eary
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
//    Place to stick constants that describe the behavior of this sample
#pragma once
#include <esent.h>

// The number of Collision Objects (CO)s to create in this demo
// Really large numbers *may* cause the host computer to screach 
// to a halt.  I think that most modern computers should be able 
// to handle 15 objects or less without a problem
unsigned int const sc_numCollisionObjectsToCreate = 15; 

// The maximum value of linear speed you want each Collision Object 
// to have.  This should be a positive value.   The units of this
// constant are assumed to be subjective.  You will just have to 
// experiment with values to get the desired result
float const sc_maxInitialLinearSpeed = 1.5f;

// The maximum value of angular speed you want each Collision Object 
// to have in radians per frame.  This should be a positive value
float const sc_maxInitialAngularSpeed = 0.08f; 

// The minimum mass value to use for COs 
float const sc_minMass = 0.5f;

// The maximum mass value to use for COs
float const sc_maxMass = 5.0f; 

// An about message that is overlaid on top of everything that is 
// going on 
WCHAR const sc_aboutMessage[] = 
	L"D2D Collision Demo\n" 
	L"©2013, 2012 - Shawn Eary (MS-LPL)\n\n"
	L"This demo used Microsoft's Direct2D Animation Sample as a template\n"
    L"http://code.msdn.microsoft.com/windowsapps/Direct2D-Basic-Animation-d0a5a4d1"
    L"\n(Portions Copyright by Microsoft)";

