#include "../../include/engine/matching_engine.h"
#include "../../include/orderbook/order_book.h"

MatchResult MatchingEngine::process(const OrderRequest& req)
{
    if(req.orderType == OrderType::Add)
    {
         return m_orderBook.addOrder(req.side,req.orderId, req.price, req.quantity);
    }
    else if(req.orderType == OrderType::Modify)
    {
        return m_orderBook.modifyOrder(req.orderId,req.price, req.quantity);
    }
    else
    {
        MatchResult result;
        result.reports.push_back(m_orderBook.cancelOrder(req.orderId));
        return result;
    }
    
}