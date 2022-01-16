#include "FowlerNollVo.h"

size_t FowlerNollVo::hashByName(const std::string& text)
{
	size_t hash = InitialFNV;

	for (const auto& character : text)
	{
		hash = hash ^ (character);
		hash = hash * FNVMultiple;
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

	const size_t rez = hashByName(text);
	return rez;
}

size_t FowlerNollVo::hashingImageFileAndVideoFile(const std::string& imagePath)
{
	const int BUFFER_SIZE = 1024;
	char bufferFile[BUFFER_SIZE];

	std::ifstream file;
	size_t result;

	file.open(imagePath, std::ios::binary);

	if (file.is_open())
	{
		file.seekg(0, std::ios::end);
		long fileSize = file.tellg();
		file.seekg(0, std::ios::beg);

		do
		{
			file.read(bufferFile, BUFFER_SIZE);
			result = hashByName(bufferFile);

		} while (file.gcount() > 0);
		file.close();
	}

	return result;
}

size_t FowlerNollVo::getHashOfFolder(const std::string& folderPath)
{
	size_t initialHash = InitialFNV;

	for (const auto& iterator : std::filesystem::recursive_directory_iterator(folderPath))
	{
		if (!std::filesystem::is_directory(iterator))
		{
			if (iterator.path().filename().string().find(".txt") != std::string::npos)
			{
				initialHash = initialHash ^ hashingTextFile(iterator.path().string());

			}
			else if (iterator.path().filename().string().find(".mp4") != std::string::npos ||
				iterator.path().filename().string().find(".jpg") != std::string::npos ||
				iterator.path().filename().string().find(".png") != std::string::npos
				)
			{
				initialHash = initialHash ^ hashingImageFileAndVideoFile(iterator.path().string());
			}
			else
			{
				initialHash = initialHash ^ hashByName(iterator.path().string());
			}
		}
	}
	return initialHash;
}

size_t FowlerNollVo::hashAll(const std::string& path)
{
	size_t hash;

	if (!std::filesystem::is_directory(path))
	{
		hash = getHashOfFolder(path);
	}

	else if (path.find(".txt") != std::string::npos)
	{
		hash = hashingTextFile(path);
	}

	else if (path.find(".jpg") != std::string::npos ||
		path.find("png") != std::string::npos ||
		path.find(".mp4") != std::string::npos)
	{
		hash = hashingImageFileAndVideoFile(path);
	}

	else hash = 0;
	return hash;
}