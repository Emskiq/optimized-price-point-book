#pragma once

#include <vector>
#include <algorithm>
#include <optional>
#include <utility>

using Price = double;
using Quantity = double;
using Order = std::pair<Price, Quantity>;
using Orders = std::vector<Order>;

class OrderBookArray final {
public:
	void initFromSnapshot(const Orders& bidsSnapshot, const Orders& asksSnapshot);

	void applyUpdates(const Orders& bidUpdates, const Orders& askUpdates);

	std::optional<Order> getBestBid() const;
	std::optional<Order> getBestAsk() const;

private:
	Orders bids;
	Orders asks;

	void applySideUpdates(Orders &side, const Orders &updates);
	Orders::iterator findPrice(Orders &side, Price p);
};
