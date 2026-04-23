#pragma once

#include "order_book.h"
#include "order_execution_report.h"
#include "order_request.h"

class MatchingEngine
{
    OrderBook m_orderBook; 
public:
    MatchResult process(const OrderRequest& req);
};