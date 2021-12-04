#include "FowlerNollVo.h"

static const size_t InitialFNV = 2166136261U;
static const size_t FNVMultiple = 16777619;

size_t getHash(const std::string& text)
{
	size_t hash = InitialFNV;
	
	for (const auto& character : text)
	{
		hash = hash ^ (character);       /* xor  the low 8 bits */
		hash = hash * FNVMultiple;  /* multiply by the magic number */
	}

	return hash;
}

size_t FowlerNollVo::hashingTextFile(const std::string& filePath)
{
	return size_t();
}
