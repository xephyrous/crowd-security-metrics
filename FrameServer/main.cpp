#include <iostream>
#include <string>
#include <Client.h>
#include <Data.h>

int main() {
    Client connection(R"(\\.\pipe\my_pipe)");

    if (!connection.connect()) {
        (void)fprintf(stderr, "Failed to connect to pipe, aborting!\n");
        return -1;
    }
    printf("Connected to server!");

    std::string data = "Hello, Server!";
    if (!connection.sendData((void*)data.c_str(), CastType::CAST_STRING, data.size() + 1)) {
        (void)fprintf(stderr, "Failed to send data!\n");
    } else {
        printf("Data sent: %s", data.c_str());
    }

    while (true) {
        char* incomingData = connection.receiveData();
        if(incomingData) {
            printf("Received data: %s", incomingData);
            delete[] incomingData;
        } else {
            (void)fprintf(stderr, "Failed to receive data or connection closed!\n");
            break;
        }
    }

    connection.disconnect();
    std::cout << "Disconnected from server." << std::endl;
    return 0;
}
