#pragma once
#include <cstdint>
#include <map>
#include <deque>
#include <algorithm>
#include <iostream>
#include <unordered_map>
#include "order_model.h"
#include "order_execution_report.h"

using OrderQueue = std::deque<Order>;
using BidBook = std::map<int64_t,OrderQueue,std::greater<int64_t>>;
using AskBook = std::map<int64_t,OrderQueue>;

struct OrderLocation
{
    Side side;
    int64_t price;
    OrderQueue::iterator orderIt;
};

class OrderBook
{
private:
    
    BidBook m_bids;
    AskBook m_asks;
    uint64_t m_globalTimeStamp{0};
    std::unordered_map<uint64_t, OrderLocation> m_Orderlookup;

    uint64_t nextTimeStamp(){
        return ++m_globalTimeStamp;
    }

    bool isActive(const Order& o) const
    {
        return o.orderStatus == OrderStatus::New ||
            o.orderStatus == OrderStatus::PartiallyFilled;
    }

    void match(MatchResult& matchResult);

public:
    OrderBook():m_globalTimeStamp(0) {}

    MatchResult addOrder(Side side,uint64_t orderId, int64_t price, uint64_t quantity);
    MatchResult modifyOrder( uint64_t orderId,  int64_t newPrice, uint64_t newQty);
    ExecutionReport cancelOrder( uint64_t orderId);
};