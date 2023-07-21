#ifdef _WIN32
    #define _WIN32_WINNT 0x0600  // Define the minimum Windows version (Windows Vista)
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #pragma comment(lib, "ws2_32.lib")
#else
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    #define SOCKET int
    #define INVALID_SOCKET -1
    #define SOCKET_ERROR -1
#endif

#define PORT 4242
#define BUFFER_SIZE 1024

int main() {
    #ifdef _WIN32
        WSADATA wsa_data;
        if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) {
            perror("WSAStartup failed");
            return 1;
        }
    #endif

    SOCKET sockfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len;
    char buffer[BUFFER_SIZE];

    // Create a UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == INVALID_SOCKET) {
        perror("Socket creation failed");
        #ifdef _WIN32
            WSACleanup();
        #endif
        return 1;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind the socket to the specified address and port
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        perror("Bind failed");
        close(sockfd);
        #ifdef _WIN32
            WSACleanup();
        #endif
        return 1;
    }

    printf("Listening for incoming messages on UDP port %d...\n", PORT);

    while (1) {
        // Receive a packet and its source address
        client_len = sizeof(client_addr);
        size_t num_bytes = recvfrom(sockfd, buffer, BUFFER_SIZE - 1, 0, (struct sockaddr *)&client_addr, &client_len);

        if (num_bytes < 0) {
            perror("Error while receiving");
            continue;
        }

        // Null-terminate the received data
        buffer[num_bytes] = '\0';

        // Display the received data and the source address
        printf("Received message from %s:%d: %s\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), buffer);
    }

    // Close the socket
    #ifdef _WIN32
        closesocket(sockfd);
        WSACleanup();
    #else
        close(sockfd);
    #endif

    return 0;
}