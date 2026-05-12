#pragma once
#include <string>
#include <vector>
#include <memory>
#include "Produto.h"

// Classe responsável por gerenciar o estoque de produtos
class Estoque{
    private:
        // unique_ptr permite guardar Bebida e Comida polimorficamente
        std::vector<std::unique_ptr<Produto>> m_produtos;
    
        // Estoque mínimo para considerar baixo
        static constexpr int ESTOQUE_MINIMO = 5;

    public:
        // Adiciona produto; lança se já existir produto com mesmo id
        void adicionarProduto(std::unique_ptr<Produto> p);

        // Lista todos os produtos
        void listarProdutos() const;

        // Busca por nome, retorna "nullptr" caso não encontre o produto
        Produto* buscarProduto(const std::string& nome);

        // Busca por id, retorna "nullptr" caso não encontre o produto
        Produto* buscarProdutoPorId(int id);

        // Retorna a lista completa
        const std::vector<std::unique_ptr<Produto>>& getProdutos() const;

        // Retorna nomes dos produtos com quantidade <= ESTOQUE_MINIMO
        std::vector<std::string> verificarItensBaixos() const;

        // Serializa todo o estoque como array JSON
        std::string toJson() const;

};