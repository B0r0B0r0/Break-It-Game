Nume: Bordei Alin-Viorel
Grupa: C111A

Break It

Detalii la instalare:
Fisierele "Help.txt" si "Matrice_menu.txt" trebuie introduse in acelasi folder cu "Source.cpp"

Logica Jocului:
In joc exista doua matrici. Prima se numeste scena si este initializata cu cadranul, mingea 
si bara.
A doua matrice se numeste caramizi.
A doua matrica are jumatate din inaltimea primei matrici, dar este la fel de lunga. In aceasta
se initializeaza toate caramizile.
La inceput, toate caramizile sunt caramize destructibile, dupa care, printr-un random, aproximativ
cinci caramizi speciale pe linie (in general, doua indestructibile, doua destructibile la a doua coliziune
si una verde, ce ofera o putere speciala) sunt distribuite in toata matricea.
Numarul de caramizi (atat cele normale, cat si cele cu proprietati speciale) depind de marimea cadranului,
astfel, daca marimile sunt modificate din cod, din #define DIM_X si #define DIM_Y, jocul se va recalibra
singur, fara a fi necesara modificarea altei linii de cod.
Pentru buna rulare a mingii, aceasta a avut nevoie de patru variabile globale, minge_x, minge_y (care
tin minte coordonatele x si y in matrice), viteza_mminge_x si viteza_minge_y (care dicteaza unde
se va duce mingea in urmatoarea executare a loop-ului while din main).
Bara foloseste acelasi principiu de miscare ca la minge, doar ca a fost nevoie doar de o variabila globala
care sa tina minte pozitia pe orizontala. De mentionat este ca bara se misca cu doua unitati, in timp ce
mingea se misca cu una, astfel, bara este suficient de rapida sa prinda mingea.
Daca mingea atinge o bara verde, este apelata o functie care face un random intre 1 si 3.
Acest numar indica ce putere va primi jucatorul.
Mingea nu diferentiaza intre bari, ci apeleaza o functie (care recunoaste bara si actioneaza in functie de
proprietati) daca atinge una.

Probleme intampinate:
Modificarea vitezei. Initial am incercat printr-un sleep, dictat de o variabila globala, modificata prin taste.
Nu a mers. Apoi am incercat printr-un loop gol, in care doar cresteam o variabila si verificam tastele. Nu a
mers. Apoi am incercat printr-un if care in loop-ul while din main, printr-o variabila modificata de user,
care trebuia sa fie diferita de o variabila care crestea constant. Nu a mers. Apoi am incercat din nou 
printr-un sleep dictat de o variabila globala, modificata prin taste si a mers.
O alta problema am avut la rachete, acestea intrau in coliziune cu mingea. Am rezolvat problema schimband
conditia If-ului.


 