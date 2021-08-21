#pragma once
#include "CirtWin.h"
#include <string>
#include <memory>

class Surface
{
public:
	class Color
	{
	public:
		unsigned int dword;
	public:
		constexpr Color() noexcept : dword()
		{}
		constexpr Color(unsigned int dw) noexcept
			:
			dword(dw)
		{}
		Color& operator=(Color color) noexcept
		{
			dword = color.dword;
			return *this;
		}
	};
public:
	Surface(unsigned int width, unsigned int height) noexcept;
	Surface(Surface&& source) noexcept;
	Surface(Surface&) = delete;
	Surface& operator=(Surface&& donor) noexcept;
	Surface& operator=(const Surface&) = delete;
	~Surface();
	unsigned int GetWidth() const noexcept;
	unsigned int GetHeight() const noexcept;
	const Color* GetBufferPtr() const noexcept;
	static Surface FromFile(const std::string& name);
public:
	Surface(unsigned int width, unsigned int height, std::unique_ptr<Color[]> pBufferParam) noexcept;
private:
	std::unique_ptr<Color[]> pBuffer;
	unsigned int width;
	unsigned int height;
};