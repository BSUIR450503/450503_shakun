#ifndef CLIENT_H
#define CLIENT_H

#include <WinSock2.h>
#include <Windows.h>
#include <iostream>
#include <MSWSock.h>
#include "form.h"


class Client
{
private:
    Form *form;

    string username;
    string password;
    string ipAdress;
    int port;

    bool connectStatus;

    char bufferForMessage[1000];

    SOCKET messagingSocket;
    SOCKET dataSocket;

    int loginToServer();
    int receivingMessageFromServer();
    int sendMessageToServer(char*str);
    int initializeDataSocket();
    int getListOfFiles();

public:
    Client();
    ~Client();
    Form *getForm() const;
    void setForm(Form *value);
    int connectToServer(string ipAdress, int port, string username, string password);
    int deleteFile(char *nameOfFile);
    int goToTheDirectory(char *nameOfDirectory);
    int goToTheRootDirectory();
    int disconnect();
    int transmissionInterrupt();
    int downloadFileFromServer(char *nameOfFile, char *pathToDirForSaving);
    int uploadFileToServer(char *nameOfFile, char *pathToFile);
};

#endif // CLIENT_H
