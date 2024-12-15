#include <fstream>
#include <random>
#include <filesystem>
#include <unordered_map>
#include <unordered_set>

#include "../src/OrderBook.h"

#include "Utilities.h"

namespace fs = std::filesystem;

static constexpr size_t NUM_LEVELS = 1000;
static constexpr size_t NUM_UPDATES = 1'000'000;

double roundDecimal(double value) {
	return std::round(value * 10000.0) / 10000.0;
}

int main() {
	if (!fs::exists("data")) {
		fs::create_directories("data");
	}

	const std::string snapshotFile = "data/snapshot.txt";
	const std::string updatesFile = "data/updates.txt";

	std::ofstream snapshotOut(snapshotFile);
	std::ofstream updatesOut(updatesFile);

	if (!snapshotOut || !updatesOut) {
		std::cerr << "Failed to open files for writing." << std::endl;
		return 1;
	}

	std::mt19937_64 rng(std::random_device{}());
	std::uniform_real_distribution<double> priceDist(0.0, 1.0);
	std::uniform_real_distribution<double> qtyDist(1.0, 100.0);
	std::uniform_int_distribution<int> removeDist(1, 20);        // 5% chance to remove

	Orders bidPrices, askPrices;

	// Step 1: Generate the initial snapshot
	std::unordered_set<Price> uniqueBidPrices, uniqueAskPrices;

	for (size_t i = 0; i < NUM_LEVELS; ++i) {
		Price bidPrice = roundDecimal(priceDist(rng));
		Price askPrice = roundDecimal(priceDist(rng));

		if (uniqueBidPrices.insert(bidPrice).second) {
			Quantity bidQty = qtyDist(rng);
			snapshotOut << "b " << bidPrice << " " << bidQty << "\n";
			bidPrices.emplace_back(bidPrice, bidQty);
		}

		if (uniqueAskPrices.insert(askPrice).second) {
			Quantity askQty = qtyDist(rng);
			snapshotOut << "a " << askPrice << " " << askQty << "\n";
			askPrices.emplace_back(askPrice, askQty);
		}
	}
	snapshotOut.close();
	std::cout << "Initial Snapshot written to " << snapshotFile << "\n";

	// Step 2: Generate updates (80-90% existing prices, 10-20% new prices)
	constexpr double EXISTING_RATIO = 0.9;
	size_t existingCount = static_cast<size_t>(NUM_UPDATES * EXISTING_RATIO);
	size_t newCount = NUM_UPDATES - existingCount;

	std::uniform_int_distribution<size_t> bidIndexDist(0, bidPrices.size() - 1);
	std::uniform_int_distribution<size_t> askIndexDist(0, askPrices.size() - 1);

	// Updates for existing price levels
	for (size_t i = 0; i < existingCount; ++i) {
		bool isBid = (rng() % 2 == 0);
		if (isBid) {
			size_t index = bidIndexDist(rng);
			Price price = bidPrices[index].first;
			Quantity qty = (removeDist(rng) == 1) ? 0.0 : qtyDist(rng);
			updatesOut << "b " << price << " " << qty << "\n";
		} else {
			size_t index = askIndexDist(rng);
			Price price = askPrices[index].first;
			Quantity qty = (removeDist(rng) == 1) ? 0.0 : qtyDist(rng);
			updatesOut << "a " << price << " " << qty << "\n";
		}
	}

	// Introduce new price levels
	for (size_t i = 0; i < newCount; ++i) {
		bool isBid = (rng() % 2 == 0);
		Price price = roundDecimal(priceDist(rng));
		Quantity qty = qtyDist(rng);
		updatesOut << (isBid ? "b " : "a ") << price << " " << qty << "\n";
	}

	updatesOut.close();
	std::cout << "Updates written to " << updatesFile << "\n";
}
