#ifndef ESTOQUE_H
#define ESTOQUE_H

#include <string>
#include <vector>
#include "Produto.h"

using namespace std;

// Classe responsável por gerenciar o estoque de produtos
class Estoque{
    private:
        // Vetor que armazena os produtos cadastrados
        vector<Produto> produtos;

    public:
        // Métodos
        void adicionarProduto(const Produto& p);
        void listarProdutos();
        Produto* buscarProduto(const string& nome);

        vector<string> verificarItensBaixos();

};

#endif