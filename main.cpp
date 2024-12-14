#include "OrderBookManager.h"

#include <iostream>

int main() {
	OrderBookManager mgr;

	const std::string symbol = "BNBBTC";
	std::vector<std::pair<Price, Quantity>> initialBids = {
		{0.0024, 14.7},
		{0.0022, 6.4},
		{0.0020, 9.7}
	};
	std::vector<std::pair<Price, Quantity>> initialAsks = {
		{0.0024, 14.9},
		{0.0026, 3.6},
		{0.0028, 1.0}
	};

	mgr.initBook(symbol, initialBids, initialAsks);

	auto bestBid = mgr.getBestBid(symbol);
	auto bestAsk = mgr.getBestAsk(symbol);

	if (bestBid) {
		std::cout << "Initial Best Bid: " << bestBid->first << " @ " << bestBid->second << "\n";
	} else {
		std::cout << "No bids.\n";
	}

	if (bestAsk) {
		std::cout << "Initial Best Ask: " << bestAsk->first << " @ " << bestAsk->second << "\n";
	} else {
		std::cout << "No asks.\n";
	}

	std::vector<std::pair<Price, Quantity>> bidUpdates = { {0.0024, 10} };
	std::vector<std::pair<Price, Quantity>> askUpdates = { {0.0026, 100} };

	mgr.applyEvent(symbol, bidUpdates, askUpdates);

	bestBid = mgr.getBestBid(symbol);
	bestAsk = mgr.getBestAsk(symbol);

	if (bestBid) {
		std::cout << "Updated Best Bid: " << bestBid->first << " @ " << bestBid->second << "\n";
	} else {
		std::cout << "No bids.\n";
	}

	if (bestAsk) {
		std::cout << "Updated Best Ask: " << bestAsk->first << " @ " << bestAsk->second << "\n";
	} else {
		std::cout << "No asks.\n";
	}

	return 0;
}

