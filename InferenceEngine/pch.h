#pragma once

#include <WinSDKVer.h>
#define _WIN32_WINNT 0x0A00
#include <SDKDDKVer.h>

#define _CRT_RAND_S

// Use the C++ standard templated min/max
#define NOMINMAX

// DirectX apps don't need GDI
#define NODRAWTEXT

// Include <mcx.h> if you need this
#define NOMCX

// Include <winsvc.h> if you need this
#define NOSERVICE

// WinHelp is deprecated
#define NOHELP

#include <windows.h>

#include <wrl/client.h>
#include <wrl/event.h>

#include <d3d12.h>
#include <d3d11_1.h>
#include <d3d10.h>
#include <DescriptorHeap.h>

#if defined(NTDDI_WIN10_RS2)
#include <dxgi1_6.h>
#else
#include <dxgi1_5.h>
#endif

#include <DirectXMath.h>
#include <DirectXColors.h>
#include <wincodec.h>

#include "d3dx12.h"

#include <algorithm>
#include <exception>
#include <memory>
#include <stdexcept>
#include <vector>
#ifdef __cpp_lib_span 
#include <span>
#endif
#include <map>
#include <set>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

#ifdef _DEBUG
#include <dxgidebug.h>
#endif

#include <stdio.h>
#include <pix3.h>
#include <variant>
#include <optional>

#include "CommonStates.h"
#include "Effects.h"
#include "GamePad.h"
#include "GraphicsMemory.h"
#include "DescriptorHeap.h"
#include "DirectXHelpers.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "PrimitiveBatch.h"
#include "RenderTargetState.h"
#include "ResourceUploadBatch.h"
#include "SimpleMath.h"
#include "SpriteBatch.h"
#include "SpriteFont.h"
#include "VertexTypes.h"
#include "WindowCapture.h"

#define DML_TARGET_VERSION_USE_LATEST
#include "DirectML.h"
#include "../DirectML/DirectMLX.h"

// Use video frames as input to the DirectML model, instead of a static texture.
#define USE_VIDEO 0 // 1

// Let DirectML manage the data in the weight tensors. This can be faster on some hardware.
#define DML_MANAGED_WEIGHTS 1

namespace DX
{
    // Helper class for COM exceptions
    class com_exception : public std::exception
    {
    public:
        com_exception(HRESULT hr) : result(hr) {}

        virtual const char* what() const override
        {
            static char s_str[64] = {};
            sprintf_s(s_str, "Failure with HRESULT of %08X", static_cast<unsigned int>(result));
            return s_str;
        }

    private:
        HRESULT result;
    };

    // Helper utility converts D3D API failures into exceptions.
    inline void ThrowIfFailed(HRESULT hr)
    {
        if (FAILED(hr))
        {
            throw com_exception(hr);
        }
    }
}

inline std::vector<uint8_t> LoadBGRAImage(const wchar_t *filename, uint32_t &width, uint32_t &height)
{
    using Microsoft::WRL::ComPtr;

    ComPtr<IWICImagingFactory> wicFactory;
    DX::ThrowIfFailed(CoCreateInstance(CLSID_WICImagingFactory2, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&wicFactory)));

    ComPtr<IWICBitmapDecoder> decoder;
    DX::ThrowIfFailed(wicFactory->CreateDecoderFromFilename(filename, nullptr, GENERIC_READ, WICDecodeMetadataCacheOnDemand, decoder.GetAddressOf()));

    ComPtr<IWICBitmapFrameDecode> frame;
    DX::ThrowIfFailed(decoder->GetFrame(0, frame.GetAddressOf()));

    DX::ThrowIfFailed(frame->GetSize(&width, &height));

    WICPixelFormatGUID pixelFormat;
    DX::ThrowIfFailed(frame->GetPixelFormat(&pixelFormat));

    uint32_t rowPitch = width * sizeof(uint32_t);
    uint32_t imageSize = rowPitch * height;

    std::vector<uint8_t> image;
    image.resize(size_t(imageSize));

    if (memcmp(&pixelFormat, &GUID_WICPixelFormat32bppBGRA, sizeof(GUID)) == 0)
    {
        DX::ThrowIfFailed(frame->CopyPixels(nullptr, rowPitch, imageSize, reinterpret_cast<BYTE *>(image.data())));
    }
    else
    {
        ComPtr<IWICFormatConverter> formatConverter;
        DX::ThrowIfFailed(wicFactory->CreateFormatConverter(formatConverter.GetAddressOf()));

        BOOL canConvert = FALSE;
        DX::ThrowIfFailed(formatConverter->CanConvert(pixelFormat, GUID_WICPixelFormat32bppBGRA, &canConvert));
        if (!canConvert)
        {
            throw std::exception("CanConvert");
        }

        DX::ThrowIfFailed(formatConverter->Initialize(frame.Get(), GUID_WICPixelFormat32bppBGRA,
            WICBitmapDitherTypeErrorDiffusion, nullptr, 0, WICBitmapPaletteTypeMedianCut));

        DX::ThrowIfFailed(formatConverter->CopyPixels(nullptr, rowPitch, imageSize, reinterpret_cast<BYTE *>(image.data())));
    }

    return image;
}

