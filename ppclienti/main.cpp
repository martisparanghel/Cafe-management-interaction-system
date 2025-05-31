#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm> // Pentru std::transform si std::find_if, std::remove_if
#include <cctype>    // Pentru std::tolower
#include <iomanip>   // Pentru std::fixed si std::setprecision
#include <limits>    // Pentru std::numeric_limits

// Includem headerele comune din folderul partajat
#include "../common_headers/Bautura.hpp"
#include "../common_headers/Produs.hpp"    // Pentru ProdusComanda
#include "../common_headers/Data.hpp"
#include "../common_headers/Comanda.hpp"

// Constante pentru caile catre fisierele de date
const std::string MENU_FILE = "../data/meniu.txt";
const std::string COS_FILE = "../data/cos.txt";
const std::string ORDERS_FILE = "../data/comenzi.txt";

// --- Functie ajutatoare pentru conversia la litere mici ---
std::string to_lower_case(std::string str) {
    std::transform(str.begin(), str.end(), str.begin(),
                   [](unsigned char c){ return std::tolower(c); });
    return str;
}

// --- Prototypes pentru functiile de baza ale clientului ---
void afiseaza_meniu_pentru_client_impl();
void afiseaza_cos_impl();
void adauga_in_cos_impl(const std::string& denumire, int cantitate);
void sterge_din_cos_impl(const std::string& denumire);
void modifica_cantitate_impl(const std::string& denumire, int cantitate_noua);
void plaseaza_comanda_impl();
void afiseaza_total_impl();

// --- Definitii pentru functii ajutatoare (helper) ---
std::vector<Bautura> citesteMeniuDinFisier() {
    std::vector<Bautura> meniu;
    std::ifstream fisier(MENU_FILE);
    if (!fisier.is_open()) {
        return meniu;
    }
    Bautura b;
    while (fisier >> b) {
        meniu.push_back(b);
    }
    fisier.close();
    return meniu;
}

std::vector<ProdusComanda> citesteCosDinFisier() {
    std::vector<ProdusComanda> cos;
    std::ifstream fisier(COS_FILE);
    if (!fisier.is_open()) {
        return cos;
    }
    ProdusComanda pc;
    while (fisier >> pc) {
        cos.push_back(pc);
    }
    fisier.close();
    return cos;
}

void scrieCosInFisier(const std::vector<ProdusComanda>& cos) {
    std::ofstream fisier(COS_FILE);
    if (!fisier.is_open()) {
        std::cerr << "Eroare: Nu s-a putut deschide fisierul cos.txt (" << COS_FILE << ") pentru scriere." << std::endl;
        return;
    }
    for (const auto& pc : cos) {
        fisier << pc << std::endl;
    }
    fisier.close();
}

void adaugaComandaInFisier(const Comanda& comanda) {
    std::ofstream fisier(ORDERS_FILE, std::ios_base::app);
    if (!fisier.is_open()) {
        std::cerr << "Eroare: Nu s-a putut deschide fisierul comenzi.txt (" << ORDERS_FILE << ") pentru adaugare comanda." << std::endl;
        return;
    }
    fisier << comanda;
    fisier.close();
}

// --- Implementarea functiei pentru afisarea meniului clientului ---
void afiseaza_meniu_pentru_client_impl() {
    std::vector<Bautura> meniu = citesteMeniuDinFisier();
    if (meniu.empty()) {
        std::cout << "Meniul este momentan indisponibil sau gol." << std::endl;
        return;
    }
    std::cout << "--- Meniu Cafenea ---" << std::endl;
    for (const auto& b : meniu) {
        std::cout << b.denumire << " - " << b.pret << " RON" << std::endl;
    }
    std::cout << "---------------------" << std::endl;
}

// --- Implementari COMPLETE pentru functiile clientului (cu case-insensitivity) ---
void afiseaza_cos_impl() {
    std::vector<ProdusComanda> cos = citesteCosDinFisier();
    if (cos.empty()) {
        std::cout << "Cosul este gol." << std::endl;
        return;
    }
    std::cout << "--- Cosul de Cumparaturi ---" << std::endl;
    for (const auto& pc : cos) {
        std::cout << "- " << pc.bautura.denumire << ", Cantitate: " << pc.cantitate << std::endl;
    }
    std::cout << "----------------------------" << std::endl;
}

