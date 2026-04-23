#include <iostream>
#include <random>
#include "src/order_book.h"
#include "src/order_execution_report.h"
#include "src/matching_engine.h"
#include "src/order_request.h"

using std::cout;

int main()
{

    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<> sideDist(0, 1);
    std::uniform_int_distribution<> priceDist(90, 110);
    std::uniform_int_distribution<> qtyDist(1, 10);
    
    MatchingEngine matchEngine;

    for (int i = 0; i < 10; ++i) 
    {
        Side side = sideDist(gen) == 0 ? Side::Buy : Side::Sell;
        int price = priceDist(gen);
        int qty = qtyDist(gen);

        OrderRequest req;
        req.orderType = OrderType::Add;
        req.orderId = i;
        req.side = side;
        req.price = price;
        req.quantity = qty;
        
        MatchResult result = matchEngine.process(req);
        // Print reports
        for (const ExecutionReport& er : result.reports)
        {
            std::cout << "REPORT "
                      << er.orderId << " "
                      << static_cast<int>(er.orderStatus) << " "
                      << er.filledQty << " "
                      << er.remainingQty << std::endl;
        }

        // Print trades
        for (const Trade& t : result.trades)
        {
            std::cout << "TRADE "
                      << t.buyOrderId << " "
                      << t.sellOrderId << " "
                      << t.price << " "
                      << t.quantity << std::endl;
        }

        std::cout << "------------------------\n";

    }

    return 0;
}