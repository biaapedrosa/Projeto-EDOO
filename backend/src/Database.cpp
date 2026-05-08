#include "../include/Database.h"
#include <stdexcept>
#include <sstream>
#include <iomanip>
#include <iostream>


// CONSTRUTOR/ DESTRUTOR
Database::Database(const std::string& caminho) {
    // sqlite3_open abre o arquivo .db (se não existir cria automaticamente)
    if (sqlite3_open(caminho.c_str(), &db) != SQLITE_OK)
        throw std::runtime_error("Erro ao abrir banco de dados: " + std::string(sqlite3_errmsg(db)));

    // Ativa suporte a chaves estrangeiras (desativado por padrão no SQLite)
    executar("PRAGMA foreign_keys = ON;");

    // CREATE TABLE IF NOT EXISTS garante que as tabelas só são criadas na primeira execução — nas seguintes, o comando é ignorado
    executar(R"(
        CREATE TABLE IF NOT EXISTS produtos (
            id          INTEGER PRIMARY KEY,
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
            numero_mesa INTEGER NOT NULL,
            status      TEXT    NOT NULL DEFAULT 'ABERTO',
            total       REAL    NOT NULL DEFAULT 0,
            data_hora   TEXT    NOT NULL
        );
    )");

    // itens_pedido é a tabela de junção entre pedidos e produtos
    // REFERENCES garante integridade referencial (não dá pra inserir um item com pedido_id ou produto_id que não existam)
    executar(R"(
        CREATE TABLE IF NOT EXISTS itens_pedido (
            id          INTEGER PRIMARY KEY AUTOINCREMENT,
            pedido_id   INTEGER NOT NULL REFERENCES pedidos(id),
            produto_id  INTEGER NOT NULL REFERENCES produtos(id),
            quantidade  INTEGER NOT NULL,
            subtotal    REAL    NOT NULL
        );
    )");

    std::cout << "Banco de dados iniciado com sucesso.\n";
}

Database::~Database() {
    // Fecha a conexão com o banco ao destruir o objeto
    if (db) sqlite3_close(db);
}


// HELPERS
void Database::executar(const std::string& sql) {
    char* erro = nullptr;
    // sqlite3_exec executa o SQL e armazena a mensagem de erro em &erro se falhar
    if (sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &erro) != SQLITE_OK) {
        std::string msg = erro ? erro : "erro desconhecido";
        sqlite3_free(erro); // libera a memória alocada pelo sqlite3 para a mensagem
        throw std::runtime_error("Erro SQL: " + msg);
    }
}


