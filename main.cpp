#include "OrderBook.h"
#include "OrderBookArray.h"
#include "OrderBooksCollection.h"

#include "Utilities.h"

#include <random>
#include <chrono>
#include <iostream>

int main() {
	Orders initialBids, initialAsks, bidUpdates, askUpdates;
	fillWithGeneratedData(initialBids, initialAsks, bidUpdates, askUpdates);

	// Benchmark for std::map version of the Order Book
	{
		std::cout << "Benchmark for std::map filling with " << NUM_UPDATES << " updates...\n";

		OrderBook book;
		book.initFromSnapshot("TEST", initialBids, initialAsks);

		auto start = std::chrono::high_resolution_clock::now();
		book.applyUpdates(bidUpdates, askUpdates);
		auto end = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> diff = end - start;

		std::cout << "[std::map] Applied " << NUM_UPDATES << " updates in " << diff.count() << " s\n";
		auto bestBid = book.getBestBid();
		auto bestAsk = book.getBestAsk();

		if (bestBid) {
			std::cout << "Best Bid: " << bestBid->first << " @ " << bestBid->second << "\n";
		}
		if (bestAsk) {
			std::cout << "Best Ask: " << bestAsk->first << " @ " << bestAsk->second << "\n";
		}
	}

	// Benchmark for array version of the Order Book
	{
		std::cout << "Benchmark for sorted vector + binary search filling with " << NUM_UPDATES << " updates...\n";

		OrderBookArray book;
		book.initFromSnapshot(initialBids, initialAsks);

		auto start = std::chrono::high_resolution_clock::now();
		book.applyUpdates(bidUpdates, askUpdates);
		auto end = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> diff = end - start;

		std::cout << "[std::vector + binary search] Applied " << NUM_UPDATES << " updates in " << diff.count() << " s\n";
		auto bestBid = book.getBestBid();
		auto bestAsk = book.getBestAsk();

		if (bestBid) {
			std::cout << "Best Bid: " << bestBid->first << " @ " << bestBid->second << "\n";
		}
		if (bestAsk) {
			std::cout << "Best Ask: " << bestAsk->first << " @ " << bestAsk->second << "\n";
		}

		return 0;
	}
}
