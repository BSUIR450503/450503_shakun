#include "client.h"


Form *Client::getForm() const
{
    return form;
}

void Client::setForm(Form *value)
{
    form = value;
}

int Client::connectToServer(string ipAdress, int port, string username, string password)
{
    this->ipAdress = ipAdress;
    this->port = port;
    this->username = username;
    this->password = password;
    this->connectStatus = false;

    WSADATA WSAData;
    if (WSAStartup(MAKEWORD(2, 0), &WSAData) != 0)
    {
        cout << "Ошибка инициализации сокета." << endl;
        return -1;
    }

    SOCKET socketClient;
    socketClient = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in dest_addr;
    dest_addr.sin_family=AF_INET;
    dest_addr.sin_addr.s_addr = inet_addr(this->ipAdress.c_str());
    dest_addr.sin_port=htons(this->port);
    HOSTENT *hst;
    if (dest_addr.sin_addr.s_addr == INADDR_NONE)
    {
        hst = NULL;
        hst = gethostbyname(this->ipAdress.c_str());

        if (hst == NULL)
        {
            return -1;
        }
        memcpy(&dest_addr.sin_addr, hst->h_addr_list[0], hst->h_length);
    }
    this->form->showClientMessage(QString("Попытка установки соединения с " + QString::fromStdString(this->ipAdress) + ":" + QString::number(this->port)).toStdString().c_str());
    int res = connect(socketClient,(sockaddr *)&dest_addr,sizeof(dest_addr));
    if (res == -1)
    {
        this->form->showClientMessage("Ошибка. Не удалось подключится к серверу");
        return -1;
    }
    this->form->showClientMessage(QString("Соединение с " + QString::fromStdString(this->ipAdress) + ":" + QString::number(this->port) + " установлено").toStdString().c_str());
    this->messagingSocket = socketClient;
    this->connectStatus = true;
    this->receivingMessageFromServer();
    this->loginToServer();
}

int Client::deleteFile(char *nameOfFile)
{
    cout << "FILENAME " << nameOfFile << endl;
    memset(this->bufferForMessage, 0, sizeof(this->bufferForMessage));
    sprintf(this->bufferForMessage, "DELE %s\r\n", nameOfFile);
    this->sendMessageToServer(this->bufferForMessage);

    memset(this->bufferForMessage, 0, sizeof(this->bufferForMessage));
    recv(this->messagingSocket, this->bufferForMessage, 1000, 0);
    if (this->bufferForMessage[0] == '5')
    {
        this->form->showServerMessage(this->bufferForMessage);
        return -1;
    }

    this->form->showServerMessage(this->bufferForMessage);
    recv(this->messagingSocket, this->bufferForMessage, 1000, 0);
    this->getListOfFiles();
    return 0;
}

int Client::goToTheDirectory(char *nameOfDirectory)
{
    memset(this->bufferForMessage, 0, sizeof(this->bufferForMessage));
    sprintf(this->bufferForMessage, "CWD %s\r\n", nameOfDirectory);
    this->sendMessageToServer(this->bufferForMessage);
    this->receivingMessageFromServer();
    if(this->bufferForMessage[30]==0)
    this->receivingMessageFromServer();
    this->getListOfFiles();
    return 0;
}

int Client::goToTheRootDirectory()
{
    memset(this->bufferForMessage, 0, sizeof(this->bufferForMessage));
    this->sendMessageToServer("XCUP\r\n");
    this->receivingMessageFromServer();
    this->receivingMessageFromServer();
    this->getListOfFiles();
    return 0;
}

int Client::disconnect()
{
    memset(this->bufferForMessage, 0, sizeof(this->bufferForMessage));
    this->sendMessageToServer("QUIT\r\n");
    this->receivingMessageFromServer();
    closesocket(this->messagingSocket);
    closesocket(this->dataSocket);
    WSACleanup();
    return 0;
}

int Client::transmissionInterrupt()
{
    this->sendMessageToServer("ABOR\r\n");
    this->receivingMessageFromServer();
    closesocket(this->dataSocket);
    return 0;
}

int Client::downloadFileFromServer(char *nameOfFile, char* pathToDirForSaving)
{
    char request[1000];
    char outputFile[1000];
    recv(this->messagingSocket, this->bufferForMessage, 1000, 0);
    this->initializeDataSocket();
    sprintf(request, "RETR %s\r\n", nameOfFile);
    this->sendMessageToServer(request);
    char buf[1000];
    for (int i = 0; i<1000; i++)
        buf[i] = '\0';
    recv(this->messagingSocket, buf, 1000, 0);
    if (buf[0] == '5')
    {
        cout << buf << endl;
        return -1;
    }
    else
    {
        sprintf(outputFile, "%s//%s", pathToDirForSaving, nameOfFile);
        FILE *file;
        file = fopen(outputFile, "wb");
        int read = 0;
        int fullSize;

        do
        {
            char buff[1000];
            int readed = recv(this->dataSocket, buff, sizeof(buff), 0);
            fwrite(buff, 1, readed, file);
            fullSize = read;
            read += readed;
        } while (fullSize != read);

        this->receivingMessageFromServer();
        cout << "Save to dir:" << pathToDirForSaving << endl;
        closesocket(this->dataSocket);
        fclose(file);
        this->getListOfFiles();
        return 0;
    }
}

