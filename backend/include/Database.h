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

        // Produtos 
        void inserirProduto(const Produto& p, const std::string& tipo,
                            int volume_ml, const std::string& tipo_bebida, bool alcoolica,
                            bool vegana, const std::string& porcao);

        std::vector<std::string> listarProdutosJson() const;
        bool removerProduto(int id);
        bool atualizarPrecoProduto(int id, double novoPreco);
        bool atualizarQuantidadeProduto(int id, int novaQtd);

        // Pedidos 
        int  inserirPedido(int numeroMesa);
        bool inserirItemPedido(int pedidoId, int produtoId, int quantidade, double subtotal);
        std::vector<std::string> listarPedidosJson() const;
        bool atualizarStatusPedido(int id, const std::string& status);

        // Relatório 
        std::string getRelatorioJson() const;
};