#include "../include/ItemPedido.h"
#include <stdexcept>
#include <sstream>
#include <iomanip>

// construtor a partir de um objeto Produto
ItemPedido::ItemPedido(const Produto& produto, int quantidade)
    : m_produtoId(produto.getId()), 
      m_nomeProduto(produto.getNome()), 
      m_preco(produto.getPreco()), 
      m_quantidade(quantidade) 
{
    if (quantidade <= 0)
        throw std::invalid_argument("Quantidade deve ser maior que zero.");
    
    recalcularSubtotal();
}

// construtor para dados vindos do banco
ItemPedido::ItemPedido(int produtoId, const std::string& nome, double precoUnit, int quantidade)
    : m_produtoId(produtoId), m_nomeProduto(nome), m_preco(precoUnit), m_quantidade(quantidade)
{
    if (quantidade <= 0)
        throw std::invalid_argument("Quantidade deve ser maior que zero.");
        
    recalcularSubtotal();
}

void ItemPedido::recalcularSubtotal() {
    m_subtotal = m_preco * m_quantidade;
}

//getters
int ItemPedido::getProdutoId() const { return m_produtoId; }
int ItemPedido::getQuantidade() const { return m_quantidade; }
const std::string& ItemPedido::getNomeProduto() const { return m_nomeProduto; }
double ItemPedido::getPreco() const { return m_preco; }
double ItemPedido::getSubtotal() const { return m_subtotal; }

std::string ItemPedido::toJson() const {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2);
    oss << "{"
        << "\"produto_id\":"   << m_produtoId    << ","
        << "\"nome\":\""       << m_nomeProduto  << "\","
        << "\"quantidade\":"   << m_quantidade   << ","
        << "\"preco_unit\":"   << m_preco        << ","
        << "\"subtotal\":"     << m_subtotal
        << "}";
    return oss.str();
}