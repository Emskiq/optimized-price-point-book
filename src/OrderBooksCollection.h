#pragma once

#include "OrderBook.h"
#include "OrderBookArray.h"

#include <string>
#include <unordered_map>
#include <optional>

class OrderBooksCollection final {
public:
	void addSymbol(const std::string& symbol);
	void applyEvent(const std::string &symbol, const Orders& bidUpdates, const Orders& askUpdates);

	std::optional<Order> getBestBid(const std::string &symbol) const;
	std::optional<Order> getBestAsk(const std::string &symbol) const;

private:
	std::unordered_map<std::string, OrderBook> books{};
};