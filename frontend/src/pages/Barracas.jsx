import { useState } from 'react';
import { authService } from '../services/api';

export default function Barracas() {
  // Controla qual aba está ativa: 'registro' ou 'login'
  const [aba, setAba]           = useState('registro');
  const [nome, setNome]         = useState('');
  const [usuario, setUsuario]   = useState('');
  const [senha, setSenha]       = useState('');
  const [mensagem, setMensagem] = useState('');
  const [erro, setErro]         = useState('');

  const handleRegistro = async (e) => {
    e.preventDefault();
    setMensagem(''); setErro('');
    try {
      await authService.registrar({ nome, usuario, senha });
      setMensagem('Barraca registrada com sucesso! Faça login para continuar.');
      setNome(''); setUsuario(''); setSenha('');
      setAba('login');
    } catch (err) {
      setErro(err.response?.data?.erro || 'Erro ao registrar barraca.');
    }
  };

  const handleLogin = async (e) => {
    e.preventDefault();
    setMensagem(''); setErro('');
    try {
      const res = await authService.login({ usuario, senha });
      // Salva o token no localStorage — o interceptor do api.js injeta em todas as requisições
      localStorage.setItem('token', res.data.token);
      setMensagem('Login realizado com sucesso!');
      setUsuario(''); setSenha('');
    } catch (err) {
      setErro(err.response?.data?.erro || 'Usuário ou senha inválidos.');
    }
  };

  return (
    <div className="max-w-2xl mx-auto py-8">
      <div className="mb-8">
        <h1 className="text-3xl font-extrabold text-slate-800 mb-2">Cadastro de Barraca</h1>
        <p className="text-slate-500">Registre ou entre na sua barraca na orla de Recife.</p>
      </div>

      {/* Abas */}
      <div className="flex gap-2 mb-6">
        <button
          onClick={() => { setAba('registro'); setMensagem(''); setErro(''); }}
          className={`px-5 py-2 rounded-lg font-medium transition-colors ${
            aba === 'registro'
              ? 'bg-[#1281b3] text-white'
              : 'bg-slate-100 text-slate-600 hover:bg-slate-200'
          }`}>
          Registrar
        </button>
        <button
          onClick={() => { setAba('login'); setMensagem(''); setErro(''); }}
          className={`px-5 py-2 rounded-lg font-medium transition-colors ${
            aba === 'login'
              ? 'bg-[#1281b3] text-white'
              : 'bg-slate-100 text-slate-600 hover:bg-slate-200'
          }`}>
          Login
        </button>
      </div>

      <div className="bg-white p-6 md:p-8 rounded-xl shadow-sm border border-slate-100">

        {/* Mensagem de sucesso */}
        {mensagem && (
          <div className="mb-4 p-3 bg-green-50 border border-green-200 text-green-700 rounded-lg text-sm">
            {mensagem}
          </div>
        )}

        {/* Mensagem de erro */}
        {erro && (
          <div className="mb-4 p-3 bg-red-50 border border-red-200 text-red-700 rounded-lg text-sm">
            {erro}
          </div>
        )}

        {/* Formulário de Registro */}
        {aba === 'registro' && (
          <form className="space-y-4" onSubmit={handleRegistro}>
            <div>
              <label className="block text-sm font-medium text-slate-700 mb-2">Nome da Barraca</label>
              <input type="text" value={nome} onChange={e => setNome(e.target.value)}
                placeholder="Ex: Barraca do Sol"
                className="w-full p-3 bg-slate-50 border border-slate-200 rounded-lg focus:outline-none focus:ring-2 focus:ring-[#1281b3]" />
            </div>
            <div>
              <label className="block text-sm font-medium text-slate-700 mb-2">Usuário</label>
              <input type="text" value={usuario} onChange={e => setUsuario(e.target.value)}
                placeholder="Ex: barraca_sol"
                className="w-full p-3 bg-slate-50 border border-slate-200 rounded-lg focus:outline-none focus:ring-2 focus:ring-[#1281b3]" />
            </div>
            <div>
              <label className="block text-sm font-medium text-slate-700 mb-2">Senha</label>
              <input type="password" value={senha} onChange={e => setSenha(e.target.value)}
                placeholder="••••••••"
                className="w-full p-3 bg-slate-50 border border-slate-200 rounded-lg focus:outline-none focus:ring-2 focus:ring-[#1281b3]" />
            </div>
            <button type="submit"
              className="w-full bg-[#1281b3] hover:bg-[#0e668f] text-white font-medium py-3 rounded-lg transition-colors">
              Cadastrar Barraca
            </button>
          </form>
        )}

        {/* Formulário de Login */}
        {aba === 'login' && (
          <form className="space-y-4" onSubmit={handleLogin}>
            <div>
              <label className="block text-sm font-medium text-slate-700 mb-2">Usuário</label>
              <input type="text" value={usuario} onChange={e => setUsuario(e.target.value)}
                placeholder="Ex: barraca_sol"
                className="w-full p-3 bg-slate-50 border border-slate-200 rounded-lg focus:outline-none focus:ring-2 focus:ring-[#1281b3]" />
            </div>
            <div>
              <label className="block text-sm font-medium text-slate-700 mb-2">Senha</label>
              <input type="password" value={senha} onChange={e => setSenha(e.target.value)}
                placeholder="••••••••"
                className="w-full p-3 bg-slate-50 border border-slate-200 rounded-lg focus:outline-none focus:ring-2 focus:ring-[#1281b3]" />
            </div>
            <button type="submit"
              className="w-full bg-[#1281b3] hover:bg-[#0e668f] text-white font-medium py-3 rounded-lg transition-colors">
              Entrar
            </button>
          </form>
        )}
      </div>
    </div>
  );
}
