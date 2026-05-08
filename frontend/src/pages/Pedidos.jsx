import { ShoppingCart, Plus } from 'lucide-react';

export default function Pedidos() {
  return (
    <div className="py-8">
      <div className="mb-8">
        <h1 className="text-3xl font-extrabold text-slate-800 mb-2">Pedidos</h1>
        <p className="text-slate-500">Crie pedidos e acompanhe o histórico.</p>
      </div>

      <div className="max-w-xl">
        <div className="bg-white p-6 rounded-xl shadow-sm border border-slate-100">
          <h2 className="text-lg font-bold text-slate-800 flex items-center gap-2 mb-6">
            <ShoppingCart className="text-[#1281b3]" size={20} /> Novo Pedido
          </h2>
          
          <form className="space-y-4">
            <div>
              <label className="block text-sm font-medium text-slate-700 mb-2">Cliente</label>
              <input 
                type="text" 
                placeholder="Nome do cliente (opcional)" 
                className="w-full p-3 bg-slate-50 border border-slate-200 rounded-lg focus:outline-none focus:ring-2 focus:ring-[#1281b3]"
              />
            </div>

            <button 
              type="button" 
              className="flex items-center gap-2 text-slate-600 bg-slate-50 border border-slate-200 hover:bg-slate-100 font-medium px-4 py-2 rounded-lg transition-colors"
            >
              <Plus size={18} /> Adicionar Item
            </button>

          </form>
        </div>
      </div>
    </div>
  );
}