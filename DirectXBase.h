//// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
//// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
//// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
//// PARTICULAR PURPOSE.
////
//// Copyright (c) Microsoft Corporation. All rights reserved

// Note from Shawn Eary - This is a ALL Microsoft code from the 
// Direct2D Animation Sample [1].  Shawn Eary did **not** write any of it.
// I *think* it is just a bunch of Microsoft DirectX Metro Glue
// 
// I *believe* this Microsoft file is released under the MS-LPL and that
// you can get the latest version of it here:
// http://code.msdn.microsoft.com/windowsapps/Direct2D-Basic-Animation-d0a5a4d1/sourcecode?fileId=44721&pathId=983074683

// References: 
// [1] - Microsoft
//       Direct2D animation sample
//       Windows | Dev Center - Windows Store apps
//       http://code.msdn.microsoft.com/windowsapps/Direct2D-Basic-Animation-d0a5a4d1
//       (Last Accessed: 10-NOV-2013) 

#pragma once

#include "DirectXSample.h"
#include <d3d11_2.h>
#include <d2d1_2.h>
#include <dwrite_2.h>
#include <wincodec.h>
#include <agile.h>

// Helper class that initializes DirectX APIs for both 2D and 3D rendering.
// Some of the code in this class may be omitted if only 2D or only 3D rendering is being used.
ref class DirectXBase abstract
{
internal:
    DirectXBase();

    virtual void Initialize(Windows::UI::Core::CoreWindow^ window, float dpi);
    virtual void HandleDeviceLost();
    virtual void CreateDeviceIndependentResources();
    virtual void CreateDeviceResources();
    virtual void SetDpi(float dpi);
    virtual void UpdateForWindowSizeChange();
    virtual void CreateWindowSizeDependentResources();
    virtual void Render() = 0;
    virtual void Present();
    void ValidateDevice();
    void Trim();

protected private:
    Platform::Agile<Windows::UI::Core::CoreWindow>  m_window;

    // DirectWrite & Windows Imaging Component Objects.
    Microsoft::WRL::ComPtr<IDWriteFactory2>         m_dwriteFactory;
    Microsoft::WRL::ComPtr<IWICImagingFactory2>     m_wicFactory;

    // DirectX Core Objects. Required for 2D and 3D.
    Microsoft::WRL::ComPtr<ID3D11Device2>           m_d3dDevice;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext2>    m_d3dContext;
    Microsoft::WRL::ComPtr<IDXGISwapChain1>         m_swapChain;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView>  m_d3dRenderTargetView;

    // Direct2D Rendering Objects. Required for 2D.
    Microsoft::WRL::ComPtr<ID2D1Factory2>           m_d2dFactory;
    Microsoft::WRL::ComPtr<ID2D1Device1>            m_d2dDevice;
    Microsoft::WRL::ComPtr<ID2D1DeviceContext1>     m_d2dContext;
    Microsoft::WRL::ComPtr<ID2D1Bitmap1>            m_d2dTargetBitmap;

    // Direct3D Rendering Objects. Required for 3D.
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView>  m_d3dDepthStencilView;

    // Cached renderer properties.
    D3D_FEATURE_LEVEL                               m_featureLevel;
    Windows::Foundation::Size                       m_renderTargetSize;
    Windows::Foundation::Rect                       m_windowBounds;
    float                                           m_dpi;
    bool                                            m_windowSizeChangeInProgress;
};
