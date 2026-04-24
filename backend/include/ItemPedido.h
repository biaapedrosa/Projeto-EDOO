#pragma once
#include "Produto.h"
#include <string>
 
class ItemPedido {
private:
    const Produto* m_produto;    // associação — não possui o Produto
    int           m_quantidade;
    double         m_subtotal;   // calculado no construtor
 
public:
    ItemPedido(const Produto* produto, int quantidade);
 
    const Produto* getProduto()    const { return m_produto; }
    int            getQuantidade() const { return m_quantidade; }
    double         getSubtotal()   const { return m_subtotal; }
 
    // recalcula o subtotal (se o preço do produto mudar)
    void recalcularSubtotal();
 
    // serializa o item como JSON para enviar para o frontend
    std::string toJson() const;
};
 