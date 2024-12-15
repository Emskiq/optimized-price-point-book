#include <gtest/gtest.h>

#include "src/OrderBook.h"
#include "src/OrderBookArray.h"
#include "src/OrderBooksCollection.h"

TEST(OrderBookTest, EmptyBook) {
	OrderBooksCollection orderBooks;
	const std::string symbol = "EMPTY";
	orderBooks.addSymbol(symbol);

	auto bestBid = orderBooks.getBestBid(symbol);
	auto bestAsk = orderBooks.getBestAsk(symbol);

	EXPECT_FALSE(bestBid.has_value());
	EXPECT_FALSE(bestAsk.has_value());
}


TEST(OrderBookTest, CorrectnessBasicInitialization) {
	OrderBooksCollection orderBooks;
	const std::string symbol = "TEST1";
	const std::string symbol2 = "TEST2";

	orderBooks.addSymbol(symbol);
	orderBooks.addSymbol(symbol2);

	const Orders bids = {{100.0, 10.0}, {99.0, 20.0}};
	const Orders asks = {{101.0, 5.0}, {102.0, 7.0}};
	orderBooks.applyEvent(symbol, bids, asks);

	auto bestBid = orderBooks.getBestBid(symbol);
	auto bestAsk = orderBooks.getBestAsk(symbol);

	ASSERT_TRUE(bestBid.has_value());
	ASSERT_TRUE(bestAsk.has_value());
	EXPECT_EQ(bestBid->first, 100.0);
	EXPECT_EQ(bestBid->second, 10.0);
	EXPECT_EQ(bestAsk->first, 101.0);
	EXPECT_EQ(bestAsk->second, 5.0);

	const Orders bids2 = {{101.0, 10.0}, {97.1, 19.0}};
	const Orders asks2 = {{103.3, 4.1}, {101.1, 5.1}};
	orderBooks.applyEvent(symbol2, bids2, asks2);

	auto bestBid2 = orderBooks.getBestBid(symbol2);
	auto bestAsk2 = orderBooks.getBestAsk(symbol2);

	ASSERT_TRUE(bestBid2.has_value());
	ASSERT_TRUE(bestAsk2.has_value());
	EXPECT_EQ(bestBid2->first, 101.0);
	EXPECT_EQ(bestBid2->second, 10.0);
	EXPECT_EQ(bestAsk2->first, 101.1);
	EXPECT_EQ(bestAsk2->second, 5.1);
}

TEST(OrderBookTest, Correctness) {
	// Step 1: Initialize OrderBook
	OrderBook book("TEST");

	// Step 2: Define test input data
	Orders bids = {
		{100.0, 10.0}, {101.5, 5.0}, {99.0, 20.0}, {105.3, 12.0}, {103.7, 15.0}
	};
	Orders asks = {
		{106.0, 8.0}, {110.0, 5.0}, {107.3, 10.0}, {105.7, 9.0}, {108.5, 7.0}
	};

	// Step 3: Insert data into the OrderBook
	book.update(bids, asks);

	auto bestBidIt = std::max_element(bids.begin(), bids.end(), [](const Order& a, const Order& b) {
		return a.first < b.first;
	});
	auto bestAskIt = std::min_element(asks.begin(), asks.end(), [](const Order& a, const Order& b) {
		return a.first < b.first;
	});

	auto orderBookBestBid = book.getBestBid();
	auto orderBookBestAsk = book.getBestAsk();

	ASSERT_TRUE(orderBookBestBid.has_value());
	ASSERT_TRUE(orderBookBestAsk.has_value());

	// Compare bid
	EXPECT_EQ(orderBookBestBid->first, bestBidIt->first);
	EXPECT_EQ(orderBookBestBid->second, bestBidIt->second);

	// Compare ask
	EXPECT_EQ(orderBookBestAsk->first, bestAskIt->first);
	EXPECT_EQ(orderBookBestAsk->second, bestAskIt->second);

	// Print results (optional, for debugging purposes)
	std::cout << "Best Bid (Manual): " << bestBidIt->first << " @ " << bestBidIt->second << "\n";
	std::cout << "Best Ask (Manual): " << bestAskIt->first << " @ " << bestAskIt->second << "\n";
	std::cout << "Best Bid (OrderBook): " << orderBookBestBid->first << " @ " << orderBookBestBid->second << "\n";
	std::cout << "Best Ask (OrderBook): " << orderBookBestAsk->first << " @ " << orderBookBestAsk->second << "\n";
}

TEST(OrderBookTest, CorrectnessAfterUpdate) {
	OrderBooksCollection orderBooks;
	const std::string symbol = "TEST"; orderBooks.addSymbol(symbol);

	const Orders bids = {{100.0, 10.0}, {99.0, 20.0}};
	const Orders asks = {{101.0, 5.0}, {102.0, 7.0}};
	orderBooks.applyEvent(symbol, bids, asks);

	const Orders updateBids = {{100.0, 9.0}, {99.0, 22.3}, {100.0, 30.0}};
	const Orders updateAsks = {{101.0, 4.2}, {98.0, 11.0}};

	orderBooks.applyEvent(symbol, updateBids, updateAsks);

	auto bestBid = orderBooks.getBestBid(symbol);
	auto bestAsk = orderBooks.getBestAsk(symbol);

	ASSERT_TRUE(bestBid.has_value());
	ASSERT_TRUE(bestAsk.has_value());
	EXPECT_EQ(bestBid->first, 100.0);
	EXPECT_EQ(bestBid->second, 30.0);
	EXPECT_EQ(bestAsk->first, 98.0);
	EXPECT_EQ(bestAsk->second, 11.0);
}
