//
// Created by serg on 4/5/20.
//

#include <utility>
#include "OrderBook.h"
#include <algorithm>

void OrderBook::addSellOrder(Order &&order, bool find_place) {
    if (find_place) {
        auto it = std::lower_bound(_sell_orders.begin(), _sell_orders.end(), order); // find proper position in descending order
        _sell_orders.emplace(it, order); // insert before iterator it
    } else {
        _sell_orders.emplace_back(order);

    }
}

void OrderBook::addBuyOrder(Order &&order, bool find_place) {
    if(find_place){
        auto it = std::lower_bound( _buy_orders.begin(), _buy_orders.end(), order); // find proper position in descending order
        _buy_orders.emplace( it, order ); // insert before iterator it
    } else {
        _buy_orders.emplace_back(order);
    }
}

OrderBook::OrderBook() {
    _sell_orders.reserve(100000);
    _buy_orders.reserve(100000);
}

void OrderBook::sort() {
    std::sort(_sell_orders.begin(), _sell_orders.end());
    std::sort(_buy_orders.begin(), _buy_orders.end());
}

std::pair<OrderBook::OrderIterator, OrderBook::OrderIterator> OrderBook::getBuyOrdersView(size_t count, OrderBook::Direction direction) {
    if(direction == OrderBook::Direction::Begin){
        return {std::cbegin(_buy_orders), std::cbegin(_buy_orders) + count};
    } else {
        return {std::cbegin(_buy_orders) + _buy_orders.size() - count, std::cend(_buy_orders)};
    }
}

std::pair<OrderBook::OrderIterator, OrderBook::OrderIterator> OrderBook::getSellOrdersView(size_t count, OrderBook::Direction direction) {
    if(direction == OrderBook::Direction::Begin){
        return {std::cbegin(_sell_orders), std::cbegin(_sell_orders) + count};
    } else {
        return {std::cbegin(_sell_orders) + _sell_orders.size() - count, std::cend(_sell_orders)};
    }
}

std::pair<size_t, size_t> OrderBook::getOrdersCount() const {
    return {_buy_orders.size(), _sell_orders.size()};
}
