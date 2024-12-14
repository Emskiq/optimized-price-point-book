#include <gtest/gtest.h>
#include "OrderBooksCollection.h"

TEST(OrderBookTest, BasicInitialization) {
	OrderBooksCollection mgr;
	const std::string symbol = "TEST";

	const Orders bids = {{100.0, 10.0}, {99.0, 20.0}};
	const Orders asks = {{101.0, 5.0}, {102.0, 7.0}};
	mgr.initBook(symbol, bids, asks);

	auto bestBid = mgr.getBestBid(symbol);
	auto bestAsk = mgr.getBestAsk(symbol);

	ASSERT_TRUE(bestBid.has_value());
	ASSERT_TRUE(bestAsk.has_value());
	EXPECT_EQ(bestBid->first, 100.0);
	EXPECT_EQ(bestBid->second, 10.0);
	EXPECT_EQ(bestAsk->first, 101.0);
	EXPECT_EQ(bestAsk->second, 5.0);
}

TEST(OrderBookTest, Update) {
	OrderBooksCollection mgr;
	const std::string symbol = "TEST";

	const Orders bids = {{100.0, 10.0}, {99.0, 20.0}};
	const Orders asks = {{101.0, 5.0}, {102.0, 7.0}};
	mgr.initBook(symbol, bids, asks);

	const Orders updateBids = {{100.0, 9.0}, {99.0, 22.3}, {100.0, 30.0}};
	const Orders updateAsks = {{101.0, 4.2}, {98.0, 11.0}};

	mgr.applyEvent(symbol, updateBids, updateAsks);

	auto bestBid = mgr.getBestBid(symbol);
	auto bestAsk = mgr.getBestAsk(symbol);

	ASSERT_TRUE(bestBid.has_value());
	ASSERT_TRUE(bestAsk.has_value());
	EXPECT_EQ(bestBid->first, 100.0);
	EXPECT_EQ(bestBid->second, 30.0);
	EXPECT_EQ(bestAsk->first, 98.0);
	EXPECT_EQ(bestAsk->second, 11.0);
}