void adauga_in_cos_impl(const std::string& denumire_introdusa, int cantitate) {
    std::string denumire_introdusa_lower = to_lower_case(denumire_introdusa);

    std::vector<Bautura> meniu = citesteMeniuDinFisier();
    auto it_meniu = std::find_if(meniu.begin(), meniu.end(), [&](const Bautura& b_meniu){
        return to_lower_case(b_meniu.denumire) == denumire_introdusa_lower;
    });

    if (it_meniu == meniu.end()) {
        std::cout << "Produsul '" << denumire_introdusa << "' nu exista in meniu." << std::endl;
        return;
    }

    std::string denumire_canonica_din_meniu = it_meniu->denumire; // Denumirea oficiala din meniu

    std::vector<ProdusComanda> cos = citesteCosDinFisier();
    auto it_cos = std::find_if(cos.begin(), cos.end(), [&](const ProdusComanda& pc){
        return to_lower_case(pc.bautura.denumire) == denumire_introdusa_lower; // Comparam case-insensitive
    });

    if (it_cos != cos.end()) { // Produsul exista deja in cos
        it_cos->bautura.denumire = denumire_canonica_din_meniu; // Asiguram denumirea canonica
        it_cos->cantitate += cantitate;
        std::cout << "Cantitatea pentru '" << denumire_canonica_din_meniu << "' a fost actualizata in cos." << std::endl;
    } else { // Produsul nu exista in cos, il adaugam
        ProdusComanda pc_nou;
        pc_nou.bautura.denumire = denumire_canonica_din_meniu; // Stocam denumirea canonica
        pc_nou.cantitate = cantitate;
        cos.push_back(pc_nou);
        std::cout << "Produsul '" << denumire_canonica_din_meniu << "' (cantitate: " << cantitate << ") a fost adaugat in cos." << std::endl;
    }

    scrieCosInFisier(cos);
}

void sterge_din_cos_impl(const std::string& denumire_introdusa) {
    std::string denumire_introdusa_lower = to_lower_case(denumire_introdusa);
    bool gasit = false;

    std::vector<ProdusComanda> cos = citesteCosDinFisier();

    cos.erase(std::remove_if(cos.begin(), cos.end(), [&](const ProdusComanda& pc){
        if (to_lower_case(pc.bautura.denumire) == denumire_introdusa_lower) {
            gasit = true;
            return true;
        }
        return false;
    }), cos.end());

    if (gasit) {
        scrieCosInFisier(cos);
        std::cout << "Produsul care corespunde cu '" << denumire_introdusa << "' a fost sters din cos." << std::endl;
    } else {
        std::cout << "Produsul '" << denumire_introdusa << "' nu a fost gasit in cos." << std::endl;
    }
}

void modifica_cantitate_impl(const std::string& denumire_introdusa, int cantitate_noua) {
    std::string denumire_introdusa_lower = to_lower_case(denumire_introdusa);
    bool gasit_si_modificat = false;

    std::vector<ProdusComanda> cos = citesteCosDinFisier();
    std::string denumire_canonica_gasita;

    for (auto& pc : cos) {
        if (to_lower_case(pc.bautura.denumire) == denumire_introdusa_lower) {
            // Optional: daca vrem sa fim siguri ca denumirea din cos e cea canonica, o putem actualiza aici.
            // Insa, daca adauga_in_cos stocheaza corect denumirea canonica, nu e neaparat nevoie.
            // Pentru consistenta mesajului afisat:
            denumire_canonica_gasita = pc.bautura.denumire;
            pc.cantitate = cantitate_noua;
            gasit_si_modificat = true;
            break;
        }
    }

    if (gasit_si_modificat) {
        scrieCosInFisier(cos);
        std::cout << "Cantitatea pentru produsul '" << denumire_canonica_gasita << "' a fost modificata la " << cantitate_noua << "." << std::endl;
    } else {
        std::cout << "Produsul '" << denumire_introdusa << "' nu a fost gasit in cos." << std::endl;
    }
}

