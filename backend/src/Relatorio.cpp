#include "Relatorio.h"
#include <map>
#include <algorithm>
#include <sstream>
#include <iomanip>

Relatorio::Relatorio(const std::vector<Pedido>& pedidos, const Estoque& estoque)
    : m_pedidos(pedidos), m_estoque(estoque) {}

int Relatorio::totalPedidosDia() const {
    return static_cast<int>(m_pedidos.size());
}

double Relatorio::faturamentoTotal() const {
    double total = 0.0;
    for (const auto& pedido : m_pedidos) {
        if (pedido.getStatus() == StatusPedido::ENTREGUE)
            total += pedido.getTotal();
    }
    return total;
}

std::string Relatorio::produtosMaisVendidos() const {
    if (m_pedidos.empty()) return "-";

    std::map<std::string, int> contagem;
    for (const auto& pedido : m_pedidos) {
        for (const auto& item : pedido.getItens()) {
            if (item.getProduto() != nullptr) {
                contagem[item.getProduto()->getNome()] += item.getQuantidade();
            }
        }
    }

    if (contagem.empty()) return "-";

    auto maisVendido = std::max_element(
        contagem.begin(), contagem.end(),
        [](const std::pair<std::string, int>& a,
           const std::pair<std::string, int>& b) {
            return a.second < b.second;
        }
    );

    return maisVendido->first;
}

std::vector<std::string> Relatorio::alertaEstoqueBaixo() const {
    return m_estoque.verificarItensBaixos();
}

std::string Relatorio::toJson() const {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2);

    auto alertas = alertaEstoqueBaixo();

    oss << "{"
        << "\"totalPedidos\":"  << totalPedidosDia()    << ","
        << "\"faturamento\":"   << faturamentoTotal()   << ","
        << "\"maisVendido\":\"" << produtosMaisVendidos() << "\","
        << "\"alertas\":[";

    for (size_t i = 0; i < alertas.size(); ++i) {
        oss << "\"" << alertas[i] << "\"";
        if (i + 1 < alertas.size()) oss << ",";
    }

    oss << "]}";
    return oss.str();
}