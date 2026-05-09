#include "../include/Database.h"
#include <stdexcept>
#include <sstream>
#include <iomanip>
#include <iostream>

// CONSTRUTOR/ DESTRUTOR
Database::Database(const std::string& caminho) {
    if (sqlite3_open(caminho.c_str(), &db) != SQLITE_OK)
        throw std::runtime_error("Erro ao abrir banco: " + std::string(sqlite3_errmsg(db)));

    executar("PRAGMA foreign_keys = ON;");

    // Tabela de barracas — cada barraca tem usuário único e um token de sessão
    executar(R"(
        CREATE TABLE IF NOT EXISTS barracas (
            id         INTEGER PRIMARY KEY AUTOINCREMENT,
            nome       TEXT    NOT NULL,
            usuario    TEXT    NOT NULL UNIQUE,
            senha_hash TEXT    NOT NULL,
            token      TEXT
        );
    )");

    // barraca_id vincula cada produto à sua barraca dona
    executar(R"(
        CREATE TABLE IF NOT EXISTS produtos (
            id          INTEGER PRIMARY KEY,
            barraca_id  INTEGER NOT NULL REFERENCES barracas(id),
            tipo        TEXT    NOT NULL,
            nome        TEXT    NOT NULL,
            preco       REAL    NOT NULL,
            categoria   TEXT    NOT NULL,
            quantidade  INTEGER NOT NULL,
            volume_ml   INTEGER,
            tipo_bebida TEXT,
            alcoolica   INTEGER,
            vegana      INTEGER,
            porcao      TEXT
        );
    )");

    executar(R"(
        CREATE TABLE IF NOT EXISTS pedidos (
            id          INTEGER PRIMARY KEY AUTOINCREMENT,
            barraca_id  INTEGER NOT NULL REFERENCES barracas(id),
            numero_mesa INTEGER NOT NULL,
            status      TEXT    NOT NULL DEFAULT 'ABERTO',
            total       REAL    NOT NULL DEFAULT 0,
            data_hora   TEXT    NOT NULL
        );
    )");

    executar(R"(
        CREATE TABLE IF NOT EXISTS itens_pedido (
            id         INTEGER PRIMARY KEY AUTOINCREMENT,
            pedido_id  INTEGER NOT NULL REFERENCES pedidos(id),
            produto_id INTEGER NOT NULL REFERENCES produtos(id),
            quantidade INTEGER NOT NULL,
            subtotal   REAL    NOT NULL
        );
    )");

    std::cout << "Banco de dados iniciado com sucesso.\n";
}

Database::~Database() {
    if (db) sqlite3_close(db);
}

// HELPERS
void Database::executar(const std::string& sql) {
    char* erro = nullptr;
    if (sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &erro) != SQLITE_OK) {
        std::string msg = erro ? erro : "erro desconhecido";
        sqlite3_free(erro);
        throw std::runtime_error("Erro SQL: " + msg);
    }
}

//AUTENTICAÇÃO
bool Database::registrarBarraca(const std::string& nome,
                                 const std::string& usuario,
                                 const std::string& senhaHash) {
    sqlite3_stmt* stmt;
    const char* sql = "INSERT INTO barracas (nome, usuario, senha_hash) VALUES (?, ?, ?);";
    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, nome.c_str(),      -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, usuario.c_str(),   -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, senhaHash.c_str(), -1, SQLITE_TRANSIENT);

    bool ok = sqlite3_step(stmt) == SQLITE_DONE;
    sqlite3_finalize(stmt);
    return ok; // false se o usuário já existir (UNIQUE constraint)
}

