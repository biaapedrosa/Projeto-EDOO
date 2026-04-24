#pragma once
#include "ItemPedido.h"   
#include "Produto.h"
#include <vector>
#include <string>
 
// evita colisões de nomes.
enum class StatusPedido {
    ABERTO,
    EM_PREPARO,
    ENTREGUE,
    CANCELADO
};

std::string statusParaString(StatusPedido s);

class Pedido {
private:
    int                      m_id;
    int                      m_numeroMesa;
    std::vector<ItemPedido>  m_itens;
    StatusPedido             m_status;
    double                   m_total;
    std::string              m_dataHora;
    
 
    void recalcularTotal();   // metodo privado auxiliar
 
public:
    Pedido(int id, const int numeroMesa);
 
    // adiciona um item e lança se quantidade <= 0
    void adicionarItem(const Produto* produto, int quantidade);
 
    // remove um item pelo índice
    bool removerItem(int index);
 
    // avança o status para o próximo estado
    void avancarStatus();
 
    // cancela independentemente do estado atual
    void cancelar();
 
    //getters 
    StatusPedido                   getStatus()      const { return m_status; }
    double                         getTotal()       const { return m_total; }
    int                            getId()          const { return m_id; }
    int                            getNumeroMesa() const { return m_numeroMesa; }
    const std::string&             getDataHora()    const { return m_dataHora; }
    const std::vector<ItemPedido>& getItens()       const { return m_itens; }
 
    double getValorTotal() const;
    std::vector<Produto> getProdutos() const;

    std::string toJson() const;
};