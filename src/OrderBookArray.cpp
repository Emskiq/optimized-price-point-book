#include "OrderBookArray.h"

void OrderBookArray::update(const Orders& bidUpdates, const Orders& askUpdates) {
	for (const auto& bidOrder : bidUpdates) {
		applySingleUpdate(bids, countBids, bidOrder);
	}
	for (const auto& askOrder : askUpdates) {
		applySingleUpdate(asks, countAsks, askOrder);
	}
}

std::optional<Order> OrderBookArray::getBestBid() const {
	if (countBids == 0) return std::nullopt;
	return bids[countBids - 1];
}

std::optional<Order> OrderBookArray::getBestAsk() const {
	if (countAsks == 0) return std::nullopt;
	return asks[0];
}


template<size_t N>
void OrderBookArray::applySingleUpdate(OrdersArray<N>& arr, ssize_t &count, const Order &upd) {
	auto [currentPos, insertPos] = findPrice(arr, count, upd.first);

	if (upd.second == 0.0) {
		if (currentPos >= 0) {
			for (auto i = currentPos; i < count - 1; ++i) {
				arr[i] = arr[i+1];
			}
			count--;
		}
	} else {
		if (currentPos >= 0) {
			arr[currentPos].second = upd.second;
		} else {
			// Insert new if we have space
			if (count < ESTIMATED_MAX_UNIQUE_ORDERS) {
				// Shift right
				for (auto i = count; i > insertPos; --i) {
					arr[i] = arr[i-1];
				}
				arr[insertPos] = upd;
				count++;
			} else {
				// No space, ignore or handle differently
			}
		}
	}
}

template <size_t N>
std::pair<ssize_t, ssize_t> OrderBookArray::findPrice(const OrdersArray<N>& arr, const ssize_t& count, Price p) const {
	size_t low = 0;
	size_t high = count;
	while (low < high) {
		size_t mid = (low + high) / 2;
		if (arr[mid].first == p) {
			return {(ssize_t)mid, -1};
		} else if (arr[mid].first < p) {
			low = mid + 1;
		} else {
			high = mid;
		}
	}
	return {-1, low}; // not found + return the insert position
}
