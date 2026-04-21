#include "Produto.h"
#include <string>

using namespace std;

Produto::Produto(int id, string nome, double p, string c){
    this->id = id; // this->id atributo da classe
    
    nome_produto = nome;

    if (p < 0)
        preco = 0.0;
    else
        preco = p;
    
    categoria = c;

}

// Função para colocar nome do produto
string Produto::getNome() const{
    return nome_produto;
}

// Função para colocar preço do produto
double Produto::getPreco() const{
    return preco;
}

// Função para colocar categoria do produto
string Produto::getCategoria() const{
    return categoria;
}

// Função para validar e atualizar o preço
void Produto::setPreco(double novoPreco){
    if (novoPreco >= 0){
        preco = novoPreco;
    }
}

// Função para retornar uma descrição formatada
string Produto::toString() const{
    return "Produto: " + nome_produto +
            " | Preco: " + to_string(preco) + // "Preço na forma de string"
            " | Categoria: " +categoria;
}


