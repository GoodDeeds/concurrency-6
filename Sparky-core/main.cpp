

#include <iostream>
#include "MainGame.h"
#include "BasicGame.h"

#include "BasicGameServer.h"
#include <Bengine/Errors.h>
#include "Sockets.h"
#include <thread>
#include <vector>
#include "player.h"

void processString(std::string & input, std::string & name2, int & indexOfClient, int & noOfPlayers, std::vector<Player> & players);

int main(int argc, char** argv)
{
	int noOfPlayers = 0;
	int indexOfClient = 0;
	std::vector<Player> players;                      // the vector of our players, containes their attribute
	char name[100];
	std::cout << "Enter your username\n";
	std::cin >> name;
	std::string name2;
	name2 = std::string(name);
	
	std::cout << "Enter your character choice 0-5\n";
	char playerChoice[100];
	std::cin >> playerChoice;
	strcat_s(name, "|");
	strcat_s(name, playerChoice);
	strcat_s(name, "|");


	int choice;
	std::cout << "Enter 1 to be server,2 to be client\n";
	std::cin >> choice;


	std::string SOCK_PORT = "100";
	std::thread sockThread;

	WSADATA info;
	if (WSAStartup(MAKEWORD(2, 0), &info))
		Bengine::fatalError("Could not start WSA");

	if (choice == 1)                            // for server
	{
		int clients;
		std::cout << "Enter number of Clients" << std::endl;
		std::cin >> clients;
		socketServer server(SOCK_PORT, clients, Socket::ConnectionType::NonBlocking, 2048);
		std::string temp(name);
		server.sendData(temp);
		sockThread = std::thread(&socketServer::select_activity, &server);
		while (server.init);
		std::string input = "";
		server.receiveData(input); 
		std::cout <<"receiving in server -- "<< input << std::endl;
		processString(input, name2, indexOfClient, noOfPlayers, players);
		
		BasicGameServer basicGame(noOfPlayers, indexOfClient, players, &server);
		basicGame.run();
		
		sockThread.join();
	}
	else if (choice == 2)                         // for client
	{
		std::cout << "Enter server's IP address\n";
		std::string ip;
		std::cin >> ip;
		socketClient client(ip, SOCK_PORT, 2048);
		char input[1000];
		client.receiveBytes(input);
		std::cout << input << std::endl;		// connected  msg

		std::cout << "name --" << name << std::endl;

		client.sendBytes(name);

		client.receiveBytes(input);
		std::cout << "receiving in client --  " << input << std::endl;		// connected  msg



		std::string temp(input);
		processString(temp, name2, indexOfClient, noOfPlayers, players);
		BasicGame mainGame(noOfPlayers, indexOfClient, players, &client);
		mainGame.run();
		
	}
	int x;
	std::cout << "Enter any character to quit" << std::endl;
	std::cin >> x;
	return 0;
}

// process the input data received, the individual data sets are separated by '|'
void processString(std::string & input, std::string & name2, int & indexOfClient, int & noOfPlayers, std::vector<Player> & players)
{
	int i = 1;
	while (input[i] != '|')
	{
		noOfPlayers = input[i] - '0' + 10 * noOfPlayers;
		i++;
	}
	i++;
	std::string *pname = new std::string[noOfPlayers];
	int *pchoice = new int[noOfPlayers];
	for (int j = 0; j < noOfPlayers; j++)
	{
		pname[j] = "";
		pchoice[j] = 0;
		while (input[i] != '|')
		{
			pname[j] += input[i];
			i++;
		}
		if (name2 == pname[j])
			indexOfClient = j;

		i++;
		std::string temp = "";
		while (input[i] != '|')
		{
			temp += input[i];
			pchoice[j] = std::stoi(temp);
			i++;
		}
		i++;
	}
	glm::vec2 *pos = new glm::vec2[noOfPlayers];

	for (int j = 0; j < noOfPlayers; j++)
	{
		std::string temp = "";
		while (input[i] != ' ')
		{
			temp += input[i];
			i++;
		}
		i++;
		float x = std::stof(temp);
		temp = "";
		while (input[i] != '&')
		{
			temp += input[i];
			i++;
		}
		i++;
		float y = std::stof(temp);
		pos[j] = glm::vec2(x, y);
	}
	for (int j = 0; j < noOfPlayers; j++)
	{
		players.emplace_back(pname[j], pos[j], pchoice[j]);
	}
}

