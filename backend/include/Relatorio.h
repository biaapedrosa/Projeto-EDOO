#ifndef RELATORIO_H
#define RELATORIO_H

#pragma once
#include <vector>
#include <string>
#include "Pedido.h"
#include "Estoque.h"

class Relatorio {
private:
    const std::vector<Pedido>& m_pedidos;
    const Estoque& m_estoque;

public:
    Relatorio(const std::vector<Pedido>& pedidos, const Estoque& estoque);

    std::string produtosMaisVendidos() const;
    int totalPedidosDia() const;
    double faturamentoTotal() const;
    std::vector<std::string> alertaEstoqueBaixo() const;
    std::string toJson() const;
};

#endif