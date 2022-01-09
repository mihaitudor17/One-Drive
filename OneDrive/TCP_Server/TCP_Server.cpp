#include "Server.h"
#include <filesystem>
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
					server.sendFileSize(server.getSock(), -1);
				}
				else
					if (strstr(command, "upload"))
					{
						std::filesystem::remove_all(path);
						std::filesystem::create_directory(path);
						int ok;
						do {
							char fileName[FILENAME_MAX];
							ok = 0;
							char fileType[FILENAME_MAX];
							strcpy(fileName, server.recvFileName(server.getSock()));
							strcpy(fileType, server.recvFileName(server.getSock()));
							if (fileName == "/eof" || fileType == "/eof")
								ok = 1;
							if (ok != 1)
							{
								std::cout << fileName << std::endl << fileType << std::endl;
								if (strstr(fileType, "folder"))
								{
									std::filesystem::create_directory(fileName);
								}
								else
								{
									long size = server.recvFileSize(server.getSock());
									if (size > 0)
									{

										if (server.writeToFile(server.getSock(), fileName, size) == 0)
											ok = 1;

									}
									else if (size == 0)
									{
										std::ofstream file;
										file.open(fileName);
									}
									else if (size == -1)
										ok = 1;
								}
							}
						} while (ok != 1);
						int flush = 0;
						char fileRequested[FILENAME_MAX];
						do
						{
							flush = recv(server.getSock(), fileRequested, FILENAME_MAX, 0);
							std::cout << flush << std::endl;
						} while (flush > 0);
					}
			/*long temp=server.recvFileSize(server.getSock());
			if(temp==-1)*/
			closesocket(server.getSock());
			WSACleanup();
			main();
	}