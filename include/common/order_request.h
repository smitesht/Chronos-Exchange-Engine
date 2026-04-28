#pragma once
#include <cstdint>
#include "common.h"

struct OrderRequest
{
    OrderType orderType;
    uint64_t orderId;
    Side side;
    int64_t price;
    uint64_t quantity;
};
