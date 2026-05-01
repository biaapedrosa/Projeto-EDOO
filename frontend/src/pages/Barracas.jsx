export default function Barracas() {
  return (
    <div className="max-w-2xl mx-auto py-8">
      <div className="mb-8">
        <h1 className="text-3xl font-extrabold text-slate-800 mb-2">Cadastro de Barraca</h1>
        <p className="text-slate-500">Registre ou atualize sua barraca na orla de Recife.</p>
      </div>

      <div className="bg-white p-6 md:p-8 rounded-xl shadow-sm border border-slate-100">
        <form className="space-y-6">
          <div>
            <label className="block text-sm font-medium text-slate-700 mb-2">Nome da Barraca</label>
            <input 
              type="text" 
              placeholder="Ex: Barraca do Sol" 
              className="w-full p-3 bg-slate-50 border border-slate-200 rounded-lg focus:outline-none focus:ring-2 focus:ring-[#1281b3] focus:border-transparent transition-all"
            />
          </div>
          
          <div>
            <label className="block text-sm font-medium text-slate-700 mb-2">Localização</label>
            <input 
              type="text" 
              placeholder="Ex: Praia de Boa Viagem" 
              className="w-full p-3 bg-slate-50 border border-slate-200 rounded-lg focus:outline-none focus:ring-2 focus:ring-[#1281b3] focus:border-transparent transition-all"
            />
          </div>

          <button 
            type="button" 
            className="w-full bg-[#1281b3] hover:bg-[#0e668f] text-white font-medium py-3 rounded-lg transition-colors mt-4"
          >
            Cadastrar Barraca
          </button>
        </form>
      </div>
    </div>
  );
}