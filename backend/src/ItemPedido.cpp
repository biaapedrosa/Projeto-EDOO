#include "../include/ItemPedido.h"
#include <stdexcept>
#include <sstream>

ItemPedido::ItemPedido(const Produto* produto, int quantidade)
    : m_produto(produto), m_quantidade(quantidade)
{
    if (!produto)
        throw std::invalid_argument("Produto não pode ser nulo.");
    if (quantidade <= 0)
        throw std::invalid_argument("Quantidade deve ser maior que zero.");
 
    m_subtotal = m_produto->getPreco() * m_quantidade;
}
 
void ItemPedido::recalcularSubtotal() {
    m_subtotal = m_produto->getPreco() * m_quantidade;
}
 
std::string ItemPedido::toJson() const {
    std::ostringstream oss;
    oss << "{"
        << "\"nome\":\""      << m_produto->getNome()  << "\","
        << "\"quantidade\":"  << m_quantidade          << ","
        << "\"preco_unit\":"  << m_produto->getPreco() << ","
        << "\"subtotal\":"    << m_subtotal
        << "}";
    return oss.str();
}