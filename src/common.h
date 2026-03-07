#pragma once


enum class Side
{
    Buy = 0,
    Sell
};


enum class OrderStatus
{
    New, //accepted
    PartiallyFilled,
    Filled,
    Cancelled,
    Rejected
};

enum class OrderType
{
    Add,
    Cancel,
    Modify
};