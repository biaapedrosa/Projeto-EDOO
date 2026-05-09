// CORREÇÃO: adicionar Trash2 ao import (BUG 4)
import { Plus, PackageX, Trash2 } from 'lucide-react';
import { useState, useEffect } from 'react';
import { produtoService } from '../services/api';

export default function Produtos() {
  const [produtos, setProdutos] = useState([]);
  const [nome, setNome] = useState('');
  const [preco, setPreco] = useState('');
  const [quantidade, setQuantidade] = useState('');

  useEffect(() => { carregarProdutos(); }, []);

  const carregarProdutos = async () => {
    const res = await produtoService.getProdutos();
    setProdutos(res.data);
  };

  const handleCadastrar = async () => {
    await produtoService.saveProduto({ 
      nome, 
      preco: parseFloat(preco), 
      quantidade: parseInt(quantidade),
      tipo: "Comida",
      categoria: "Geral",
      vegana: false,   // campos exigidos pelo construtor comida no backend
      porcao: "Porção" 
    });
    setNome(''); setPreco(''); setQuantidade('');
    carregarProdutos();
  };

  return (
    <div className="py-8">
      <div className="mb-8">
        <h1 className="text-3xl font-extrabold text-slate-800 mb-2">Produtos</h1>
        <p className="text-slate-500">Cadastre e visualize os produtos da barraca.</p>
      </div>

      <div className="grid grid-cols-1 lg:grid-cols-3 gap-8">
        <div className="lg:col-span-1">
          <div className="bg-white p-6 rounded-xl shadow-sm border border-slate-100">
            <h2 className="text-lg font-bold text-slate-800 flex items-center gap-2 mb-6">
              <Plus className="text-[#1281b3]" size={20} /> Novo Produto
            </h2>
            
            
            <form className="space-y-4" onSubmit={(e) => { e.preventDefault(); handleCadastrar(); }}>
              <div>
                <label className="block text-sm font-medium text-slate-700 mb-2">Nome</label>
                <input 
                  type="text" value={nome} onChange={e => setNome(e.target.value)} 
                  placeholder="Ex: Água de Coco" 
                  className="w-full p-3 bg-slate-50 border border-slate-200 rounded-lg" />
              </div>
              
              <div className="grid grid-cols-2 gap-4">
                <div>
                  <label className="block text-sm font-medium text-slate-700 mb-2">Preço (R$)</label>
                  <input type="number" value={preco} onChange={e => setPreco(e.target.value)} 
                    placeholder="8.00" step="0.01" 
                    className="w-full p-3 bg-slate-50 border border-slate-200 rounded-lg" />
                </div>
                <div>
                  <label className="block text-sm font-medium text-slate-700 mb-2">Estoque</label>
                  <input type="number" value={quantidade} onChange={e => setQuantidade(e.target.value)} 
                    placeholder="50" 
                    className="w-full p-3 bg-slate-50 border border-slate-200 rounded-lg" />
                </div>
              </div>

              <button 
                type="submit" 
                className="w-full bg-[#1281b3] hover:bg-[#0e668f] text-white font-medium py-3 rounded-lg transition-colors mt-2"
              >
                Cadastrar
              </button>
            </form>
          </div>
        </div>

        <div className="lg:col-span-2">
          {produtos.length === 0 ? (
            <div className="flex flex-col items-center justify-center py-20 text-slate-400">
              <PackageX size={64} className="mb-4 text-slate-300" strokeWidth={1} />
              <p>Nenhum produto cadastrado ainda.</p>
            </div>
          ) : (
            <div className="grid gap-4">
              {produtos.map(p => (
                <div key={p.id} className="bg-white p-4 rounded-lg border border-slate-100 flex justify-between items-center">
                  <span className="font-medium text-slate-700">{p.nome} - R$ {p.preco}</span>
                  <button 
                    onClick={async () => { await produtoService.deleteProduto(p.id); carregarProdutos(); }} 
                    className="text-red-400 hover:text-red-600">
                    <Trash2 size={18}/>
                  </button>
                </div>
              ))}
            </div>
          )}
        </div>
      </div>
    </div>
  );
}