std::string Database::loginBarraca(const std::string& usuario,
                                    const std::string& senhaHash) {
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db,
        "SELECT id FROM barracas WHERE usuario = ? AND senha_hash = ?;",
        -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, usuario.c_str(),   -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, senhaHash.c_str(), -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) != SQLITE_ROW) {
        sqlite3_finalize(stmt);
        return ""; // credenciais inválidas
    }
    int id = sqlite3_column_int(stmt, 0);
    sqlite3_finalize(stmt);

    // Token simples: "barraca_<id>_<timestamp>"
    std::string token = "barraca_" + std::to_string(id)
                      + "_" + std::to_string(time(nullptr));

    // Salva o token no banco para validação futura
    sqlite3_prepare_v2(db,
        "UPDATE barracas SET token = ? WHERE id = ?;",
        -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, token.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int (stmt, 2, id);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    return token;
}

int Database::getBarracaIdPorToken(const std::string& token) const {
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db,
        "SELECT id FROM barracas WHERE token = ?;",
        -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, token.c_str(), -1, SQLITE_TRANSIENT);

    int id = -1; // -1 significa token inválido
    if (sqlite3_step(stmt) == SQLITE_ROW)
        id = sqlite3_column_int(stmt, 0);

    sqlite3_finalize(stmt);
    return id;
}

//PRODUTOS
void Database::inserirProduto(const Produto& p, const std::string& tipo,
                               int volume_ml, const std::string& tipo_bebida,
                               bool alcoolica, bool vegana,
                               const std::string& porcao,
                               int barraca_id) {
    sqlite3_stmt* stmt;
    const char* sql = R"(
        INSERT INTO produtos (id, barraca_id, tipo, nome, preco, categoria, quantidade,
                              volume_ml, tipo_bebida, alcoolica, vegana, porcao)
        VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);
    )";
    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    sqlite3_bind_int   (stmt,  1, p.getId());
    sqlite3_bind_int   (stmt,  2, barraca_id);
    sqlite3_bind_text  (stmt,  3, tipo.c_str(),             -1, SQLITE_TRANSIENT);
    sqlite3_bind_text  (stmt,  4, p.getNome().c_str(),      -1, SQLITE_TRANSIENT);
    sqlite3_bind_double(stmt,  5, p.getPreco());
    sqlite3_bind_text  (stmt,  6, p.getCategoria().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int   (stmt,  7, p.getQuantidadeEmEstoque());
    sqlite3_bind_int   (stmt,  8, volume_ml);
    sqlite3_bind_text  (stmt,  9, tipo_bebida.c_str(),      -1, SQLITE_TRANSIENT);
    sqlite3_bind_int   (stmt, 10, alcoolica ? 1 : 0);
    sqlite3_bind_int   (stmt, 11, vegana    ? 1 : 0);
    sqlite3_bind_text  (stmt, 12, porcao.c_str(),           -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) != SQLITE_DONE)
        throw std::runtime_error("Erro ao inserir produto: " + std::string(sqlite3_errmsg(db)));
    sqlite3_finalize(stmt);
}

std::vector<std::string> Database::listarProdutosJson(int barraca_id) const {
    std::vector<std::string> resultado;
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db,
        "SELECT id, tipo, nome, preco, categoria, quantidade, "
        "volume_ml, tipo_bebida, alcoolica, vegana, porcao "
        "FROM produtos WHERE barraca_id = ?;",
        -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, barraca_id);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(2);

        int id          = sqlite3_column_int(stmt, 0);
        auto tipo       = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
        auto nome       = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
        double preco    = sqlite3_column_double(stmt, 3);
        auto categoria  = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4)));
        int qtd         = sqlite3_column_int(stmt, 5);

        oss << "{\"id\":" << id
            << ",\"tipo\":\"" << tipo << "\""
            << ",\"nome\":\"" << nome << "\""
            << ",\"preco\":"  << preco
            << ",\"categoria\":\"" << categoria << "\""
            << ",\"quantidade\":" << qtd;

        if (tipo.find("Bebida") != std::string::npos) {
            int  vol      = sqlite3_column_int(stmt, 6);
            auto tipoBeb  = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 7)));
            int  alcool   = sqlite3_column_int(stmt, 8);
            oss << ",\"volume_ml\":" << vol
                << ",\"tipo_bebida\":\"" << tipoBeb << "\""
                << ",\"alcoolica\":"     << (alcool ? "true" : "false");
        }
        if (tipo == "Comida") {
            int  veg    = sqlite3_column_int(stmt, 9);
            auto porcao = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 10)));
            oss << ",\"vegana\":"   << (veg ? "true" : "false")
                << ",\"porcao\":\"" << porcao << "\"";
        }
        oss << "}";
        resultado.push_back(oss.str());
    }
    sqlite3_finalize(stmt);
    return resultado;
}

