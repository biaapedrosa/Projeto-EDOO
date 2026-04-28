#include <iostream>
#include "../include/Estoque.h"

void Estoque::adicionarProduto(const Produto& p) {
    produtos.push_back(p);
}

void Estoque::listarProdutos() {
    if (produtos.empty()) {
        std::cout << "Nenhum produto cadastrado." << std::endl;
    } else {
        for (size_t i = 0; i < produtos.size(); i++) {
            std::cout << produtos[i].toString() << std::endl;
        }
    }
}

Produto* Estoque::buscarProduto(const std::string& nome) {
    for (size_t i = 0; i < produtos.size(); i++) {
        if (nome == produtos[i].getNome()) {
            return &produtos[i];
        }
    }
    return nullptr;
}

std::vector<std::string> Estoque::verificarItensBaixos() const {
    std::vector<std::string> itensBaixos;

    return itensBaixos;
}