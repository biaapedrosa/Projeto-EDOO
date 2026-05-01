import { ShoppingCart, TrendingUp, Trophy, BarChart3 } from 'lucide-react';

export default function Relatorio() {
  return (
    <div className="py-8">
      <div className="mb-8">
        <h1 className="text-3xl font-extrabold text-slate-800 mb-2">Relatório de Vendas</h1>
        <p className="text-slate-500">Análise consolidada das vendas da barraca.</p>
      </div>

      {/* Cards de Resumo */}
      <div className="grid grid-cols-1 md:grid-cols-3 gap-6 mb-16">
        <div className="bg-white p-6 rounded-xl shadow-sm border border-slate-100">
          <div className="flex items-center gap-3 mb-4 text-slate-500">
            <div className="bg-cyan-50 p-2 rounded-lg">
              <ShoppingCart className="text-[#1281b3]" size={20} />
            </div>
            <span className="text-sm font-medium">Total de Pedidos</span>
          </div>
          <p className="text-4xl font-extrabold text-slate-800">0</p>
        </div>

        <div className="bg-white p-6 rounded-xl shadow-sm border border-slate-100">
          <div className="flex items-center gap-3 mb-4 text-slate-500">
            <div className="bg-green-50 p-2 rounded-lg">
              <TrendingUp className="text-green-600" size={20} />
            </div>
            <span className="text-sm font-medium">Receita Total</span>
          </div>
          <p className="text-4xl font-extrabold text-slate-800">R$ 0,00</p>
        </div>

        <div className="bg-white p-6 rounded-xl shadow-sm border border-slate-100">
          <div className="flex items-center gap-3 mb-4 text-slate-500">
            <div className="bg-orange-50 p-2 rounded-lg">
              <Trophy className="text-orange-500" size={20} />
            </div>
            <span className="text-sm font-medium">Mais Vendido</span>
          </div>
          <p className="text-4xl font-extrabold text-slate-800">—</p>
        </div>
      </div>

      {/* Nada registrado */}
      <div className="flex flex-col items-center justify-center py-10 text-slate-500">
        <BarChart3 size={64} className="mb-4 text-slate-300" strokeWidth={1} />
        <p>Nenhuma venda registrada ainda. Crie pedidos para ver o relatório.</p>
      </div>
    </div>
  );
}