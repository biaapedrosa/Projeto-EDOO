import { Plus, PackageX, Trash2 } from 'lucide-react';
import { useState, useEffect } from 'react';
import { produtoService } from '../services/api';

export default function Produtos() {
  const [produtos, setProdutos] = useState([]);

  // Campos comuns
  const [tipo, setTipo]           = useState('Comida');
  const [nome, setNome]           = useState('');
  const [preco, setPreco]         = useState('');
  const [quantidade, setQuantidade] = useState('');
  const [categoria, setCategoria] = useState('');

  // Campos específicos de Bebida
  const [volumeMl, setVolumeMl]     = useState('');
  const [tipoBebida, setTipoBebida] = useState('');
  const [alcoolica, setAlcoolica]   = useState(false);

  // Campos específicos de Comida
  const [vegana, setVegana] = useState(false);
  const [porcao, setPorcao] = useState('');

  useEffect(() => { carregarProdutos(); }, []);

  const carregarProdutos = async () => {
    try {
      const res = await produtoService.getProdutos();
      setProdutos(res.data);
    } catch (err) {
      console.error('Erro ao carregar produtos:', err);
    }
  };

  const handleCadastrar = async () => {
    try {
      // Monta o objeto dependendo do tipo escolhido
      const base = {
        nome,
        preco:      parseFloat(preco),
        quantidade: parseInt(quantidade),
        categoria,
      };

      if (tipo === 'Bebida Alcoolica' || tipo === 'Bebida Nao Alcoolica') {
        await produtoService.saveProduto({
          ...base,
          tipo,
          volume_ml:   parseInt(volumeMl),
          tipo_bebida: tipoBebida,
          alcoolica,
        });
      } else {
        await produtoService.saveProduto({
          ...base,
          tipo: 'Comida',
          vegana,
          porcao,
        });
      }

      // Limpa os campos após cadastro
      setNome(''); setPreco(''); setQuantidade(''); setCategoria('');
      setVolumeMl(''); setTipoBebida(''); setAlcoolica(false);
      setVegana(false); setPorcao('');

      carregarProdutos();
    } catch (err) {
      console.error('Erro ao cadastrar produto:', err);
    }
  };

  const isBebida = tipo === 'Bebida Alcoolica' || tipo === 'Bebida Nao Alcoolica';

  return (
    <div className="py-8">
      <div className="mb-8">
        <h1 className="text-3xl font-extrabold text-slate-800 mb-2">Produtos</h1>
        <p className="text-slate-500">Cadastre e visualize os produtos da barraca.</p>
      </div>

      <div className="grid grid-cols-1 lg:grid-cols-3 gap-8">
        {/* Formulário */}
        <div className="lg:col-span-1">
          <div className="bg-white p-6 rounded-xl shadow-sm border border-slate-100">
            <h2 className="text-lg font-bold text-slate-800 flex items-center gap-2 mb-6">
              <Plus className="text-[#1281b3]" size={20} /> Novo Produto
            </h2>

            <form className="space-y-4" onSubmit={(e) => { e.preventDefault(); handleCadastrar(); }}>

              {/* Tipo */}
              <div>
                <label className="block text-sm font-medium text-slate-700 mb-2">Tipo</label>
                <select
                  value={tipo} onChange={e => setTipo(e.target.value)}
                  className="w-full p-3 bg-slate-50 border border-slate-200 rounded-lg">
                  <option value="Comida">Comida</option>
                  <option value="Bebida Nao Alcoolica">Bebida Não Alcoólica</option>
                  <option value="Bebida Alcoolica">Bebida Alcoólica</option>
                </select>
              </div>

              {/* Campos comuns */}
              <div>
                <label className="block text-sm font-medium text-slate-700 mb-2">Nome</label>
                <input type="text" value={nome} onChange={e => setNome(e.target.value)}
                  placeholder="Ex: Água de Coco"
                  className="w-full p-3 bg-slate-50 border border-slate-200 rounded-lg" />
              </div>

              <div>
                <label className="block text-sm font-medium text-slate-700 mb-2">Categoria</label>
                <input type="text" value={categoria} onChange={e => setCategoria(e.target.value)}
                  placeholder="Ex: Bebidas, Lanches"
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

              {/* Campos específicos de Bebida */}
              {isBebida && (
                <>
                  <div>
                    <label className="block text-sm font-medium text-slate-700 mb-2">Tipo da Bebida</label>
                    <input type="text" value={tipoBebida} onChange={e => setTipoBebida(e.target.value)}
                      placeholder="Ex: Suco, Cerveja, Água de Coco"
                      className="w-full p-3 bg-slate-50 border border-slate-200 rounded-lg" />
                  </div>
                  <div>
                    <label className="block text-sm font-medium text-slate-700 mb-2">Volume (ml)</label>
                    <input type="number" value={volumeMl} onChange={e => setVolumeMl(e.target.value)}
                      placeholder="350"
                      className="w-full p-3 bg-slate-50 border border-slate-200 rounded-lg" />
                  </div>
                  <div className="flex items-center gap-2">
                    <input type="checkbox" checked={alcoolica} onChange={e => setAlcoolica(e.target.checked)}
                      className="w-4 h-4 accent-[#1281b3]" />
                    <label className="text-sm font-medium text-slate-700">É alcoólica?</label>
                  </div>
                </>
              )}

              {/* Campos específicos de Comida */}
              {!isBebida && (
                <>
                  <div>
                    <label className="block text-sm font-medium text-slate-700 mb-2">Porção</label>
                    <input type="text" value={porcao} onChange={e => setPorcao(e.target.value)}
                      placeholder="Ex: individual, para 2"
                      className="w-full p-3 bg-slate-50 border border-slate-200 rounded-lg" />
                  </div>
                  <div className="flex items-center gap-2">
                    <input type="checkbox" checked={vegana} onChange={e => setVegana(e.target.checked)}
                      className="w-4 h-4 accent-[#1281b3]" />
                    <label className="text-sm font-medium text-slate-700">É vegana?</label>
                  </div>
                </>
              )}

              <button type="submit"
                className="w-full bg-[#1281b3] hover:bg-[#0e668f] text-white font-medium py-3 rounded-lg transition-colors mt-2">
                Cadastrar
              </button>
            </form>
          </div>
        </div>

        {/* Lista de produtos */}
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
                  <div>
                    <span className="font-medium text-slate-700">{p.nome}</span>
                    <span className="ml-2 text-xs text-slate-400 bg-slate-100 px-2 py-0.5 rounded-full">{p.tipo}</span>
                    <p className="text-sm text-slate-500">R$ {p.preco} | Estoque: {p.quantidade}</p>
                  </div>
                  <button
                    onClick={async () => { await produtoService.deleteProduto(p.id); carregarProdutos(); }}
                    className="text-red-400 hover:text-red-600">
                    <Trash2 size={18} />
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
