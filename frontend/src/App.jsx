import { BrowserRouter, Routes, Route } from 'react-router-dom';
import Navbar from './components/Navbar';
import Inicio from './pages/Inicio';
import Barracas from './pages/Barracas';
import Produtos from './pages/Produtos';
import Pedidos from './pages/Pedidos';
import Estoque from './pages/Estoque';
import Relatorio from './pages/Relatorio';

function App() {
  return (
    <BrowserRouter>
      <div className="min-h-screen flex flex-col">
        <Navbar />
        <main className="grow container mx-auto px-4 py-8 max-w-6xl">
          <Routes>
            <Route path="/" element={<Inicio />} />
            <Route path="/barracas" element={<Barracas />} />
            <Route path="/produtos" element={<Produtos />} />
            <Route path="/pedidos" element={<Pedidos />} />
            <Route path="/estoque" element={<Estoque />} />
            <Route path="/relatorio" element={<Relatorio />} />
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