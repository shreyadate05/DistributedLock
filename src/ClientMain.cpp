/*
 * ClientMain.cpp
 *
 *  Created on: Oct 1, 2019
 *      Author: shreya
 */

#include "GringottsClient.h"
#include <iostream>

int main(int argc, char* argv[])
{
	GringottsClient client;
	if (argc != 3)
	{
		std::cout << "Command to run client: ./client <port_num> <num_clients>\n";
		return -1;
	}

	client.runClient(atoi(argv[1]),"localhost", atoi(argv[2]));

	std::cout << std::endl;
	return 0;
}
