

#include <iostream>
#include "MainGame.h"

#include <Bengine/Errors.h>
#include "Sockets.h"
#include <thread>
#include <vector>


int main(int argc, char** argv)
{
	int noOfPlayers = 0;
	int indexOfClient = 0;
	//std::vector<Player> players;
	char name[100];
	std::cout << "Enter your username\n";
	std::cin >> name;
	std::string name2;
	name2 = std::string(name);


	int choice;
	std::cout << "Enter 1 to be server,2 to be client\n";
	std::cin >> choice;


	std::string SOCK_PORT = "100";
	std::thread sockThread;

	WSADATA info;
	if (WSAStartup(MAKEWORD(2, 0), &info))
		Bengine::fatalError("Could not start WSA");

	if (choice == 1)
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
		std::cout << input << std::endl;
		//processString(input, name2, indexOfClient, noOfPlayers, players);
		//SimpleGameServer simpleGame(noOfPlayers, indexOfClient, players, &server);
		//simpleGame.run();
		std::cout << "I am here client " << std::endl;
		MainGame mainGame;
		mainGame.run();

		sockThread.join();
	}
	else if (choice == 2)
	{
		std::cout << "Enter server's IP address\n";
		std::string ip;
		std::cin >> ip;
		socketClient client(ip, SOCK_PORT, 2048);
		char input[1000];
		client.receiveBytes(input);
		std::cout << input;	//connected msg

		client.sendBytes(name);

		client.receiveBytes(input);
		std::cout << input << std::endl;
		std::cout << "I am here client " << std::endl;
		//processString(std::string(input), name2, indexOfClient, noOfPlayers, players);
		//SimpleGame mainGame(noOfPlayers, indexOfClient, players, &client);
		MainGame mainGame;
		mainGame.run();
	}
	int x;
	std::cout << "Enter any character to quit" << std::endl;
	std::cin >> x;
	return 0;
}

/*MainGame maingame;
maingame.run();

return 0;
}*/