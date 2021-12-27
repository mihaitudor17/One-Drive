#include "Server.h"
#include <filesystem>
int main() {
	Server server;
		char username[FILENAME_MAX];
		server.connectClient();
			strcpy(username, server.recvUser(server.getSock()));
			std::string path = "./Stored Files/";
			path += username;
			if (std::filesystem::exists(path))
			{
				for (const auto& it : std::filesystem::recursive_directory_iterator(path))
				{
					if (it.is_directory())
					{
						char fileName[FILENAME_MAX];
						strcpy(fileName, it.path().string().c_str());
						server.sendFileName(server.getSock(), fileName);
						char fileType[FILENAME_MAX];
						strcpy(fileType, "folder");
						server.sendFileName(server.getSock(), fileType);
					}
					else
					{
						char fileName[FILENAME_MAX];
						strcpy(fileName, it.path().string().c_str());
						server.sendFileName(server.getSock(), fileName);
						char fileType[FILENAME_MAX];
						strcpy(fileType, "file");
						server.sendFileName(server.getSock(), fileType);
						server.sendFile(server.getSock(), it.path().string());
					}
					
				}
			}
			else
			{
				std::filesystem::create_directory(path);
			}
			char fileName[FILENAME_MAX];
			strcpy(fileName, "/eof");
			server.sendFileName(server.getSock(), fileName);
			server.sendFileName(server.getSock(), fileName); 
	}