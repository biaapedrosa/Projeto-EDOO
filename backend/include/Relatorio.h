#ifndef RELATORIO_H
#define RELATORIO_H

#pragma once
#include <vector>
#include <string>
#include "Pedido.h"
#include "Estoque.h"

class Relatorio {
private:
    // Referências constantes aos dados de origem
    const std::vector<Pedido>& m_pedidos;
    const Estoque& m_estoque;

public:
    // Construtor
    Relatorio(const std::vector<Pedido>& pedidos, const Estoque& estoque);

    // Métodos de análise
    std::string produtosMaisVendidos() const;
    int totalPedidosDia() const;
    double faturamentoTotal() const;

    // Métodos de monitoramento e exportação
    std::vector<std::string> alertaEstoqueBaixo() const;
    std::string toJson() const;
};

#endif