#include "OrderBookArray.h"

#include <vector>
#include <algorithm>
#include <optional>
#include <utility>

constexpr uint16_t ORDERS_COUNT_ESTIMATE = 10'000;

void OrderBookArray::initFromSnapshot(const Orders& bidsSnapshot, const Orders& asksSnapshot) {
	bids = bidsSnapshot;
	asks = asksSnapshot;

	bids.reserve(ORDERS_COUNT_ESTIMATE);
	asks.reserve(ORDERS_COUNT_ESTIMATE);

	std::sort(bids.begin(), bids.end(), [](const Order &a, const Order &b){return a.first < b.first;});
	std::sort(asks.begin(), asks.end(), [](const Order &a, const Order &b){return a.first < b.first;});
}

void OrderBookArray::applyUpdates(const Orders& bidUpdates, const Orders& askUpdates) {
	applySideUpdates(bids, bidUpdates);
	applySideUpdates(asks, askUpdates);
}

std::optional<Order> OrderBookArray::getBestBid() const {
	if (bids.empty()) {
		return std::nullopt;
	}

	return bids.back(); // The highest price is at the end if sorted ascending
}

std::optional<Order> OrderBookArray::getBestAsk() const {
	if (asks.empty()){
		return std::nullopt;
	}
	return asks.front(); // The lowest price is at the front if sorted ascending
}

void OrderBookArray::applySideUpdates(Orders &side, const Orders &updates) {
	for (const auto &u : updates) {
		if (u.second == 0.0) {
			// Remove level if exists
			auto it = findPrice(side, u.first);
			if (it != side.end() && it->first == u.first) {
				side.erase(it);
			}
		} else {
			// Insert or update
			auto it = findPrice(side, u.first);
			if (it != side.end() && it->first == u.first) {
				it->second = u.second;  // update quantity
			} else {
				// Insert new level maintaining order
				side.insert(it, u);
			}
		}
	}
}

Orders::iterator OrderBookArray::findPrice(Orders &side, Price p) {
	return std::lower_bound(side.begin(), side.end(), p, [](const Order &o, Price val){
		return o.first < val;
	});
}
