#pragma once

#include <atomic>
#include <array>
#include <cstdint>
#include <mutex>
#include <optional>
#include <string_view>
#include <vector>

using Price = long double;
using Quantity = long double;
using Order = std::pair<Price, Quantity>;

using Orders = std::vector<Order>;

template<size_t N>
using OrdersArray = std::array<Order, N>;

static constexpr uint16_t ESTIMATED_MAX_UNIQUE_ORDERS = 10'000;

class OrderBookArray final {
public:
	OrderBookArray(std::string_view const symbol) : symbol{symbol}, countBids(0), countAsks(0) {}

	void update(const Orders& bidUpdates, const Orders& askUpdates);

	std::optional<Order> getBestBid() const;
	std::optional<Order> getBestAsk() const;

private:
	const std::string_view symbol;

	OrdersArray<ESTIMATED_MAX_UNIQUE_ORDERS> bids;
	OrdersArray<ESTIMATED_MAX_UNIQUE_ORDERS> asks;
	ssize_t countBids;
	ssize_t countAsks;

	template<size_t N>
	void applySingleUpdate(OrdersArray<N>& arr, ssize_t &count, const Order &upd);

	template<size_t N>
	std::pair<ssize_t, ssize_t> findPrice(const OrdersArray<N>& arr, const ssize_t& count, Price p) const;
};
