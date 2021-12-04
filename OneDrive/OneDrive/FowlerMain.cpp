#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>

static const size_t InitialFNV = 2166136261U;
static const size_t FNVMultiple = 16777619;

size_t myhash(const std::string& s)
{
	size_t hash = InitialFNV;
	for (size_t i = 0; i < s.length(); i++)
	{
		hash = hash ^ (s[i]);       /* xor  the low 8 bits */
		hash = hash * FNVMultiple;  /* multiply by the magic number */
	}
	return hash;
}

int main()
{
	//std::string text;
	


	////////text

	//for (std::ifstream fis("text.txt"); !fis.eof(); /*empty*/)
	//{
	//	std::string aux;
	//	getline(fis, aux);

	//	text += aux;
	//}

	//const size_t rez = myhash(text);

	//std::cout << rez << '\n';


	//for (std::ifstream fis("FolderIcon.png", std::ios::binary); !fis.eof(); /*empty*/)
	//{
		//std::filesystem::file_size(image);
	//}


	///////imagini

	/*std::ifstream fis("./Assets/ImageIcon.jpg", std::ios::binary);
	fis.seekg(0, std::ios::end);

	int size = fis.tellg();
	char* image = new char[size];
	fis.seekg(0, std::ios::beg);
	fis.read(image, size);
	std::cout << "Size of the file is" << " " << size << " " << "bytes\n";

	const size_t rez = myhash(image);
	std::cout << rez << '\n';*/


	////////videos

	std::ifstream fis("./Assets/FolderIcon.png", std::ios::binary);
	fis.seekg(0, std::ios::end);

	int size = fis.tellg();
	char* video = new char[size];
	fis.seekg(0, std::ios::beg);
	fis.read(video, size);
	std::cout << "Size of the file is" << " " << size << " " << "bytes.\n";

	//std::cout << fis;

	const size_t rez = myhash(video);
	std::cout << rez << '\n';


	//ImageIcon.jpg
	return 0;
}