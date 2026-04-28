#pragma once
#include <iostream>
#include <cstdint>
#include "common.h"

struct Order
{
    uint64_t orderId;
    Side side;
    int64_t price;
    uint64_t quantity;
    uint64_t filledQty;
    uint64_t timestamp;
    OrderStatus orderStatus;
};
