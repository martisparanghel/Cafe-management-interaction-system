#ifndef DATA_HPP
#define DATA_HPP

#include <iostream>

class Data {
public:
    int zi;
    int luna;
    int an;

    // Default constructor
    Data(int z = 1, int l = 1, int a = 2000) : zi(z), luna(l), an(a) {}

    // Overload << for easy printing
    friend std::ostream& operator<<(std::ostream& os, const Data& d) {
        os << d.zi << " " << d.luna << " " << d.an;
        return os;
    }

    // Overload >> for easy reading
    friend std::istream& operator>>(std::istream& is, Data& d) {
        is >> d.zi >> d.luna >> d.an;
        return is;
    }
};

#endif //DATA_HPP