int Client::uploadFileToServer(char *nameOfFile, char* pathToFile)
{
    recv(this->messagingSocket, this->bufferForMessage, 1000, 0);
    this->initializeDataSocket();
    char request[1000];
    cout << "input sohr" << endl;
    sprintf(request, "STOR %s\r\n", nameOfFile);
    this->sendMessageToServer(request);

    char buff[1000];
    for (int i = 0; i<1000; i++)
        buff[i] = '\0';
    recv(this->messagingSocket, buff, 1000, 0);
    if (buff[0] == '5')
    {
        cout << buff << endl;
        return -1;
    }
    char fileN[1000];
    sprintf(fileN, "%s", pathToFile);

    FILE *file;
    file = fopen(fileN, "rb");
    if (file == 0)
    {
        cout << "Файл не найден в папке " << pathToFile << endl;
        return -1;
    }
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    int read = 0;
    int sizeFull = 0;
    int reading;
    rewind(file);
    if (size > 1000)
    {
        do
        {
            char buff[1000];
            fread(buff, sizeof(buff), 1, file);
            reading = send(this->dataSocket, buff, sizeof(buff), 0);
            sizeFull += reading;
        } while (sizeFull < size - 1000);
    }
    do
    {
        char buf[1];
        fread(buf, sizeof(buf), 1, file);
        reading = send(this->dataSocket, buf, sizeof(buf), 0);
        sizeFull += reading;
    } while (sizeFull != size);
    closesocket(this->dataSocket);
    fclose(file);
    this->receivingMessageFromServer();
    this->getListOfFiles();
    return 0;
}

int Client::loginToServer()
{
    sprintf(this->bufferForMessage, "USER %s\r\n", this->username.c_str());
    this->sendMessageToServer(this->bufferForMessage);
    this->receivingMessageFromServer();
    sprintf(this->bufferForMessage, "PASS %s\r\n", this->password.c_str());
    this->sendMessageToServer(this->bufferForMessage);
    this->receivingMessageFromServer();
    if (this->bufferForMessage[0] == '5')
        return -1;

    this->getListOfFiles();
    return 0;
}

int Client::receivingMessageFromServer()
{
    memset(this->bufferForMessage, 0, sizeof(this->bufferForMessage));
    recv(this->messagingSocket, this->bufferForMessage, 1000, 0);
    this->form->showServerMessage(this->bufferForMessage);
    cout << "SERVER MESSAGE: " << this->bufferForMessage << endl;
    return 0;
}

int Client::sendMessageToServer(char *str)
{
    send(this->messagingSocket, str, strlen(str), 0);
    this->form->showClientMessage(str);
    cout << "CLIENT MESSAGE: " << str << endl;
    memset(this->bufferForMessage, 0, sizeof(this->bufferForMessage));
    return 0;
}

int Client::initializeDataSocket()
{\
    cout << "initializeDataSocket" << endl;
    SOCKET ds;
    send(this->messagingSocket, "PASV\r\n", strlen("PASV\r\n"), 0);
    this->form->showClientMessage("PASV\r\n");

    memset(this->bufferForMessage, 0, sizeof(this->bufferForMessage));

    int numberByte = recv(this->messagingSocket, this->bufferForMessage, 1000, 0);
            cout << bufferForMessage<< endl;

    this->form->showServerMessage(this->bufferForMessage);

    cout << "NUMBER OF BYTE " << numberByte << endl;
    cout << "SERVER MESSAGE: " << this->bufferForMessage << endl;

    int a, b;

int result;


 hostent *host = gethostbyname(this->ipAdress.c_str());

if (host == NULL)
{
    cout << "HOST ERROR: " << GetLastError() << endl;
    return -1;
}


    ////////////////////////////////////////////////////
    // extract the sample string "216,92,6,187,194,13"
    char szIP[40];
    char* start = strchr(this->bufferForMessage, '(');
    char* end = strchr(this->bufferForMessage, ')');
    int num = end - start;

    char str[30] = { '\0' };
    strncpy(str, start + 1, num - 1);

    // Lets break the string up using the ',' character as a seperator
    char* token = strtok(str, ",");

    // Lets get the IP address from the string.
    strcpy(szIP, "");
    strcat(szIP, token);
    strcat(szIP, ".");  //szIP contains "216."

    token = strtok(NULL, ",");
    strcat(szIP, token);
    strcat(szIP, ".");  //szIP contains "216.92."

    token = strtok(NULL, ",");
    strcat(szIP, token);
    strcat(szIP, "."); // szIP contains "216.92.6."

    token = strtok(NULL, ",");
    strcat(szIP, token);// szIP contains "216.92.6.187"

    cout << "szIP: " << szIP << endl;

    // Now lets get the port number
    token = strtok(NULL, ",");
    int intA = atoi(token);  // 194

    token = strtok(NULL, ",");
    int intB = atoi(token);  // 13

    int port = (intA * 256) + intB;

    sockaddr_in address = { AF_INET, htons(port) }; // Set up socket information.

    ds = socket(AF_INET, SOCK_STREAM, 0);
    address.sin_addr.s_addr = inet_addr(inet_ntoa(*((struct in_addr *)host->h_addr)));

    result = connect(ds, (sockaddr *)&address, sizeof(address));
    if (result == -1)
    {
        cout << "ERROR " << endl;
        cout << GetLastError() << endl;
        return -1;
    }
    this->dataSocket = ds;
    return 0;
}

int Client::getListOfFiles()
{
    this->initializeDataSocket();
    this->sendMessageToServer("LIST\r\n");
    this->receivingMessageFromServer();

    memset(this->bufferForMessage, 0, sizeof(this->bufferForMessage));
    int no_of_bytes = 0;
    QString m;

    while ((no_of_bytes =  recv(this->dataSocket, this->bufferForMessage, 2000, 0)) > 0)
    {
               m += m.asprintf(this->bufferForMessage);
                fflush(stdout);
    }
    if (no_of_bytes == -1)
    {
        cout <<  "Connection lost..." << endl;
        return 0;
    }

    this->form->showListOfFile(m);
    closesocket(this->dataSocket);
    return 0;
}

Client::Client()
{
    connectStatus = false;
}

Client::~Client()
{

}

