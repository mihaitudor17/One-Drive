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

size_t FowlerNollVo::hashingImageFile(const std::string& imagePath)
{
	const int BUFFER_SIZE = 1024;
	char bufferFile[BUFFER_SIZE];

	std::ifstream file;
	size_t result;

	file.open(imagePath, std::ios::binary);

	if (file.is_open())
	{
		
	}

	return result;
}
