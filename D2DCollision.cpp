// Copyright 2012 - Shawn Eary
// (Portions Copyright by Microsoft [1] with rights reserved) 
//
// I took the frame for this class form Microsoft's MS-LPL Direct2D 
// animation sample and then modified it to fit the needs for this
// demo
// 
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
//    Main frame for application logic.  Code is based upon Microsoft's
//    Direct2D Animation Sample.  Modified to suit Shawn Eary's needs. 
// 
// References: 
// [1] - Microsoft
//       Direct2D animation sample
//       Windows | Dev Center - Windows Store apps
//       http://code.msdn.microsoft.com/windowsapps/Direct2D-Basic-Animation-d0a5a4d1
//       (Last Accessed: 10-NOV-2013) 
//
// [2] - Microsoft
//       How to Draw Text 
//       Windows | Dev Center - Desktop
//       http://msdn.microsoft.com/en-us/library/windows/desktop/dd756692(v=vs.85).aspx
//       (Last Accessed: 14-JUN-2012) 
//
// [3] - User Evgeniy Petukhov
//       http://social.msdn.microsoft.com/profile/evgeniy%20petukhov/?ws=usercard-hover
//
#include "D2DCollision.h"
#include <typeinfo.h>
#include "config.h"

// BEGIN: Bunch of Microsoft Stuff
using namespace Microsoft::WRL;
using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::UI::Core;
using namespace Windows::System;
using namespace Windows::Foundation;
using namespace Windows::Graphics::Display;
// END:   Bunch of Microsoft Stuff

// See header file for description of this method
D2DCollision::D2DCollision() :
    m_windowClosed(false)    
{
	m_collisionObjectsInitialized = false; 
}

// See header file for description of this method
void D2DCollision::CreateDeviceIndependentResources()
{
    DirectXBase::CreateDeviceIndependentResources();

	// You can see a Microsoft example of how to initialize
	// text for later rendering in [2] 
	static const WCHAR msc_fontName[] = L"Verdana";
    static const FLOAT msc_fontSize = 15;

    // Create a DirectWrite text format object.
    this->m_dwriteFactory->CreateTextFormat(
        msc_fontName,
        NULL,
        DWRITE_FONT_WEIGHT_NORMAL,
        DWRITE_FONT_STYLE_NORMAL,
        DWRITE_FONT_STRETCH_NORMAL,
        msc_fontSize,
        L"", //locale
        &m_textFormat
        );

    // Center the text horizontally and vertically.
    m_textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
    m_textFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
}

// See header file for description of this method
void D2DCollision::CreateDeviceResources()
{
    DirectXBase::CreateDeviceResources();

	// This is later used to paint the text
    DX::ThrowIfFailed(
        m_d2dContext->CreateSolidColorBrush(
            D2D1::ColorF(D2D1::ColorF::White),
            &m_whiteBrush
            )
        );
}

