#include "Order.h"

Order::Order() = default;

Order::Order(const nlohmann::json& json) {
    itemId = json.at("itemId").get<int>();
    quantity = json.at("quantity").get<int>();
}

Order::~Order() = default;

lohmann::json Order::toJson() const {
    nlohmann::json jsonData;
    jsonData["id"] = orderId;
    jsonData["orderDate"] = orderDate;
    jsonData["itemId"] = itemId;
    jsonData["quantity"] = quantity;

    return jsonData;
}
