//// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
//// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
//// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
//// PARTICULAR PURPOSE.
////
//// Copyright (c) Microsoft Corporation. All rights reserved
//   *Small* portions Copyright by Shawn Eary
//   Shawn Eary's portings are licenced via Microsoft Public License (MS-LPL) 
//   http://msdn.microsoft.com/en-us/library/ff649456.aspx
//
// The *large* *majority* of this header file was writen by Microsoft in [1].  
// Shawn Eary only made *very* *minor* changes to this header file to make the 
// it meet the needs of his D2DCollision demo.  In constract, the D2DCollison.cpp
// file contains significant changes that were made by Shawn Eary
// 
// References: 
// [1] - Microsoft
//       Direct2D animation sample
//       Windows | Dev Center - Windows Store apps
//       http://code.msdn.microsoft.com/windowsapps/Direct2D-Basic-Animation-d0a5a4d1
//       (Last Accessed: 10-NOV-2013) 

#pragma once

#include "DirectXBase.h"
#include "CollisionObject.h"
#include <list>

using namespace std;

ref class D2DCollision : public DirectXBase, public Windows::ApplicationModel::Core::IFrameworkView
{
internal:
    D2DCollision();

    // DirectXBase Methods
	
	// Microsoft pattern where I *think* you are supposed to initialize your text
    virtual void CreateDeviceIndependentResources() override;

	// Microsoft pattern when I *think* you are supposed to initialize your brushes
    virtual void CreateDeviceResources() override;

	// Microsoft pattern where you render you graphics.  This is called repeatedly
	// in a loop
    virtual void Render() override;

public:
    // IFrameworkView Methods

	// I *think* this is a Microsoft Pattern where you set up the 
    // Windows 8.X activation, suspension and resume events
	// I didn't have anything to do with this.
    virtual void Initialize(_In_ Windows::ApplicationModel::Core::CoreApplicationView^ applicationView);
	 
	// I *think* this is a Microsoft Pattern where you set up the Cursor, Windows Closed 
	// and ?DPI Resolution? events
	// I didn't have anything to do with this.
    virtual void SetWindow(_In_ Windows::UI::Core::CoreWindow^ window);

	// I do not know what this does.  It came from Microsoft
	// I didn't have anything to do with this.
    virtual void Load(_In_ Platform::String^ entryPoint);

	// A Microsoft design pattern where the events are processed
	// I didn't have anything to do with this.
    virtual void Run();
    virtual void Uninitialize();

private:
    // I'm not sure what this is for
	// It appears to be a Windows 8.X design pattern from Microsoft
	// It came from the Direct2D Animation Sample [1]
    void OnWindowSizeChanged(
        _In_ Windows::UI::Core::CoreWindow^ sender,
        _In_ Windows::UI::Core::WindowSizeChangedEventArgs^ args
    );

    // I'm not sure what this is for
    // It appears to be a Windows 8.X design pattern from Microsoft
    // It came from the Direct2D Animation Sample [1]
    void OnVisibilityChanged(
        _In_ Windows::UI::Core::CoreWindow^ sender,
        _In_ Windows::UI::Core::VisibilityChangedEventArgs^ args
    );

	// I'm not sure what this is for
	// It appears to be a Windows 8.X design pattern from Microsoft
	// It came from the Direct2D Animation Sample [1]
    void OnDpiChanged(
        _In_ Windows::Graphics::Display::DisplayInformation^ sender, 
        _In_ Platform::Object^ args
    );

	// I'm not sure what this is for
	// It appears to be a Windows 8.X design pattern from Microsoft
	// It came from the Direct2D Animation Sample [1]
    void OnActivated(
        _In_ Windows::ApplicationModel::Core::CoreApplicationView^ applicationView,
        _In_ Windows::ApplicationModel::Activation::IActivatedEventArgs^ args
    );

	// I'm not sure what this is for
	// It appears to be a Windows 8.X design pattern from Microsoft
	// It came from the Direct2D Animation Sample [1]
    void OnSuspending(
        _In_ Platform::Object^ sender,
        _In_ Windows::ApplicationModel::SuspendingEventArgs^ args
    );

	// I'm not sure what this is for
	// It appears to be a Windows 8.X design pattern from Microsoft
	// It came from the Direct2D Animation Sample [1]
    void OnResuming(
        _In_ Platform::Object^ sender,
        _In_ Platform::Object^ args
    );

	// I'm not sure what this is for
	// It appears to be a Windows 8.X design pattern from Microsoft
	// It came from the Direct2D Animation Sample [1]
    void OnWindowClosed(
        _In_ Windows::UI::Core::CoreWindow^ sender,
        _In_ Windows::UI::Core::CoreWindowEventArgs^ args
    );
  
	// I *think* this is something Microsoft came up with in their [1] sample to 
	// maintain Windows 8.X state
	bool                                                            m_windowClosed;    

    // This came from the D2DAnimation Sample [1].  I *think* it is just used
    // to avoid eating up unnecessary CPU Cycles in the D2DCollision::Run method
    // when app output is not visible on the display
    bool                                                            m_windowVisible; 

	// Defines the attributes of how a text will be rendered
    Microsoft::WRL::ComPtr<IDWriteTextFormat>                       m_textFormat;
    
	// Used to define the shape properties of the Collision Objects 
    Microsoft::WRL::ComPtr<ID2D1PathGeometry>                       m_objectGeometry;

	// Used to write the text
    Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>                    m_whiteBrush;
    
	// A list I added to hold the Collision Objects displayed in this demo
	list<CollisionObject *> m_collisionObjects; 

	// A variable I used to keep from reinitalizing the resources for the 
	// Collision Objects on every render cycle
	bool m_collisionObjectsInitialized; 
};

// Not sure what this is. It's something that Microsoft had in their 
// Direct2D Animation Sample [1]
ref class DirectXAppSource : Windows::ApplicationModel::Core::IFrameworkViewSource
{
public:
    virtual Windows::ApplicationModel::Core::IFrameworkView^ CreateView();
};
