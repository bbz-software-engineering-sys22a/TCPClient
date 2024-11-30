// SES6-02b-UDP-C.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include <winsock2.h>

using namespace std;

#pragma comment(lib,"ws2_32.lib")


int main(void)
{
    const int PORT = 8888; //Server Port
    const string SERVERADDRESS = "127.0.0.1"; //server adresse
    const int MESSAGELENGTH = 512; //max Länger der Nachricht

    SOCKET tcpSocket;	//Socket Objekt erstellen, benötigt für socket()
    struct sockaddr_in serverAdress, clientAddress; //Structs in denen wir die Daten zum Server und Client Speichern
    int slen; //Variablen gebraucht in sendto() und recvfrom()
    char message[MESSAGELENGTH];	//in message speichern wir die erhaltenen/versendeten Daten
    char response[MESSAGELENGTH]; // Response buffer
    WSADATA wsaData;	//WSADATA Objekt, benötigt für WSAStartup()
    slen = sizeof(clientAddress); //Länge der Client Adresse, gebruacht in sendto()

    // Initialise Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cout << "WSAStartup fehlgeschlagen: " << WSAGetLastError() << endl;
        system("pause");
        return 1;
    }

    // Create tcp socket
    if ((tcpSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)
    {
        cout << "Socket erstellen fehlgeschlagen: " << WSAGetLastError() << endl;
        system("pause");
        return 1;
    }


    memset((char *)&serverAdress, 0, sizeof(serverAdress)); //Reserviert Platz im Memory für die Daten
    //Block 1
    serverAdress.sin_family = AF_INET;
    serverAdress.sin_port = htons(PORT);
    serverAdress.sin_addr.S_un.S_addr = inet_addr(SERVERADDRESS.c_str());

    // Connect to the server
    if (connect(tcpSocket, (struct sockaddr*)&serverAdress, sizeof(serverAdress)) == SOCKET_ERROR) {
        cout << "Connection failed: " << WSAGetLastError() << endl;
        closesocket(tcpSocket);
        WSACleanup();
        system("pause");
        return 1;
    }
    /**
    memset(response, '\0', MESSAGELENGTH); // Clear the response buffer
    int recv_len = recv(tcpSocket, response, MESSAGELENGTH, 0);
    if (recv_len == SOCKET_ERROR) {
        cout << "Data receiving failed: " << WSAGetLastError() << endl;
    } else {
        cout << "Connected: " << response << endl;
    }
    **/
    //while (true) {
        // Block 2
        printf("Enter message : ");
        cin.getline(message, MESSAGELENGTH);
        if (send(tcpSocket, message, strlen(message), 0) == SOCKET_ERROR)
        {
            cout << "Daten senden fehlgeschlagen: " << WSAGetLastError() << endl;
            system("pause");
            return 1;
        }

        // Receive the server's response
        memset(response, '\0', MESSAGELENGTH); // Clear the response buffer
        int recv_len = recv(tcpSocket, response, MESSAGELENGTH, 0);
        if (recv_len == SOCKET_ERROR) {
            cout << "Data receiving failed: " << WSAGetLastError() << endl;
        } else {
            cout << "Received from server: " << response << endl;
        }
    //}
    // Block 3
    closesocket(tcpSocket);
    WSACleanup();

    system("pause");
    return 0;
}