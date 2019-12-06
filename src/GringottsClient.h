/*
 * GringottsClient.h
 *
 *  Created on: Oct 1, 2019
 *      Author: shreya
 */

#ifndef GRINGOTTSCLIENT_H_
#define GRINGOTTSCLIENT_H_

#include <unistd.h>
#include <pthread.h>
#include <vector>
#include <map>
#include "SocketDefs.h"
#include "Socket.h"

const int MAXLINES = 500;

typedef struct _ClientData
{
	std::string sHostname;
	int         iPortNum;
} ClientData;

class GringottsClient
{
   public:

	static bool initClient(SocketData& socketData, ClientData& client);
	bool runClient(int iPortNum, const std::string sHostname, int iClients);

	bool printTransactions();

	static void* connection_handler(void* args);
};


#endif /* GRINGOTTSCLIENT_H_ */
