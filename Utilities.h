#include <vector>
#include <random>
#include <algorithm>

using Price = double;
using Quantity = double;
using Order = std::pair<Price, Quantity>;
using Orders = std::vector<Order>;

static constexpr size_t NUM_LEVELS = 10000; // 1 million
static constexpr size_t NUM_UPDATES = 10000; // 1 million

static void fillWithGeneratedData(Orders &initialBids, Orders &initialAsks, Orders &bidUpdates, Orders &askUpdates) 
{
	std::mt19937_64 rng(std::random_device{}());
	std::uniform_real_distribution<double> priceDist(0.001, 1000.0);
	std::uniform_real_distribution<double> qtyDist(1.0, 100.0);
	std::uniform_int_distribution<int> removeDist(0, 999);

	initialBids.reserve(NUM_LEVELS);
	initialAsks.reserve(NUM_LEVELS);

	// Generate initial snapshot
	for (size_t i = 0; i < NUM_LEVELS; ++i) {
		double bidPrice = priceDist(rng) * 0.5; // generally lower
		double askPrice = 500.0 + priceDist(rng) * 0.5; // generally higher
		double bidQty = qtyDist(rng);
		double askQty = qtyDist(rng);

		initialBids.push_back({bidPrice, bidQty});
		initialAsks.push_back({askPrice, askQty});
	}

	std::sort(initialBids.begin(), initialBids.end(), [](const Order &a, const Order &b){return a.first < b.first;});
	std::sort(initialAsks.begin(), initialAsks.end(), [](const Order &a, const Order &b){return a.first < b.first;});

	// Generate updates
	bidUpdates.reserve(NUM_UPDATES);
	askUpdates.reserve(NUM_UPDATES);

	for (size_t i = 0; i < NUM_UPDATES; ++i) {
		double updateBidPrice = priceDist(rng) * 0.5;
		double updateAskPrice = 500.0 + priceDist(rng) * 0.5;
		double newBidQty = (removeDist(rng) == 0) ? 0.0 : qtyDist(rng);
		double newAskQty = (removeDist(rng) == 0) ? 0.0 : qtyDist(rng);

		bidUpdates.push_back({updateBidPrice, newBidQty});
		askUpdates.push_back({updateAskPrice, newAskQty});
	}
}
