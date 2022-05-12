#pragma once

#include <string>

class WindowCapture
{
public:
	WindowCapture() = default;

	/// <summary>
	/// Captures a screenshot of the selected window's client area.
	/// </summary>
	/// <returns>The image in BGRA format with 8 bits per channel.</returns>
	std::vector<uint8_t> Capture(UINT *width, UINT *height);

protected:
	static BOOL CALLBACK EnumProc(HWND testHwnd, WindowCapture *_this);

	HWND m_target = nullptr;
	std::wstring m_prefix = L"Counter-Strike: Global Offensive";
};
