#include "CirtException.h"
#include <sstream>

CirtException::CirtException(int line, const char* file) noexcept
	: line(line), file(file)
{}

const char* CirtException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* CirtException::GetType() const noexcept
{
	return "Cirt Exception";
}

int CirtException::GetLine() const noexcept
{
	return line;
}

const std::string& CirtException::GetFile() const noexcept
{
	return file;
}

std::string CirtException::GetOriginString() const noexcept
{
	std::ostringstream oss;
	oss << "[File] " << file << std::endl
		<< "[Line] " << line;
	return oss.str();
}