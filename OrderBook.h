//
// Created by serg on 4/5/20.
//

#ifndef FIXPARSER_ORDERBOOK_H
#define FIXPARSER_ORDERBOOK_H

#include <vector>
#include "Order.h"

class OrderBook {
public:
    enum class Direction {
        Begin,
        End
    };
    OrderBook();

    void addBuyOrder(Order&& order, bool find_place = true);
    void addSellOrder(Order&& order, bool find_place = true);

    void sort();
    using OrderIterator = std::vector<Order>::const_iterator;
    std::pair<OrderIterator, OrderIterator> getBuyOrdersView(size_t count, Direction direction);
    std::pair<OrderIterator, OrderIterator> getSellOrdersView(size_t count, Direction direction);

    std::pair<size_t, size_t> getOrdersCount() const;
private:
    std::vector<Order> _buy_orders;
    std::vector<Order> _sell_orders;
};


#endif //FIXPARSER_ORDERBOOK_H
