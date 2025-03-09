#include "Client.h"

Client::Client(const std::string &name) : m_pipeName(name), m_pipe(INVALID_HANDLE_VALUE), m_running(false) { }

Client::~Client() {
    disconnect();
}

bool Client::connect() {
    m_pipe = CreateFile(
            m_pipeName.c_str(),
            GENERIC_READ | GENERIC_WRITE,
            0,
            NULL,
            OPEN_EXISTING,
            0,
            NULL);

    if (m_pipe == INVALID_HANDLE_VALUE) {
        fprintf(stderr, "Failed to connect to pipe! Error: %lu\n", GetLastError());
        return false;
    }

    DWORD mode = PIPE_READMODE_BYTE;
    if (!SetNamedPipeHandleState(m_pipe, &mode, NULL, NULL)) {
        fprintf(stderr, "Failed to set pipe mode! Error: %lu\n", GetLastError());
        disconnect();
        return false;
    }

    m_running = true;
    m_listenerThread = std::thread(&Client::waitForData, this);
    return true;
}

void Client::disconnect() {
    m_running = false;

    if (m_pipe != INVALID_HANDLE_VALUE) {
        CloseHandle(m_pipe);
        m_pipe = INVALID_HANDLE_VALUE;
    }

    if (m_listenerThread.joinable()) {
        m_listenerThread.join();
    }
}

bool Client::sendData(void *data, int castType, size_t size) {
    if (m_pipe == INVALID_HANDLE_VALUE) {
        fprintf(stderr, "Pipe is not connected!\n");
        return false;
    }

    struct PipeData {
        int castType;
        size_t size;
        char data[512];
    } packet;

    if (size > sizeof(packet.data)) {
        fprintf(stderr, "Data too large to send!\n");
        return false;
    }

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

char* Client::receiveData() {
    buffer.resize(512);  // Ensure buffer size
    DWORD bytesRead;

    while (m_running) {
        if (ReadFile(m_pipe, buffer.data(), buffer.size(), &bytesRead, NULL)) {
            buffer.resize(bytesRead);  // Resize to actual data size

            char* res = new char[bytesRead + 1];
            memcpy(res, buffer.data(), bytesRead);
            res[bytesRead] = '\0';

            return res;
        }

        fprintf(stderr, "Failed to read data from pipe! Error: %lu\n", GetLastError());
        break;
    }

    return {};  // Return empty vector if failed
}

void Client::waitForData() {
    char buffer[512];
    DWORD bytesRead;

    while (m_running) {
        if (ReadFile(m_pipe, buffer, sizeof(buffer), &bytesRead, NULL)) {
            buffer[bytesRead] = '\0'; // Null-terminate (if applicable)
            printf("Received: %s\n", buffer);  // Handle data as needed
        } else {
            DWORD error = GetLastError();
            fprintf(stderr, "ReadFile failed! Error: %lu\n", error);
            break;
        }
    }

    disconnect();
}
