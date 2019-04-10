#include "pch.h"
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma warning(disable:4996)
#include <iostream>
#include<string>
#include<WS2tcpip.h>
#include"packet.h"
#include<ctime>

#pragma comment (lib,"Ws2_32.lib")

std::string getTime() {
	time_t t = time(0);
	struct tm* now = localtime(&t);

	std::string tmp = "";
	tmp += std::to_string(now->tm_mday);
	tmp += ".";
	tmp += std::to_string(now->tm_mon+1);
	tmp += ".";
	tmp += std::to_string(now->tm_year + 1900);
	tmp += " ";
	tmp += std::to_string(now->tm_hour);
	tmp += ":";
	tmp += std::to_string(now->tm_min);
	tmp += ":";
	tmp += std::to_string(now->tm_sec);

	return tmp;
}


int main()
{
	
	std::string ipAddress = "192.168.43.146";		//adres IP serwera
	int port = 27656;							//port nasluchujacy serwera

	//inicjalizacja winsocka
	WSAData data;
	WORD ver = MAKEWORD(2, 2);
	int wsResult = WSAStartup(ver, &data);
	if (wsResult != 0) {
		std::cerr << "can't start WinSock, Err #" << wsResult << std::endl;
		return 1;
	}

	//tworzenie socketa
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) {
		std::cerr << "Can't create socket, Err #" << WSAGetLastError() << std::endl;
		WSACleanup();
		return 1;
	}

	//wypelnienie struktury hint
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);


	//polaczenie z serwerem
	int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
	if (connResult == SOCKET_ERROR) {
		std::cerr << "Can't connect to server, Err #" << WSAGetLastError() << std::endl;
		closesocket(sock);
		WSACleanup();
		return 1;
	}

	std::cout << getTime()<<" Polaczono z serwerem!" << std::endl << std::endl;

	//do-while loop wysylanie i odbior danych
	char buf[120];

	//odbierz id
	ZeroMemory(buf, 120);
	int bytesReceived = recv(sock, buf, 120, 0);
	packet ID_PACKET;
	if (bytesReceived > 0) {
		ID_PACKET.setp(buf);
		ID_PACKET.unpack();
		
		//nadanie ID
		if (ID_PACKET.oper == "Nadanie Id") { std::cout << ID_PACKET.czas << " SERVER > Identyfikator sesji: " << ID_PACKET.id; }

		long long Llong;
		bool valid = true;

		//pobierz liczbe L
		do {
			std::cout <<std::endl<< getTime() << " Podaj liczbe L: ";
			std::cin >> Llong;

			if (Llong >= INT32_MAX) {
				std::cout << "Liczba jest za duza.";
				valid = false;
			}
			else if (Llong <= INT32_MIN) {
				std::cout << "Liczba jest za mala.";
				valid = false;
			}
			else valid = true;
		} while (valid == false);

		int L = (int)Llong;

		packet L2_PACKET(ID_PACKET.id, getTime(), "Wyslanie L", "", std::to_string(L), "");
		std::string L2_P = L2_PACKET.getp();

		//wyslij liczbe L
		int sendResult2 = send(sock, L2_P.c_str(), L2_P.size(), 0);

		if (sendResult2 != SOCKET_ERROR) {
			//odbierz przedzial
			ZeroMemory(buf, 120);
			int bytesReceived = recv(sock, buf, 120, 0);
			packet RANGE_PACKET;
			if (bytesReceived > 0) {
				RANGE_PACKET.setp(buf);
				RANGE_PACKET.unpack();

				std::string przedzial = RANGE_PACKET.przedzial;
				int ODP;
				packet SERVER_RES;

				do {
					//pobierz strzal
					long long ODPlong;
					bool valid_ = true;

					do {
						std::cout << getTime() << " Podaj liczbe z przedzialu (" << przedzial << ") : ";
						std::cin >> ODPlong;

						if (ODPlong >= INT32_MAX) {
							std::cout << "Liczba jest za duza." << std::endl;
							valid_ = false;
						}
						else if (ODPlong <= INT32_MIN) {
							std::cout << "Liczba jest za mala." << std::endl;
							valid_ = false;
						}
						else valid_ = true;
					} while (valid_ == false);

					ODP = (int)ODPlong;

					//wyslanie strzalu
					packet GUESS(ID_PACKET.id, getTime(), "Wyslanie strzalu", "",std::to_string(ODP), "");
					std::string GUESS_P = GUESS.getp();

					int sendResult4 = send(sock, GUESS_P.c_str(), GUESS_P.size(), 0);
					if (sendResult4 != SOCKET_ERROR) {
						//czekaj na odpowiedz
						ZeroMemory(buf, 120);
						int bytesReceived = recv(sock, buf, 120, 0);

						if (bytesReceived > 0) {
							SERVER_RES.setp(buf);
							SERVER_RES.unpack();
							if (SERVER_RES.oper == "Wyslanie odpowiedzi"&& SERVER_RES.odp == "Odejmij") std::cout << SERVER_RES.czas << " SERVER > Wylosowana liczba jest mniejsza!" << std::endl;
							else if (SERVER_RES.oper == "Wyslanie odpowiedzi"&&SERVER_RES.odp == "Dodaj") std::cout << SERVER_RES.czas << " SERVER > Wylosowana liczba jest wieksza!" << std::endl;
							else if (SERVER_RES.oper == "Wyslanie odpowiedzi"&&SERVER_RES.odp == "Wygrana") {
								std::cout << SERVER_RES.czas << " SERVER > WYGRALES!" << std::endl;
								closesocket(sock);
								WSACleanup();
							}
							else if (SERVER_RES.oper == "Wyslanie odpowiedzi"&&SERVER_RES.odp == "Przegrana") {
								if (GUESS.strzal != SERVER_RES.strzal) { std::cout << SERVER_RES.czas << " SERVER > PRZEGRALES! Poprawna odpowiedz to " << SERVER_RES.strzal << std::endl; }
								else { std::cout << SERVER_RES.czas << " SERVER > PRZEGRALES! Byles za wolny. " <<std::endl; }
								closesocket(sock);
								WSACleanup();
							}
						}
					}
				} while (SERVER_RES.odp != "Wygrana" && SERVER_RES.odp != "Przegrana");
			}
		}
	}


	//zamknij polaczenie
	connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
	if (connResult == SOCKET_ERROR) {
		closesocket(sock);
		WSACleanup();
	}

	system("PAUSE"); 
	return 0;
}
