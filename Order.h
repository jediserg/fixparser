//
// Created by serg on 4/5/20.
//

#ifndef FIXPARSER_ORDER_H
#define FIXPARSER_ORDER_H

#include <cstddef>

class Order {
public:
    enum class Type {
        Buy,
        Sell
    };
private:
public:
    Order(double price, double size);
    double price() const { return _price; }
    double size() const { return _size; }
private:
public:
    bool operator<(const Order &rhs) const;

    bool operator>(const Order &rhs) const;

    bool operator<=(const Order &rhs) const;

    bool operator>=(const Order &rhs) const;

private:
    double _price;
    double _size;
};


#endif //FIXPARSER_ORDER_H
