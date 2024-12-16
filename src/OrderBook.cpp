#include "OrderBook.h"

void OrderBook::update(const Orders& bidUpdates, const Orders& askUpdates)
{
	for (const auto &b : bidUpdates) {
		if (b.second == 0.0) {
			bids.erase(b.first);
		} else {
			bids[b.first] = b.second;
		}
	}

	for (const auto &a : askUpdates) {
		if (a.second == 0.0) {
			asks.erase(a.first);
		} else {
			asks[a.first] = a.second;
		}
	}
}

std::optional<Order> OrderBook::getBestBid() const {
	if (bids.empty()) return std::nullopt;
	auto it = bids.begin();
	return std::make_pair(it->first, it->second);
}

std::optional<Order> OrderBook::getBestAsk() const {
	if (asks.empty()) return std::nullopt;
	auto it = asks.begin();
	return std::make_pair(it->first, it->second);
}

