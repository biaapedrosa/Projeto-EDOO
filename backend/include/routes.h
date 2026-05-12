#pragma once
#include "httplib.h"
#include "Database.h"

// Registra todas as rotas HTTP no servidor
// Chamada uma única vez no main.cpp antes de svr.listen()
void registrarRotas(httplib::Server& svr, Database& db);