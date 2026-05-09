import React, { useState, useEffect } from 'react';
import { ShoppingCart, TrendingUp, Trophy, BarChart3, AlertTriangle, RefreshCcw } from 'lucide-react';
import { relatorioService } from '../services/api';

export default function Relatorio() {
  const [dados, setDados] = useState({
    totalPedidos: 0,
    faturamento: 0,
    maisVendido: "-",
    alertas: []
  });
  const [loading, setLoading] = useState(true);

  const carregarRelatorio = async () => {
    setLoading(true);
    try {
      const response = await relatorioService.getDadosGerais();
      setDados(response.data);
    } catch (error) {
      console.error("Erro ao buscar relatório:", error);
    } finally {
      setLoading(false);
    }
  };

  useEffect(() => {
    carregarRelatorio();
  }, []);

  return (
    <div className="py-8">
      <div className="flex justify-between items-start mb-8">
        <div>
          <h1 className="text-3xl font-extrabold text-slate-800 mb-2">Relatório de Vendas</h1>
          <p className="text-slate-500">Análise consolidada (apenas pedidos ENTREGUES).</p>
        </div>
        <button 
          onClick={carregarRelatorio}
          className="p-2 text-slate-400 hover:text-[#1281b3] transition-colors"
        >
          <RefreshCcw size={24} className={loading ? "animate-spin" : ""} />
        </button>
      </div>

      {/* Cards de Resumo */}
      <div className="grid grid-cols-1 md:grid-cols-3 gap-6 mb-8">
        <div className="bg-white p-6 rounded-xl shadow-sm border border-slate-100">
          <div className="flex items-center gap-3 mb-4 text-slate-500">
            <div className="bg-cyan-50 p-2 rounded-lg"><ShoppingCart className="text-[#1281b3]" size={20} /></div>
            <span className="text-sm font-medium">Total de Pedidos</span>
          </div>
          <p className="text-4xl font-extrabold text-slate-800">{dados.totalPedidos}</p>
        </div>

        <div className="bg-white p-6 rounded-xl shadow-sm border border-slate-100">
          <div className="flex items-center gap-3 mb-4 text-slate-500">
            <div className="bg-green-50 p-2 rounded-lg"><TrendingUp className="text-green-600" size={20} /></div>
            <span className="text-sm font-medium">Receita Total</span>
          </div>
          <p className="text-4xl font-extrabold text-slate-800">
            R$ {typeof dados.faturamento === 'number' ? dados.faturamento.toFixed(2) : "0.00"}
          </p>
        </div>

        <div className="bg-white p-6 rounded-xl shadow-sm border border-slate-100">
          <div className="flex items-center gap-3 mb-4 text-slate-500">
            <div className="bg-orange-50 p-2 rounded-lg"><Trophy className="text-orange-500" size={20} /></div>
            <span className="text-sm font-medium">Produto Mais Vendido</span>
          </div>
          <p className="text-2xl font-extrabold text-slate-800 truncate">{dados.maisVendido}</p>
        </div>
      </div>

      {/* Seção de Alertas de Estoque */}
      {dados.alertas && dados.alertas.length > 0 && (
        <div className="mb-8 p-5 bg-amber-50 border border-amber-200 rounded-xl">
          <h3 className="flex items-center gap-2 text-amber-800 font-bold mb-3">
            <AlertTriangle size={20} /> Itens com Estoque Crítico
          </h3>
          <div className="grid grid-cols-1 sm:grid-cols-2 gap-2">
            {dados.alertas.map((item, idx) => (
              <div key={idx} className="text-amber-700 text-sm bg-white/50 p-2 rounded border border-amber-100">
                • {item}
              </div>
            ))}
          </div>
        </div>
      )}

      {/* Estado Vazio */}
      {dados.totalPedidos === 0 && !loading && (
        <div className="flex flex-col items-center justify-center py-16 text-slate-400">
          <BarChart3 size={64} className="mb-4 opacity-20" strokeWidth={1} />
          <p>Nenhum dado disponível para o período selecionado.</p>
        </div>
      )}
    </div>
  );
}