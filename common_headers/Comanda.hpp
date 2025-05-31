#ifndef COMANDA_HPP
#define COMANDA_HPP

#include "Produs.hpp" // Uses ProdusComanda
#include "Data.hpp"   // Uses Data
#include <vector>
#include <iostream>

class Comanda {
public:
    std::vector<ProdusComanda> produse;
    Data data;

    Comanda() = default; // Default constructor
    Comanda(const Data& d) : data(d) {}

    void adaugaProdus(const ProdusComanda& p) {
        produse.push_back(p);
    }

    // Overload << for easy printing (matches comenzi.txt format for one order)
    friend std::ostream& operator<<(std::ostream& os, const Comanda& c) {
        os << c.data << std::endl; // Prints date
        for (const auto& prod : c.produse) {
            os << prod << std::endl; // Prints each product in the order
        }
        return os;
    }
};

#endif //COMANDA_HPP