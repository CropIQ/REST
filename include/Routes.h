#pragma once

#include <crow.h>
#include <crow/middlewares/cors.h>
#include "middleware/JWTMiddleware.h"

void register_authRoutes(crow::App<crow::CORSHandler, JWTMiddleware> &app);
void register_workerRoutes(crow::App<crow::CORSHandler, JWTMiddleware> &app);
void register_farmRoutes(crow::App<crow::CORSHandler, JWTMiddleware> &app);