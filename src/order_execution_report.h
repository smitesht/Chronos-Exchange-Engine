#pragma once
#include <cstdint>
#include "common.h"

struct ExecutionReport
{
    uint64_t orderId;
    OrderStatus orderStatus;
    uint64_t filledQty;
    uint64_t remainingQty;
};