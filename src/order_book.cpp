#include "order_book.h"


MatchResult OrderBook::addOrder(Side side, uint64_t orderId, int64_t price, uint64_t quantity)
{
    if(m_Orderlookup.count(orderId))
        throw std::runtime_error("Duplicate orderId");

    MatchResult matchResult;
    Order order;
    order.orderId = orderId;
    order.orderStatus = OrderStatus::New;
    order.price = price;
    order.quantity = quantity;
    order.filledQty = 0;
    order.timestamp = nextTimeStamp();
    order.side = side;
    matchResult.reports.push_back(ExecutionReport{orderId,OrderStatus::New,order.filledQty,order.quantity});
    
    if(side == Side::Buy)
    {
        // get the queue for the price and push new order
        // also maintain order lookup for easy reference
        auto& queue = m_bids[price];
        queue.push_back(order);
        auto it = std::prev(queue.end());
        m_Orderlookup[orderId] = {side,price,it};
    }
    else
    {
        auto& queue = m_asks[price];
        queue.push_back(order);
        auto it = std::prev(queue.end());
        m_Orderlookup[orderId] = {side, price, it};
    }

    match(matchResult);
    return matchResult;
}
Order OrderBook::modifyOrder(uint64_t orderId, int64_t newPrice, uint64_t newQty)
{
    MatchResult matchResult;

    // check whether order exist in order lookup
    auto locIt = m_Orderlookup.find(orderId);

    if(locIt == m_Orderlookup.end())
        throw std::runtime_error("Order not found");
    
    // if exist get the location in the bids/aks
    auto& loc = locIt->second;
    Order& existingOrder = *(loc.orderIt);

    if(!isActive(existingOrder))
        throw std::runtime_error("Cannot modify inactive order");

    if(newQty < existingOrder.filledQty)
        throw std::runtime_error("New quantity less than already filled");

    // Remove from old price level
    if(loc.side == Side::Buy)
    {
        auto& oldQueue = m_bids[loc.price];
        oldQueue.erase(loc.orderIt);
        if(oldQueue.empty())
            m_bids.erase(loc.price);
    }
    else
    {
        auto& oldQueue = m_asks[loc.price];
        oldQueue.erase(loc.orderIt);
        if(oldQueue.empty())
            m_asks.erase(loc.price);
    }

    bool priceChanged = (newPrice != existingOrder.price);
    bool qtyIncreased = (newQty > existingOrder.quantity);


    // Create updated order
    Order updateOrder = existingOrder;
    updateOrder.price = newPrice;
    updateOrder.quantity = newQty;
    if (priceChanged || qtyIncreased) 
    {
        updateOrder.timestamp = nextTimeStamp(); // lose priority
    } 
    else 
    {
        updateOrder.timestamp = existingOrder.timestamp; // keep priority
    }


    // Insert into new price level
    if(loc.side == Side::Buy)
    {
        auto& newQueue = m_bids[newPrice];
        newQueue.push_back(updateOrder);
        auto newIt = std::prev(newQueue.end());
        loc.price = newPrice;
        loc.orderIt = newIt;
    }
    else
    {
        auto& newQueue = m_asks[newPrice];
        newQueue.push_back(updateOrder);
        auto newIt = std::prev(newQueue.end());
        loc.price = newPrice;
        loc.orderIt = newIt;
    }

    match(matchResult);
    return updateOrder;
    
}
void OrderBook::cancelOrder(uint64_t orderId)
{
    // Check order exist or not
    auto locIt = m_Orderlookup.find(orderId);
    if(locIt == m_Orderlookup.end())
    {
        throw std::runtime_error("Order Not Found");
    }

    auto& loc = locIt->second;
    Order& order = *(loc.orderIt);

    if(!isActive(order))
        throw std::runtime_error("Cannot cancel inactive order");

    order.orderStatus = OrderStatus::Cancelled;

    if(loc.side == Side::Buy)
    {
        auto& queue = m_bids[loc.price];
        queue.erase(loc.orderIt);
        if(queue.empty())
            m_bids.erase(loc.price);
    }
    else
    {
        auto& queue = m_asks[loc.price];
        queue.erase(loc.orderIt);
        if(queue.empty())
            m_asks.erase(loc.price);
    }

    m_Orderlookup.erase(locIt);
    
}

void OrderBook::match(MatchResult& matchResult)
{
    while(!m_bids.empty() && !m_asks.empty())
    {
        auto bidIt = m_bids.begin();
        auto askIt = m_asks.begin();

        if(bidIt->first < askIt->first)
            break;

        auto& buyQueue = bidIt->second;
        auto& askQueue = askIt->second;

        Order& buy = buyQueue.front();
        Order& sell = askQueue.front();

        uint64_t buyId = buy.orderId;
        uint64_t sellId = sell.orderId;

        uint64_t remainingBuy = buy.quantity - buy.filledQty;
        uint64_t remainingSell = sell.quantity - sell.filledQty;

        uint64_t tradeQty = std::min(remainingBuy, remainingSell);
        int64_t tradePrice = askIt->first;

        Trade trade;
        trade.buyOrderId = buyId;
        trade.sellOrderId = sellId;
        trade.price = tradePrice;
        trade.quantity = tradeQty;
        matchResult.trades.push_back(trade);

        // Update state
        buy.filledQty += tradeQty;
        sell.filledQty += tradeQty;

        ExecutionReport buyReport{
            buyId,
            (buy.filledQty == buy.quantity)
                ? OrderStatus::Filled : OrderStatus::PartiallyFilled,
            buy.filledQty,
            buy.quantity - buy.filledQty
        };

        ExecutionReport sellReport{
            sellId,
            sellReport.orderStatus = (sell.quantity == sell.filledQty) 
                ? OrderStatus::Filled : OrderStatus::PartiallyFilled,
            sell.filledQty,
            sell.quantity - sell.filledQty
        };

        matchResult.reports.push_back(buyReport);
        matchResult.reports.push_back(sellReport);

        buy.orderStatus = buyReport.orderStatus;
        sell.orderStatus = sellReport.orderStatus;
        
        std::cout << "TRADE "
                  << "Price: " << tradePrice
                  << " Qty: " << tradeQty
                  << " BuyID: " << buy.orderId
                  << " SellID: " << sell.orderId
                  << std::endl;

        
        if(buy.filledQty == buy.quantity)
        {
            m_Orderlookup.erase(buy.orderId);
            buyQueue.pop_front();
            if(buyQueue.empty())
                m_bids.erase(bidIt);
        }
        
        if(sell.filledQty == sell.quantity)
        {
            m_Orderlookup.erase(sell.orderId);
            askQueue.pop_front();
            if(askQueue.empty())
                m_asks.erase(askIt);
        }
        
    }
}

