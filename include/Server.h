#ifndef _SERVER_H
#define _SERVER_H

#include <string>
#include <cstring>
#include <windows.h>
#include <cstdio>
#include <thread>
#include <Data.h>
#include <vector>

/**
 * Communicates to the Frame Server application through a named pipe
 * to facilitate data transfer through a shared memory portion
 *
 * TODO : Implement data transmission over internet
 */
class Server {
public:
    explicit Server(const std::string& name);
    ~Server();

    bool start();
    void stop();
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

#endif // _SERVER_H
