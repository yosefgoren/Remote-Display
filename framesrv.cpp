#include "framesrv.hpp"
#include <cstring>
#include <iostream>
#include <array>
#include <memory>

#pragma comment(lib, "Ws2_32.lib")

FrameServer::FrameServer(std::function<void(frame&)> frame_packet_handler, unsigned portno)
    : frame_packet_handler(frame_packet_handler) {

	WSADATA wsaData;
	int iResult;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		std::cerr << "WSAStartup failed: " << iResult << std::endl;
		throw std::runtime_error("Failed to initialize Winsock");
	}

	// Create a UDP socket
	udp_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (udp_socket == INVALID_SOCKET) {
		std::cerr << "Socket creation failed with error: " << WSAGetLastError() << std::endl;
		WSACleanup();
		throw std::runtime_error("Failed to create socket");
	}

	// Configure the server address structure
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;  // Listen on all interfaces
	server_addr.sin_port = htons(portno);      // Convert port number to network byte order

	// Bind the socket to the port
	iResult = bind(udp_socket, (sockaddr*)&server_addr, sizeof(server_addr));
	if (iResult == SOCKET_ERROR) {
		std::cerr << "Bind failed with error: " << WSAGetLastError() << std::endl;
		closesocket(udp_socket);
		WSACleanup();
		throw std::runtime_error("Failed to bind socket");
	}

	// Set socket to non-blocking mode
	u_long mode = 1;  // 1 to enable non-blocking socket
	iResult = ioctlsocket(udp_socket, FIONBIO, &mode);
	if (iResult == SOCKET_ERROR) {
		std::cerr << "ioctlsocket failed with error: " << WSAGetLastError() << std::endl;
		closesocket(udp_socket);
		WSACleanup();
		throw std::runtime_error("Failed to set non-blocking mode");
	}

	recv_buf = new std::array<char, sizeof(frame_t)>();
}

bool FrameServer::poll() {
	sockaddr_in client_addr;
	int client_addr_len = sizeof(client_addr);

	// Buffer to receive the data
	char* recv_buffer = recv_buf->data();
	int bytes_received =
	        recvfrom(udp_socket, recv_buffer, sizeof(frame_t), 0, (sockaddr*)&client_addr, &client_addr_len);

	if (bytes_received == SOCKET_ERROR) {
		int error = WSAGetLastError();
		if (error == WSAEWOULDBLOCK) {
			// No data available, just return false to indicate no packet was received
			return false;
		} else {
			std::cerr << "recvfrom failed with error: " << error << std::endl;
			return false;
		}
	}

	// If bytes received is less than a full frame, pad with zeros
	if (bytes_received < sizeof(frame_t)) {
		std::memset(recv_buffer + bytes_received, 0, sizeof(frame_t) - bytes_received);
	}

	// Invoke the frame packet handler callback with the received frame
	frame_packet_handler(*reinterpret_cast<frame_t*>(recv_buf));

	return true;
}

FrameServer::~FrameServer() {
	closesocket(udp_socket);
	WSACleanup();
	delete recv_buf;
}
