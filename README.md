# CropIQ REST API
Read [`INSTALL.md`](./INSTALL.md) for installation instructions.

Read [`DATABASE.md`](./DATABASE.md) to setup a MariaDB database.

## Usage

### Compiling the project
1. Open a terminal and navigate to the project directory.
2. Open `build` directory and run cmake compilation (syntax may vary depending on your OS).
  - For example, on Windows, you can use the following command:
  ```bash
  cmake ..
  cmake --build .
  ```
  - On Linux, you can use the following command:
  ```bash
  cmake ..
  make
  ```
3. Depending on your OS, find the executable called `cropiq` and run it.