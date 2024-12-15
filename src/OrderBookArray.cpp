#include "OrderBookArray.h"

void OrderBookArray::update(const Orders& bidUpdates, const Orders& askUpdates) {
	for (const auto& bidOrder : bidUpdates) {
		applySingleUpdate(bids, countBids, bidOrder);
	}
	for (const auto& askOrder : askUpdates) {
		applySingleUpdate(asks, countAsks, askOrder);
	}
}

std::optional<Order> OrderBookArray::getBestBid() const {
	if (countBids == 0) return std::nullopt;
	return bids[countBids - 1]; // highest price at the end
}

std::optional<Order> OrderBookArray::getBestAsk() const {
	if (countAsks == 0) return std::nullopt;
	return asks[0]; // lowest price at the start
}
