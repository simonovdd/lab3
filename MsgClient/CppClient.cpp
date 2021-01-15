// MsgClient.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "pch.h"
#include "framework.h"
#include "CppClient.h"
#include "../MsgServer/Msg.h"
#include <mutex>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

mutex mut;

void ProcessMessages()
{
    while (1) {
        Sleep(1000);
        Message msg = Message::Send(M_BROKER, M_GETDATA);
        if (msg.m_Header.m_Type == M_DATA)
            cout << endl << "Message From " << msg.m_Header.m_From << ": " << msg.m_Data;
    }
}


void Client()
{
    AfxSocketInit();
    Message m = Message::Send(M_BROKER, M_INIT);
    thread t(ProcessMessages);
    t.detach();
    while (1) {
        cout << "Send Msg - 0;  Exit - 1: ";
        bool check;
        cin >> check;
        switch (check)
        {
        case 0:
        {
            mut.lock();
            cout << "Write Receiver: ";
            cin >> m.m_Header.m_To;
            cout << "Write Message: ";
            cin.ignore();
            getline(cin, m.m_Data);
            Message::Send(m.m_Header.m_To, M_DATA, m.m_Data);
            mut.unlock();
            break;
        }
        case 1:
        {
            Message::Send(M_BROKER, M_EXIT);
            return;
        }
        }
    }
}

// Единственный объект приложения

CWinApp theApp;

int main()
{
    int nRetCode = 0;

    HMODULE hModule = ::GetModuleHandle(nullptr);

    if (hModule != nullptr)
    {
        // инициализировать MFC, а также печать и сообщения об ошибках про сбое
        if (!AfxWinInit(hModule, nullptr, ::GetCommandLine(), 0))
        {
            // TODO: вставьте сюда код для приложения.
            wprintf(L"Критическая ошибка: сбой при инициализации MFC\n");
            nRetCode = 1;
        }
        else
        {
            // TODO: вставьте сюда код для приложения.
            Client();
        }
    }
    else
    {
        // TODO: измените код ошибки в соответствии с потребностями
        wprintf(L"Критическая ошибка: сбой GetModuleHandle\n");
        nRetCode = 1;
    }

    return nRetCode;
}
