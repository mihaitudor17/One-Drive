#include "Folder.h"

void copyDirectoryContents(std::filesystem::path source, std::filesystem::path destination)
{
	for (const auto& iterator : std::filesystem::directory_iterator(source))
	{
		if (iterator.is_directory())
		{
			std::filesystem::create_directory(destination / iterator.path().filename());
			copyDirectoryContents(iterator.path(), destination / iterator.path().filename());
		}
		else
		{
			if (std::filesystem::exists(destination / iterator.path().filename()))
			{
				remove(destination / iterator.path().filename());
			}
			std::filesystem::copy(iterator.path(), destination);
		}
	}
}
