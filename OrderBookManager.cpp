#include "OrderBookManager.h"

void OrderBookManager::initBook(const std::string &symbol, const Orders& bidsSnapshot, const Orders& asksSnapshot)
{
	books[symbol].initFromSnapshot(bidsSnapshot, asksSnapshot);
}

void OrderBookManager::applyEvent(const std::string &symbol, const Orders& bidUpdates, const Orders& askUpdates)
{
	auto it = books.find(symbol);
	if (it == books.end()) {
		// Symbol not found, optionally handle error or ignore
		return;
	}
	it->second.applyUpdates(bidUpdates, askUpdates);
}

std::optional<Order> OrderBookManager::getBestBid(const std::string &symbol) const {
	auto it = books.find(symbol);
	if (it == books.end()) return std::nullopt;
	return it->second.getBestBid();
}

std::optional<Order> OrderBookManager::getBestAsk(const std::string &symbol) const {
	auto it = books.find(symbol);
	if (it == books.end()) return std::nullopt;
	return it->second.getBestAsk();
}
