/*
 * ServerMain.cpp
 *
 *  Created on: Oct 1, 2019
 *      Author: shreya
 */

#include "GringottsServer.h"
#include <string.h>

using std::cout;

int main(int argc, char* argv[])
{
	GringottsServer server;

	if (argc != 2)
	{
		cout << "Command to run server: ./server <port_num> \n";
		return -1;
	}

	server.setPortNum(atoi(argv[1]));

	if(!server.startServer())
	{
		std::cout << "\nError in " << __func__;
		return -1;
	}

	if(!server.runServer())
	{
		std::cout << "\nError in " << __func__;
		return -1;
	}

	if (!server.exitServer())
	{
		std::cout << "\nError in " << __func__;
		return -1;
	}

	std::cout << std::endl;
	return 0;
}
