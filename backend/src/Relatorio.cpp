#include "Relatorio.h"
#include <map>
#include <algorithm>
#include <sstream>
#include <iomanip>

// Construtor: Inicializa as referências de pedidos e estoque
Relatorio::Relatorio(const std::vector<Pedido>& pedidos, const Estoque& estoque)
    : m_pedidos(pedidos), m_estoque(estoque) {}

// Retorna a quantidade total de pedidos na lista
int Relatorio::totalPedidosDia() const {
    return static_cast<int>(m_pedidos.size());
}

// Calcula o faturamento apenas de pedidos com status "ENTREGUE"
double Relatorio::faturamentoTotal() const {
    double total = 0.0;
    for (const auto& pedido : m_pedidos) {
        if (pedido.getStatus() == StatusPedido::ENTREGUE)
            total += pedido.getTotal();
    }
    return total;
}

// Identifica o produto com maior volume de vendas
std::string Relatorio::produtosMaisVendidos() const {
    if (m_pedidos.empty()) return "-";

    // Mapeia o nome do produto para a quantidade total vendida
    std::map<std::string, int> contagem;
    for (const auto& pedido : m_pedidos) {
        for (const auto& item : pedido.getItens()) {
            contagem[item.getNomeProduto()] += item.getQuantidade();
        }
    }

    if (contagem.empty()) return "-";

    // Busca o elemento com o maior valor (quantidade) no mapa
    auto maisVendido = std::max_element(
        contagem.begin(), contagem.end(),
        [](const std::pair<std::string, int>& a,
           const std::pair<std::string, int>& b) {
            return a.second < b.second;
        }
    );

    return maisVendido->first;
}

// Consulta o objeto estoque para listar produtos abaixo do limite
std::vector<std::string> Relatorio::alertaEstoqueBaixo() const {
    return m_estoque.verificarItensBaixos();
}

// Gera uma string formatada em JSON com os dados consolidados
std::string Relatorio::toJson() const {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2); 

    auto alertas = alertaEstoqueBaixo();

    oss << "{"
        << "\"totalPedidos\":"  << totalPedidosDia()    << ","
        << "\"faturamento\":"   << faturamentoTotal()   << ","
        << "\"maisVendido\":\"" << produtosMaisVendidos() << "\","
        << "\"alertas\":[";

    // Montagem da lista de alertas no JSON
    for (size_t i = 0; i < alertas.size(); ++i) {
        oss << "\"" << alertas[i] << "\"";
        if (i + 1 < alertas.size()) oss << ",";
    }

    oss << "]}";
    return oss.str();
}