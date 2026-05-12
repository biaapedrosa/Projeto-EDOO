#include "../include/Produto.h"
#include <stdexcept>
#include <sstream>
#include <iomanip>

// Produto (classe base)
Produto::Produto(int id, const std::string& nome, double p, const std::string& ctg, int qtd)
    : id(id), nome_produto(nome), preco(p), categoria(ctg), quantidade_em_estoque(qtd)
{
    // Validação no construtor — preço e quantidade não podem ser negativos
    if (p < 0.0)
        throw std::invalid_argument("Preço não pode ser negativo.");
    if (qtd < 0)
        throw std::invalid_argument("Quantidade não pode ser negativa.");

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

std::string Produto::getTipo() const {
    return "Produto";
}

// Serializa para JSON — usado pela API REST
std::string Produto::toJson() const {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2);
    oss << "{"
        << "\"id\":" << id << ","
        << "\"tipo\":\"" << getTipo() << "\","
        << "\"nome\":\"" << nome_produto << "\","
        << "\"preco\":" << preco << ","
        << "\"categoria\":\"" << categoria << "\","
        << "\"quantidade\":" << quantidade_em_estoque
        << "}";
    return oss.str();
}

std::string Produto::toString() const {
    return getTipo() + "[" + std::to_string(id) + "]: " + nome_produto
         + " | R$ " + std::to_string(preco)
         + " | " + categoria
         + " | Estoque: " + std::to_string(quantidade_em_estoque);
}



// Bebida (subclasse)
Bebida::Bebida(int id, const std::string& nome, double preco,
               const std::string& categoria, int qtd,
               int volume_ml, const std::string& tipo_bebida, bool alcoolica)
    : Produto(id, nome, preco, categoria, qtd),
      volume_ml(volume_ml), tipo_bebida(tipo_bebida), alcoolica(alcoolica)
{
    if (volume_ml <= 0)
        throw std::invalid_argument("Volume deve ser positivo.");
}

int Bebida::getVolume() const {return volume_ml;}
const std::string& Bebida::getTipoBebida() const {return tipo_bebida;}
bool Bebida::isAlcoolica() const {return alcoolica;}

std::string Bebida::getTipo() const {
    return alcoolica ? "Bebida Alcoolica" : "Bebida Nao Alcoolica";
}

std::string Bebida::toJson() const {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2);
    oss << "{"
        << "\"id\":" << id << ","
        << "\"tipo\":\"" << getTipo() << "\","
        << "\"nome\":\"" << nome_produto << "\","
        << "\"preco\":" << preco << ","
        << "\"categoria\":\"" << categoria << "\","
        << "\"quantidade\":" << quantidade_em_estoque << ","
        << "\"tipo_bebida\":\"" << tipo_bebida << "\","
        << "\"volume_ml\":" << volume_ml << ","
        << "\"alcoolica\":" << (alcoolica ? "true" : "false")
        << "}";
    return oss.str();
}

std::string Bebida::toString() const {
    return getTipo() + "[" + std::to_string(id) + "]: " + nome_produto
         + " | R$ " + std::to_string(preco)
         + " | " + categoria
         + " | Estoque: "+ std::to_string(quantidade_em_estoque)
         + " | Tipo: " + tipo_bebida
         + " | " + std::to_string(volume_ml) + "ml"
         + " | " + (alcoolica ? "Alcoolica" : "Nao alcoolica");
}



// Comida (subclasse)
Comida::Comida(int id, const std::string& nome, double preco,
               const std::string& categoria, int qtd,
               bool vegana, const std::string& porcao)
    : Produto(id, nome, preco, categoria, qtd),
      vegana(vegana),
      porcao(porcao)
{}

bool Comida::isVegana() const {return vegana;}
const std::string& Comida::getPorcao() const {return porcao;}

std::string Comida::getTipo() const {
    return "Comida";
}

std::string Comida::toJson() const {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2);
    oss << "{"
        << "\"id\":" << id << ","
        << "\"tipo\":\"" << getTipo()  << "\","
        << "\"nome\":\"" << nome_produto << "\","
        << "\"preco\":" << preco << ","
        << "\"categoria\":\"" << categoria << "\","
        << "\"quantidade\":" << quantidade_em_estoque << ","
        << "\"vegana\":" << (vegana ? "true" : "false") << ","
        << "\"porcao\":\"" << porcao << "\""
        << "}";
    return oss.str();
}

std::string Comida::toString() const {
    return getTipo() + "[" + std::to_string(id) + "]: " + nome_produto
         + " | R$ " + std::to_string(preco)
         + " | " + categoria
         + " | Estoque: "+ std::to_string(quantidade_em_estoque)
         + " | Porcao: " + porcao
         + " | " + (vegana ? "Vegana" : "Nao vegana");
}