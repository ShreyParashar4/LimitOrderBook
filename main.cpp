#include "OrderBook.h"

int main() {
    OrderBook book;

    // 1. Add Seller
    book.addOrder(1, OrderType::SELL, 101.0, 100);

    // 2. Add Buyer
    book.addOrder(2, OrderType::BUY, 99.0, 50);

    book.printBook(); 

    // 3. Aggressive Trade
    cout << "--- New Aggressive Order Incoming ---\n";
    book.addOrder(3, OrderType::BUY, 102.0, 60);

    book.printBook();
    return 0;
}