void plaseaza_comanda_impl() {
    std::vector<ProdusComanda> cos = citesteCosDinFisier();
    if (cos.empty()) {
        std::cout << "Cosul este gol. Nu se poate plasa comanda." << std::endl;
        return;
    }

    Data dataComenzii;
    bool dataValida = false;
    while(!dataValida) {
        std::cout << "Introduceti data comenzii (zi luna an): ";
        if (std::cin >> dataComenzii.zi >> dataComenzii.luna >> dataComenzii.an) {
            // Validare simpla a valorilor (ar putea fi imbunatatita cu zilele lunii, an bisect etc.)
            if (dataComenzii.zi >= 1 && dataComenzii.zi <= 31 &&
                dataComenzii.luna >= 1 && dataComenzii.luna <= 12 &&
                dataComenzii.an >= 2000 && dataComenzii.an <= 2100) { // Limite rezonabile pentru an
                 dataValida = true;
            } else {
                std::cout << "Data contine valori in afara limitelor permise (zi 1-31, luna 1-12, an 2000-2100)." << std::endl;
            }
        } else {
            std::cout << "Input invalid pentru data. Asigurati-va ca introduceti 3 numere separate prin spatiu." << std::endl;
            std::cin.clear(); // Curata flagurile de eroare ale cin
        }
        // Ignora restul liniei de input, indiferent daca a fost valid sau nu, pentru a preveni probleme la urmatoarea citire
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (!dataValida) {
            std::cout << "Reincercati." << std::endl;
        }
    }

    Comanda comandaNoua(dataComenzii);
    comandaNoua.produse = cos;

    adaugaComandaInFisier(comandaNoua);

    std::vector<ProdusComanda> cosGol; // Cream un vector gol
    scrieCosInFisier(cosGol); // Suprascriem fisierul cos.txt cu un continut gol

    std::cout << "Comanda a fost plasata cu succes." << std::endl;
}

void afiseaza_total_impl() {
    std::vector<ProdusComanda> cos = citesteCosDinFisier();
    if (cos.empty()) {
        std::cout << "Cosul este gol. Total: 0.00 RON" << std::endl;
        return;
    }

    std::vector<Bautura> meniu = citesteMeniuDinFisier();
    if (meniu.empty() && !cos.empty()) { // Daca avem produse in cos dar meniul e gol/indisponibil
        std::cout << "Meniul nu este disponibil. Nu se poate calcula totalul." << std::endl;
        return;
    }

    double totalGeneral = 0.0;
    bool eroarePret = false;

    std::cout << "--- Detalii Cos & Total ---" << std::endl;
    for (const auto& pc : cos) {
        auto it_meniu = std::find_if(meniu.begin(), meniu.end(), [&](const Bautura& b){
            // Comparam case-insensitive pentru a gasi pretul, desi in cos ar trebui sa fie denumirea canonica
            return to_lower_case(b.denumire) == to_lower_case(pc.bautura.denumire);
        });

        if (it_meniu != meniu.end()) {
            double pretProdus = it_meniu->pret;
            double subtotalProdus = pretProdus * pc.cantitate;
            totalGeneral += subtotalProdus;
            std::cout << "- " << pc.bautura.denumire << " (Cant: " << pc.cantitate << " x " << pretProdus << " RON) = " << subtotalProdus << " RON" << std::endl;
        } else {
            std::cout << "- " << pc.bautura.denumire << " (Cant: " << pc.cantitate << ") - Produs negasit in meniu, pret necunoscut." << std::endl;
            eroarePret = true;
        }
    }
    std::cout << "----------------------------" << std::endl;
    if (eroarePret) {
         std::cout << "Total general (poate fi incomplet din cauza produselor negasite): " << totalGeneral << " RON" << std::endl;
    } else {
         std::cout << "Total general: " << totalGeneral << " RON" << std::endl;
    }
}


