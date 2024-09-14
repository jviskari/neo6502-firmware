// *******************************************************************************************************************************
// *******************************************************************************************************************************
//
//      Name:       serial_tcp.cpp
//      Purpose:    Serial Port emulation via TCP for Windows
//      Created:    6th September 2024
//      Author:     Joonas Viskari (joonas.viskari@gmail.com)
//                  Modified for Windows by AI Assistant
//
// *******************************************************************************************************************************
// *******************************************************************************************************************************

#include "serial_emu.h"
#include <stdio.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

#define TCP_PORT 25232
#define BUFFER_SIZE 1024

// IP232 (TCP client wrapper) implementation
static SOCKET sockfd = INVALID_SOCKET;
static struct sockaddr_in server_addr;
static char buffer[BUFFER_SIZE];
static int buffer_pos = 0;
static int buffer_size = 0;

static char debug_printable(char c) {
    if (isprint((unsigned char)c)) {
        return c;
    } else {
        return ' ';
    }
}

static bool tcpsByteAvailableImpl(void) {
    if (buffer_pos < buffer_size) {
        return true;
    }

    buffer_pos = 0;
    buffer_size = recv(sockfd, buffer, BUFFER_SIZE, 0);
    if (buffer_size > 0) {
        return true;
    }
    return false;
}

static uint8_t tcpReadByteImpl(void) {
    if (tcpsByteAvailableImpl()) {
        if (buffer_pos < buffer_size) {
            char c = buffer[buffer_pos];
            printf("TCP(r):  %02X | %c \n", c, debug_printable(c));
            return (uint8_t)buffer[buffer_pos++];
        }
    }
    return 0;
}

static void tcpWriteByteImpl(uint8_t b) {
    if (sockfd != INVALID_SOCKET) {
        char byte = (char)b;
        send(sockfd, &byte, 1, 0);
    }
    printf("TCP(w): %02X | %c \n", b, debug_printable(b));
}

static void tcpSetSerialFormatImpl(uint32_t baudRate, uint32_t protocol) {
    printf("TCP Setting Serial to %d baud protocol %d.\n", baudRate, protocol);
}

static void tcpCleanupImpl(void) {
    if (sockfd != INVALID_SOCKET) {
        closesocket(sockfd);
        sockfd = INVALID_SOCKET;
    }
    WSACleanup();
}

static bool openTcpSocket() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {
        printf("WSAStartup failed\n");
        return false;
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == INVALID_SOCKET) {
        printf("Error creating socket: %d\n", WSAGetLastError());
        WSACleanup();
        return false;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(TCP_PORT);

    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        printf("Error connecting to server: %d\n", WSAGetLastError());
        closesocket(sockfd);
        WSACleanup();
        return false;
    }

    u_long mode = 1;
    ioctlsocket(sockfd, FIONBIO, &mode);

    printf("TCP Connected to server on port %d\n", TCP_PORT);
    return true;
}

// Static SerialInterface instance
static const SerialInterface gSerialInterfaceIP232 = {
    .isByteAvailable = tcpsByteAvailableImpl,
    .readByte = tcpReadByteImpl,
    .writeByte = tcpWriteByteImpl,
    .setSerialFormat = tcpSetSerialFormatImpl,
    .cleanup = tcpCleanupImpl
};

// STUB implementation
static bool stubIsByteAvailableImpl(void) {
    return false;
}

static uint8_t stubReadByteImpl(void) {
    return 0;
}

static void stubWriteByteImpl(uint8_t b) {
    (void)b;
    printf("STUB Serial write %d\n", b);
}

static void stubSetSerialFormatImpl(uint32_t baudRate, uint32_t protocol) {
    (void)baudRate;
    (void)protocol;
    printf("STUB Setting Serial to %d baud protocol %d.\n", baudRate, protocol);
}

static void stubCleanupImpl(void) {
    // Nothing to clean up for stub
}

static const SerialInterface gSerialInterfaceStub = {
    .isByteAvailable = stubIsByteAvailableImpl,
    .readByte = stubReadByteImpl,
    .writeByte = stubWriteByteImpl,
    .setSerialFormat = stubSetSerialFormatImpl,
    .cleanup = stubCleanupImpl
};

// Function to open and return the appropriate SerialInterface
const SerialInterface* SerialInterfaceOpen(enum SerialType type) {
    switch (type) {
        case SER_TCP:
            if (openTcpSocket()) {
                return &gSerialInterfaceIP232;
            }
            printf("Failed to open TCP socket. Falling back to STUB implementation.\n");
            // Fall through to SER_STUB if socket opening fails
        case SER_STUB:
            return &gSerialInterfaceStub;
        default:
            return NULL;
    }
}
