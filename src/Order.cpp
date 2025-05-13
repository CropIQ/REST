#include "Order.h"

Order::Order(const int& id, const time_t& orderDate, const int& quantity) :
    id(id),
    orderDate(orderDate),
    quantity(quantity) {}

Order::Order(const nlohmann::json& json) {
    id = json.at("id").get<int>();
    orderDate = json.at("orderDate").get<time_t>();
    quantity = json.at("quantity").get<int>();
}

Order::~Order() = default;

int Order::createOrder(const Item& item, int quantity) {
    orderContent.push_back(item);
    this->quantity = quantity;
    return id;
}

void Order::cancelOrder(int id) {// TODO
    
}

// View order history (to be extended with database retrieval)
std::vector<Order> Order::viewOrderHistory() const { //TODO
    // Placeholder implementation
    return {};
}

// Get pending orders (to be extended with database retrieval)
std::vector<Order> Order::getPendingOrders() const { //TODO
    // Placeholder implementation
    return {};
}

int Order::getId() const {
    return id;
}

time_t Order::getOrderDate() const {
    return orderDate;
}

std::vector<Item> Order::getOrderContent() const {
    return orderContent;
}

int Order::getQuantity() const {
    return quantity;
}

lohmann::json Order::toJson() const {
    nlohmann::json jsonData;
    jsonData["id"] = orderId;
    jsonData["orderDate"] = orderDate;
    jsonData["quantity"] = quantity;

    jsonData["orderContent"] = nlohmann::json::array();
    for (const auto& item : orderContent) {
        jsonData["orderContent"].push_back(item.toJson()); // Assuming Item has a toJson() method
    }

    return jsonData;
}