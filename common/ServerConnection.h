#pragma once

#include "NetworkConnection.h"
#include <vector>

class EXPORT  ServerConnection :public NetworkConnection
{
	friend class NetworkingFactory;
private:

	int i, rc, len;
	struct timeval       timeout;
	struct fd_set        master_set, working_set;
	unsigned int _maxConnections = FD_SETSIZE;
	int     max_sd, new_sd;

	int checkSelect(int max_sd, fd_set& working_set, timeval& timeout);
	ServerConnection(int port);
protected:
	virtual void setToNonBlocking() override;
	virtual bool bindSocket() override;
	virtual void postSocketCreation() override;
public:

	~ServerConnection();

	template <typename T>
	std::map<int, T*> readAsync();
	std::vector<int> socketsToClose;
};

template <typename T>
inline std::map<int, T*> ServerConnection::readAsync()
{
	if (!std::is_base_of<ISerializable, T>::value)
	{
		throw std::exception("T base type must be of type ISerializable");
	}

	std::map<int, T*> dataOfSocket;

	int desc_ready;
	/**********************************************************/
	/* Copy the master fd_set over to the working fd_set.     */
	/**********************************************************/
	memcpy(&working_set, &master_set, sizeof(master_set));

	rc = checkSelect(max_sd, working_set, timeout);
	if (rc < 1) return dataOfSocket;
	/**********************************************************/
	/* One or more descriptors are readable.  Need to         */
	/* determine which ones they are.                         */
	/**********************************************************/
	desc_ready = rc;
	int i;
	for (i = 0; i <= max_sd && desc_ready > 0; ++i)
	{
		/*******************************************************/
		/* Check to see if this descriptor is ready            */
		/*******************************************************/
		if (FD_ISSET(i, &working_set))
		{
			/****************************************************/
			/* A descriptor was found that was readable - one   */
			/* less has to be looked for.  This is being done   */
			/* so that we can stop looking at the working set   */
			/* once we have found all of the descriptors that   */
			/* were ready.                                      */
			/****************************************************/
			desc_ready -= 1;

			/****************************************************/
			/* Check to see if this is the listening socket     */
			/****************************************************/
			if (i == _socket)
			{
				printf("  Listening socket is readable\n");
				/*************************************************/
				/* Accept all incoming connections that are      */
				/* queued up on the listening socket before we   */
				/* loop back and call select again.              */
				/*************************************************/
				do
				{
					/**********************************************/
					/* Accept each incoming connection.  If       */
					/* accept fails with EWOULDBLOCK, then we     */
					/* have accepted all of them.  Any other      */
					/* failure on accept will cause us to end the */
					/* server.                                    */
					/**********************************************/
					new_sd = accept(_socket, NULL, NULL);
					if (new_sd < 0)
					{
						if (errno != EWOULDBLOCK)
						{
							perror("  accept() failed");
							return dataOfSocket;
						}
						break;
					}

					/**********************************************/
					/* Add the new incoming connection to the     */
					/* master read set                            */
					/**********************************************/
					printf("  New incoming connection - %d\n", new_sd);
					//read
					FD_SET(new_sd, &master_set);
					if (new_sd > max_sd)
						max_sd = new_sd;

					/**********************************************/
					/* Loop back up and accept another incoming   */
					/* connection                                 */
					/**********************************************/
				} while (new_sd != -1);
			}

			/****************************************************/
			/* This is not the listening socket, therefore an   */
			/* existing connection must be readable             */
			/****************************************************/
			else
			{
				printf("  Descriptor %d is readable\n", i);
				/*************************************************/
				/* Receive all incoming data on this socket      */
				/* before we loop back and call select again.    */
				/*************************************************/
				do
				{
					/**********************************************/
					/* Receive data on this connection until the  */
					/* recv fails with EWOULDBLOCK.  If any other */
					/* failure occurs, we will close the          */
					/* connection.                                */
					/**********************************************/
					char buffer[MAX_BUFFER_SIZE];
					rc = recv(i, buffer, MAX_BUFFER_SIZE, 0);
					if (rc < 0)
					{
						if (errno != EWOULDBLOCK)
						{
							perror("  recv() failed");
							close_conn = TRUE;
						}
						break;
					}

					/**********************************************/
					/* Check to see if the connection has been    */
					/* closed by the client                       */
					/**********************************************/
					if (rc == 0)
					{
						printf("  Connection closed\n");
						close_conn = TRUE;
						break;
					}

					/**********************************************/
					/* Data was received                          */
					/**********************************************/
					len = rc;
					printf("  %d bytes received\n", len);
					dataOfSocket[i] = new T();
					dataOfSocket[i]->deserialize(buffer);
					bool addForFutureClose = false;
					for each (int sockt in socketsToClose)
					{
						if (sockt == i) {
							addForFutureClose = false;
							break;
						}
						else
						{
							addForFutureClose = true;
						}
					}
					if (addForFutureClose)socketsToClose.push_back(i);
				} while (TRUE);

				/*************************************************/
				/* If the close_conn flag was turned on, we need */
				/* to clean up this active connection.  This     */
				/* clean up process includes removing the        */
				/* descriptor from the master set and            */
				/* determining the new maximum descriptor value  */
				/* based on the bits that are still turned on in */
				/* the master set.                               */
				/*************************************************/
				//if (close_conn)
				//{
				//	close(i);
				//	FD_CLR(i, &master_set);
				//	if (i == max_sd)
				//	{
				//		while (FD_ISSET(max_sd, &master_set) == FALSE)
				//			max_sd -= 1;
				//	}
				//}
			} /* End of existing connection is readable */
		} /* End of if (FD_ISSET(i, &working_set)) */
	} /* End of loop through selectable descriptors */

	return dataOfSocket;
}