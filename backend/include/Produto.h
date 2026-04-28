#pragma once
#include <string>

class Produto{
    private:
        // Atributos
        int id;
        std::string nome_produto;
        double preco;
        std::string categoria;
        int quantidade_em_estoque;

    public:
        // Construtor 
        Produto(int id = 0, 
            const std::string& nome = "", 
            double p = 0.0, 
            const std::string& ctg = "",
            int qtd = 0);

        // Métodos
        // Getters
        int getId() const;
        const std::string& getNome() const;
        double getPreco() const;
        const std::string& getCategoria() const;
        int getQuantidadeEmEstoque() const;
        
        // Setters
        void setPreco(double novoPreco);
        void setQuantidadeEmEstoque(int novaQtd);

        // Serialização
        std::string toJson() const; // para a API REST
        std::string toString() const; // para debug

};