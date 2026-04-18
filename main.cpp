#include <iostream>
#include <random>
#include "src/order_book.h"
#include "src/order_execution_report.h"

using std::cout;

int main()
{
    OrderBook ordBok; 
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<> sideDist(0, 1);
    std::uniform_int_distribution<> priceDist(90, 110);
    std::uniform_int_distribution<> qtyDist(1, 10);

    for (int i = 0; i < 10; ++i) {
    Side side = sideDist(gen) == 0 ? Side::Buy : Side::Sell;
    int price = priceDist(gen);
    int qty = qtyDist(gen);

    MatchResult result = ordBok.addOrder(side,i, price, qty);
    for(ExecutionReport er: result.reports)
    {
        std::cout << er.orderId << " " << er.filledQty << std::endl;
    }
}

    return 0;
}