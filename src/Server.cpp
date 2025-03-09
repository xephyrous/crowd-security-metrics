#include "Server.h"

Server::Server(const std::string &name) : m_pipeName(name), m_pipe(INVALID_HANDLE_VALUE), m_running(false) { }

Server::~Server() {
    stop();
}

bool Server::start() {
    m_pipe = CreateNamedPipe(
            m_pipeName.c_str(),
            PIPE_ACCESS_DUPLEX,
            PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,
            1,
            512,
            512,
            0,
            NULL
    );

    if (m_pipe == INVALID_HANDLE_VALUE) {
        fprintf(stderr, "Failed to create client pipe! Error: %lu\n", GetLastError());
        return false;
    }

    m_running = true;
    m_listenerThread = std::thread(&Server::waitForData, this);
    return true;
}

void Server::stop() {
    m_running = false;

    if (m_pipe != INVALID_HANDLE_VALUE) {
        FlushFileBuffers(m_pipe);
        DisconnectNamedPipe(m_pipe);
        CloseHandle(m_pipe);
        m_pipe = INVALID_HANDLE_VALUE;
    }

    if (m_listenerThread.joinable()) {
        m_listenerThread.join();
    }
}

bool Server::sendData(void *data, int castType, size_t size) {
    if (m_pipe == INVALID_HANDLE_VALUE) {
        fprintf(stderr, "Pipe is not connected!\n");
        return false;
    }

    if (size > sizeof(PipeData::data)) {
        fprintf(stderr, "Data size exceeds buffer limit!\n");
        return false;
    }

    PipeData packet;
    packet.castType = castType;
    packet.size = size;
    memcpy(packet.data, data, size);

    DWORD bytesWritten;
    size_t actualSize = sizeof(packet.castType) + sizeof(packet.size) + size;
    if (!WriteFile(m_pipe, &packet, actualSize, &bytesWritten, NULL)) {
        fprintf(stderr, "Failed to send data! Error: %lu\n", GetLastError());
        return false;
    }

    return bytesWritten == actualSize;
}

char* Server::receiveData() {
    char* buffer = new char[512];
    DWORD bytesRead;

    while (m_running) {
        if (ReadFile(m_pipe, buffer, sizeof(buffer) - 1, &bytesRead, NULL)) {
            buffer[bytesRead] = '\0';
            return buffer;
        }

        fprintf(stderr, "Failed to read data from pipe! Error: %lu\n", GetLastError());
        delete[] buffer;
        break;
    }

    return nullptr;
}

void Server::waitForData() {
    if (!ConnectNamedPipe(m_pipe, NULL)) {
        DWORD error = GetLastError();
        if (error != ERROR_PIPE_CONNECTED) {
            fprintf(stderr, "Failed to connect pipe! Error: %lu\n", error);
            stop();
            return;
        }
    }

    char buf[512];
    DWORD bytesRead;

    while (m_running) {
        if (ReadFile(m_pipe, buf, sizeof(buf) - 1, &bytesRead, NULL)) {
            buf[bytesRead] = '\0';
            printf("Received: %s\n", buf);
        } else {
            fprintf(stderr, "ReadFile failed! Error: %lu\n", GetLastError());
            break;
        }
    }

    stop();
}