bool Database::removerProduto(int id, int barraca_id) {
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db,
        "DELETE FROM produtos WHERE id = ? AND barraca_id = ?;",
        -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, id);
    sqlite3_bind_int(stmt, 2, barraca_id);
    bool ok = sqlite3_step(stmt) == SQLITE_DONE && sqlite3_changes(db) > 0;
    sqlite3_finalize(stmt);
    return ok;
}

bool Database::atualizarPrecoProduto(int id, double novoPreco, int barraca_id) {
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db,
        "UPDATE produtos SET preco = ? WHERE id = ? AND barraca_id = ?;",
        -1, &stmt, nullptr);
    sqlite3_bind_double(stmt, 1, novoPreco);
    sqlite3_bind_int   (stmt, 2, id);
    sqlite3_bind_int   (stmt, 3, barraca_id);
    bool ok = sqlite3_step(stmt) == SQLITE_DONE && sqlite3_changes(db) > 0;
    sqlite3_finalize(stmt);
    return ok;
}

bool Database::atualizarQuantidadeProduto(int id, int novaQtd, int barraca_id) {
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db,
        "UPDATE produtos SET quantidade = ? WHERE id = ? AND barraca_id = ?;",
        -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, novaQtd);
    sqlite3_bind_int(stmt, 2, id);
    sqlite3_bind_int(stmt, 3, barraca_id);
    bool ok = sqlite3_step(stmt) == SQLITE_DONE && sqlite3_changes(db) > 0;
    sqlite3_finalize(stmt);
    return ok;
}

//PEDIDOS
int Database::inserirPedido(int numeroMesa, int barraca_id) {
    sqlite3_stmt* stmt;
    time_t t = time(nullptr);
    tm* tm_local = localtime(&t);
    char buf[20];
    strftime(buf, sizeof(buf), "%d/%m/%Y %H:%M", tm_local);

    sqlite3_prepare_v2(db,
        "INSERT INTO pedidos (barraca_id, numero_mesa, status, total, data_hora) "
        "VALUES (?, ?, 'ABERTO', 0, ?);",
        -1, &stmt, nullptr);
    sqlite3_bind_int (stmt, 1, barraca_id);
    sqlite3_bind_int (stmt, 2, numeroMesa);
    sqlite3_bind_text(stmt, 3, buf, -1, SQLITE_TRANSIENT);

    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    return static_cast<int>(sqlite3_last_insert_rowid(db));
}

bool Database::inserirItemPedido(int pedidoId, int produtoId,
                                  int quantidade, double subtotal) {
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db,
        "INSERT INTO itens_pedido (pedido_id, produto_id, quantidade, subtotal) "
        "VALUES (?, ?, ?, ?);",
        -1, &stmt, nullptr);
    sqlite3_bind_int   (stmt, 1, pedidoId);
    sqlite3_bind_int   (stmt, 2, produtoId);
    sqlite3_bind_int   (stmt, 3, quantidade);
    sqlite3_bind_double(stmt, 4, subtotal);

    bool ok = sqlite3_step(stmt) == SQLITE_DONE;
    sqlite3_finalize(stmt);

    if (ok) {
        sqlite3_prepare_v2(db,
            "UPDATE pedidos SET total = "
            "(SELECT SUM(subtotal) FROM itens_pedido WHERE pedido_id = ?) "
            "WHERE id = ?;",
            -1, &stmt, nullptr);
        sqlite3_bind_int(stmt, 1, pedidoId);
        sqlite3_bind_int(stmt, 2, pedidoId);
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }
    return ok;
}

