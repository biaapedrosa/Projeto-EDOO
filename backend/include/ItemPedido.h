#pragma once
#include "Produto.h"
#include <string>
 
class ItemPedido {
private:
    Produto        m_produto;    // associação — não possui o Produto
    int            m_quantidade;
    double         m_subtotal;   // calculado no construtor
    
public:
    ItemPedido(const Produto& produto, int quantidade);
 
    const Produto& getProduto() const;    
    int            getQuantidade() const; 
    double         getSubtotal() const;   
 
    // recalcula o subtotal (se o preço do produto mudar)
    void recalcularSubtotal();
 
    // serializa o item como JSON para enviar para o frontend
    std::string toJson() const;
};
 