#include <iostream>
#include "../include/Estoque.h"

using namespace std;

// Função para adicionar um produto ao estoque
void Estoque::adicionarProduto(const Produto& p){
    produtos.push_back(p);
}

// Função para listar todos os produtos do estoque
void Estoque::listarProdutos(){
    if (produtos.size() == 0){
        cout << "Nenhum produto cadastrado." << endl;
    }
    else{
        // Percorre todos os produtos do vetor
        for (int i = 0; i < produtos.size(); i++){
        cout << produtos[i].toString() << endl;
        }
    }
}

// Função para buscar um produto pelo nome
Produto* Estoque::buscarProduto(const string& nome){
    // Percorre todos os produtos
    for (int i = 0; i < produtos.size(); i++){
        // Compara o nome informado com o nome do produto
        if (nome == produtos[i].getNome()){
            return &produtos[i];
        }
    }
    // Retorna caso não encontre o produto
    return nullptr;
}

vector<string> Estoque::verificarItensBaixos() {
    vector<string> itensBaixos;

    return itensBaixos;
}