#pragma once
#include <string>
#include <vector>
#include "sqlite3.h"
#include "Produto.h"
#include "Pedido.h"

// Classe que faz a comunicação com o banco de dados SQLite
// Abre e fecha a conexão, cria as tabelas e expõe métodos CRUD para cada entidade
class Database {
    private:
        sqlite3* db; // ponteiro para a conexão com o banco

        // Executa um comando SQL simples (sem retorno de linhas)
        void executar(const std::string& sql);

    public:
        // Abre (ou cria) o arquivo orla360.db e inicializa as tabelas
        Database(const std::string& caminho = "orla360.db");
        ~Database();

        // Cria uma nova barraca; retorna false se o usuário já existir
        bool registrarBarraca(const std::string& nome,
                              const std::string& usuario,
                              const std::string& senhaHash);

        // Valida usuário/senha; se ok, gera token, salva e retorna ele
        // Retorna "" se credenciais inválidas
        std::string loginBarraca(const std::string& usuario,
                                 const std::string& senhaHash);

        // Busca o barraca_id dono do token; retorna -1 se inválido
        int getBarracaIdPorToken(const std::string& token) const;

        // Produtos 
        void inserirProduto(const Produto& p, const std::string& tipo,
                            int volume_ml, const std::string& tipo_bebida, bool alcoolica,
                            bool vegana, const std::string& porcao, int barraca_id);

        std::vector<std::string> listarProdutosJson(int barraca_id) const;
        bool removerProduto(int id, int barraca_id);
        bool atualizarPrecoProduto(int id, double novoPreco, int barraca_id);
        bool atualizarQuantidadeProduto(int id, int novaQtd, int barraca_id);

        // Pedidos 
        int  inserirPedido(int numeroMesa, int barraca_id);
        bool inserirItemPedido(int pedidoId, int produtoId, int quantidade, double subtotal);
        std::vector<std::string> listarPedidosJson(int barraca_id) const;
        bool atualizarStatusPedido(int id, const std::string& status, int barraca_id);

        // Relatório 
        std::string getRelatorioJson(int barraca_id) const;
};