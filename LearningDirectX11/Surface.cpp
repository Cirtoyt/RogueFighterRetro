#define FULL_WINTARD
#include "Surface.h"
#include <algorithm>
namespace Gdiplus
{
	using std::min;
	using std::max;
}
#include <gdiplus.h>
#include <sstream>

#pragma comment(lib, "gdiplus.lib")

Surface::Surface(unsigned int width, unsigned int height) noexcept
	: pBuffer(std::make_unique<Color[]>(width * height)),
	width(width),
	height(height)
{}

Surface::Surface(Surface&& source) noexcept
	:
	pBuffer(std::move(source.pBuffer)),
	width(source.width),
	height(source.height)
{}

Surface& Surface::operator=(Surface&& donor) noexcept
{
	width = donor.width;
	height = donor.height;
	pBuffer = std::move(donor.pBuffer);
	donor.pBuffer = nullptr;
	return *this;
}

Surface::~Surface()
{}

unsigned int Surface::GetWidth() const noexcept
{
	return width;
}

unsigned int Surface::GetHeight() const noexcept
{
	return height;
}

const Surface::Color* Surface::GetBufferPtr() const noexcept
{
	return pBuffer.get();
}

Surface Surface::FromFile(const std::string & name)
{
	unsigned int width = 0;
	unsigned int height = 0;
	std::unique_ptr<Color[]> pBuffer;

	{
		wchar_t wideName[512];
		mbstowcs_s(nullptr, wideName, name.c_str(), _TRUNCATE);

		Gdiplus::Bitmap bitmap(wideName);
		if (bitmap.GetLastStatus() != Gdiplus::Status::Ok)
		{
			std::stringstream ss;
			ss << "Loading image [" << name << "]: failed to load.";
		}

		width = bitmap.GetWidth();
		height = bitmap.GetHeight();
		pBuffer = std::make_unique<Color[]>(width * height);

		for (unsigned int y = 0; y < height; y++)
		{
			for (unsigned int x = 0; x < width; x++)
			{
				Gdiplus::Color c;
				bitmap.GetPixel(x, y, &c);
				pBuffer[y * width + x] = c.GetValue();
			}
		}
	}

	return Surface(width, height, std::move(pBuffer));
}

Surface::Surface(unsigned int width, unsigned int height, std::unique_ptr<Color[]> pBufferParam) noexcept
	:
	width(width),
	height(height),
	pBuffer(std::move(pBufferParam))
{}