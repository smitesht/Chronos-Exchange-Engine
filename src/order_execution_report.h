#pragma once
#include <cstdint>
#include <vector>
#include "common.h"

struct ExecutionReport
{
    uint64_t orderId;
    OrderStatus orderStatus;
    uint64_t filledQty;
    uint64_t remainingQty;
};

struct Trade
{
    uint64_t buyOrderId;
    uint64_t sellOrderId;
    int64_t price;
    uint64_t quantity;
};

struct MatchResult
{
    std::vector<ExecutionReport> reports;
    std::vector<Trade> trades;
};
