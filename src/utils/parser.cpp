#include "../../include/utils/parser.h"
#include "../../include/utils/utils.h"
#include <sstream>
#include <stdexcept>
#include <iostream>

OrderRequest Parser::parse(const std::string& line)
{
    std::istringstream iss(line);
    std::string typestr, sidestr;

    if(!(iss >> typestr))
        throw std::runtime_error("Empty command");

    OrderRequest req{};

    if(typestr == "ADD")
    {
        req.orderType = OrderType::Add;
        if(!(iss >> req.orderId >> sidestr >> req.price >> req.quantity))
            throw std::runtime_error("Invalid ADD format");

        if(sidestr == "BUY")
            req.side = Side::Buy;
        else if(sidestr == "SELL")
            req.side = Side::Sell;
        else
            throw std::runtime_error("Invalid side");

        if (req.price <= 0)
            throw std::runtime_error("Price must be > 0");

        if (req.quantity == 0)
            throw std::runtime_error("Quantity must be > 0");
    }
    else if(typestr == "MODIFY")
    {
        req.orderType = OrderType::Modify;

        if (!(iss >> req.orderId >> req.price >> req.quantity))
            throw std::runtime_error("Invalid MODIFY format");

        if (req.price <= 0)
            throw std::runtime_error("Price must be > 0");

        if (req.quantity == 0)
            throw std::runtime_error("Quantity must be > 0");
    }
    else if (typestr == "CANCEL")
    {
        req.orderType = OrderType::Cancel;

        if (!(iss >> req.orderId))
            throw std::runtime_error("Invalid CANCEL format");
    }
    else
    {
        throw std::runtime_error("Unknown command");
    }

    return req;
}

std::string Parser::serialize(const MatchResult& result)
{
    std::ostringstream oss;

    for (const auto& trade : result.trades) {
        oss << "TRADE "
            << trade.buyOrderId << " "
            << trade.sellOrderId << " "
            << trade.price << " "
            << trade.quantity << "\n";
    }

    for (const auto& report : result.reports) {
        oss << "REPORT "
            << report.orderId << " "
            << Utils::toString(report.orderStatus) << " "
            << report.filledQty << " "
            << report.remainingQty << "\n";
    }

    

    return oss.str();
}