// --- Functia principala (main) pentru client.exe ---
int main(int argc, char* argv[]) {
    std::cout << std::fixed << std::setprecision(2);
    std::cerr << std::fixed << std::setprecision(2);

    if (argc < 2) {
        std::cerr << "Utilizare: ./client.exe <comanda> [optiuni]" << std::endl;
        std::cerr << "Comenzi disponibile:" << std::endl;
        std::cerr << "  afiseaza_meniu" << std::endl;
        std::cerr << "  afiseaza_cos" << std::endl;
        std::cerr << "  adauga_in_cos <denumire_bautura> <cantitate>" << std::endl;
        std::cerr << "  sterge_din_cos <denumire_bautura>" << std::endl;
        std::cerr << "  modifica_cantitate <denumire_bautura> <cantitate_noua>" << std::endl;
        std::cerr << "  plaseaza_comanda" << std::endl;
        std::cerr << "  afiseaza_total" << std::endl;
        return 1;
    }

    std::string command = argv[1];

    if (command == "afiseaza_meniu") {
        if (argc != 2) {
             std::cerr << "Utilizare: ./client.exe afiseaza_meniu" << std::endl; return 1;
        }
        afiseaza_meniu_pentru_client_impl();
    } else if (command == "afiseaza_cos") {
        if (argc != 2) {
             std::cerr << "Utilizare: ./client.exe afiseaza_cos" << std::endl; return 1;
        }
        afiseaza_cos_impl();
    } else if (command == "adauga_in_cos") {
        if (argc != 4) {
            std::cerr << "Utilizare: ./client.exe adauga_in_cos <denumire_bautura> <cantitate>" << std::endl;
            return 1;
        }
        std::string denumire = argv[2];
        int cantitate;
        try {
            cantitate = std::stoi(argv[3]);
            if (cantitate <= 0) {
                std::cerr << "Eroare: Cantitatea trebuie sa fie un numar pozitiv." << std::endl;
                return 1;
            }
        } catch (const std::invalid_argument& e) {
            std::cerr << "Eroare: Format invalid pentru cantitate." << std::endl;
            return 1;
        } catch (const std::out_of_range& e) {
            std::cerr << "Eroare: Valoarea cantitatii este in afara intervalului permis." << std::endl;
            return 1;
        }
        adauga_in_cos_impl(denumire, cantitate);
    } else if (command == "sterge_din_cos") {
        if (argc != 3) {
            std::cerr << "Utilizare: ./client.exe sterge_din_cos <denumire_bautura>" << std::endl;
            return 1;
        }
        std::string denumire = argv[2];
        sterge_din_cos_impl(denumire);
    } else if (command == "modifica_cantitate") {
        if (argc != 4) {
            std::cerr << "Utilizare: ./client.exe modifica_cantitate <denumire_bautura> <cantitate_noua>" << std::endl;
            return 1;
        }
        std::string denumire = argv[2];
        int cantitate_noua;
        try {
            cantitate_noua = std::stoi(argv[3]);
            if (cantitate_noua <= 0) {
                std::cerr << "Eroare: Cantitatea noua trebuie sa fie un numar pozitiv. Pentru stergere folositi sterge_din_cos." << std::endl;
                return 1;
            }
        } catch (const std::invalid_argument& e) {
            std::cerr << "Eroare: Format invalid pentru cantitatea noua." << std::endl;
            return 1;
        } catch (const std::out_of_range& e) {
            std::cerr << "Eroare: Valoarea cantitatii noi este in afara intervalului permis." << std::endl;
            return 1;
        }
        modifica_cantitate_impl(denumire, cantitate_noua);
    } else if (command == "plaseaza_comanda") {
         if (argc != 2) {
             std::cerr << "Utilizare: ./client.exe plaseaza_comanda" << std::endl; return 1;
        }
        plaseaza_comanda_impl();
    } else if (command == "afiseaza_total") {
         if (argc != 2) {
             std::cerr << "Utilizare: ./client.exe afiseaza_total" << std::endl; return 1;
        }
        afiseaza_total_impl();
    }
    else {
        std::cerr << "Comanda necunoscuta: " << command << std::endl;
        return 1;
    }

    return 0;
}