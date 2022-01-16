#include "Server.h"
#include <filesystem>
#include <thread>
bool uploadServer(Server client, std::string path) {
	std::string temp = path;
	std::size_t found = temp.find("StoredFiles");
	if (found != std::string::npos)
	{
		found = temp.find("Files");
		temp.insert(found, "Server");
		path = temp;
		//std::cout << path << std::endl;
	}
	while (true)
	{
		char fileName[FILENAME_MAX];
		char fileType[FILENAME_MAX];
		strcpy(fileName, client.recvFileName(client.getSock()));
		strcpy(fileType, client.recvFileName(client.getSock()));
		if (fileName == "NULL" || fileType == "NULL")
			return 0;
		if (fileName == "/eof" || fileType == "/eof")
			return 1;
		//std::cout << fileName << " " << fileType << std::endl;
		std::string temp = fileName;
		std::size_t found = temp.find("StoredFiles");
		if (found != std::string::npos)
		{
			found = temp.find("Files");
			temp.insert(found, "Server");
			strcpy(fileName, temp.c_str());
			std::cout << fileName << std::endl;
			std::cout << fileType << std::endl;
		}
		if (strstr(fileType, "folder"))
		{
			std::filesystem::create_directory(fileName);
		}
		else if (strstr(fileType, "file"))
		{
			long size = client.recvFileSize(client.getSock());
			std::cout << size << std::endl;
			if (size > 0)
			{

				if (client.writeToFile(client.getSock(), fileName, size) == 0)
					return 0;

			}
			else if (size == 0)
			{
				std::ofstream file;
				file.open(fileName);
			}
			else if (size == -1)
				return 0;
		}
	}
}
bool downloadServer(Server client, std::string path) {
	if (std::filesystem::exists(path))
	{
		for (const auto& it : std::filesystem::recursive_directory_iterator(path))
		{
			if (it.is_directory())
			{
				char fileName[FILENAME_MAX];
				strcpy(fileName, it.path().string().c_str());
				if (client.sendFileName(client.getSock(), fileName) == 0)
					return 0;
				char fileType[FILENAME_MAX];
				strcpy(fileType, "folder");
				if (client.sendFileName(client.getSock(), fileType) == 0)
					return 0;
			}
			else
			{
				char fileName[FILENAME_MAX];
				strcpy(fileName, it.path().string().c_str());
				if (client.sendFileName(client.getSock(), fileName) == 0)
					return 0;
				char fileType[FILENAME_MAX];
				strcpy(fileType, "file");
				if (client.sendFileName(client.getSock(), fileType) == 0)
					return 0;
				if (client.sendFile(client.getSock(), it.path().string()) == 0)
					return 0;
			}

		}
		char fileName[FILENAME_MAX];
		strcpy(fileName, "/eof");
		if (client.sendFileName(client.getSock(), fileName) == 0)
			return 0;
		if (client.sendFileName(client.getSock(), fileName) == 0)
			return 0;
		return 1;
	}
	else
	{
		std::filesystem::create_directory(path);
		return 1;
	}
}
int main() {
	while (true)
	{
		Server server;
		char username[FILENAME_MAX];
		char command[FILENAME_MAX];
		server.connectClient();
		strcpy(username, server.recvUser(server.getSock()));
		std::string path = "./StoredServerFiles/";
		path += username;
		strcpy(command, server.recvUser(server.getSock()));
		if (strstr(command, "deleteFile"))
		{
			char fileName[FILENAME_MAX];
			strcpy(fileName, server.recvFileName(server.getSock()));
			if (fileName == "NULL")
				return 0;
			path += "/";
			path+=fileName;
			std::cout << path << std::endl;
			std::filesystem::remove_all(path);
		}
		else
			if (strstr(command, "renameFile"))
			{
				char fileOldName[FILENAME_MAX];
				char fileNewName[FILENAME_MAX];
				strcpy(fileOldName, server.recvFileName(server.getSock()));
				if (fileOldName == "NULL")
					return 0;
				strcpy(fileNewName, server.recvFileName(server.getSock()));
				if (fileNewName == "NULL")
					return 0;
				path += "/";
				std::filesystem::rename(path + fileOldName, path + fileNewName);
			}
			else
				if (strstr(command, "downloadFile"))
				{
					char fileName[FILENAME_MAX];
					strcpy(fileName, server.recvFileName(server.getSock()));
					if (fileName == "NULL")
						return 0;
					if (server.sendFileName(server.getSock(), fileName));
					std::cout << fileName << std::endl;
					if (server.sendFile(server.getSock(), fileName) == 0)
						return 0;
					strcpy(fileName, "/eof");
					if (server.sendFileName(server.getSock(), fileName) == 0)
						return 0;
				}
				else
					if (strstr(command, "updateFile"))
					{
						uploadServer(server, path);
					}
					else
			if (strstr(command, "download"))
			{
				downloadServer(server, path);

			}
			else
				if (strstr(command, "upload"))
				{
					std::filesystem::remove_all(path);
					std::filesystem::create_directory(path);
					uploadServer(server, path);
				}
		closesocket(server.getSock());
		WSACleanup();
	}
}