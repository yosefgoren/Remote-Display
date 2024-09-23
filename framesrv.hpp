#pragma once

#include "frames.hpp"
#include "pollable.hpp"

#include <functional>

#include <winsock2.h>
#include <ws2tcpip.h>

class FrameServer : public Pollable {
	public:
	/**
	* The frame server listens to UDP on port number 'portno' and invokes the 'frame_packet_handler' callback when new frames arrive.
	* All arriving packets are interpreted as a frames.
	* Packets which are too long are trimed, and those which are too shord are padded with zeros.
	*/
	FrameServer(std::function<void(frame&)> frame_packet_handler, unsigned portno = 8080);
	virtual ~FrameServer();

	/**
	* Invoking this function gives the server a chance to check wether new packets (frames) have arrived.
	* This function will NEVER BLOCK the calling thread.
	* 
	* @return wether the server should stop.
	*/
	virtual bool poll();

	private:
	std::function<void(frame&)> frame_packet_handler;
	SOCKET udp_socket;
	sockaddr_in server_addr;
	std::array<char, sizeof(frame_t)>* recv_buf;
};
