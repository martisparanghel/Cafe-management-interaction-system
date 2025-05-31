#ifndef BAUTURA_HPP
#define BAUTURA_HPP

#include <string>
#include <iostream>
#include <iomanip>

class Bautura {
public:
    std::string denumire;
    double pret;

    Bautura(std::string d = "", double p = 0.0) : denumire(d), pret(p) {}


    friend std::ostream& operator<<(std::ostream& os, const Bautura& b) {
        os << b.denumire << " " << std::fixed << std::setprecision(2) << b.pret;
        return os;
    }

    friend std::istream& operator>>(std::istream& is, Bautura& b) {
        is >> b.denumire >> b.pret;
        return is;
    }
};

#endif