#include "../src/OrderBook.h"
#include "../src/OrderBookArray.h"

#include "Utilities.h"

#include <chrono>
#include <iostream>

int main() {
	MemoryUsageInfo memoryBefore, memoryAfter;


	// Benchmark std::map implementation
	// HERE Start memory measure
	{
		memoryBefore = getMemoryUsageInfo(); // Start memory measurement

		std::cout << "\nBenchmarking std::map implementation...\n";
		// Initialize order book
		OrderBook book("TEST");

		auto start = std::chrono::high_resolution_clock::now();

		// Load the initial snapshot (all bids and asks)
		streamUpdatesIntoOrderBook("data/snapshot.txt", book);
		std::cout << "Initial Snapshot Loaded.\n";

		streamUpdatesIntoOrderBook("data/updates.txt", book);

		auto end = std::chrono::high_resolution_clock::now();

		std::chrono::duration<double> diff = end - start;
		std::cout << "Time Taken: " << diff.count() << " s\n";

		memoryAfter = getMemoryUsageInfo(); // End memory measurement
		printMemoryDifferenceMallinfo("Memory Used by std::map implementation", memoryBefore, memoryAfter);


		const auto bestBid = book.getBestBid();
		const auto bestAsk = book.getBestAsk();
		if (bestBid) std::cout << "Best Bid: " << bestBid->first << " @ " << bestBid->second << "\n";
		if (bestAsk) std::cout << "Best Ask: " << bestAsk->first << " @ " << bestAsk->second << "\n";

	}

	// Benchmark Fixed array impl
	// HERE Start anothert memory measure
	{
		std::cout << "\nBenchmarking fixed array + binary search implementation...\n";

		OrderBookArray book("TEST");

		auto start = std::chrono::high_resolution_clock::now();

		// Load the initial snapshot (all bids and asks)
		streamUpdatesIntoOrderBook("data/snapshot.txt", book);
		std::cout << "Initial Snapshot Loaded.\n";

		streamUpdatesIntoOrderBook("data/updates.txt", book);

		auto end = std::chrono::high_resolution_clock::now();

		std::chrono::duration<double> diff = end - start;
		std::cout << "Time Taken: " << diff.count() << " s\n";

		const size_t memoryUsed = ESTIMATED_MAX_UNIQUE_ORDERS * sizeof(Order);
		std::cout << "Fixed Array Memory Size: " << memoryUsed / 1024.0 << " KB\n";



		const auto bestBid = book.getBestBid();
		const auto bestAsk = book.getBestAsk();
		if (bestBid) std::cout << "Best Bid: " << bestBid->first << " @ " << bestBid->second << "\n";
		if (bestAsk) std::cout << "Best Ask: " << bestAsk->first << " @ " << bestAsk->second << "\n";
	}

	return 0;
}
