/*
 * GringottsClient.cpp
 *
 *  Created on: Oct 1, 2019
 *      Author: shreya
 */

#include "GringottsClient.h"
#include <fstream>
#include <pthread.h>
#include <sstream>

using std::cout;
using std::string;
using std::vector;
using std::map;
using std::ofstream;
using std::ifstream;

int i = 0;

void* GringottsClient::connection_handler(void* args)
{
	ClientData* client = static_cast<ClientData*>(args);
	SocketData newSocket;
	if (!GringottsClient::initClient(newSocket, *client))
	{
		cout << "\nError while creating client in thread " << pthread_self() << "";
	}

	string sReq = std::to_string(newSocket.iSocket);
	Socket::sendData(newSocket, sReq.c_str());

	int iAccess = 0;
	string sAccess;
	Socket::recvData(newSocket, sAccess);
	iAccess = stoi(sAccess);

	if(iAccess == 1)
	{
		cout << "\nAccess to file granted to client " << newSocket.iSocket;

		string sVar;
		ifstream infile("meraki.txt");
		string line;
		int counter;
		if(infile.is_open())
		{
			std::getline(infile, line);
			std::istringstream ss(line);
			ss >> counter;
			infile.close();
		}

		cout << "Initial Counter :" << counter ;
		counter ++;

		cout << "\n[Client " << newSocket.iSocket << "] writing to file";
		ofstream outfile("meraki.txt");
		if(outfile.is_open())
		{
			outfile << counter;
			outfile.close();
		}
		cout << "\nFinal counter value: " << counter;
		cout << "\n[Client " << newSocket.iSocket << "] Updated counter value: " << counter;

    	string sMessage = "Writing to file done";
    	Socket::sendData(newSocket, sMessage);
	}

	return NULL;
}

bool GringottsClient::initClient(SocketData& socketData, ClientData& client)
{
	bool res = true;

	socketData.iPortNum = client.iPortNum;
	socketData.sHostname = client.sHostname;

	if (!Socket::createSocket(socketData))
	{
		cout << "\nError in " << __func__;
		res =  false;
	}

	if (!Socket::connect(socketData))
	{
		cout << "\nError in " << __func__;
		res =  false;
	}
	return res;
}

bool GringottsClient::runClient(int iPortNum, const string sHostname, int iClients)
{
	bool res = true;

	// create 1 threads per transaction
	pthread_t threadIds[iClients];
	ClientData clients[iClients];

	for (int iClientThread = 0; iClientThread < iClients; iClientThread++)
	{
		clients[iClientThread].iPortNum     = iPortNum;
		clients[iClientThread].sHostname    = sHostname;
	}

	for (int iClientThread = 0; iClientThread < iClients; iClientThread++)
	{
		if(pthread_create(&threadIds[iClientThread], NULL, GringottsClient::connection_handler, (void*)(&clients[iClientThread])))
		{
			cout << "\nError in " << __func__;
			return false;
		}
		sleep(0.5);
	}

	for (int i = 0; i < iClients; i++)
	{
		pthread_join(threadIds[i],NULL);
	}

	return res;
}
