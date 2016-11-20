//
// pch.h
// Header for standard system include files.
//

#pragma once

#define DEBUG
#define TEXTURE_COLOR_ENABLED
#define TEXTURES_ENABLED
#ifdef TEXTURES_ENABLED
	#ifdef TEXTURE_COLOR_ENABLED
		#define TEXTURED_VERTEX_TYPE DirectX::VertexPositionColorTexture
	#else // TEXTURE_COLOR_ENABLED
		#define TEXTURED_VERTEX_TYPE DirectX::VertexPositionTexture
	#endif
#else
	#define TEXTURED_VERTEX_TYPE DirectX::VertexPositionColor
#endif

#include <WinSDKVer.h>
#define _WIN32_WINNT 0x0600
#include <SDKDDKVer.h>

// Use the C++ standard templated min/max
#define NOMINMAX

// DirectX apps don't need GDI
#define NODRAWTEXT
#define NOGDI
#define NOBITMAP

// Include <mcx.h> if you need this
#define NOMCX

// Include <winsvc.h> if you need this
#define NOSERVICE

// WinHelp is deprecated
#define NOHELP

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#define _USE_MATH_DEFINES
#include <math.h>

#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <iostream>

#include <wrl/client.h>

#include <d3d11_1.h>
#include <DirectXMath.h>
#include <DirectXColors.h>

#include <algorithm>
#include <exception>
#include <memory>
#include <stdexcept>
#include <time.h>
#include <string>

#include "CommonStates.h"
#include "DDSTextureLoader.h"
#include "DirectXHelpers.h"
#include "Effects.h"
#include "GamePad.h"
#include "GeometricPrimitive.h"
#include "GraphicsMemory.h"
#include "Keyboard.h"
#include "Model.h"
#include "Mouse.h"
#include "PrimitiveBatch.h"
#include "ScreenGrab.h"
#include "SimpleMath.h"
#include "SpriteBatch.h"
#include "SpriteFont.h"
#include "VertexTypes.h"
#include "WICTextureLoader.h"

#include <btBulletDynamicsCommon.h>

#include "comdef.h"
namespace DX {

	inline std::wstring AnsiToWString(const std::string& str) {
		WCHAR buffer[512];
		MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, buffer, 512);
		return std::wstring(buffer);
	}

	class DxException {
	public:
		DxException() = default;
		DxException(HRESULT hr, const std::wstring& functionName, const std::wstring& filename, int lineNumber) : 
			ErrorCode(hr),
			FunctionName(functionName),
			Filename(filename),
			LineNumber(lineNumber) {}

		HRESULT ErrorCode = S_OK;
		std::wstring FunctionName;
		std::wstring Filename;
		int LineNumber = -1;
		inline std::wstring toString() const {
			// Get the string description of the error code.
			_com_error err(ErrorCode);
			std::wstring msg = err.ErrorMessage();

			return FunctionName + L" failed in " + Filename + L"; line " + std::to_wstring(LineNumber) + L"; error: " + msg;
		}
	};

	#ifndef ThrowIfFailed
	#define ThrowIfFailed(x) { \
		HRESULT hr__ = (x); \
		std::wstring wfn = DX::AnsiToWString(__FILE__); \
		if(FAILED(hr__)) { \
			throw DX::DxException(hr__, L#x, wfn, __LINE__); \
		} \
	}
	#endif



	/*inline void ThrowIfFailed(HRESULT hr) {
		if (FAILED(hr)) {
			throw std::exception();
		}
	}*/
}

#include "Math.h"
#include "Debug.h"
#include "Object.h"
#include "Cube.h"
#include "Floor.h"
#include "Pyramid.h"
#include "Simulation.h"
#include "Window.h"

#ifndef DEBUG
#define _HAS_ITERATOR_DEBUGGING 0
#define _ITERATOR_DEBUG_LEVEL 0
//_ITERATOR_DEBUG_LEVEL = 1 // enabled (if _SECURE_SCL is defined)
//_ITERATOR_DEBUG_LEVEL = 2 // enabled (for debug builds)
#endif
