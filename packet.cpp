#include "pch.h"
#include "packet.h"

packet::packet()
{
	id = "";
	czas = "";
	oper = "";
	odp = "";
	strzal = "";
	przedzial = "";
	pakiet = "";
}

packet::packet(std::string id_, std::string czas_, std::string oper_, std::string odp_, std::string strzal_, std::string przedzial_)
{
	id = "";
	czas = "";
	oper = "";
	odp = "";
	strzal = "";
	przedzial = "";

	int licznik = 1;
	pakiet = "";

	if (id_.size() > 0) { id = id_; pakiet += "^" + std::to_string(licznik) + "::Id{" + id + "}+"; licznik++; }
	if (czas_.size() > 0) { czas = czas_; pakiet += "^" + std::to_string(licznik) + "::Czas{" + czas + "}+"; licznik++; }
	if (oper_.size() > 0) { oper = oper_; pakiet += "^" + std::to_string(licznik) + "::Oper{" + oper + "}+"; licznik++; }
	odp = odp_; pakiet += "^" + std::to_string(licznik) + "::Odp{" + odp + "}+"; licznik++;
	if (strzal_.size() > 0) { strzal = strzal_; pakiet += "^" + std::to_string(licznik) + "::Strzal{" + strzal + "}+"; licznik++; }
	if (przedzial_.size() > 0) { przedzial = przedzial_; pakiet += "^" + std::to_string(licznik) + "::Przedzial{" + przedzial + "}+"; licznik++; }

}

std::string packet::getp() const {
	return pakiet;
}


void packet::setp(std::string p) {
	pakiet = p;
}

void packet::unpack()
{
	int petle = 0;

	for (int i = 0; i < pakiet.size(); i++) {
		std::string tmp = "^";
		if (pakiet.substr(i, 1) == tmp) { petle++; }
	}

	int k = 0;
	
	for (int j = 1; j <= petle; j++) {

		std::string buf = "";
		std::string bufvalue = "";
		std::string tmp = "^" + std::to_string(j) + "::";

		for (int p = 0; p < pakiet.size(); p++) {
			if (pakiet.substr(p, 4) == tmp) {
				for (int i = p + 4; i < pakiet.size();i++) {
					if(pakiet.substr(i, 1) != "+") {
						buf += pakiet[i];
						//std::cout << "buf: " << buf;
					}
					else break;
				}

					int klamra;
					for (int m = 0; m < buf.size(); m++) {
						if (buf.substr(m, 1) == "{") {
							klamra = m + 1;
							for (int n = klamra; n < buf.size(); n++) {
								if (buf.substr(n, 1) != "}") {
									bufvalue += buf.substr(n, 1);
								}
							}
						}
					}

					if (buf.substr(0, 2) == "Id") { id = bufvalue; }
					else if (buf.substr(0, 2) == "Cz") { czas = bufvalue; }
					else if (buf.substr(0, 2) == "Op") { oper = bufvalue; }
					else if (buf.substr(0, 2) == "Od") { odp = bufvalue; }
					else if (buf.substr(0, 2) == "St") { strzal = bufvalue; }
					else if (buf.substr(0, 2) == "Pr") { przedzial = bufvalue; }
				
			}
		}
	}
}

void packet::display()
{
	std::cout << std::endl << "Id: " << id << std::endl;
	std::cout << "Czas: " << czas << std::endl;
	std::cout << "Operacja: " << oper << std::endl;
	std::cout << "Odpowiedz: " << odp << std::endl;
	std::cout << "Strzal: " << strzal << std::endl;
	std::cout << "Przedzial: " << przedzial << std::endl;
}


packet::~packet()
{
}
