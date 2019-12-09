// WindowsSocketServer.cpp : 定义控制台应用程序的入口点。
//

//#include "stdafx.h"
#include <iostream>
#include <Winsock2.h>
#include <Windows.h>
#include "crkmessage.h"
#include "rk100_msg.h"

#pragma comment(lib,"Ws2_32.lib")

using namespace std;
#define PORT 8000
#define IP_ADDRESS "172.16.160.113"
//#define IP_ADDRESS "192.169.0.159"
#define VERSION_INFO "V6.0.0.4.0.20198888"

static UINT     g_nTransTimeID = 0;                     //Timer id

VOID  CALLBACK  CWaitTimerFun( HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime )
{   
    BOOL bOk = KillTimer( NULL, g_nTransTimeID );	//关掉定时器
}

DWORD WINAPI ThreadWaitSend(LPVOID lpParameter)
{
    Sleep(10000);  //延时

    SOCKET ClientSocket = (SOCKET)lpParameter;
    int Ret = 0;
    char bySendBuffer[256] = {0};
    ZeroMemory(bySendBuffer, 256);
    TRK100MsgHead tRK100MsgHead;
    memset(&tRK100MsgHead, 0x0, sizeof(TRK100MsgHead));
    //while ( 1 )
    {
        tRK100MsgHead.dwEvent = htonl(RK100_EVT_TFTP_UPDATE_SECOND_NOTIFY);
        tRK100MsgHead.wOptRtn = htons(0);
            //send msg to client
            Ret = send(ClientSocket, (char*)&tRK100MsgHead, (int)sizeof(TRK100MsgHead), 0);
            if ( Ret == SOCKET_ERROR )
            {
                cout<<"Send Info Error::"<<GetLastError()<<endl;
                return -1;
            }
            cout<<"After Send Msg:"<<ntohl(tRK100MsgHead.dwEvent)<<endl;
    }
    return 0;
}