// PRODUTOS
void Database::inserirProduto(const Produto& p, const std::string& tipo,
                               int volume_ml, const std::string& tipo_bebida, bool alcoolica,
                               bool vegana, const std::string& porcao) {
    sqlite3_stmt* stmt;
    const char* sql = R"(
        INSERT INTO produtos (id, tipo, nome, preco, categoria, quantidade,
                              volume_ml, tipo_bebida, alcoolica, vegana, porcao)
        VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);
    )";

    // sqlite3_prepare_v2 compila o SQL com ? como placeholders
    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

    // sqlite3_bind_* preenche cada ? com o valor correto (isso evita SQL injection e lida com tipos corretamente)
    sqlite3_bind_int (stmt, 1,  p.getId());
    sqlite3_bind_text (stmt, 2,  tipo.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text (stmt, 3,  p.getNome().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_double(stmt, 4,  p.getPreco());
    sqlite3_bind_text (stmt, 5,  p.getCategoria().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int (stmt, 6,  p.getQuantidadeEmEstoque());
    sqlite3_bind_int (stmt, 7,  volume_ml);
    sqlite3_bind_text (stmt, 8,  tipo_bebida.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int (stmt, 9,  alcoolica ? 1 : 0);
    sqlite3_bind_int (stmt, 10, vegana    ? 1 : 0);
    sqlite3_bind_text (stmt, 11, porcao.c_str(), -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) != SQLITE_DONE)
        throw std::runtime_error("Erro ao inserir produto: " + std::string(sqlite3_errmsg(db)));

    sqlite3_finalize(stmt); // libera a memória do statement
}

std::vector<std::string> Database::listarProdutosJson() const {
    std::vector<std::string> resultado;
    sqlite3_stmt* stmt;
    const char* sql = "SELECT id, tipo, nome, preco, categoria, quantidade, "
                      "volume_ml, tipo_bebida, alcoolica, vegana, porcao FROM produtos;";

    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

    // sqlite3_step retorna SQLITE_ROW enquanto houver linhas para ler
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(2);

        int id = sqlite3_column_int   (stmt, 0);
        auto tipo = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
        auto nome = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
        double preco = sqlite3_column_double(stmt, 3);
        auto categoria = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4)));
        int qtd = sqlite3_column_int   (stmt, 5);

        oss << "{\"id\":" << id
            << ",\"tipo\":\"" << tipo << "\""
            << ",\"nome\":\"" << nome << "\""
            << ",\"preco\":"  << preco
            << ",\"categoria\":\"" << categoria << "\""
            << ",\"quantidade\":" << qtd;

        // Campos extras só aparecem no JSON se o produto for do tipo correto
        if (tipo.find("Bebida") != std::string::npos) {
            int  vol = sqlite3_column_int (stmt, 6);
            auto tipoBeb = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 7)));
            int  alcoolica = sqlite3_column_int (stmt, 8);
            oss << ",\"volume_ml\":" << vol
                << ",\"tipo_bebida\":\"" << tipoBeb << "\""
                << ",\"alcoolica\":"     << (alcoolica ? "true" : "false");
        }

        if (tipo == "Comida") {
            int  vegana = sqlite3_column_int(stmt, 9);
            auto porcao = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 10)));
            oss << ",\"vegana\":"  << (vegana ? "true" : "false")
                << ",\"porcao\":\"" << porcao << "\"";
        }

        oss << "}";
        resultado.push_back(oss.str());
    }

    sqlite3_finalize(stmt);
    return resultado;
}

bool Database::removerProduto(int id) {
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, "DELETE FROM produtos WHERE id = ?;", -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, id);
    bool ok = sqlite3_step(stmt) == SQLITE_DONE && sqlite3_changes(db) > 0;
    sqlite3_finalize(stmt);
    return ok;
}

bool Database::atualizarPrecoProduto(int id, double novoPreco) {
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, "UPDATE produtos SET preco = ? WHERE id = ?;", -1, &stmt, nullptr);
    sqlite3_bind_double(stmt, 1, novoPreco);
    sqlite3_bind_int   (stmt, 2, id);
    // sqlite3_changes retorna quantas linhas foram afetadas — se 0, o id não existe
    bool ok = sqlite3_step(stmt) == SQLITE_DONE && sqlite3_changes(db) > 0;
    sqlite3_finalize(stmt);
    return ok;
}

bool Database::atualizarQuantidadeProduto(int id, int novaQtd) {
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, "UPDATE produtos SET quantidade = ? WHERE id = ?;", -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, novaQtd);
    sqlite3_bind_int(stmt, 2, id);
    bool ok = sqlite3_step(stmt) == SQLITE_DONE && sqlite3_changes(db) > 0;
    sqlite3_finalize(stmt);
    return ok;
}


// PEDIDOS
int Database::inserirPedido(int numeroMesa) {
    sqlite3_stmt* stmt;

    // Gera o timestamp atual no formato DD/MM/AAAA HH:MM
    time_t t = time(nullptr);
    tm* tm_local = localtime(&t);
    char buf[20];
    strftime(buf, sizeof(buf), "%d/%m/%Y %H:%M", tm_local);

    const char* sql = "INSERT INTO pedidos (numero_mesa, status, total, data_hora) "
                      "VALUES (?, 'ABERTO', 0, ?);";
    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    sqlite3_bind_int (stmt, 1, numeroMesa);
    sqlite3_bind_text(stmt, 2, buf, -1, SQLITE_TRANSIENT);

    sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    // sqlite3_last_insert_rowid retorna o id gerado pelo AUTOINCREMENT
    return static_cast<int>(sqlite3_last_insert_rowid(db));
}

