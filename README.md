# textprotocol
 Komunikacja pomiędzy klientami poprzez serwer (2:1), w oparciu o autorski protokół tekstowy.
 
 Protokół połączeniowy, wszystkie dane przesyłane w postaci tekstowej (sekwencja znaków ASCII).
 
Format komunikatu:
^1::Pole{wartosc}+^2::Pole{wartosc}+ …

np. ^1::Oper{Nadanie Id}+^2::Id{2367}+^3::Czas{13:45:27}+

Pola: (* - pole obowiązkowe)
   ● Czas
   ● Oper * - operacja, którą wykonuje klient lub serwer przesyłając komunikat:
   
○ Nadanie Id - serwer wysyła Id klientowi;

○ Wysłanie L - klient wysyła do serwera liczbę L;

○ Wysłanie przedziału - serwer wysyła do klienta przedział, w którym zawiera się wylosowana przez niego liczba;

○ Wysłanie strzału - klient wysyła do serwera liczbę, którą podejrzewa o bycie wylosowaną;

○ Wysłanie odpowiedzi - serwer wysyła do klienta odpowiedź o przegranej/wygranej lub podpowiedź na temat wylosowanej liczby.

● Id * - identyfikator sesji;
● Przedział - przedział liczbowy wyznaczany przez serwer - znajduje się
wylosowana przez niego liczba, którą musi odgadnąć klient.
● Strzał - strzał, który klient wysyła do serwera.
● Odp * - odpowiedź serwera do klienta:

○ Wygrana - klient wygrał grę;

○ Przegrana - klient przegrał grę;

○ Odejmij - liczba wylosowana przez serwer jest mniejsza niż strzał klienta;

○ Dodaj - liczba wylosowana przez serwer jest większa niż strzał klienta.

Schemat sesji komunikacyjnej:
1. Dwóch klientów łączy się z serwerem, każdy z nich dostaje od serwera
komunikat z czasem i identyfikatorem sesji.
2. Każdy z klientów wysyła do serwera komunikat z czasem, identyfikatorem
sesji i liczbą L.
3. Serwer wysyła do każdego z klientów komunikat z czasem, identyfikatorem i
przedziałem, w którym zawiera się wylosowana przez niego liczba.
4. Klienci rozpoczynają grę - każdy z nich wysyła do serwera komunikat z
czasem, identyfikatorem i liczbą - strzałem zawierającym się w przedziale.
5. Serwer odpowiada komunikatem z czasem, identyfikatorem i informacją o
przegranej, wygranej lub o tym, czy wylosowana przez niego liczba jest
mniejsza lub większa od strzału wysłanego przez danego klienta w
poprzednim komunikacie.
6. Po wygranej któregoś z klienta drugi klient otrzymuje komunikat o swojej
przegranej.

Przykład:
1. Klient 1 łączy się z serwerem i dostaje od niego identyfikator sesji 1.
Klient 2 łączy się z serwerem i dostaje od niego identyfikator sesji 2.
2. Klient 1 wysyła do serwera liczbę L = 50.
Klient 2 wysyła do serwera liczbę L = 30.
3. Serwer wysyła do obu klientów komunikat z przedziałem (-20; 80), w którym
zawiera się wylosowana przez niego liczba 33.
4. Klient 1 wysyła komunikat ze strzałem 35.
5. Serwer wysyła do klienta 1 informację, że wylosowana przez niego liczba jest
mniejsza.
6. Klient 2 wysyła komunikat ze strzałem 70.
7. Serwer wysyła do klienta 2 informację, że wylosowana przez niego liczba jest
mniejsza.
8. Klient 1 wysyła komunikat ze strzałem 33.
9. Serwer wysyła do klienta 1 informację o wygranej.
10.Serwer wysyła do klienta 2 informację o przegranej.
