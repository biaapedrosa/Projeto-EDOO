import { Umbrella, Package, ShoppingCart, Archive, BarChart2 } from 'lucide-react';
import { Link } from 'react-router-dom';

export default function Inicio() {
  const features = [
    { title: 'Cadastro de Barracas', desc: 'Registre sua barraca e localização na orla', icon: <Umbrella className="text-[#1281b3]" size={24} /> },
    { title: 'Gestão de Produtos', desc: 'Cadastre e gerencie seus produtos e preços', icon: <Package className="text-[#1281b3]" size={24} /> },
    { title: 'Sistema de Pedidos', desc: 'Crie pedidos e atualize o estoque automaticamente', icon: <ShoppingCart className="text-[#1281b3]" size={24} /> },
    { title: 'Controle de Estoque', desc: 'Visualize e reponha o estoque em tempo real', icon: <Archive className="text-[#1281b3]" size={24} /> },
    { title: 'Relatório de Vendas', desc: 'Analise suas vendas e identifique tendências', icon: <BarChart2 className="text-[#1281b3]" size={24} /> },
  ];

  return (
    <div className="space-y-10 animate-in fade-in duration-500">
      {/* Banner */}
      <div className="bg-[#1281b3] rounded-2xl p-8 md:p-12 text-white shadow-lg relative overflow-hidden">
        <div className="absolute inset-0 opacity-10 bg-[url('data:image/svg+xml;base64,PHN2ZyB3aWR0aD0iMjAiIGhlaWdodD0iMjAiIHhtbG5zPSJodHRwOi8vd3d3LnczLm9yZy8yMDAwL3N2ZyI+PHBhdGggZD0iTTEwIDB2MjBtLTEwLTEwaDIwIiBzdHJva2U9IiNmZmYiIHN0cm9rZS13aWR0aD0iMSIgZmlsbD0ibm9uZSIvPjwvc3ZnPg==')]"></div>
        
        <div className="relative z-10 max-w-2xl">
          <h1 className="text-3xl md:text-5xl font-extrabold tracking-tight mb-4 leading-tight">
            Gestão inteligente para sua barraca de praia ⛱️
          </h1>
          <p className="text-blue-100 text-lg mb-8 max-w-xl">
            Controle produtos, pedidos, estoque e vendas da sua barraca na orla de Recife de forma simples e organizada.
          </p>
          <Link to="/barracas" className="inline-block bg-[#f49342] hover:bg-[#e07d2b] text-white font-medium px-6 py-3 rounded-md transition-colors">
            Começar agora →
          </Link>
        </div>
      </div>

      {/* Funcionalidades */}
      <div>
        <h2 className="text-2xl font-bold text-slate-800 mb-6">Funcionalidades</h2>
        <div className="grid grid-cols-1 md:grid-cols-2 lg:grid-cols-3 gap-6">
          {features.map((item, index) => (
            <div key={index} className="bg-white p-6 rounded-xl border border-slate-100 shadow-sm hover:shadow-md transition-shadow">
              <div className="bg-blue-50 w-12 h-12 rounded-lg flex items-center justify-center mb-4">
                {item.icon}
              </div>
              <h3 className="text-lg font-bold text-slate-800 mb-2">{item.title}</h3>
              <p className="text-slate-500 text-sm">{item.desc}</p>
            </div>
          ))}
        </div>
      </div>
    </div>
  );
}