//
// Created by serg on 4/5/20.
//

#include "Order.h"

Order::Order(double price, double size) : _price(price), _size(size) {

}

bool Order::operator<(const Order &rhs) const {
    return _price < rhs._price;
}

bool Order::operator>(const Order &rhs) const {
    return rhs < *this;
}

bool Order::operator<=(const Order &rhs) const {
    return !(rhs < *this);
}

bool Order::operator>=(const Order &rhs) const {
    return !(*this < rhs);
}
