import { Plus, PackageX } from 'lucide-react';

export default function Produtos() {
  return (
    <div className="py-8">
      <div className="mb-8">
        <h1 className="text-3xl font-extrabold text-slate-800 mb-2">Produtos</h1>
        <p className="text-slate-500">Cadastre e visualize os produtos da barraca.</p>
      </div>

      <div className="grid grid-cols-1 lg:grid-cols-3 gap-8">
        {/* Formulário de Cadastro */}
        <div className="lg:col-span-1">
          <div className="bg-white p-6 rounded-xl shadow-sm border border-slate-100">
            <h2 className="text-lg font-bold text-slate-800 flex items-center gap-2 mb-6">
              <Plus className="text-[#1281b3]" size={20} /> Novo Produto
            </h2>
            
            <form className="space-y-4">
              <div>
                <label className="block text-sm font-medium text-slate-700 mb-2">Nome</label>
                <input 
                  type="text" 
                  placeholder="Ex: Água de Coco" 
                  className="w-full p-3 bg-slate-50 border border-slate-200 rounded-lg focus:outline-none focus:ring-2 focus:ring-[#1281b3]"
                />
              </div>
              
              <div className="grid grid-cols-2 gap-4">
                <div>
                  <label className="block text-sm font-medium text-slate-700 mb-2">Preço (R$)</label>
                  <input 
                    type="number" 
                    placeholder="8.00" 
                    step="0.01"
                    className="w-full p-3 bg-slate-50 border border-slate-200 rounded-lg focus:outline-none focus:ring-2 focus:ring-[#1281b3]"
                  />
                </div>
                <div>
                  <label className="block text-sm font-medium text-slate-700 mb-2">Estoque</label>
                  <input 
                    type="number" 
                    placeholder="50" 
                    className="w-full p-3 bg-slate-50 border border-slate-200 rounded-lg focus:outline-none focus:ring-2 focus:ring-[#1281b3]"
                  />
                </div>
              </div>

              <button 
                type="button" 
                className="w-full bg-[#1281b3] hover:bg-[#0e668f] text-white font-medium py-3 rounded-lg transition-colors mt-2"
              >
                Cadastrar
              </button>
            </form>
          </div>
        </div>

        {/* Lista Vazia */}
        <div className="lg:col-span-2 flex flex-col items-center justify-center py-20 text-slate-400">
          <PackageX size={64} className="mb-4 text-slate-300" strokeWidth={1} />
          <p>Nenhum produto cadastrado ainda.</p>
        </div>
      </div>
    </div>
  );
}