DWORD WINAPI ClientThread(LPVOID lpParameter)
{
    SOCKET ClientSocket = (SOCKET)lpParameter;
    int Ret = 0;
    char RecvBuffer[1024];

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
        if ( ntohl(tRK100MsgHead->dwEvent) ==  RK100_EVT_LOGIN)
        {
            char bySendBuffer[1024];
            ZeroMemory(bySendBuffer, 1024);
            TRK100MsgHead tRK100MsgSend;
            ZeroMemory(&tRK100MsgSend, sizeof(TRK100MsgHead));
            tRK100MsgSend.dwEvent = htonl(RK100_EVT_LOGIN_ACK);
            tRK100MsgSend.wMsgLen = htons(3*sizeof(TTPMoonCamInfo));
            tRK100MsgSend.wOptRtn = htons(0x800A);

            TTPMoonCamInfo tCamInfo[3];
            ZeroMemory(tCamInfo, 3*sizeof(TTPMoonCamInfo));
            tCamInfo[0].TCamIDIndex.CamNum1Flag = 1;
            tCamInfo[0].dwZoomPos = 100;
            tCamInfo[0].OutputFmt.FMT4K_25fps_flag = 1;

            tCamInfo[1].TCamIDIndex.CamNum2Flag = 1;
            tCamInfo[1].dwZoomPos = 200;
            tCamInfo[1].FocusMode.ManualModeFlag = 1;
            tCamInfo[1].CamImagParam.Gamma_opt_2_flag = 1;
            tCamInfo[1].OutputFmt.FMT4K_25fps_flag = 1;

            tCamInfo[2].TCamIDIndex.CamNum3Flag = 1;
            tCamInfo[2].dwZoomPos = 300;
            tCamInfo[2].OutputFmt.FMT4K_25fps_flag = 1;

            memcpy( bySendBuffer, &tRK100MsgSend, sizeof(TRK100MsgHead) );
            memcpy( bySendBuffer+sizeof(TRK100MsgHead), tCamInfo, 3*sizeof(TTPMoonCamInfo) );

            //send msg to client
            Ret = send(ClientSocket, (char *)bySendBuffer, (int)( sizeof(TRK100MsgHead) + 3*sizeof(TTPMoonCamInfo) ), 0);
            if ( Ret == SOCKET_ERROR )
            {
                cout<<"Send Info Error::"<<GetLastError()<<endl;
                break;
            }
            cout<<"After Send Msg:"<<ntohl(tRK100MsgSend.dwEvent)<<endl;

            //版本号
            /*HANDLE hThread = NULL;
            hThread = CreateThread(NULL, 0, ThreadWaitSend, (LPVOID)ClientSocket, 0, NULL);
            if ( hThread == NULL )
            {
                cout<<"Create Thread Failed!"<<endl;
                break;
            }
            CloseHandle(hThread);*/
        }
        //心跳
        else if ( ntohl(tRK100MsgHead->dwEvent) ==  2524)
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
        //ZOOM值调节
        else if ( ntohl(tRK100MsgHead->dwEvent) == RK100_EVT_SET_CAM_ZOOM_VAL )
        {
            tRK100MsgHead->dwEvent = htonl(RK100_EVT_SET_CAM_ZOOM_VAL_ACK);
            tRK100MsgHead->wOptRtn = htons(0);
            TCamZoomVal *ptZoomVal = (TCamZoomVal*)(RecvBuffer + sizeof(TRK100MsgHead));
            if ( ptZoomVal->ZoomUpFlag )
            {
                ptZoomVal->InputVal += 1; 
            }
            else if ( ptZoomVal->ZoomDownFlag )
            {
                ptZoomVal->InputVal -= 1;
            }
            //send msg to client
            Ret = send(ClientSocket, (char*)RecvBuffer, (int)(sizeof(TRK100MsgHead)+ntohs(tRK100MsgHead->wMsgLen)), 0);
            if ( Ret == SOCKET_ERROR )
            {
                cout<<"Send Info Error::"<<GetLastError()<<endl;
                break;
            }
            cout<<"After Send Msg:"<<ntohl(tRK100MsgHead->dwEvent)<<endl;
        }
        //ZOOM值调节
        else if ( ntohl(tRK100MsgHead->dwEvent) == RK100_EVT_SET_CAM_GAIN )
        {
            tRK100MsgHead->dwEvent = htonl(RK100_EVT_SET_CAM_GAIN_ACK);
            tRK100MsgHead->wOptRtn = htons(0);
            TGainMode *ptCainVal = (TGainMode*)(RecvBuffer + sizeof(TRK100MsgHead));
            if ( ptCainVal->CAMGainCmdUpFlag )
            {
                ptCainVal->GainInputVal += 3; 
            }
            else if ( ptCainVal->CAMGainCmdDownFlag )
            {
                if ( ptCainVal->GainInputVal <= 3)
                {
                    ptCainVal->GainInputVal = 0;
                }
                else
                {
                    ptCainVal->GainInputVal -= 3;
                } 
            }
            //send msg to client
            Ret = send(ClientSocket, (char*)RecvBuffer, (int)(sizeof(TRK100MsgHead)+ntohs(tRK100MsgHead->wMsgLen)), 0);
            if ( Ret == SOCKET_ERROR )
            {
                cout<<"Send Info Error::"<<GetLastError()<<endl;
                break;
            }
            cout<<"After Send Msg:"<<ntohl(tRK100MsgHead->dwEvent)<<endl;
        }
        //获取网络参数
        else if ( ntohl(tRK100MsgHead->dwEvent) == RK100_EVT_GET_NETPARAM )
        {
            tRK100MsgHead->dwEvent = htonl(RK100_EVT_GET_NETPARAM_ACK);
            tRK100MsgHead->wOptRtn = htons(0);
            tRK100MsgHead->wMsgLen = htons(sizeof(TRK100NetParam));
            TRK100NetParam tNetInfo;
            ZeroMemory(&tNetInfo, sizeof(TRK100NetParam));
            tNetInfo.dwIP = htonl(inet_addr("192.169.0.100"));
            tNetInfo.dwSubMask = htonl(inet_addr("255.255.254.0"));
            tNetInfo.dwGateway = htonl(inet_addr("192.169.1.1"));

            memcpy( RecvBuffer + sizeof(TRK100MsgHead), &tNetInfo, sizeof(TRK100NetParam) );

            //send msg to client
            Ret = send(ClientSocket, (char*)RecvBuffer, (int)(sizeof(TRK100MsgHead)+sizeof(TRK100NetParam)), 0);
            if ( Ret == SOCKET_ERROR )
            {
                cout<<"Send Info Error::"<<GetLastError()<<endl;
                break;
            }
            cout<<"After Send Msg:"<<ntohl(tRK100MsgHead->dwEvent)<<endl;
        }
        //获取版本信息
        else if ( ntohl(tRK100MsgHead->dwEvent) == RK100_EVT_GET_VERSION_INFO )
        {
            tRK100MsgHead->dwEvent = htonl(RK100_EVT_GET_VERSION_INFO_ACK);
            tRK100MsgHead->wOptRtn = htons(0);
            tRK100MsgHead->wMsgLen = htons(38);
            char achVerInfo[38] = {0};
            memcpy(achVerInfo, VERSION_INFO, strlen(VERSION_INFO) );
            memcpy( RecvBuffer + sizeof(TRK100MsgHead), achVerInfo, strlen(achVerInfo) );

            //send msg to client
            Ret = send(ClientSocket, (char*)RecvBuffer, (int)(sizeof(TRK100MsgHead)+38), 0);
            if ( Ret == SOCKET_ERROR )
            {
                cout<<"Send Info Error::"<<GetLastError()<<endl;
                break;
            }
            cout<<"After Send Msg:"<<ntohl(tRK100MsgHead->dwEvent)<<endl;
        }
        //恢复默认
        else if ( ntohl(tRK100MsgHead->dwEvent) == RK100_EVT_RECOVERY_DEFAULT_SET )
        {
            char bySendBuffer[1024];
            ZeroMemory(bySendBuffer, 1024);
            TRK100MsgHead tRK100MsgSend;
            ZeroMemory(&tRK100MsgSend, sizeof(TRK100MsgHead));
            tRK100MsgSend.dwEvent = htonl(RK100_EVT_RECOVERY_DEFAULT_SET_ACK);
            tRK100MsgSend.wMsgLen = htons(sizeof(TTPMoonCamInfo) + sizeof(TRK100NetParam));
            tRK100MsgSend.wOptRtn = htons(0);

            TTPMoonCamInfo tCamInfo;
            ZeroMemory(&tCamInfo, sizeof(TTPMoonCamInfo));
            tCamInfo.TCamIDIndex.CamNum1Flag = 1;
            tCamInfo.dwZoomPos = 110;
            tCamInfo.OutputFmt.FMT4K_30fps_flag = 1;

            TRK100NetParam tNetParam;
            ZeroMemory(&tNetParam, sizeof(TRK100NetParam));
            tNetParam.dwIP = htonl(inet_addr("192.169.0.100"));
            tNetParam.dwSubMask = htonl(inet_addr("255.255.254.0"));
            tNetParam.dwGateway = htonl(inet_addr("192.169.1.1"));

            memcpy( bySendBuffer, &tRK100MsgSend, sizeof(TRK100MsgHead) );
            memcpy( bySendBuffer+sizeof(TRK100MsgHead), &tCamInfo, sizeof(TTPMoonCamInfo) );
            memcpy( bySendBuffer+sizeof(TRK100MsgHead) + sizeof(TTPMoonCamInfo), &tNetParam, sizeof(TRK100NetParam) );

            Sleep(3000);
            //send msg to client
            Ret = send(ClientSocket, (char *)bySendBuffer, (int)( sizeof(TRK100MsgHead) + sizeof(TTPMoonCamInfo) + sizeof(TRK100NetParam) ), 0);
            if ( Ret == SOCKET_ERROR )
            {
                cout<<"Send Info Error::"<<GetLastError()<<endl;
                break;
            }
            cout<<"After Send Msg:"<<ntohl(tRK100MsgSend.dwEvent)<<endl;
        }
        //tftp first
        else if ( ntohl(tRK100MsgHead->dwEvent) == RK100_EVT_TFTP_UPDATE_FIRST )
        {
            tRK100MsgHead->dwEvent = htonl(ntohl(tRK100MsgHead->dwEvent)+1);
            tRK100MsgHead->wOptRtn = htons(0);
            //send msg to client
            Ret = send(ClientSocket, (char*)RecvBuffer, (int)(sizeof(TRK100MsgHead)+ntohs(tRK100MsgHead->wMsgLen)), 0);
            if ( Ret == SOCKET_ERROR )
            {
                cout<<"Send Info Error::"<<GetLastError()<<endl;
                break;
            }
            cout<<"After Send Msg:"<<ntohl(tRK100MsgHead->dwEvent)<<endl;

            //Sleep(1000);

            tRK100MsgHead->dwEvent = htonl(RK100_EVT_TFTP_UPDATE_FIRST_NOTIFY);
            tRK100MsgHead->wOptRtn = htons(0);
            //send msg to client
            Ret = send(ClientSocket, (char*)RecvBuffer, (int)(sizeof(TRK100MsgHead)+ntohs(tRK100MsgHead->wMsgLen)), 0);
            if ( Ret == SOCKET_ERROR )
            {
                cout<<"Send Info Error::"<<GetLastError()<<endl;
                break;
            }
            cout<<"After Send Msg:"<<ntohl(tRK100MsgHead->dwEvent)<<endl;
        }

        //同步
        else if ( ntohl(tRK100MsgHead->dwEvent) == RK100_EVT_SET_CAM_ID )
        {
            tRK100MsgHead->dwEvent = htonl(ntohl(tRK100MsgHead->dwEvent)+1);
            tRK100MsgHead->wOptRtn = htons(0);

            //Sleep(2000);
            //send msg to client
            Ret = send(ClientSocket, (char*)RecvBuffer, (int)(sizeof(TRK100MsgHead)+ntohs(tRK100MsgHead->wMsgLen)), 0);
            if ( Ret == SOCKET_ERROR )
            {
                cout<<"Send Info Error::"<<GetLastError()<<endl;
                break;
            }
            cout<<"After Send Msg:"<<ntohl(tRK100MsgHead->dwEvent)<<endl;
        }
        //tftp second
        else if ( ntohl(tRK100MsgHead->dwEvent) == RK100_EVT_TFTP_UPDATE_SECOND )
        {
            tRK100MsgHead->dwEvent = htonl(ntohl(tRK100MsgHead->dwEvent)+1);
            tRK100MsgHead->wOptRtn = htons(0);
            //send msg to client
            Ret = send(ClientSocket, (char*)RecvBuffer, (int)(sizeof(TRK100MsgHead)+ntohs(tRK100MsgHead->wMsgLen)), 0);
            if ( Ret == SOCKET_ERROR )
            {
                cout<<"Send Info Error::"<<GetLastError()<<endl;
                break;
            }
            cout<<"After Send Msg:"<<ntohl(tRK100MsgHead->dwEvent)<<endl;

            //g_nTransTimeID = SetTimer( NULL, 0, 10000, CWaitTimerFun );
            HANDLE hThread = NULL;
            hThread = CreateThread(NULL, 0, ThreadWaitSend, (LPVOID)ClientSocket, 0, NULL);
            if ( hThread == NULL )
            {
                cout<<"Create Thread Failed!"<<endl;
                break;
            }
            CloseHandle(hThread);

            tRK100MsgHead->dwEvent = htonl(RK100_EVT_TFTP_UPDATE_SECOND_NOTIFY);
            tRK100MsgHead->wOptRtn = htons(0);
            //send msg to client
            Ret = send(ClientSocket, (char*)RecvBuffer, (int)(sizeof(TRK100MsgHead)+ntohs(tRK100MsgHead->wMsgLen)), 0);
            if ( Ret == SOCKET_ERROR )
            {
                cout<<"Send Info Error::"<<GetLastError()<<endl;
                break;
            }
            cout<<"After Send Msg:"<<ntohl(tRK100MsgHead->dwEvent)<<endl;
        }
        //白平衡调节
        else if ( ntohl(tRK100MsgHead->dwEvent) == RK100_EVT_SET_CAM_WB )
        {
            tRK100MsgHead->dwEvent = htonl(RK100_EVT_SET_CAM_WB_ACK);
            tRK100MsgHead->wOptRtn = htons(0);
            TCamWBMode *ptCamWBMode = (TCamWBMode*)(RecvBuffer + sizeof(TRK100MsgHead));
            if (ptCamWBMode->CamWBAutoModeFlag == 1)
            {
                ptCamWBMode->RGainVal = 100;
                ptCamWBMode->BGainVal = 100;
            }

            //send msg to client
            Ret = send(ClientSocket, (char*)RecvBuffer, (int)(sizeof(TRK100MsgHead)+ntohs(tRK100MsgHead->wMsgLen)), 0);
            if ( Ret == SOCKET_ERROR )
            {
                cout<<"Send Info Error::"<<GetLastError()<<endl;
                break;
            }
            cout<<"After Send Msg:"<<ntohl(tRK100MsgHead->dwEvent)<<endl;
        }

        //其它消息
        else
        {
            tRK100MsgHead->dwEvent = htonl(ntohl(tRK100MsgHead->dwEvent)+1);
            tRK100MsgHead->wOptRtn = htons(0);
            //send msg to client
            Ret = send(ClientSocket, (char*)RecvBuffer, (int)(sizeof(TRK100MsgHead)+ntohs(tRK100MsgHead->wMsgLen)), 0);
            if ( Ret == SOCKET_ERROR )
            {
                cout<<"Send Info Error::"<<GetLastError()<<endl;
                break;
            }
            cout<<"After Send Msg:"<<ntohl(tRK100MsgHead->dwEvent)<<endl;
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