# 📈 Limit Order Book (HFT Core)

![Language](https://img.shields.io/badge/language-C%2B%2B17-blue.svg)
![Type](https://img.shields.io/badge/type-FinTech-green.svg)

A high-performance **Limit Order Book (LOB)** implementation in C++, designed to simulate the core matching engine of a stock exchange. It supports **Limit Orders**, **Market Orders**, and efficient **O(log N)** order matching using standard STL data structures.

## 🚀 Key Features

- **Price-Time Priority**: Executes trades based on the standard matching algorithm (Best Price first, then Earliest Time).
- **Efficient Matching**: Uses `std::map` (Red-Black Tree) to maintain sorted order books, ensuring **O(log N)** insertion and deletion.
- **Instant Execution**: Automatically matches aggressive orders that cross the spread.
- **Partial Fills**: Handles orders that can only be partially executed, keeping the remainder on the book.
- **Memory Efficient**: Uses standard STL containers (`vector`, `map`) for robust memory management.

## 🏗️ Architecture

The engine maintains two sorted books:
1.  **Bids (Buy Orders)**: Sorted by **Price Descending** (Highest Bid = Highest Priority).
    * *Implementation:* `std::map<double, vector<Order>, greater<double>>`
2.  **Asks (Sell Orders)**: Sorted by **Price Ascending** (Lowest Ask = Highest Priority).
    * *Implementation:* `std::map<double, vector<Order>>`

## 📊 Time Complexity

| Operation | Complexity | Description |
| :--- | :--- | :--- |
| **Add Order** | **O(log N)** | Insertion into `std::map` (Red-Black Tree). |
| **Match Trade** | **O(1)** | Peeking the best bid/ask is constant time. |
| **Cancel Order** | **O(log N)** | Finding and removing the price level. |

## 💻 Usage

### 1. Build
```bash
g++ engine.cpp -o engine
```
### 2. Run
```bash
./engine
```
### 3. Output Example
```bash
[TRADE] Buy #3 matched with Sell #1 -> 60 shares @ $101
[PENDING] Sell #1 added to book: 40 @ $101
```
🛠️ Tech Stack
Language: C++ (STL)

Data Structures: Maps (Trees), Vectors

Algorithm: Price-Time Priority Matching