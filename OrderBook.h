#ifndef ORDERBOOK_H
#define ORDERBOOK_H

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <ctime>

// Use standard namespace carefully in headers
using namespace std;

enum class OrderType { BUY, SELL };

struct Order {
    int id;
    OrderType type;
    double price;
    int quantity;
    long timestamp;

    Order(int i, OrderType t, double p, int q);
};

class OrderBook {
private:
    // Bids: High Price Priority
    map<double, vector<Order>, greater<double>> bids;
    // Asks: Low Price Priority
    map<double, vector<Order>> asks;

    void logTrade(int buyID, int sellID, double price, int qty);

public:
    void addOrder(int id, OrderType type, double price, int quantity);
    void matchBuyOrder(int id, double price, int quantity);
    void matchSellOrder(int id, double price, int quantity);
    void printBook();
};

#endif