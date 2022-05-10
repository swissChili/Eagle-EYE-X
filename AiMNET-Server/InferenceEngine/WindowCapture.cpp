#include "pch.h"
#include "WindowCapture.h"
#define ATL_NO_LEAN_AND_MEAN
#define min std::min
#define max std::max
#include <atlimage.h>

BOOL CALLBACK WindowCapture::EnumProc(HWND testHwnd, WindowCapture *_this)
{
	static wchar_t buffer[256];

	GetWindowText(testHwnd, (LPWSTR)buffer, 256);

	std::wstring windowText(buffer);

	if (windowText.rfind(_this->m_prefix, 0) == 0)
	{
		_this->m_target = testHwnd;
		return FALSE;
	}

	return TRUE;
}

std::vector<uint8_t> WindowCapture::Capture(UINT *width, UINT *height)
{
	CImage image;

	if (!m_target)
	{
		EnumWindows((WNDENUMPROC)&WindowCapture::EnumProc, (LPARAM)this);
	}

	if (!m_target)
	{
		throw std::exception("Window could not be found");
	}

	RECT targetSize;
	GetClientRect(m_target, &targetSize);

	image.Create(targetSize.right, targetSize.bottom, 24);

	CImageDC imageDC(image);
	PrintWindow(m_target, imageDC, 2);

	*width = image.GetWidth();
	*height = image.GetHeight();

	std::vector<uint8_t> buffer;
	buffer.reserve(image.GetWidth() * image.GetHeight() * sizeof(uint32_t));

	for (int y = 0; y < image.GetHeight(); y++)
	{
		for (int x = 0; x < image.GetWidth(); x++)
		{
			COLORREF pixel = image.GetPixel(x, y);

			if (pixel)
			{
				std::cerr << "Pixel is " << pixel << std::endl;
			}
			buffer.push_back((pixel >> 16) & 0xFF);
			buffer.push_back((pixel >> 8) & 0xFF);
			buffer.push_back((pixel) & 0xFF);
			buffer.push_back(0xFF);

			//buffer.push_back(0xFF);
			//buffer.push_back(0);
			//buffer.push_back(0);
			//buffer.push_back(0xFF);
		}
	}
	
	return buffer;
}
