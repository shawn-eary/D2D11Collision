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
// http://code.msdn.microsoft.com/windowsapps/Direct2D-Basic-Animation-d0a5a4d1/sourcecode?fileId=44721&pathId=579721725

// References: 
// [1] - Microsoft
//       Direct2D animation sample
//       Windows | Dev Center - Windows Store apps
//       http://code.msdn.microsoft.com/windowsapps/Direct2D-Basic-Animation-d0a5a4d1
//       (Last Accessed: 10-NOV-2013) 

#pragma once

#include <d3d11_2.h>

namespace DX
{
    // Convert DirectX error codes to exceptions.
    inline void ThrowIfFailed(HRESULT hr)
    {
        if (FAILED(hr))
        {
            // Set a breakpoint on this line to catch DX API errors.
            throw Platform::Exception::CreateException(hr);
        }
    }

#if defined(_DEBUG)
    // Check for SDK Layer support.
    inline bool SdkLayersAvailable()
    {
        HRESULT hr = D3D11CreateDevice(
            nullptr,
            D3D_DRIVER_TYPE_NULL,       // There is no need to create a real hardware device.
            0,
            D3D11_CREATE_DEVICE_DEBUG,  // Check for the SDK layers.
            nullptr,                    // Any feature level will do.
            0,
            D3D11_SDK_VERSION,          // Always set this to D3D11_SDK_VERSION for Windows Store apps.
            nullptr,                    // No need to keep the D3D device reference.
            nullptr,                    // No need to know the feature level.
            nullptr                     // No need to keep the D3D device context reference.
            );

        return SUCCEEDED(hr);
    }
#endif
}
