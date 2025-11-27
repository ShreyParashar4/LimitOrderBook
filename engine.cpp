#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <ctime>

using namespace std;

enum class OrderType { BUY, SELL };

struct Order {
    int id;
    OrderType type;
    double price;
    int quantity;
    long timestamp;

    Order(int i, OrderType t, double p, int q) 
        : id(i), type(t), price(p), quantity(q) {
        timestamp = time(0);
    }
};

class OrderBook {
private:
    // Bids: High Price Priority (greater<double>)
    map<double, vector<Order>, greater<double>> bids;
    // Asks: Low Price Priority (default less<double>)
    map<double, vector<Order>> asks;

    // Helper to log trades
    void logTrade(int buyID, int sellID, double price, int qty) {
        cout << "[TRADE] Buy #" << buyID << " matched with Sell #" << sellID 
             << " -> " << qty << " shares @ $" << price << endl;
    }

public:
    void addOrder(int id, OrderType type, double price, int quantity) {
        if (type == OrderType::BUY) {
            matchBuyOrder(id, price, quantity);
        } else {
            matchSellOrder(id, price, quantity);
        }
    }

    void matchBuyOrder(int id, double price, int quantity) {
        // While I still need shares AND there are sellers available...
        while (quantity > 0 && !asks.empty()) {
            // Look at the Best Ask (Cheapest Seller)
            auto bestAskIter = asks.begin();
            double bestAskPrice = bestAskIter->first;
            vector<Order> &orders = bestAskIter->second;

            // If Seller wants MORE than I am willing to pay, stop matching.
            if (bestAskPrice > price) break;

            // Match with the oldest order at this price (FIFO)
            while (quantity > 0 && !orders.empty()) {
                Order &sellOrder = orders.front();
                
                int tradeQty = min(quantity, sellOrder.quantity);
                
                // EXECUTE TRADE
                logTrade(id, sellOrder.id, bestAskPrice, tradeQty);

                // Update quantities
                quantity -= tradeQty;
                sellOrder.quantity -= tradeQty;

                // If sell order is fully filled, remove it
                if (sellOrder.quantity == 0) {
                    orders.erase(orders.begin());
                }
            }

            // If no more orders at this price level, remove the price level
            if (orders.empty()) {
                asks.erase(bestAskIter);
            }
        }

        // If I still have quantity left, add rest to BIDS book
        if (quantity > 0) {
            bids[price].push_back(Order(id, OrderType::BUY, price, quantity));
            cout << "[PENDING] Buy #" << id << " added to book: " << quantity << " @ $" << price << endl;
        }
    }

    void matchSellOrder(int id, double price, int quantity) {
        // While I still have shares to sell AND there are buyers available...
        while (quantity > 0 && !bids.empty()) {
            // Look at Best Bid (Highest Payer)
            auto bestBidIter = bids.begin();
            double bestBidPrice = bestBidIter->first;
            vector<Order> &orders = bestBidIter->second;

            // If Buyer offers LESS than I want, stop matching.
            if (bestBidPrice < price) break;

            while (quantity > 0 && !orders.empty()) {
                Order &buyOrder = orders.front();
                int tradeQty = min(quantity, buyOrder.quantity);

                // EXECUTE TRADE
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

    void printBook() {
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
};

int main() {
    OrderBook book;

    // SCENARIO:
    // 1. Someone wants to SELL 100 shares at $101 (High price, nobody wants it yet)
    book.addOrder(1, OrderType::SELL, 101.0, 100);

    // 2. Someone wants to BUY 50 shares at $99 (Low price, nobody selling yet)
    book.addOrder(2, OrderType::BUY, 99.0, 50);

    book.printBook(); 
    // Expect: 
    // ASKS: $101 (100)
    // BIDS: $99 (50)

    // 3. AGGRESSIVE BUYER: Wants to buy 60 shares at $102!
    // This crosses the spread ($102 > $101). Should TRADE immediately.
    cout << "--- New Aggressive Order Incoming ---\n";
    book.addOrder(3, OrderType::BUY, 102.0, 60);

    book.printBook();
    // Expect:
    // ASKS: $101 (40 left) -> Because 60 were eaten
    // BIDS: $99 (50)
    
    return 0;
}