bool Database::inserirItemPedido(int pedidoId, int produtoId, int quantidade, double subtotal) {
    sqlite3_stmt* stmt;
    const char* sql = "INSERT INTO itens_pedido (pedido_id, produto_id, quantidade, subtotal) "
                      "VALUES (?, ?, ?, ?);";
    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    sqlite3_bind_int (stmt, 1, pedidoId);
    sqlite3_bind_int (stmt, 2, produtoId);
    sqlite3_bind_int (stmt, 3, quantidade);
    sqlite3_bind_double(stmt, 4, subtotal);

    bool ok = sqlite3_step(stmt) == SQLITE_DONE;
    sqlite3_finalize(stmt);

    // Recalcula o total do pedido somando todos os subtotais dos seus itens
    if (ok) {
        sqlite3_stmt* upd;
        sqlite3_prepare_v2(db,
            "UPDATE pedidos SET total = (SELECT SUM(subtotal) FROM itens_pedido WHERE pedido_id = ?) "
            "WHERE id = ?;", -1, &upd, nullptr);
        sqlite3_bind_int(upd, 1, pedidoId);
        sqlite3_bind_int(upd, 2, pedidoId);
        sqlite3_step(upd);
        sqlite3_finalize(upd);
    }
    return ok;
}

std::vector<std::string> Database::listarPedidosJson() const {
    std::vector<std::string> resultado;
    sqlite3_stmt* stmt;
    // ORDER BY id DESC traz os pedidos mais recentes primeiro
    sqlite3_prepare_v2(db,
        "SELECT id, numero_mesa, status, total, data_hora FROM pedidos ORDER BY id DESC;",
        -1, &stmt, nullptr);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(2);

        int id = sqlite3_column_int (stmt, 0);
        int mesa = sqlite3_column_int (stmt, 1);
        auto status = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
        double total = sqlite3_column_double(stmt, 3);
        auto data = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4)));

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

bool Database::atualizarStatusPedido(int id, const std::string& status) {
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, "UPDATE pedidos SET status = ? WHERE id = ?;", -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, status.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int (stmt, 2, id);
    bool ok = sqlite3_step(stmt) == SQLITE_DONE && sqlite3_changes(db) > 0;
    sqlite3_finalize(stmt);
    return ok;
}


// RELATÓRIO
std::string Database::getRelatorioJson() const {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2);

    // Conta todos os pedidos independente do status
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, "SELECT COUNT(*) FROM pedidos;", -1, &stmt, nullptr);
    sqlite3_step(stmt);
    int totalPedidos = sqlite3_column_int(stmt, 0);
    sqlite3_finalize(stmt);

    // Faturamento considera só pedidos ENTREGUE
    // COALESCE retorna 0 se não houver nenhum pedido entregue
    sqlite3_prepare_v2(db,
        "SELECT COALESCE(SUM(total), 0) FROM pedidos WHERE status = 'ENTREGUE';",
        -1, &stmt, nullptr);
    sqlite3_step(stmt);
    double faturamento = sqlite3_column_double(stmt, 0);
    sqlite3_finalize(stmt);

    // JOIN entre itens_pedido e produtos para somar por produto
    // LIMIT 1 após ORDER BY total DESC pega só o mais vendido
    std::string maisVendido = "-";
    sqlite3_prepare_v2(db, R"(
        SELECT p.nome, SUM(i.quantidade) as total
        FROM itens_pedido i
        JOIN produtos p ON p.id = i.produto_id
        GROUP BY p.id
        ORDER BY total DESC
        LIMIT 1;
    )", -1, &stmt, nullptr);
    if (sqlite3_step(stmt) == SQLITE_ROW)
        maisVendido = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
    sqlite3_finalize(stmt);

    // Produtos com estoque abaixo de 5 unidades
    std::vector<std::string> alertas;
    sqlite3_prepare_v2(db,
        "SELECT nome FROM produtos WHERE quantidade <= 5;",
        -1, &stmt, nullptr);
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