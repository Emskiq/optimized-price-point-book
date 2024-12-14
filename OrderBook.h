#pragma once

#include <map>
#include <optional>
#include <vector>
#include <utility>

// TODO: Rework to be integer pbly
using Price = double;
using Quantity = double;

using Order = std::pair<Price, Quantity>;
using Orders = std::vector<Order>;

// The OrderBook class manages the bids and asks for a single symbol.
// It stores price levels in sorted order using std::map.
class OrderBook {
public:
	void initFromSnapshot(const Orders& bidsSnapshot, const Orders& asksSnapshot);

	void applyUpdates(const Orders& bidUpdates,
	                  const Orders& askUpdates);

	std::optional<Order> getBestBid() const;
	std::optional<Order> getBestAsk() const;

private:
	std::map<Price, Quantity> bids; 
	std::map<Price, Quantity> asks; 
};
