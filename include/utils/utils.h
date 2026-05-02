#pragma once
#include <string>
#include "../common/common.h"

class Utils
{
public:
    static std::string toString(OrderStatus orderStatus);
    static std::string toString(Side side);
};