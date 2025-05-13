#pragma once

#include <vector>
#include "Item.h"
#include <nlohmann/json.hpp>

class Order {
private:
    int id;
    int orderDate;
    std::vector<Item> orderContent;
    int quantity;

public:
    Order(const int& id, const time_t& orderDate, const int& quantity);
    Order(const nlohmann::json& json);
    ~Order();

    int createOrder(const Item& item, int quantity);
    void cancelOrder(int id);
    std::vector<Order> viewOrderHistory() const;
    std::vector<Order> getPendingOrders() const;
    nlohmann::json toJson() const;

    int getId() const;
    time_t getOrderDate() const;
    std::vector<Item> getOrderContent() const;
    int getQuantity() const;

};