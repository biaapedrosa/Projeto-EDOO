import { Link, useLocation } from 'react-router-dom';
import { Umbrella, Home, Package, ShoppingCart, Archive, BarChart2, Menu, LogOut } from 'lucide-react';
import { useState } from 'react';

export default function Navbar() {
  const location = useLocation();
  const [menuOpen, setMenuOpen] = useState(false);

  // Lê o nome da barraca salvo no localStorage após o login
  const nomeBarraca = localStorage.getItem('nomeBarraca');

  const handleLogout = () => {
    localStorage.removeItem('token');
    localStorage.removeItem('nomeBarraca');
    window.location.href = '/barracas';
  };

  const navLinks = [
    { name: 'Início',    path: '/',          icon: <Home size={18} /> },
    { name: 'Barracas',  path: '/barracas',  icon: <Umbrella size={18} /> },
    { name: 'Produtos',  path: '/produtos',  icon: <Package size={18} /> },
    { name: 'Pedidos',   path: '/pedidos',   icon: <ShoppingCart size={18} /> },
    { name: 'Estoque',   path: '/estoque',   icon: <Archive size={18} /> },
    { name: 'Relatório', path: '/relatorio', icon: <BarChart2 size={18} /> },
  ];

  return (
    <header className="bg-white shadow-sm border-b border-slate-200 sticky top-0 z-50">
      <div className="container mx-auto px-4 max-w-6xl flex justify-between items-center h-16">
        {/* Logo */}
        <Link to="/" className="flex items-center gap-2 text-xl font-bold text-slate-800">
          <div className="bg-[#1281b3] text-white p-1.5 rounded-full">
            <Umbrella size={20} />
          </div>
          <span className="text-[#1281b3]">Orla360</span>
        </Link>

        {/* Mobile Menu Button */}
        <button
          className="md:hidden p-2 text-slate-600"
          onClick={() => setMenuOpen(!menuOpen)}
        >
          <Menu size={24} />
        </button>

        {/* Desktop Nav */}
        <nav className="hidden md:flex gap-1 items-center">
          {navLinks.map((link) => {
            const isActive = location.pathname === link.path;
            return (
              <Link
                key={link.name}
                to={link.path}
                className={`flex items-center gap-2 px-3 py-2 rounded-md text-sm font-medium transition-colors ${
                  isActive
                    ? 'bg-[#1281b3] text-white'
                    : 'text-slate-600 hover:bg-slate-100 hover:text-slate-900'
                }`}
              >
                {link.icon}
                {link.name}
              </Link>
            );
          })}

          {/* Perfil da barraca logada */}
          {nomeBarraca && (
            <div className="flex items-center gap-2 ml-3 pl-3 border-l border-slate-200">
              <span className="text-sm font-medium text-slate-700">🏖️ {nomeBarraca}</span>
              <button
                onClick={handleLogout}
                title="Sair"
                className="text-slate-400 hover:text-red-500 transition-colors"
              >
                <LogOut size={16} />
              </button>
            </div>
          )}
        </nav>
      </div>

      {/* Mobile Nav */}
      {menuOpen && (
        <nav className="md:hidden bg-white border-t border-slate-100 p-4 space-y-2">
          {navLinks.map((link) => {
            const isActive = location.pathname === link.path;
            return (
              <Link
                key={link.name}
                to={link.path}
                onClick={() => setMenuOpen(false)}
                className={`flex items-center gap-3 px-4 py-3 rounded-md text-sm font-medium ${
                  isActive
                    ? 'bg-[#1281b3] text-white'
                    : 'text-slate-600 hover:bg-slate-50'
                }`}
              >
                {link.icon}
                {link.name}
              </Link>
            );
          })}

          {/* Perfil no mobile */}
          {nomeBarraca && (
            <button
              onClick={handleLogout}
              className="flex items-center gap-3 px-4 py-3 rounded-md text-sm font-medium text-red-500 hover:bg-red-50 w-full"
            >
              <LogOut size={18} />
              Sair ({nomeBarraca})
            </button>
          )}
        </nav>
      )}
    </header>
  );
}
