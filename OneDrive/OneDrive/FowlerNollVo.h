#pragma once
#include <iostream>

class FowlerNollVo
{
private:
	std::string m_text;

public:
	uint32_t FNV1a(const std::string& m_text, uint32_t hash);
};

