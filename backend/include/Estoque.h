#ifndef ESTOQUE_H
#define ESTOQUE_H

#include <string>
#include <vector>
#include "Produto.h"

class Estoque {
private:
    std::vector<Produto> produtos;

public:
    void adicionarProduto(const Produto& p);
    void listarProdutos();
    Produto* buscarProduto(const std::string& nome);

    std::vector<std::string> verificarItensBaixos() const;
};

#endif