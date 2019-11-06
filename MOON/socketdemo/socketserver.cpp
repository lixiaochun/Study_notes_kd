// WindowsSocketServer.cpp : 定义控制台应用程序的入口点。
//

//#include "stdafx.h"
#include <iostream>
#include <Winsock2.h>
#include <Windows.h>
#include "crkmessage.h"

#pragma comment(lib,"Ws2_32.lib")

using namespace std;
#define PORT 8000
#define IP_ADDRESS "172.16.160.113"

DWORD WINAPI ThreadHeartBeat(LPVOID lpParameter)
{
    SOCKET ClientSocket = (SOCKET)lpParameter;
    int Ret = 0;
    TRK100MsgHead tRK100MsgHead;
    memset(&tRK100MsgHead, 0x0, sizeof(TRK100MsgHead));
    while ( 1 )
    {
        tRK100MsgHead.dwEvent = htonl(2525);
        tRK100MsgHead.wOptRtn = htons(0);
            //send msg to client
            Ret = send(ClientSocket, (char*)&tRK100MsgHead, (int)sizeof(TRK100MsgHead), 0);
            if ( Ret == SOCKET_ERROR )
            {
                cout<<"Send Info Error::"<<GetLastError()<<endl;
                break;
            }
            cout<<"After Send Msg:"<<ntohl(tRK100MsgHead.dwEvent)<<endl;

            Sleep(3000);
    }
    return 0;
}


DWORD WINAPI ClientThread(LPVOID lpParameter)
{
    SOCKET ClientSocket = (SOCKET)lpParameter;
    int Ret = 0;
    char RecvBuffer[MAX_PATH];

    while ( true )
    {
        memset(RecvBuffer, 0x00, sizeof(RecvBuffer));
        Ret = recv(ClientSocket, RecvBuffer, MAX_PATH, 0);
        if ( Ret == 0 || Ret == SOCKET_ERROR ) 
        {
            cout<<"客户端退出!"<<endl;
            break;
        }
        TRK100MsgHead *tRK100MsgHead = (TRK100MsgHead*)(RecvBuffer);
        cout<<"接收到客户信息为:\n"<< "Event:" << ntohl(tRK100MsgHead->dwEvent)<< endl;
        //cout<< "Content:" << *(int*)(RecvBuffer+sizeof(TRK100MsgHead))<< endl;
        //登陆消息
        if ( ntohl(tRK100MsgHead->dwEvent) ==  2500)
        {
            TRK100MsgHead tRK100MsgSend;
            memset(&tRK100MsgSend, 0, sizeof(TRK100MsgHead));

            tRK100MsgSend.dwEvent = htonl(2501);
            tRK100MsgSend.wOptRtn = htons(0x800A);
            //send msg to client
            Ret = send(ClientSocket, (char *)&tRK100MsgSend, (int)sizeof(TRK100MsgHead), 0);
            if ( Ret == SOCKET_ERROR )
            {
                cout<<"Send Info Error::"<<GetLastError()<<endl;
                break;
            }
            cout<<"After Send Msg:"<<ntohl(tRK100MsgSend.dwEvent)<<endl;
        }

        //心跳
        if ( ntohl(tRK100MsgHead->dwEvent) ==  2524)
        {
            TRK100MsgHead tRK100MsgHead;
            memset(&tRK100MsgHead, 0x0, sizeof(TRK100MsgHead));

            tRK100MsgHead.dwEvent = htonl(2525);
            tRK100MsgHead.wOptRtn = htons(0);
            //send msg to client
            Ret = send(ClientSocket, (char*)&tRK100MsgHead, (int)sizeof(TRK100MsgHead), 0);
            if ( Ret == SOCKET_ERROR )
            {
                cout<<"Send Info Error::"<<GetLastError()<<endl;
                break;
            }
            cout<<"After Send Msg:"<<ntohl(tRK100MsgHead.dwEvent)<<endl;

            /*
            HANDLE hThread = NULL;
            hThread = CreateThread(NULL, 0, ThreadHeartBeat, (LPVOID)ClientSocket, 0, NULL);
            if ( hThread == NULL )
            {
                cout<<"Create Thread Failed!"<<endl;
                break;
            }

            CloseHandle(hThread);
            */
            //cout<<"After Send Msg:"<<RecvBuffer<<endl;
        }
    }

    return 0;
}

int main(int argc, char* argv[])
{
    WSADATA  Ws;
    SOCKET ServerSocket, ClientSocket;
    struct sockaddr_in LocalAddr, ClientAddr;
    int Ret = 0;
    int AddrLen = 0;
    HANDLE hThread = NULL;

    //Init Windows Socket
    //The WSAStartup function initiates use of WS2_32.DLL by a process.

    if ( WSAStartup(MAKEWORD(2,2), &Ws) != 0 )
    {
        cout<<"Init Windows Socket Failed::"<<GetLastError()<<endl;
        return -1;
    }

    //Create Socket
    ServerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if ( ServerSocket == INVALID_SOCKET )
    {
        cout<<"Create Socket Failed::"<<GetLastError()<<endl;
        return -1;
    }

    //the address of family specification
    LocalAddr.sin_family = AF_INET;

    //The inet_addr function converts a string containing an (Ipv4) Internet Protocol dotted address into a proper address for the IN_ADDR structure.
    LocalAddr.sin_addr.s_addr = inet_addr(IP_ADDRESS); 

    //The htons function converts a u_short from host to TCP/IP network byte order (which is big-endian).
    LocalAddr.sin_port = htons(PORT);

    //Sets buffers to a specified character.
    memset(LocalAddr.sin_zero, 0x00, 8);

    //Bind Socket,The bind function associates a local address with a socket.
    Ret = bind(ServerSocket, (struct sockaddr*)&LocalAddr, sizeof(LocalAddr));
    if ( Ret != 0 )
    {
        cout<<"Bind Socket Failed::"<<GetLastError()<<endl;
        return -1;
    }

    //The listen function places a socket in a state in which it is listening for an incoming connection.
    Ret = listen(ServerSocket, 10);
    if ( Ret != 0 )
    {
        cout<<"listen ClientSocket Failed::"<<GetLastError()<<endl;
        return -1;
    }

    cout<<"服务端已经启动"<<endl;

    while ( true )
    {
        AddrLen = sizeof(ClientAddr);

        //The accept function permits an incoming connection attempt on a socket.
        ClientSocket = accept(ServerSocket, (struct sockaddr*)&ClientAddr, &AddrLen);

        if ( ClientSocket == INVALID_SOCKET )
        {
            cout<<"Accept Failed::"<<GetLastError()<<endl;
            break;
        }

        //The inet_ntoa function converts an (Ipv4) Internet network address into a string in Internet standard dotted format.
        cout<<"客户端连接::"<<inet_ntoa(ClientAddr.sin_addr)<<":"<<ClientAddr.sin_port<<endl;


        //Call this function to create a thread that can use CRT functions.
        hThread = CreateThread(NULL, 0, ClientThread, (LPVOID)ClientSocket, 0, NULL);
        if ( hThread == NULL )
        {
            cout<<"Create Thread Failed!"<<endl;
            break;
        }

        CloseHandle(hThread);
    }

    closesocket(ServerSocket);
    closesocket(ClientSocket);
    WSACleanup();

    return 0;
}