import { Archive } from 'lucide-react';

export default function Estoque() {
  return (
    <div className="py-8">
      <div className="mb-8">
        <h1 className="text-3xl font-extrabold text-slate-800 mb-2">Controle de Estoque</h1>
        <p className="text-slate-500">Visualize e reponha o estoque dos produtos.</p>
      </div>

      <div className="flex flex-col items-center justify-center py-32 text-slate-500">
        <Archive size={64} className="mb-4 text-slate-300" strokeWidth={1} />
        <p>Nenhum produto cadastrado. Cadastre produtos primeiro.</p>
      </div>
    </div>
  );
}