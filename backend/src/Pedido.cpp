#include "../include/Pedido.h"  // já inclui ItemPedido.h 
#include <stdexcept>
#include <sstream>
#include <ctime>

// status 
std::string statusParaString(StatusPedido s) {
    switch (s) {
        case StatusPedido::ABERTO:      return "ABERTO";
        case StatusPedido::EM_PREPARO:  return "EM_PREPARO";
        case StatusPedido::ENTREGUE:    return "ENTREGUE";
        case StatusPedido::CANCELADO:   return "CANCELADO";
    }
    return "DESCONHECIDO";
}

// Gera timestamp no formato DD/MM/AAAA HH:MM
static std::string agora() {
    std::time_t t = std::time(nullptr);
    std::tm* tm_local = std::localtime(&t);
    char buf[20];
    std::strftime(buf, sizeof(buf), "%d/%m/%Y %H:%M", tm_local);
    return std::string(buf);
}

Pedido::Pedido(int m_id, const int m_numeorMesa)
    :m_id(m_id), m_status(StatusPedido::ABERTO),
      m_total(0.0), m_numeroMesa(m_numeroMesa)
{
    m_dataHora = agora();
}

void Pedido::recalcularTotal() {
    m_total = 0.0;
    for (const auto& item : m_itens)
        m_total += item.getSubtotal();
}

void Pedido::adicionarItem(const Produto* produto, int quantidade) {
    if (!produto)
        throw std::invalid_argument("Produto inválido.");
    if (quantidade <= 0)
        throw std::invalid_argument("Quantidade deve ser positiva.");
    if (m_status != StatusPedido::ABERTO)
        throw std::runtime_error("Só é possível adicionar itens a pedidos ABERTOS.");

    // Verifica se o produto já está no pedido — soma a quantidade
    for (auto& item : m_itens) {
        if (item.getProduto()->getNome() == produto->getNome()) {
            int novaQtd = item.getQuantidade() + quantidade;
            item = ItemPedido(produto, novaQtd);
            recalcularTotal();
            return;
        }
    }

    m_itens.emplace_back(produto, quantidade);
    recalcularTotal();
}

bool Pedido::removerItem(int index) {
    if (index < 0 || index >= (int)m_itens.size()) return false;
    m_itens.erase(m_itens.begin() + index);
    recalcularTotal();
    return true;
}

void Pedido::avancarStatus() {
    switch (m_status) {
        case StatusPedido::ABERTO:     m_status = StatusPedido::EM_PREPARO; break;
        case StatusPedido::EM_PREPARO: m_status = StatusPedido::ENTREGUE;   break;
        default: break; // ENTREGUE e CANCELADO não avançam
    }
}

void Pedido::cancelar() {
    if (m_status == StatusPedido::ENTREGUE)
        throw std::runtime_error("Não é possível cancelar um pedido já entregue.");
    m_status = StatusPedido::CANCELADO;
}

std::string Pedido::toJson() const {
    std::ostringstream oss;
    oss << "{"
        << "\"id\":"          << m_id                           << ","
        << "\"Mesa\":\""      << m_numeroMesa                  << "\","
        << "\"status\":\""    << statusParaString(m_status)      << "\","
        << "\"total\":"       << m_total                         << ","
        << "\"dataHora\":\""  << m_dataHora                      << "\","
        << "\"itens\":[";

    for (size_t i = 0; i < m_itens.size(); ++i) {
        oss << m_itens[i].toJson();
        if (i + 1 < m_itens.size()) oss << ",";
    }

    oss << "]}";
    return oss.str();
}