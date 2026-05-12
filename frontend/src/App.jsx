import { BrowserRouter, Routes, Route, Navigate } from 'react-router-dom';
import Navbar from './components/Navbar';
import Inicio from './pages/Inicio';
import Barracas from './pages/Barracas';
import Produtos from './pages/Produtos';
import Pedidos from './pages/Pedidos';
import Estoque from './pages/Estoque';
import Relatorio from './pages/Relatorio';

// Componente que protege rotas — redireciona para /barracas se não tiver token
function RotaProtegida({ children }) {
  const token = localStorage.getItem('token');
  if (!token) {
    return <Navigate to="/barracas" replace />;
  }
  return children;
}

function App() {
  return (
    <BrowserRouter>
      <div className="min-h-screen flex flex-col">
        <Navbar />
        <main className="grow container mx-auto px-4 py-8 max-w-6xl">
          <Routes>
            {/* Rotas públicas — acessíveis sem login */}
            <Route path="/" element={<Inicio />} />
            <Route path="/barracas" element={<Barracas />} />

            {/* Rotas protegidas — exigem login */}
            <Route path="/produtos" element={
              <RotaProtegida><Produtos /></RotaProtegida>
            } />
            <Route path="/pedidos" element={
              <RotaProtegida><Pedidos /></RotaProtegida>
            } />
            <Route path="/estoque" element={
              <RotaProtegida><Estoque /></RotaProtegida>
            } />
            <Route path="/relatorio" element={
              <RotaProtegida><Relatorio /></RotaProtegida>
            } />
          </Routes>
        </main>
        <footer className="text-center py-6 text-sm text-slate-500 border-t border-slate-200 mt-auto">
          Sistema de Gerenciamento de Barracas de Praia — Orla de Recife, PE
        </footer>
      </div>
    </BrowserRouter>
  );
}

export default App;
