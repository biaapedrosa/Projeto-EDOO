#include <iostream>
#include "include/httplib.h"
#include "include/Database.h"
#include "include/routes.h"

int main() {
    // Singleton obtém a única instância do banco de dados
    // Na primeira chamada cria o objeto e nas seguintes retorna o mesmo
    Database& db = Database::getInstance();

    // Cria o servidor HTTP
    httplib::Server svr;

    // Registra todas as rotas (produtos, estoque, pedidos, relatório)
    registrarRotas(svr, db);

    std::cout << "╔══════════════════════════════╗\n";
    std::cout << "║   Orla360 — Servidor HTTP    ║\n";
    std::cout << "║   Rodando em :8080           ║\n";
    std::cout << "╚══════════════════════════════╝\n";
    std::cout << "Pressione Ctrl+C para encerrar.\n\n";

    // Inicia o servidor — bloqueia até Ctrl+C
    svr.listen("0.0.0.0", 8080);

    return 0;
}