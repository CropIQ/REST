#include "Medicament.h"

Medicament::Medicament(int id, const std::string& name, const std::string& description, time_t injectionDate)
    : medicamentId(id), name(name), description(description), injectionDate(injectionDate) {}

Medicament::Medicament(const nlohmann::json& json) {
    medicamentId = json.at("medicamentId").get<int>();
    name = json.at("name").get<std::string>();
    description = json.at("description").get<std::string>();
    injectionDate = json.at("injectionDate").get<time_t>();
}

nlohmann::json Medicament::toJson() const {
    return {
        {"medicamentId", medicamentId},
        {"name", name},
        {"description", description},
        {"injectionDate", injectionDate}
    };
}

int Medicament::getId() const { return medicamentId; }
std::string Medicament::getName() const { return name; }
std::string Medicament::getDescription() const { return description; }
time_t Medicament::getInjectionDate() const { return injectionDate; }