#include "../../include/utils/utils.h"

std::string Utils::toString(OrderStatus orderStatus)
{
    switch (orderStatus)
    {
        case OrderStatus::New: return "NEW";
        case OrderStatus::Cancelled: return "CANCELLED";
        case OrderStatus::Filled: return "FILLED";
        case OrderStatus::PartiallyFilled: return "PARTIAL";
        case OrderStatus::Rejected: return "REJECTED";
    }

    return "UNKNOWN";
}

std::string Utils::toString(Side side)
{
    switch (side)
    {
        case Side::Buy: return "BUY";
        case Side::Sell: return "SELL";   
    }

    return "UNKNOWN";
}