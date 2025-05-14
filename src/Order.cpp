#include "Order.h"

Order::Order() = default;

Order::Order(const nlohmann::json& json) {
    itemId = json.at("itemId").get<int>();
    quantity = json.at("quantity").get<int>();
}

Order::~Order() = default;

nlohmann::json Order::toJson() const {
    return {
        {"id", id},
        {"orderDate", orderDate},
        {"itemId", itemId},
        {"quantity", quantity}
    };
}
