#ifndef _CLIENT_H
#define _CLIENT_H

#include <string>
#include <cstring>
#include <windows.h>
#include <cstdio>
#include <thread>
#include <vector>

class Client {
public:
    explicit Client(const std::string& name);
    ~Client();

    bool connect();
    void disconnect();
    bool sendData(void* data, int castType, size_t size);
    char* receiveData();

private:
    std::string m_pipeName;
    HANDLE m_pipe;
    bool m_running;
    std::vector<char> buffer;
    std::thread m_listenerThread;

    void* m_data;
    int m_castType;

    void waitForData();
};

#endif // _CLIENT_H
