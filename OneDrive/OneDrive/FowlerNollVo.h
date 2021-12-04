#pragma once
#include <iostream>

class FowlerNollVo
{
private:
	std::string m_text;

public:
	size_t getHash(const std::string& m_text, uint32_t hash);
};

