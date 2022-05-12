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
	PrintWindow(m_target, imageDC, PW_CLIENTONLY);

	*width = image.GetWidth();
	*height = image.GetHeight();

	//LPCTSTR filename = L"C:\\Users\\ch\\Downloads\\aimnet_screenshot.png";
	//image.Save(filename);

	//std::vector<uint8_t> buffer;
	//buffer.resize(image.GetWidth() * image.GetHeight() * sizeof(uint32_t), 0xFF);

	static wchar_t tempPath[MAX_PATH] = { 0 };
	GetTempPathW(MAX_PATH, tempPath);
	std::wstring path = std::wstring(tempPath) + L"\\aimnet-tmp.png";

	DX::ThrowIfFailed(image.Save(path.data()));

	return LoadBGRAImage(path.data(), *width, *height);

#if 0
	BITMAPINFO bitmapInfo = { 0 };

	int firstRes = (GetDIBits(imageDC, image, 0, image.GetHeight(), nullptr, &bitmapInfo, DIB_RGB_COLORS));

	UNREFERENCED_PARAMETER(firstRes);

	std::memset(&bitmapInfo, 0, sizeof(BITMAPINFO));

	bitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitmapInfo.bmiHeader.biWidth = image.GetWidth();
	bitmapInfo.bmiHeader.biHeight = -image.GetHeight();
	bitmapInfo.bmiHeader.biPlanes = 1;
	bitmapInfo.bmiHeader.biBitCount = 32;
	bitmapInfo.bmiHeader.biCompression = BI_RGB;
	bitmapInfo.bmiHeader.biSize = (DWORD)buffer.size();

	int copied = GetDIBits(imageDC, (HBITMAP)image, 0, image.GetHeight(), buffer.data(), &bitmapInfo, DIB_RGB_COLORS);

	if (!copied)
	{
		throw std::exception("GetDIBits returned 0");
	}

	std::cerr << "copied " << copied << std::endl;
#endif

	//for (int y = 0; y < image.GetHeight(); y++)
	//{
	//	for (int x = 0; x < image.GetWidth(); x++)
	//	{
	//		COLORREF pixel = image.GetPixel(x, y);

	//		buffer.push_back((pixel >> 16) & 0xFF);
	//		buffer.push_back((pixel >> 8) & 0xFF);
	//		buffer.push_back((pixel) & 0xFF);
	//		buffer.push_back(0xFF);

	//		//buffer.push_back(0xFF);
	//		//buffer.push_back(0);
	//		//buffer.push_back(0);
	//		//buffer.push_back(0xFF);
	//	}
	//}
}

void WindowCapture::GetWindowSize(int *width, int *height) const
{
	RECT rect;
	GetClientRect(m_target, &rect);
	*width = rect.right;
	*height = rect.bottom;
}
