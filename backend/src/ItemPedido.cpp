#include "../include/ItemPedido.h"
#include <stdexcept>
#include <sstream>

ItemPedido::ItemPedido(const Produto& produto, int quantidade)
    : m_produto(produto), m_quantidade(quantidade)
{
    if (quantidade <= 0)
        throw std::invalid_argument("Quantidade deve ser maior que zero.");
 
    m_subtotal = m_produto.getPreco() * m_quantidade;
}

void ItemPedido::recalcularSubtotal() {
    m_subtotal = m_produto.getPreco() * m_quantidade;
}

const Produto& ItemPedido::getProduto()    const { return m_produto; }
int            ItemPedido::getQuantidade() const { return m_quantidade; }
double         ItemPedido::getSubtotal()   const { return m_subtotal; }
 
std::string ItemPedido::toJson() const {
    std::ostringstream oss;
    oss << "{"
        << "\"nome\":\""      << m_produto.getNome()  << "\","
        << "\"quantidade\":"  << m_quantidade          << ","
        << "\"preco_unit\":"  << m_produto.getPreco() << ","
        << "\"subtotal\":"    << m_subtotal
        << "}";
    return oss.str();
}