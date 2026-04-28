#pragma once

#include "../orderbook/order_book.h"
#include "../common/order_execution_report.h"
#include "../common/order_request.h"

class MatchingEngine
{
    OrderBook m_orderBook; 
public:
    MatchResult process(const OrderRequest& req);
};