#pragma once

#include <map>
#include <optional>
#include <vector>
#include <utility>
#include <string_view>

// TODO: Rework to convert double to custom integer type
using Price = long double;
using Quantity = long double;

using Order = std::pair<Price, Quantity>;
using Orders = std::vector<Order>;

// The OrderBook class manages the bids and asks for a single symbol.
// It stores price levels in sorted order using std::map.
class OrderBook final {
public:
	explicit OrderBook(std::string_view symbol) : symbol{symbol}
	{ }

	void update(const Orders& bidUpdates, const Orders& askUpdates);

	std::optional<Order> getBestBid() const;
	std::optional<Order> getBestAsk() const;

private:
	const std::string_view symbol;

	std::map<Price, Quantity, std::greater<Price>> bids; // buy orders
	std::map<Price, Quantity, std::less<Price>> asks; // sell orders
};
