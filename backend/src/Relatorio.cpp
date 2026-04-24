#include "Relatorio.h"
#include <map>
#include <algorithm>

int Relatorio::totalPedidosDia() {
    return static_cast<int>(pedidos.size());
}

double Relatorio::faturamentoTotal() {
    double total = 0.0;
    for (const auto& pedido : pedidos) {
        total += pedido.getValorTotal();
    }
    return total;
}

string Relatorio::produtosMaisVendidos() {
    if (pedidos.empty()) {
        return "-";
    }

    map<string, int> contagem;

    for (const auto& pedido : pedidos) {
        for (const auto& produto : pedido.getProdutos()) {
            contagem[produto.getNome()]++;
        }
    }

    if (contagem.empty()) {
        return "-";
    }

    auto maisVendido = max_element(
        contagem.begin(),
        contagem.end(),
        [](const pair<string, int>& a, const pair<string, int>& b) {
            return a.second < b.second;
        }
    );

    return maisVendido->first;
}

vector<string> Relatorio::alertaEstoqueBaixo() {
    return estoque.verificarItensBaixos();
}