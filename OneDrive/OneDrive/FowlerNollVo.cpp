#include "FowlerNollVo.h"

static const size_t InitialFNV = 2166136261U;
static const size_t FNVMultiple = 16777619;

size_t getHash(const std::string& s)
{
	size_t hash = InitialFNV;

	for (size_t i = 0; i < s.length(); i++)
	{
		hash = hash ^ (s[i]);       /* xor  the low 8 bits */
		hash = hash * FNVMultiple;  /* multiply by the magic number */
	}
	return hash;
}
