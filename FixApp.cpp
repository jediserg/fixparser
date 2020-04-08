//
// Created by serg on 3/31/20.
//
#include "FixApp.h"
#include "aixlog.hpp"
#include "FixMessage.h"
#include <filesystem>
#include <iostream>

void FixApp::_printStats(size_t n){
    auto [buy_count, sell_count] = _order_book.getOrdersCount();
    auto [buy_orders_start, buy_orders_end] = _order_book.getBuyOrdersView(n, OrderBook::Direction::End);
    auto [sell_orders_start, sell_orders_end] = _order_book.getSellOrdersView(n, OrderBook::Direction::Begin);

    auto revers_sell_orders_start = std::make_reverse_iterator(sell_orders_end);
    auto revers_sell_orders_end = std::make_reverse_iterator(sell_orders_start);

    std::cout << "Total SELL: " << sell_count << std::endl;

    for(size_t i = 0; i < n; i++){
        if(revers_sell_orders_start != revers_sell_orders_end){
            std::cout << "[" << n - i - 1 << "] price:" << revers_sell_orders_start->price() << " (" << revers_sell_orders_start->size() << ")" << std:: endl;
            revers_sell_orders_start++;
        }
    }
    std::cout << std::string(10, '=') << std::endl;
    std::cout << "Total BUY: " << buy_count << std::endl;
    for(size_t i = 0; i < n; i++){
        if (buy_orders_start != buy_orders_end) {
            std::cout << "[" << i << "] price:" << buy_orders_start->price() << " ("
                      << buy_orders_start->size() << ")" << std::endl;
            buy_orders_start++;
        }
    }
    std::cout << std::endl;
}

std::optional<OrderAction> FixApp::_getOrderAction(const FixGroup::FixRecord& record){
    const auto& it = record.find(FixTag::MDEntryType);

    if(it == std::end(record)){
        return {};
    }

    switch (it->second.value()[0]) {
        case static_cast<char>(OrderAction::Bid): return OrderAction::Bid;
        case static_cast<char>(OrderAction::Offer): return OrderAction::Offer;
        case static_cast<char>(OrderAction::Trade): return OrderAction::Trade;
    }

    return {};
}

std::optional<double> FixApp::_getValueDouble(FixTag tag, const FixGroup::FixRecord& record){
    const auto& it = record.find(tag);

    if(it == std::end(record)){
        return {};
    }

    return it->second.value_double();
}

FixApp::FixApp(int argc, char **argv) {
    for(int i = 0; i < argc; i++)
        _args.emplace_back(argv[i]);
}

bool FixApp::run() {
    if(_args.size() <= 1){
        LOG(ERROR) << "Usage: fixparser input_file_name";
        return false;
    }

    std::string input_file_name = _args[1];

    if(!std::filesystem::exists(input_file_name)){
        LOG(ERROR) << "File:" << input_file_name << " doesn't exist";
        return false;
    }

    std::vector<char> buffer(IO_BUFFER_SIZE);
    std::ifstream input;
    input.rdbuf()->pubsetbuf(buffer.data(), buffer.size());
    input.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        input.open(input_file_name);
        if (!input.is_open()) {
            LOG(ERROR) << "Couldn't open:" << input_file_name << " doesn't exist";
            return false;
        }

        //int message_counter = 0;
        while (!input.eof()){
            FixMessage msg;
            if(!msg.read(input)){
                LOG(ERROR) << "Couldn't read message";
                break;
            }

            handleMessage(msg);
        }
    } catch (std::ios_base::failure& e) {
        return false;
    }

    return true;
}

void FixApp::handleMessage(const FixMessage &message) {
    auto type = message.type();
    if(!type.has_value())
        return;

    switch (type.value()) {
        case FixMessage::Type::Snapshot:
            _loadOrders(message, true);
            break;
        case FixMessage::Type::IncrementalRefresh:
            _loadOrders(message, false);
            break;
        case FixMessage::Type::DataRequest:
            ;
    }
}

void FixApp::_loadOrders(const FixMessage &message, bool is_snapshot) {
    auto group = message.getGroup(FixTag::NoMDEntries);
    if(!group) {
        LOG(WARNING) << "there are no groups in 'W' message";
        return;
    }

    for(const auto& record: group->get()){
        auto order_type = _getOrderAction(record);
        if(!order_type.has_value()){
            LOG(WARNING) << "there is not " << fix_tag_to_str(FixTag::MDEntryType) << " in 'W' message";
            return;
        }
        auto price = _getValueDouble(FixTag::MDEntryPx, record);
        auto size = _getValueDouble(FixTag::MDEntrySize, record);

        if(!price.has_value() || ! size.has_value()){
            LOG(WARNING) << "No price or size in 'W' message:";
            return;
        }

        switch(order_type.value()){
            case OrderAction::Bid:
                _order_book.addBuyOrder({price.value(), size.value()}, !is_snapshot);
                break;
            case OrderAction::Offer:
                _order_book.addSellOrder({price.value(), size.value()}, !is_snapshot);
                break;
            case OrderAction::Trade:
                break;
        }
    }

    if(is_snapshot)
        _order_book.sort();
    _printStats(5);
}

