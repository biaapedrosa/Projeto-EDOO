#pragma once
#include <string>
#include <vector>
#include "Produto.h"

// Classe responsável por gerenciar o estoque de produtos
class Estoque{
    private:
        // Vetor que armazena os produtos cadastrados
        std::vector<Produto> m_produtos;

        // Estoque mínimo para considerar baixo
        static constexpr int ESTOQUE_MINIMO = 5;

    public:
        // Adiciona produto; lança se já existir produto com mesmo id
        void adicionarProduto(const Produto& p);

        // Lista todos os produtos
        void listarProdutos() const;

        // Busca por nome, retorna "nullptr" caso não encontre o produto
        Produto* buscarProduto(const std::string& nome);

        // Busca por id, retorna "nullptr" caso não encontre o produto
        Produto* buscarProdutoPorId(int id);

        // Retorna a lista completa para serialização
        const std::vector<Produto>& getProdutos() const;

        // Retorna nomes dos produtos com quantidade <= ESTOQUE_MINIMO
        std::vector<std::string> verificarItensBaixos() const;

        // Serializa todo o estoque como array JSON
        std::string toJson() const;

};