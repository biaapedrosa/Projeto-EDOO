#ifndef PRODUTO_H
#define PRODUTO_H

#include <string>
using namespace std;

class Produto{
    private:
        // Atributos
        int id;
        string nome_produto;
        double preco;
        string categoria;

    public:
        // Construtor 
        Produto(int id = 0, string nome = "", double p = 0.0, string c = "");

        // Métodos
        string getNome() const;
        double getPreco() const;
        string getCategoria() const;
        void setPreco(double novoPreco);
        string toString() const;

};

#endif