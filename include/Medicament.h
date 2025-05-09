#ifndef MEDICAMENT_H
#define MEDICAMENT_H
#include <nlohmann/json.hpp>

class Medicament {
private:
    int medicamentId;
    std::string name;
    std::string description;
    time_t injectionDate;
public:
    Medicament(int id, const std::string& name, const std::string& description, time_t injectionDate);
    Medicament(const nlohmann::json& json);
    nlohmann::json toJson() const;
    int getId() const;
    std::string getName() const;
    std::string getDescription() const;
    time_t getInjectionDate() const;
};
#endif // MEDICAMENT_H