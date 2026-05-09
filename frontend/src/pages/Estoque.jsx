import { Archive } from 'lucide-react';
import { useState, useEffect } from 'react';
import { produtoService, estoqueService } from '../services/api';

export default function Estoque() {
  const [produtos, setProdutos] = useState([]);

  useEffect(() => { carregarEstoque(); }, []);

  const carregarEstoque = async () => {
    const res = await produtoService.getProdutos();
    setProdutos(res.data);
  };

  const atualizar = async (id, novaQtd) => {
    await estoqueService.atualizarQuantidade(id, novaQtd);
    carregarEstoque();
  };

  return (
    <div className="py-8">
      <div className="mb-8">
        <h1 className="text-3xl font-extrabold text-slate-800 mb-2">Controle de Estoque</h1>
        <p className="text-slate-500">Visualize e reponha o estoque dos produtos.</p>
      </div>

      {produtos.length === 0 ? (
        <div className="flex flex-col items-center justify-center py-32 text-slate-500">
          <Archive size={64} className="mb-4 text-slate-300" strokeWidth={1} />
          <p>Nenhum produto cadastrado. Cadastre produtos primeiro.</p>
        </div>
      ) : (
        <div className="grid grid-cols-1 md:grid-cols-2 gap-4">
          {produtos.map(p => (
            <div key={p.id} className="bg-white p-6 rounded-xl border border-slate-100 shadow-sm flex justify-between items-center">
              <span className="font-bold text-slate-800">{p.nome} (Qtd: {p.quantidade})</span>
              <div className="flex gap-2">
                <button onClick={() => atualizar(p.id, p.quantidade - 1)} className="px-3 py-1 bg-slate-100 rounded">-</button>
                <button onClick={() => atualizar(p.id, p.quantidade + 1)} className="px-3 py-1 bg-[#1281b3] text-white rounded">+</button>
              </div>
            </div>
          ))}
        </div>
      )}
    </div>
  );
}