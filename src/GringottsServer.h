/*
 * GringottsServer.h
 *
 *  Created on: Oct 1, 2019
 *      Author: shreya
 */

#ifndef GRINGOTTSSERVER_H_
#define GRINGOTTSSERVER_H_

#include "Socket.h"
#include <pthread.h>
#include <vector>

class GringottsServer
{
	SocketData socketData;
	static pthread_mutex_t lock;

	public:

		bool setHostname(std::string sHostname);
		bool setPortNum(int iPortNum);
		bool startServer();
		bool runServer();
		bool exitServer();

		static void* connection_handler(void* args);
		static bool acquireLock();
		static bool releaseLock();
		static bool isLocked();
		static bool wait(int iSocket);
		static bool grantAccess(int iSocket);
		static std::vector<std::string> StringSplitByDelim(std::string str, char& cDeLim);
};


#endif /* GRINGOTTSSERVER_H_ */
