#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <cstdlib>

#include "../common_headers/Bautura.hpp"
#include "../common_headers/Produs.hpp"
#include "../common_headers/Data.hpp"
#include "../common_headers/Comanda.hpp"

const std::string MENU_FILE = "../data/meniu.txt";
const std::string ORDERS_FILE = "../data/comenzi.txt";

// Helper function to read the menu from meniu.txt
std::vector<Bautura> citesteMeniuDinFisier() {
    std::vector<Bautura> meniu;
    std::ifstream fisier(MENU_FILE);
    if (!fisier.is_open()) {
        // std::cerr << "Info: Fisierul meniu.txt nu a fost gasit. Se va crea unul nou la adaugarea de produse." << std::endl;
        return meniu; // Return empty menu
    }
    Bautura b;
    while (fisier >> b) { // Uses overloaded Bautura::operator>>
        meniu.push_back(b);
    }
    fisier.close();
    return meniu;
}

// Helper function to write the menu to meniu.txt
void scrieMeniuInFisier(const std::vector<Bautura>& meniu) {
    std::ofstream fisier(MENU_FILE);
    if (!fisier.is_open()) {
        std::cerr << "Eroare: Nu s-a putut deschide fisierul " << MENU_FILE << " pentru scriere." << std::endl;
        return;
    }
    for (const auto& b : meniu) {
        fisier << b << std::endl; // Uses overloaded Bautura::operator<<
    }
    fisier.close();
}

// Helper function to read orders from comenzi.txt [cite: 4]
std::vector<Comanda> citesteComenziDinFisier() {
    std::vector<Comanda> comenzi;
    std::ifstream fisier(ORDERS_FILE);
    if (!fisier.is_open()) {
        // std::cerr << "Info: Fisierul comenzi.txt nu a fost gasit sau este gol." << std::endl;
        return comenzi;
    }

    std::string linie;
    Comanda comandaCurenta;
    bool inProcesareComanda = false;

    while (std::getline(fisier, linie)) {
        if (linie.empty()) continue; // Skip empty lines

        std::stringstream ss_linie(linie);
        Data dataTest;

        // Try to parse the line as a Date
        // Peek at the first char; if it's not a digit, it's unlikely a date for this format.

        std::stringstream ss_test_date(linie);
        int z, l, a;
        char remaining_char;
        if ( (ss_test_date >> z >> l >> a) && !(ss_test_date >> remaining_char) ) { // Successfully read 3 numbers and nothing else
            if (inProcesareComanda && !comandaCurenta.produse.empty()) {
                comenzi.push_back(comandaCurenta); // Save previous command
            }
            comandaCurenta = Comanda(Data(z,l,a));
            inProcesareComanda = true;
        } else if (inProcesareComanda) { // If not a date and a command is being processed, assume it's a product
            ProdusComanda pc;
            std::stringstream ss_produs(linie);
            if (ss_produs >> pc) { // Uses overloaded ProdusComanda::operator>>
                 comandaCurenta.adaugaProdus(pc);
            } else {
                 std::cerr << "Warning: Format incorect pentru produs in comenzi.txt: " << linie << std::endl;
            }
        } else {
            // Line is not a date and no command is being processed (e.g., product line before any date)
            std::cerr << "Warning: Linie ignorata in comenzi.txt (posibil produs fara data anterioara): " << linie << std::endl;
        }
    }

    // Add the last command being processed if it has products and was initiated
    if (inProcesareComanda && !comandaCurenta.produse.empty()) {
        comenzi.push_back(comandaCurenta);
    }

    fisier.close();
    return comenzi;
}


// Function implementations for cafenea.exe commands

void afiseaza_meniu_impl() {
    std::vector<Bautura> meniu = citesteMeniuDinFisier();
    if (meniu.empty()) {
        std::cout << "Meniul este gol." << std::endl;
        return;
    }
    std::cout << "--- Meniu Cafenea ---" << std::endl;
    for (const auto& b : meniu) {
        // Bautura::operator<< already handles formatting including std::fixed and std::setprecision
        std::cout << b.denumire << " - " << std::fixed << std::setprecision(2) << b.pret << " RON" << std::endl;
    }
    std::cout << "---------------------" << std::endl;
}

void adauga_bautura_impl(const std::string& denumire, double pret) {

    if (pret < 0) {
        std::cerr << "Eroare: Pretul nu poate fi negativ." << std::endl;
        return;
    }
    std::vector<Bautura> meniu = citesteMeniuDinFisier();
    auto it = std::find_if(meniu.begin(), meniu.end(), [&](const Bautura& b_aux){
        return b_aux.denumire == denumire;
    });

    if (it != meniu.end()) {
        std::cout << "Bautura '" << denumire << "' exista deja in meniu. Folositi 'modifica_pret' pentru a actualiza pretul." << std::endl;
        system("cls");
        return;
    }

    meniu.push_back(Bautura(denumire, pret));
    scrieMeniuInFisier(meniu);
    std::cout << "Bautura '" << denumire << "' a fost adaugata in meniu cu pretul " << std::fixed << std::setprecision(2) << pret << " RON." << std::endl;
}

