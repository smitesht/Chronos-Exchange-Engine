#pragma once
#include <cstdint>
#include <vector>
#include <string>
#include "common.h"

struct ExecutionReport
{
    uint64_t orderId;
    OrderStatus orderStatus;
    uint64_t filledQty;
    uint64_t remainingQty;

    static std::string toStringOrderStatus(OrderStatus ordStatus){
        switch (ordStatus)
        {
        case OrderStatus::New:
            return "New";
            break;

        case OrderStatus::Cancelled:
            return "Cancelled";
            break;
        
        case OrderStatus::Filled:
            return "Filled";
            break;

        case OrderStatus::PartiallyFilled:
            return "Partially Filled";
            break;

        case OrderStatus::Rejected:
            return "Rejected";
            break;

        
        default:
            return "Unknown";
            break;
        }
    }

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
