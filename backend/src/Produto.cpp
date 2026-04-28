#include "Produto.h"
#include <stdexcept>
#include <sstream>
#include <iomanip>

Produto::Produto(int id, const std::string& nome, double p,
                 const std::string& ctg, int qtd)
    : id(id), nome_produto(nome), categoria(ctg)
{
    // Validação no construtor — preço e quantidade não podem ser negativos
    if (p < 0.0)
        throw std::invalid_argument("Preço não pode ser negativo.");
    if (qtd < 0)
        throw std::invalid_argument("Quantidade não pode ser negativa.");

    preco     = p;
    quantidade_em_estoque = qtd;
}

int Produto::getId() const {return id;}
const std::string& Produto::getNome() const {return nome_produto;}
double Produto::getPreco() const {return preco;}
const std::string& Produto::getCategoria() const {return categoria;}
int Produto::getQuantidadeEmEstoque() const {return quantidade_em_estoque;}

void Produto::setPreco(double novoPreco) {
    if (novoPreco < 0.0)
        throw std::invalid_argument("Preço não pode ser negativo.");
    preco = novoPreco;
}

void Produto::setQuantidadeEmEstoque(int novaQtd) {
    if (novaQtd < 0)
        throw std::invalid_argument("Quantidade não pode ser negativa.");
    quantidade_em_estoque = novaQtd;
}

// Serializa para JSON — usado pela API REST
std::string Produto::toJson() const {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2);
    oss << "{"
        << "\"id\":" << id << ","
        << "\"nome\":\"" << nome_produto << "\","
        << "\"preco\":" << preco << ","
        << "\"categoria\":\"" << categoria << "\","
        << "\"quantidade\":" << quantidade_em_estoque
        << "}";
    return oss.str();
}

std::string Produto::toString() const {
    return "Produto[" + std::to_string(id) + "]: " + nome_produto
         + " | R$ " + std::to_string(preco)
         + " | " + categoria
         + " | Estoque: " + std::to_string(quantidade_em_estoque);
}
