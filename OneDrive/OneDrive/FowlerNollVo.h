#pragma once
#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>

static const size_t InitialFNV = 2166136261U;
static const size_t FNVMultiple = 16777619;

class FowlerNollVo
{
private:
	std::string m_text;

public:
	size_t hashByName(const std::string& text);
	size_t hashingTextFile(const std::string& filePath);
	size_t hashingImageFileAndVideoFile(const std::string& imagePath);
	size_t getHashOfFolder(const std::string& folderPath);

	size_t hashAll(const std::string& path);
};