std::vector<std::string> Database::listarPedidosJson(int barraca_id) const {
    std::vector<std::string> resultado;
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db,
        "SELECT id, numero_mesa, status, total, data_hora "
        "FROM pedidos WHERE barraca_id = ? ORDER BY id DESC;",
        -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, barraca_id);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(2);
        int id       = sqlite3_column_int(stmt, 0);
        int mesa     = sqlite3_column_int(stmt, 1);
        auto status  = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
        double total = sqlite3_column_double(stmt, 3);
        auto data    = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4)));

        oss << "{\"id\":" << id
            << ",\"mesa\":" << mesa
            << ",\"status\":\"" << status << "\""
            << ",\"total\":" << total
            << ",\"dataHora\":\"" << data << "\"}";
        resultado.push_back(oss.str());
    }
    sqlite3_finalize(stmt);
    return resultado;
}

bool Database::atualizarStatusPedido(int id, const std::string& status, int barraca_id) {
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db,
        "UPDATE pedidos SET status = ? WHERE id = ? AND barraca_id = ?;",
        -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, status.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int (stmt, 2, id);
    sqlite3_bind_int (stmt, 3, barraca_id);
    bool ok = sqlite3_step(stmt) == SQLITE_DONE && sqlite3_changes(db) > 0;
    sqlite3_finalize(stmt);
    return ok;
}

//RELATÓRIO
std::string Database::getRelatorioJson(int barraca_id) const {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2);

    sqlite3_stmt* stmt;

    sqlite3_prepare_v2(db,
        "SELECT COUNT(*) FROM pedidos WHERE barraca_id = ?;",
        -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, barraca_id);
    sqlite3_step(stmt);
    int totalPedidos = sqlite3_column_int(stmt, 0);
    sqlite3_finalize(stmt);

    sqlite3_prepare_v2(db,
        "SELECT COALESCE(SUM(total), 0) FROM pedidos "
        "WHERE barraca_id = ? AND status = 'ENTREGUE';",
        -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, barraca_id);
    sqlite3_step(stmt);
    double faturamento = sqlite3_column_double(stmt, 0);
    sqlite3_finalize(stmt);

    std::string maisVendido = "-";
    sqlite3_prepare_v2(db, R"(
        SELECT p.nome, SUM(i.quantidade) as total
        FROM itens_pedido i
        JOIN produtos p ON p.id = i.produto_id
        JOIN pedidos   pd ON pd.id = i.pedido_id
        WHERE pd.barraca_id = ?
        GROUP BY p.id
        ORDER BY total DESC
        LIMIT 1;
    )", -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, barraca_id);
    if (sqlite3_step(stmt) == SQLITE_ROW)
        maisVendido = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
    sqlite3_finalize(stmt);

    std::vector<std::string> alertas;
    sqlite3_prepare_v2(db,
        "SELECT nome FROM produtos WHERE barraca_id = ? AND quantidade <= 5;",
        -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, barraca_id);
    while (sqlite3_step(stmt) == SQLITE_ROW)
        alertas.push_back(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
    sqlite3_finalize(stmt);

    oss << "{\"totalPedidos\":" << totalPedidos
        << ",\"faturamento\":"  << faturamento
        << ",\"maisVendido\":\"" << maisVendido << "\""
        << ",\"alertas\":[";
    for (size_t i = 0; i < alertas.size(); ++i) {
        oss << "\"" << alertas[i] << "\"";
        if (i + 1 < alertas.size()) oss << ",";
    }
    oss << "]}";
    return oss.str();
}