// See header file for description of this method
void D2DCollision::Render()
{
	// Some of the patterns in this function were taken from 
	// Microsoft's [1], but the main idea here is from Shawn Eary
	// 
    // Retrieve the size of the render target.
    D2D1_SIZE_F renderTargetSize = this->m_d2dContext->GetSize();

	if (!m_collisionObjectsInitialized) {
		for (unsigned int i = 0; i < sc_numCollisionObjectsToCreate; i++) {	
			// Create Collision Objects on the heap and put them into 
			// this world's collision objects list.  While I feel that 
			// it is generally best to try avoiding use of the heep
			// when possible, I find that the ability to user pointers
			// in the m_collisionObjects container seems to make
			// comparisons between collision objects easier when 
			// attempting to update momentums 
			CollisionObject * myCOP = new CollisionObject(renderTargetSize); 			
			myCOP->create(this->m_d2dContext, this->m_d2dFactory, &m_collisionObjects);
			m_collisionObjects.push_back(myCOP);
		}			
		m_collisionObjectsInitialized = true; 
	}
	
	HRESULT hr = S_OK;
	
    this->m_d2dContext->BeginDraw();
    this->m_d2dContext->Clear(D2D1::ColorF(D2D1::ColorF::Black));
	hr = this->m_d2dContext->EndDraw();
	if (hr == D2DERR_RECREATE_TARGET)
    {
        m_d2dContext->SetTarget(nullptr);
        m_d2dTargetBitmap = nullptr;
        CreateWindowSizeDependentResources();
    }
    else
    {
        DX::ThrowIfFailed(hr);
    }

    this->m_d2dContext->SetTransform(D2D1::Matrix3x2F::Identity());
	   
	// ScaleFactor idea came from Microsoft's [1]
    float const scaleFactor = 
		min((float)renderTargetSize.width, (float)renderTargetSize.height) / 512.0f;

    D2D1::Matrix3x2F scaleMatrix = D2D1::Matrix3x2F::Scale(
        scaleFactor, scaleFactor        
    );

	// Translation idea came from Microsoft's [1]
	float const halfWidth = renderTargetSize.width / 2.0f;
	float const halfHeight = renderTargetSize.height / 2.0f;
    D2D1::Matrix3x2F translation = D2D1::Matrix3x2F::Translation(
        halfWidth, halfHeight
	);

	for (list<CollisionObject *>::iterator it = m_collisionObjects.begin(); it != m_collisionObjects.end(); it++) {
		hr = (*it)->renderCO(
			m_d2dContext, 
			scaleMatrix, 
			translation
		); 
		if (hr == D2DERR_RECREATE_TARGET)
		{
			m_d2dContext->SetTarget(nullptr);
			m_d2dTargetBitmap = nullptr;
			CreateWindowSizeDependentResources();
		}
		else
		{
			DX::ThrowIfFailed(hr);
		}		
	}
	
	this->m_d2dContext->SetTransform(D2D1::Matrix3x2F::Identity());
	D2D1_RECT_F textRect = D2D1::RectF(0, 0, renderTargetSize.width, renderTargetSize.height); 

	this->m_d2dContext->BeginDraw();
    this->m_d2dContext->DrawText(
		sc_aboutMessage,
        ARRAYSIZE(sc_aboutMessage) - 1,
        m_textFormat.Get(),
        &textRect,
        m_whiteBrush.Get()
    );
	this->m_d2dContext->EndDraw(); 

	for (list<CollisionObject *>::iterator it = m_collisionObjects.begin(); it != m_collisionObjects.end(); it++) {	    
		(*it)->updateDynamics(this->m_d2dContext); 
	}

	// Clear out any OLD veclocities now that the dynamics of each CollisionObject
	// have been updated
	for (list<CollisionObject *>::iterator it = m_collisionObjects.begin(); it != m_collisionObjects.end(); it++) {
		CollisionObject * COP = (*it);
		if (COP->m_oldV != NULL) {
			free(COP->m_oldV); 
			COP->m_oldV = NULL; 
		}
	}
}

// ##########################################################################
// # Pretty much all of the *good* code below this point is Microsoft's     #
// # code from [1].  There is some possibility that I might have messed     #
// # something up on the below code, but I did not make any intentional     #
// # contributions.                                                         #
// ##########################################################################

// See header file for description of this method
//
// This came from Microsoft's [1].  Shawn Eary had
// little to do with it. It's just a basic Direct2D pattern
void D2DCollision::Initialize(
    _In_ CoreApplicationView^ applicationView
    )
{
    applicationView->Activated +=
        ref new TypedEventHandler<CoreApplicationView^, IActivatedEventArgs^>(this, &D2DCollision::OnActivated);

    CoreApplication::Suspending +=
        ref new EventHandler<SuspendingEventArgs^>(this, &D2DCollision::OnSuspending);

    CoreApplication::Resuming +=
        ref new EventHandler<Platform::Object^>(this, &D2DCollision::OnResuming);
}

// See header file for description of this method
//
// This came from Microsoft's [1].  Shawn Eary had
// little to do with it.  It's jsut a basic Direct2D pattern
void D2DCollision::SetWindow(
    _In_ CoreWindow^ window
    )
{
    window->PointerCursor = ref new CoreCursor(CoreCursorType::Arrow, 0);

    window->SizeChanged +=
        ref new TypedEventHandler<CoreWindow^, WindowSizeChangedEventArgs^>(this, &D2DCollision::OnWindowSizeChanged);

    window->VisibilityChanged +=
        ref new TypedEventHandler<CoreWindow^, VisibilityChangedEventArgs^>(this, &D2DCollision::OnVisibilityChanged);

    window->Closed +=
        ref new TypedEventHandler<CoreWindow^, CoreWindowEventArgs^>(this, &D2DCollision::OnWindowClosed);

    DisplayInformation::GetForCurrentView()->DpiChanged +=
        ref new TypedEventHandler<DisplayInformation^, Platform::Object^>(this, &D2DCollision::OnDpiChanged);

    // [BEGIN shawn.eary: What is this about? ] 
    // Disable all pointer visual feedback for better performance when touching.
    // auto pointerVisualizationSettings = PointerVisualizationSettings::GetForCurrentView();
    // pointerVisualizationSettings->IsContactFeedbackEnabled = false;
    // pointerVisualizationSettings->IsBarrelButtonFeedbackEnabled = false;
    // [END   shawn.eary: What is this about? ] 

    DirectXBase::Initialize(window, DisplayInformation::GetForCurrentView()->LogicalDpi);
}

