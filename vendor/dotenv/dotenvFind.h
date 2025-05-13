#pragma once

#include "dotenv.h"
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

class dotenvFind : public dotenv {
public:
  static void init() {
      fs::path current = fs::current_path();
      std::cout << current << std::endl;

      while (!fs::exists(current / ".env")) {
        if (current.has_parent_path()) {
          current = current.parent_path();
        } else {
          std::cerr << "[dotenv::initFind] Could not find .env\n";
          return;
        }
    }

    std::string found_path = (current / ".env").string();
    dotenv::init(found_path.c_str());
  }
};