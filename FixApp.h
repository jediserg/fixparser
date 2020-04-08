//
// Created by serg on 3/31/20.
//

#ifndef FIXPARSER_FIXAPP_H
#define FIXPARSER_FIXAPP_H


static const int IO_BUFFER_SIZE = 10 * 1024 * 1024;

#include <string>
#include <vector>
#include <optional>
#include "FixMessage.h"
#include "OrderBook.h"

enum class OrderAction {
    Bid = '0',
    Offer = '1',
    Trade = '2'
};

class FixApp {
public:
    FixApp(int argc, char** argv);
    bool run();
private:
    std::vector<std::string> _args;
    OrderBook _order_book;
    void handleMessage(const FixMessage &message);

    void _loadOrders(const FixMessage &message, bool is_snapshot);

    std::optional<OrderAction> _getOrderAction(const FixGroup::FixRecord &record);

    std::optional<double> _getValueDouble(FixTag tag, const FixGroup::FixRecord& record);

    void _printStats(size_t n);
};


#endif //FIXPARSER_FIXAPP_H
