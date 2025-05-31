#ifndef PRODUS_HPP
#define PRODUS_HPP

#include "Bautura.hpp" // Contains Bautura object
#include <iostream>

class ProdusComanda {
public:
    Bautura bautura; // The Bautura object. Only 'denumire' is relevant when reading from comenzi.txt
    int cantitate;

    ProdusComanda(Bautura b = Bautura(), int c = 0) : bautura(b), cantitate(c) {}

    // Overload << for easy printing (used for writing to comenzi.txt format)
    friend std::ostream& operator<<(std::ostream& os, const ProdusComanda& pc) {
        // Format: <denumire_bautura> <cantitate>
        os << pc.bautura.denumire << " " << pc.cantitate;
        return os;
    }

    // Overload >> for easy reading from comenzi.txt
    // Assumes denumire_bautura does not contain spaces.

    friend std::istream& operator>>(std::istream& is, ProdusComanda& pc) {
        is >> pc.bautura.denumire >> pc.cantitate;
        // pc.bautura.pret remains default or uninitialized as it's not in comenzi.txt
        return is;
    }
};

#endif //PRODUS_HPP