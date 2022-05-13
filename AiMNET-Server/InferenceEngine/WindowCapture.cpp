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
	//GetTempPathW(MAX_PATH, tempPath);
	//std::wstring path = std::wstring(tempPath) + L"\\aimnet-tmp.png";

	//DX::ThrowIfFailed(image.Save(path.data()));

	//return LoadBGRAImage(path.data(), *width, *height);


	BITMAPINFOHEADER bmih;
	ZeroMemory(&bmih, sizeof(BITMAPINFOHEADER));
	bmih.biSize = sizeof(BITMAPINFOHEADER);
	bmih.biPlanes = 1;
	bmih.biBitCount = 32;
	bmih.biWidth = image.GetWidth();
	bmih.biHeight = 0 - image.GetHeight();
	bmih.biCompression = BI_RGB;
	bmih.biSizeImage = 0;

	int bytesPerPixel = bmih.biBitCount / 8;

	std::vector<uint8_t> buffer(image.GetWidth() * image.GetHeight() * bytesPerPixel);

	BITMAPINFO bmi = { 0 };
	bmi.bmiHeader = bmih;

	int row_count = GetDIBits(imageDC, image, 0, image.GetHeight(), buffer.data(), &bmi, DIB_RGB_COLORS);

	std::cerr << row_count << std::endl;

	return buffer;
}

void WindowCapture::GetWindowSize(int *width, int *height) const
{
	RECT rect;
	GetClientRect(m_target, &rect);
	*width = rect.right;
	*height = rect.bottom;
}
