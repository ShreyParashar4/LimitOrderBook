#include "OrderBook.h"

// Constructor implementation
Order::Order(int i, OrderType t, double p, int q) 
    : id(i), type(t), price(p), quantity(q) {
    timestamp = time(0);
}

void OrderBook::logTrade(int buyID, int sellID, double price, int qty) {
    cout << "[TRADE] Buy #" << buyID << " matched with Sell #" << sellID 
         << " -> " << qty << " shares @ $" << price << endl;
}

void OrderBook::addOrder(int id, OrderType type, double price, int quantity) {
    if (type == OrderType::BUY) {
        matchBuyOrder(id, price, quantity);
    } else {
        matchSellOrder(id, price, quantity);
    }
}

void OrderBook::matchBuyOrder(int id, double price, int quantity) {
    while (quantity > 0 && !asks.empty()) {
        auto bestAskIter = asks.begin();
        double bestAskPrice = bestAskIter->first;
        vector<Order> &orders = bestAskIter->second;

        if (bestAskPrice > price) break;

        while (quantity > 0 && !orders.empty()) {
            Order &sellOrder = orders.front();
            int tradeQty = min(quantity, sellOrder.quantity);
            
            logTrade(id, sellOrder.id, bestAskPrice, tradeQty);

            quantity -= tradeQty;
            sellOrder.quantity -= tradeQty;

            if (sellOrder.quantity == 0) {
                orders.erase(orders.begin());
            }
        }

        if (orders.empty()) {
            asks.erase(bestAskIter);
        }
    }

    if (quantity > 0) {
        bids[price].push_back(Order(id, OrderType::BUY, price, quantity));
        cout << "[PENDING] Buy #" << id << " added to book: " << quantity << " @ $" << price << endl;
    }
}

void OrderBook::matchSellOrder(int id, double price, int quantity) {
    while (quantity > 0 && !bids.empty()) {
        auto bestBidIter = bids.begin();
        double bestBidPrice = bestBidIter->first;
        vector<Order> &orders = bestBidIter->second;

        if (bestBidPrice < price) break;

        while (quantity > 0 && !orders.empty()) {
            Order &buyOrder = orders.front();
            int tradeQty = min(quantity, buyOrder.quantity);

            logTrade(buyOrder.id, id, bestBidPrice, tradeQty);

            quantity -= tradeQty;
            buyOrder.quantity -= tradeQty;

            if (buyOrder.quantity == 0) {
                orders.erase(orders.begin());
            }
        }

        if (orders.empty()) {
            bids.erase(bestBidIter);
        }
    }

    if (quantity > 0) {
        asks[price].push_back(Order(id, OrderType::SELL, price, quantity));
        cout << "[PENDING] Sell #" << id << " added to book: " << quantity << " @ $" << price << endl;
    }
}

void OrderBook::printBook() {
    cout << "\n------- ORDER BOOK -------\n";
    cout << "ASKS (Sellers)\n";
    for (auto it = asks.rbegin(); it != asks.rend(); ++it) {
        double p = it->first;
        int q = 0;
        for (auto &o : it->second) q += o.quantity;
        cout << "  $" << p << " : " << q << "\n";
    }
    cout << "--------------------------\n";
    cout << "BIDS (Buyers)\n";
    for (auto &[p, orders] : bids) {
        int q = 0;
        for (auto &o : orders) q += o.quantity;
        cout << "  $" << p << " : " << q << "\n";
    }
    cout << "--------------------------\n\n";
}