#include "FowlerNollVo.h"

size_t FowlerNollVo::getHash(const std::string& text)
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
	std::string text;

	for (std::ifstream fis(filePath); !fis.eof(); /*empty*/)
	{
		std::string aux;
		getline(fis, aux);

		text += aux;
	}

	const size_t rez = getHash(text);

	return rez;
}
