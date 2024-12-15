#include "OrderBooksCollection.h"

void OrderBooksCollection::addSymbol(const std::string& symbol)
{
	books.emplace(symbol, OrderBook{symbol});
}

void OrderBooksCollection::applyEvent(const std::string &symbol, const Orders& bidUpdates, const Orders& askUpdates)
{
	auto it = books.find(symbol);
	if (it == books.end()) {
		// Symbol not found, optionally handle error or ignore
		return;
	}
	it->second.update(bidUpdates, askUpdates);
}

std::optional<Order> OrderBooksCollection::getBestBid(const std::string &symbol) const {
	auto it = books.find(symbol);
	if (it == books.end()) return std::nullopt;
	return it->second.getBestBid();
}

std::optional<Order> OrderBooksCollection::getBestAsk(const std::string &symbol) const {
	auto it = books.find(symbol);
	if (it == books.end()) return std::nullopt;
	return it->second.getBestAsk();
}
