#pragma once

#include <array>
#include <cstdint>
#include <optional>
#include <string_view>
#include <vector>

using Price = double;
using Quantity = double;
using Order = std::pair<Price, Quantity>;

using Orders = std::vector<Order>;

template<size_t N>
using OrdersArray = std::array<Order, N>;

static constexpr uint16_t ESTIMATED_MAX_UNIQUE_ORDERS = 10'000;

class OrderBookArray {
public:
	OrderBookArray(std::string_view const symbol) : symbol{symbol}, countBids(0), countAsks(0) {}

	void update(const Orders& bidUpdates, const Orders& askUpdates);

	std::optional<Order> getBestBid() const;
	std::optional<Order> getBestAsk() const;

private:
	std::string_view const symbol;

	OrdersArray<ESTIMATED_MAX_UNIQUE_ORDERS> bids;
	OrdersArray<ESTIMATED_MAX_UNIQUE_ORDERS> asks;
	ssize_t countBids;
	ssize_t countAsks;


	template <size_t N>
	void applySingleUpdate(OrdersArray<N>& arr, ssize_t &count, const Order &upd) {
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
	std::pair<ssize_t, ssize_t> findPrice(const OrdersArray<N>& arr, const ssize_t& count, Price p) const {
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
};

