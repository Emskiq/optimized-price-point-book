#include <algorithm>
#include <fstream>
#include <iostream>
#include <random>
#include <vector>

#include <malloc.h> // For mallinfo
#include <sys/resource.h>

using Price = long double;
using Quantity = long double;
using Order = std::pair<Price, Quantity>;
using Orders = std::vector<Order>;

struct MemoryUsageInfo {
	size_t arena;     // Total allocated memory from OS
	size_t hblkhd;    // Total memory allocated in heap
	size_t uordblks;  // Total used memory in heap
};

MemoryUsageInfo getMemoryUsageInfo() {
	struct mallinfo2 info = mallinfo2();
	return {info.arena, info.hblkhd, info.uordblks};
}

void printMemoryDifferenceMallinfo(const std::string& label, const MemoryUsageInfo& before, const MemoryUsageInfo& after) {
	std::cout << label << ": Memory Used = "
		<< (after.uordblks - before.uordblks) / 1024.0 << " KB\n";
}

template<typename OrderBookType>
void streamUpdatesIntoOrderBook(const std::string& filename, OrderBookType& orderBook, size_t chunkSize = 1000) {
	std::ifstream in(filename);
	if (!in) {
		std::cerr << "Failed to open " << filename << " for reading." << std::endl;
		return;
	}

	Orders bidUpdates, askUpdates;
	bidUpdates.reserve(chunkSize);
	askUpdates.reserve(chunkSize);

	Price price;
	Quantity quantity;
	char side;
	size_t count = 0;

	while (in >> side >> price >> quantity) {
		if (side == 'b') {
			bidUpdates.emplace_back(price, quantity);
		} else if (side == 'a') {
			askUpdates.emplace_back(price, quantity);
		}

		++count;

		// Process in chunks when enough data is collected
		if (count == chunkSize) {
			orderBook.update(bidUpdates, askUpdates);
			bidUpdates.clear();
			askUpdates.clear();
			count = 0;
		}
	}

	// Process any remaining data
	if (!bidUpdates.empty() || !askUpdates.empty()) {
		orderBook.update(bidUpdates, askUpdates);
	}
}
