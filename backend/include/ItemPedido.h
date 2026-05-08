#pragma once
#include "Produto.h"
#include <string>
 
class ItemPedido {
private:
    std::string    m_nomeProduto;
    double         m_preco;
    int            m_produtoId;      
    int            m_quantidade;
    double         m_subtotal;   // calculado no construtor
    
public:
    // Construtor principal
    ItemPedido(const Produto& produto, int quantidade);

    //construtor para a construção do banco de dados
    ItemPedido(int produtoId, const std::string& nome, double precoUnit, int quantidade);
 
    //getters
    int            getProdutoId()  const;
    const std::string& getNomeProduto() const;
    double         getPreco()  const;
    int            getQuantidade() const;
    double         getSubtotal()   const;
 
 
    // recalcula o subtotal (se o preço do produto mudar)
    void recalcularSubtotal();
 
    // serializa o item como JSON para enviar para o frontend
    std::string toJson() const;
};
 