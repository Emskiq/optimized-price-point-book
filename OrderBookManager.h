#pragma once

#include "OrderBook.h"

#include <string>
#include <unordered_map>
#include <optional>

class OrderBookManager {
public:
	void initBook(const std::string &symbol,
	              const Orders& bidsSnapshot,
	              const Orders& asksSnapshot);

	void applyEvent(const std::string &symbol,
	                const Orders& bidUpdates,
	                const Orders& askUpdates);

	std::optional<Order> getBestBid(const std::string &symbol) const;
	std::optional<Order> getBestAsk(const std::string &symbol) const;

private:
	std::unordered_map<std::string, OrderBook> books;
};
