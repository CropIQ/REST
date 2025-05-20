#pragma once
#pragma warning(disable: 4996)

#include "dotenv.h"
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

class dotenvFind : public dotenv {
public:
  dotenvFind() = delete;
  ~dotenvFind() = delete;

  static void init() {
    fs::path current = fs::current_path();

    while (!fs::exists(current / ".env")) {
      fs::path parent = current.parent_path();
      if (parent == current) {
        std::cout << "[dotenv::initFind] Could not find .env\n";
        exit(1);
      }

      current = parent;
    }

    std::string found_path = (current / ".env").string();
    dotenv::init(found_path.c_str());
  }
};