#pragma once

#include "ServerConnection.h"
#include "NetworkingFactory.h"

inline bool ServerConnection::bindSocket()
{
	/* Bind local address to socket */
	if (bind(_socket, (struct sockaddr *)&_server, sizeof(struct sockaddr_in)) < 0)
	{
		//"Cannot bind address to socket.");
		close(_socket);
		return false;
	}

	return true;
}

ServerConnection::ServerConnection(int port)
	:NetworkConnection(port)
{
}

ServerConnection::~ServerConnection()
{
}

inline void ServerConnection::postSocketCreation()
{
	/* listen on the specified port a max of connections*/
	/* call  applies only to sockets of type SOCK_STREAM or SOCK_SEQPACKET */
	if (listen(_socket, _maxConnections) < 0)
	{
		/*Error on listen*/
		close(_socket);
	}
	/*************************************************************/
	/* Initialize the master fd_set                              */
	/*************************************************************/
	FD_ZERO(&master_set);
	max_sd = _socket;
	// add the listener to the master set
	FD_SET(_socket, &master_set);

	/*************************************************************/
	/* Initialize the timeval struct to 3 minutes.  If no        */
	/* activity after 3 minutes this program will end.           */
	/*************************************************************/
	timeout.tv_sec = 3 * 60;
	timeout.tv_usec = 0;
}

int ServerConnection::checkSelect(int max_sd, fd_set& working_set, timeval& timeout) {
	/**********************************************************/
	/* Call select() and wait for it to complete.   */
	/**********************************************************/
	printf("Waiting on select()...\n");
	int rc = select(max_sd + 1, &working_set, NULL, NULL, &timeout);

	/**********************************************************/
	/* Check to see if the select call failed.                */
	/**********************************************************/
	if (rc < 0)
	{
		perror("  select() failed");
		return -1;
	}

	/**********************************************************/
	/* Check to see if the time out expired.         */
	/**********************************************************/
	if (rc == 0)
	{
		printf("  select() timed out.  End program.\n");
		return -1;
	}
	return rc;
}

void ServerConnection::setToNonBlocking() {
	unsigned long on = 1;
	int rc = -1;
#if _WIN32

	rc = setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on));
	//windows stuff to make socket non blocking
	if (rc >= 0)
		rc = ioctlsocket(_socket, FIONBIO, &on);

#else
	//make socket non blocking
	setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on));
	rc = ioctl(_socket, FIONBIO, (char *)&on);
#endif
	if (rc < 0) {
		/*error making non blocking socket*/
		close(_socket);
	}
}