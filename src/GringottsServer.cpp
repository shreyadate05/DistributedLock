/*
 * GringottsServer.cpp
 *
 *  Created on: Oct 1, 2019
 *      Author: shreya
 */
#include "GringottsServer.h"
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <map>
#include <sstream>
#include <string.h>
#include <algorithm>

using std::vector;
using std::string;
using std::stringstream;

using std::cout;
using std::string;
using std::vector;
using std::map;
using std::endl;
using std::setw;

pthread_mutex_t GringottsServer::lock;
vector<string> transactionQueue;
int iAccess = 0; //0 is available and 1 is locked
std::map<int, int> server_client;

std::string & ltrim(std::string & str)
{
  auto it2 =  std::find_if( str.begin() , str.end() , [](char ch){ return !std::isspace<char>(ch , std::locale::classic() ) ; } );
  str.erase( str.begin() , it2);
  return str;
}

std::string & rtrim(std::string & str)
{
  auto it1 =  std::find_if( str.rbegin() , str.rend() , [](char ch){ return !std::isspace<char>(ch , std::locale::classic() ) ; } );
  str.erase( it1.base() , str.end() );
  return str;
}

std::string & trim(std::string & str)
{
   return ltrim(rtrim(str));
}

void* GringottsServer::connection_handler(void* args)
{
	SocketData* newSocket = static_cast<SocketData*>(args);

	while (isLocked())
	{
		wait(newSocket->iSocket);
	}

	pthread_mutex_lock(&lock);
	acquireLock();
	cout << "\nClient " << transactionQueue[0] << " got access to file";
	Socket::sendData(*newSocket, "1");
	string sRecv;
	Socket::recvData(*newSocket, sRecv); //recv is a blocking call
	transactionQueue.erase(transactionQueue.begin());
	cout << "\nClient " << newSocket->iSocket << " updated file";
	releaseLock();
	pthread_mutex_unlock(&lock);

	return NULL;
}

bool GringottsServer::wait(int iSocket)
{
	//cout << "\nClient " << iSocket <<"waiting for access";
	sleep(1);
	return true;
}

bool GringottsServer::isLocked()
{
	return iAccess == 1;
}

bool GringottsServer::acquireLock()
{
	bool res = true;
	iAccess = 1;
	return res;
}

bool GringottsServer::releaseLock()
{
	bool res = true;
	iAccess = 0;
	return res;
}

bool GringottsServer::setHostname(string sHostname)
{
	socketData.sHostname = sHostname;
	return true;
}

bool GringottsServer::setPortNum(int iPortNum)
{
	socketData.iPortNum = iPortNum;
	return true;
}

bool GringottsServer::startServer()
{
	bool res = true;

	if ( ! Socket::createSocket(socketData) )
	{
		cout << "\nError in " << __func__;
		return false;
	}

	if ( ! Socket::bind(socketData) )
	{
		cout << "\nError in " << __func__;
		return false;
	}

	if ( ! Socket::listen(socketData) )
	{
		cout << "\nError in " << __func__;
		return false;
	}

	return res;
}

bool GringottsServer::runServer()
{
	bool res = true;
	int i = 0;
	while(true)
	{
		SocketData* newSocket = new SocketData();
		newSocket->iPortNum = socketData.iPortNum;
		newSocket->sHostname = socketData.sHostname;
		pthread_t threadId;
		string sClientData;
		char c = ' ';

		Socket::accept(socketData, *newSocket);
		Socket::recvData(*newSocket, sClientData);
		cout << "\nServer received message: \"" << sClientData << "\"";
		transactionQueue.push_back(sClientData.c_str());

		if(pthread_create(&threadId, NULL, GringottsServer::connection_handler, (void*)(newSocket)))
		{
			cout << "\nError in " << __func__;
			return false;
		}

		sleep(0.5);
		i++;
	}
	return res;
}

bool GringottsServer::exitServer()
{
	bool res = true;

	return res;
}

vector<string> GringottsServer::StringSplitByDelim(std::string str, char& cDeLim)
{
	vector<string> vTokens;
	stringstream ss(str);
	string item;

	while (std::getline(ss, item, cDeLim))
	{
		if(item != " " || item != "\n" || item!= "\r")
			vTokens.push_back (item);
	}

//	for (int i=0; i< vTokens.size(); i++)
//	{
//		cout << "\nvToken: " << vTokens[i];
//	}

	return vTokens;
}




