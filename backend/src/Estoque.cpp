#include "../include/Estoque.h"
#include <iostream>
#include <stdexcept>
#include <sstream>

using namespace std;

void Estoque::adicionarProduto(const Produto& p){
    // Impede produtos duplicados pelo id
    for (const auto& existente : m_produtos) {
        if (existente.getId() == p.getId())
            throw invalid_argument(
                "Produto com id " + to_string(p.getId()) + " já existe no estoque.");
    }
    m_produtos.push_back(p);
}

void Estoque::listarProdutos() const{
    if (m_produtos.empty()){
        cout << "Nenhum produto cadastrado." << endl;
        return;
    }
    for (const auto& p : m_produtos){
        cout << p.toString() << endl;
    }
}

Produto* Estoque::buscarProduto(const string& nome){
    for (auto& p : m_produtos){
        // Compara o nome informado com o nome do produto
        if (nome == p.getNome()){
            return &p;
        }
    }
    return nullptr;
}

Produto* Estoque::buscarProdutoPorId(int id){
    for (auto& p : m_produtos){
        if (id == p.getId()){
            return &p;
        }
    }
    return nullptr;
}

const vector<Produto>& Estoque::getProdutos() const{
    return m_produtos;
}

vector<string> Estoque::verificarItensBaixos() const{
    vector<string> baixos;
    for (const auto& p : m_produtos) {
        if (p.getQuantidadeEmEstoque() <= ESTOQUE_MINIMO)
            baixos.push_back(p.getNome());
    }
    return baixos;
}

string Estoque::toJson() const {
    ostringstream oss;
    oss << "[";
    for (size_t i = 0; i < m_produtos.size(); ++i) {
        oss << m_produtos[i].toJson();
        if (i + 1 < m_produtos.size()) oss << ",";
    }
    oss << "]";
    return oss.str();
}