void sterge_bautura_impl(const std::string& denumire) {
    std::vector<Bautura> meniu = citesteMeniuDinFisier();
    auto initial_size = meniu.size();
    meniu.erase(std::remove_if(meniu.begin(), meniu.end(), [&](const Bautura& b_aux){
        return b_aux.denumire == denumire;
    }), meniu.end());

    if (meniu.size() < initial_size) {
        scrieMeniuInFisier(meniu);
        std::cout << "Bautura '" << denumire << "' a fost stearsa din meniu." << std::endl;
    } else {
        std::cout << "Bautura '" << denumire << "' nu a fost gasita in meniu." << std::endl;
    }
}

void modifica_pret_impl(const std::string& denumire, double pret_nou) {
    if (pret_nou < 0) {
        std::cerr << "Eroare: Pretul nou nu poate fi negativ." << std::endl;
        return;
    }
    std::vector<Bautura> meniu = citesteMeniuDinFisier();
    auto it = std::find_if(meniu.begin(), meniu.end(), [&](const Bautura& b_aux){
        return b_aux.denumire == denumire;
    });

    if (it != meniu.end()) {
        it->pret = pret_nou;
        scrieMeniuInFisier(meniu);
        std::cout << "Pretul bauturii '" << denumire << "' a fost modificat la " << std::fixed << std::setprecision(2) << pret_nou << " RON." << std::endl;
    } else {
        std::cout << "Bautura '" << denumire << "' nu a fost gasita in meniu." << std::endl;
    }
}

void afiseaza_comenzi_impl() {
    std::vector<Comanda> comenzi = citesteComenziDinFisier();
    if (comenzi.empty()) {
        std::cout << "Nu exista comenzi plasate." << std::endl;
        return;
    }
    std::cout << "--- Comenzi Plasate ---" << std::endl;
    for (const auto& comanda : comenzi) {
        std::cout << "Data: " << comanda.data.zi << "/" << comanda.data.luna << "/" << comanda.data.an << std::endl;
        if (comanda.produse.empty()) {
            std::cout << "  (Comanda nu contine produse)" << std::endl;
        } else {
            for (const auto& prod : comanda.produse) {
                std::cout << "  - " << prod.bautura.denumire << ", Cantitate: " << prod.cantitate << std::endl;
            }
        }
        std::cout << "-----------------------" << std::endl;
    }
}


int main(int argc, char* argv[]) {
    // Ensure cout prints doubles with 2 decimal places by default for prices
    std::cout << std::fixed << std::setprecision(2);
    std::cerr << std::fixed << std::setprecision(2);


    if (argc < 2) {
        std::cerr << "Utilizare: ./cafenea.exe <comanda> [optiuni]" << std::endl;
        std::cerr << "Comenzi disponibile:" << std::endl;
        std::cerr << "  afiseaza_meniu" << std::endl;
        std::cerr << "  adauga_bautura <denumire> <pret>" << std::endl;
        std::cerr << "  sterge_bautura <denumire>" << std::endl;
        std::cerr << "  modifica_pret <denumire> <pret_nou>" << std::endl;
        std::cerr << "  afiseaza_comenzi" << std::endl;
        return 1;
    }

    std::string command = argv[1];

    if (command == "afiseaza_meniu") {
        if (argc != 2) {
             std::cerr << "Utilizare: ./cafenea.exe afiseaza_meniu" << std::endl; return 1;
        }
        afiseaza_meniu_impl();
    } else if (command == "adauga_bautura") {
        if (argc != 4) {
            std::cerr << "Utilizare: ./cafenea.exe adauga_bautura <denumire> <pret>" << std::endl;
            return 1;
        }
        std::string denumire = argv[2];
        double pret;
        try {
            pret = std::stod(argv[3]);
        } catch (const std::invalid_argument& e) {
            std::cerr << "Eroare: Format invalid pentru pret." << std::endl;
            return 1;
        } catch (const std::out_of_range& e) {
            std::cerr << "Eroare: Valoarea pretului este in afara intervalului permis." << std::endl;
            return 1;
        }
        adauga_bautura_impl(denumire, pret);
    } else if (command == "sterge_bautura") {
        if (argc != 3) {
            std::cerr << "Utilizare: ./cafenea.exe sterge_bautura <denumire>" << std::endl;
            return 1;
        }
        std::string denumire = argv[2];
        sterge_bautura_impl(denumire);
    } else if (command == "modifica_pret") {
        if (argc != 4) {
            std::cerr << "Utilizare: ./cafenea.exe modifica_pret <denumire> <pret_nou>" << std::endl;
            return 1;
        }
        std::string denumire = argv[2];
        double pret_nou;
        try {
            pret_nou = std::stod(argv[3]);
        } catch (const std::invalid_argument& e) {
            std::cerr << "Eroare: Format invalid pentru pretul nou." << std::endl;
            return 1;
        } catch (const std::out_of_range& e) {
            std::cerr << "Eroare: Valoarea pretului nou este in afara intervalului permis." << std::endl;
            return 1;
        }
        modifica_pret_impl(denumire, pret_nou);
    } else if (command == "afiseaza_comenzi") {
         if (argc != 2) {
             std::cerr << "Utilizare: ./cafenea.exe afiseaza_comenzi" << std::endl; return 1;
        }
        afiseaza_comenzi_impl();
    } else {
        std::cerr << "Comanda necunoscuta: " << command << std::endl;
        return 1;
    }

    return 0;
}


