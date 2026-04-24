#ifndef RELATORIO_H
#define RELATORIO_H

#include <vector>
#include <string>
#include "Pedido.h"
#include "Estoque.h"

using namespace std;

class Relatorio {
    private:
        vector<Pedido>& pedidos;
        Estoque& estoque;

    public:
        Relatorio(vector<Pedido>& p, Estoque& e) : pedidos(p), estoque(e) {}

    string produtosMaisVendidos();
    int totalPedidosDia();
    double faturamentoTotal();
    vector<string> alertaEstoqueBaixo();
}; 

#endif