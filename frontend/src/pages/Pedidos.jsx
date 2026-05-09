import React, { useState, useEffect } from 'react';
import { ShoppingCart, Plus, Send, Trash2 } from 'lucide-react';
import { pedidoService, produtoService } from '../services/api';

export default function Pedidos() {
  const [mesa, setMesa] = useState("");
  const [produtosDisponiveis, setProdutosDisponiveis] = useState([]);
  const [carrinho, setCarrinho] = useState([]);
  const [loading, setLoading] = useState(false);

  // Carrega produtos para poder selecionar no pedido
  useEffect(() => {
    produtoService.getProdutos()
      .then(res => setProdutosDisponiveis(res.data))
      .catch(err => console.error("Erro ao carregar produtos", err));
  }, []);

  const adicionarAoCarrinho = (produtoId) => {
    const produto = produtosDisponiveis.find(p => p.id === parseInt(produtoId));
    if (!produto) return;

    setCarrinho([...carrinho, { 
      produtoId: produto.id, 
      nome: produto.nome, 
      quantidade: 1, 
      preco: produto.preco 
    }]);
  };

  const removerDoCarrinho = (index) => {
    setCarrinho(carrinho.filter((_, i) => i !== index));
  };

  const finalizarPedido = async (e) => {
    e.preventDefault();
    if (!mesa || carrinho.length === 0) return alert("Preencha a mesa e adicione itens!");

    setLoading(true);
    try {
      const dadosPedido = {
        numeroMesa: parseInt(mesa),
        itens: carrinho.map(item => ({
          produtoId: item.produtoId,
          quantidade: item.quantidade
        }))
      };

      await pedidoService.criarPedido(dadosPedido);
      alert("Pedido realizado com sucesso!");
      setCarrinho([]);
      setMesa("");
    } catch (error) {
      alert("Erro ao salvar pedido. Verifique a conexão.");
    } finally {
      setLoading(false);
    }
  };

  return (
    <div className="py-8 max-w-4xl mx-auto">
      <div className="mb-8">
        <h1 className="text-3xl font-extrabold text-slate-800 mb-2">Novo Pedido</h1>
        <p className="text-slate-500">Selecione os itens e informe a mesa.</p>
      </div>

      <div className="grid grid-cols-1 md:grid-cols-2 gap-8">
        {/* Formulário de Entrada */}
        <div className="bg-white p-6 rounded-xl shadow-sm border border-slate-100">
          <form onSubmit={finalizarPedido} className="space-y-6">
            <div>
              <label className="block text-sm font-medium text-slate-700 mb-2">Número da Mesa</label>
              <input 
                type="number" 
                required
                value={mesa}
                onChange={(e) => setMesa(e.target.value)}
                className="w-full p-3 bg-slate-50 border border-slate-200 rounded-lg focus:ring-2 focus:ring-[#1281b3] outline-none"
                placeholder="Ex: 12"
              />
            </div>

            <div>
              <label className="block text-sm font-medium text-slate-700 mb-2">Adicionar Produto</label>
              <select 
                onChange={(e) => adicionarAoCarrinho(e.target.value)}
                className="w-full p-3 bg-slate-50 border border-slate-200 rounded-lg outline-none"
                defaultValue=""
              >
                <option value="" disabled>Selecione um produto...</option>
                {produtosDisponiveis.map(p => (
                  <option key={p.id} value={p.id}>{p.nome} - R$ {p.preco.toFixed(2)}</option>
                ))}
              </select>
            </div>

            <button 
              type="submit"
              disabled={loading}
              className="w-full flex items-center justify-center gap-2 bg-[#1281b3] text-white py-3 rounded-lg font-bold hover:bg-[#0e668e] transition-colors disabled:bg-slate-300"
            >
              {loading ? "Enviando..." : <><Send size={18} /> Finalizar Pedido</>}
            </button>
          </form>
        </div>

        {/* Resumo do Pedido (Carrinho) */}
        <div className="bg-slate-50 p-6 rounded-xl border border-dashed border-slate-300">
          <h2 className="font-bold text-slate-800 mb-4 flex items-center gap-2">
            <ShoppingCart size={20} /> Itens Selecionados
          </h2>
          {carrinho.length === 0 ? (
            <p className="text-slate-400 text-sm">Nenhum item adicionado.</p>
          ) : (
            <div className="space-y-3">
              {carrinho.map((item, index) => (
                <div key={index} className="flex justify-between items-center bg-white p-3 rounded-lg shadow-sm">
                  <div>
                    <p className="font-medium text-slate-800">{item.nome}</p>
                    <p className="text-xs text-slate-500">{item.quantidade}x R$ {item.preco.toFixed(2)}</p>
                  </div>
                  <button onClick={() => removerDoCarrinho(index)} className="text-red-500 hover:bg-red-50 p-2 rounded-full">
                    <Trash2 size={16} />
                  </button>
                </div>
              ))}
              <div className="border-t pt-3 mt-4">
                <p className="text-right font-bold text-lg text-[#1281b3]">
                  Total: R$ {carrinho.reduce((acc, i) => acc + (i.preco * i.quantidade), 0).toFixed(2)}
                </p>
              </div>
            </div>
          )}
        </div>
      </div>
    </div>
  );
}