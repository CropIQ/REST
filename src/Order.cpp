#include "Order.h"

Order::Order() = default;

Order::Order(const nlohmann::json& json) {
    itemId = json.at("itemId").get<int>();
    quantity = json.at("quantity").get<int>();
}

Order::~Order() = default;

nlohmann::json Order::toJson() const {
    nlohmann::json jsonData;
    jsonData["id"] = id;
    jsonData["orderDate"] = orderDate;
    jsonData["itemId"] = itemId;
    jsonData["quantity"] = quantity;

    return jsonData;
}