// See header file for description of this method
//
// This came from Microsoft's [1].  Shawn Eary had
// little to do with it.  It's jsut a basic Direct2D pattern
void D2DCollision::Load(
    _In_ Platform::String^ entryPoint
    )
{
}

// See header file for description of this method
//
// This came from Microsoft's [1].  Shawn Eary had
// little to do with it.  It's jsut a basic Direct2D pattern
void D2DCollision::Run()
{
    while (!m_windowClosed)
    {
        if (m_windowVisible)
        {
            m_window->Dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessAllIfPresent);
            Render();
            Present();
        }
        else
        {
            m_window->Dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessOneAndAllPending);
        }
    }
}

// See header file for description of this method
//
// This came from Microsoft's [1].  Shawn Eary had
// little to do with it.  It's jsut a basic Direct2D pattern
void D2DCollision::Uninitialize()
{
	// Deallocate all of the CollisionObjects that were created on the heap
	// 
	// Mr. Petukhov [3] caught that I was getting a compiler error on 
	// 27-SEP-2012.  I moved this code out of a destructor and into 
	// this Uninitialize method to get rid of the error that 
	// Mr. Petukhov mentioned.   I suspect the error came about because
	// I did something goofy (but allowed) in Windows 8 Consumer Preview 
	// or VS2012 RC that was denied in later versions.  It is most likely
	// logical that Microsoft has made the change and I am glad to comply
	// with the new standard
	for (list<CollisionObject *>::iterator it = m_collisionObjects.begin(); it != m_collisionObjects.end(); it++) {
		delete (*it); 
		(*it) = nullptr; 
	}	
}

// See header file for description of this method
//
// This came from Microsoft's [1].  Shawn Eary had
// little to do with it.  It's jsut a basic Direct2D pattern
void D2DCollision::OnWindowSizeChanged(
    _In_ CoreWindow^ sender,
    _In_ WindowSizeChangedEventArgs^ args
    )
{
    UpdateForWindowSizeChange();    
}

// See header file for description of this method
//
// This came from Microsoft's [1].  Shawn Eary had
// little to do with it.  It's jsut a basic Direct2D pattern
void D2DCollision::OnVisibilityChanged(
    _In_ CoreWindow^ sender,
    _In_ VisibilityChangedEventArgs^ args
    )
{
    m_windowVisible = args->Visible;
}

// See header file for description of this method
//
// This came from Microsoft's [1].  Shawn Eary had
// little to do with it.  It's jsut a basic Direct2D pattern
void D2DCollision::OnWindowClosed(
    _In_ CoreWindow^ sender,
    _In_ CoreWindowEventArgs^ args
) {
    m_windowClosed = true;
}

// See header file for description of this method
//
// This came from Microsoft's [1].  Shawn Eary had
// little to do with it.  It's just a basic Direct2D pattern
void D2DCollision::OnDpiChanged(
    _In_ DisplayInformation^ sender, 
    _In_ Platform::Object^ args
) {
    SetDpi(sender->LogicalDpi);
}

// See header file for description of this method
//
// This came from Microsoft's [1].  Shawn Eary had
// little to do with it.  It's jsut a basic Direct2D pattern
void D2DCollision::OnActivated(
    _In_ CoreApplicationView^ applicationView,
    _In_ IActivatedEventArgs^ args
    )
{
    m_window->Activate();
}

// See header file for description of this method
//
// This came from Microsoft's [1].  Shawn Eary had
// little to do with it.  It's jsut a basic Direct2D pattern
void D2DCollision::OnSuspending(
    _In_ Platform::Object^ sender,
    _In_ SuspendingEventArgs^ args
    )
{
}

// See header file for description of this method
//
// This came from Microsoft's [1].  Shawn Eary had
// little to do with it.  It's jsut a basic Direct2D pattern
void D2DCollision::OnResuming(
    _In_ Platform::Object^ sender,
    _In_ Platform::Object^ args
    )
{
}

// See header file for description of this method
//
// This came from Microsoft's [1].  Shawn Eary had
// little to do with it.  It's jsut a basic Direct2D pattern
IFrameworkView^ DirectXAppSource::CreateView()
{
    return ref new D2DCollision();
}

// See header file for description of this method
//
// This came from Microsoft's [1].  Shawn Eary had
// little to do with it.  It's jsut a basic Direct2D pattern
[Platform::MTAThread]
int main(Platform::Array<Platform::String^>^)
{
    auto directXAppSource = ref new DirectXAppSource();
    CoreApplication::Run(directXAppSource);
    return 0;
}
