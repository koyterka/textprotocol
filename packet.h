#pragma once
#include<string>
#include<iostream>

class packet
{
public:
	std::string id;
	std::string czas;
	std::string oper; //Nadanie Id / Wyslanie L / Wyslanie przedzialu / Wyslanie odpowiedzi / Wyslanie strzalu
	std::string odp; //od serwera do klienta Wygrana/Przegrana/Dodaj/Odejmij
	std::string strzal;
	std::string przedzial;
	std::string pakiet;

	packet();
	packet(std::string id_, std::string czas_, std::string oper_, std::string odp_, std::string strzal_, std::string przedzial_);
	std::string getp() const;
	void setp(std::string p);
	void unpack();
	void display();
	~packet();
};
