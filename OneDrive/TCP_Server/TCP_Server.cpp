#include "Server.h"
#include <filesystem>
bool uploadServer(Server client, std::string path) {
	std::filesystem::remove_all(path);
	std::filesystem::create_directory(path);
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
		if (strstr(fileType, "folder"))
		{
			std::filesystem::create_directory(fileName);
		}
		else if (strstr(fileType, "file"))
		{
			long size = client.recvFileSize(client.getSock());
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
	Server server;
	char username[FILENAME_MAX];
	char command[FILENAME_MAX];
	server.connectClient();
	strcpy(username, server.recvUser(server.getSock()));
	std::string path = "./StoredServerFiles/";
	path += username;
	strcpy(command, server.recvUser(server.getSock()));
	if (strstr(command, "delete"))
	{
		std::filesystem::remove_all(path);
		std::filesystem::create_directory(path);
	}
	else
		if (strstr(command, "download"))
		{
			downloadServer(server, path);

		}
		else
			if (strstr(command, "upload"))
			{
				uploadServer(server, path);
			}
	closesocket(server.getSock());
	WSACleanup();
	main();
}