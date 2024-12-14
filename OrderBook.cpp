#include "OrderBook.h"

void OrderBook::initFromSnapshot(const Orders& bidsSnapshot, const Orders& asksSnapshot)
{
	bids.clear();
	asks.clear();
	for (const auto &b : bidsSnapshot) {
		if (b.second > 0) {
			bids[b.first] = b.second;
		}
	}
	for (const auto &a : asksSnapshot) {
		if (a.second > 0) {
			asks[a.first] = a.second;
		}
	}
}

void OrderBook::applyUpdates(const Orders& bidUpdates,
                             const Orders& askUpdates)
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
	auto it = bids.end();
	--it; // highest price is at the end
	return std::make_pair(it->first, it->second);
}

std::optional<Order> OrderBook::getBestAsk() const {
	if (asks.empty()) return std::nullopt;
	auto it = asks.begin(); // lowest price at the beginning
	return std::make_pair(it->first, it->second);
}

