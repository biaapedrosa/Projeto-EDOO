import axios from 'axios';

const api = axios.create({
    baseURL: 'http://localhost:8080',
    headers: {
        'Content-Type': 'application/json',
    },
    timeout: 10000,
});

// Interceptor — injeta o token em TODA requisição automaticamente
api.interceptors.request.use((config) => {
    const token = localStorage.getItem('token');
    if (token) {
        config.headers['Authorization'] = `Bearer ${token}`;
    }
    return config;
});

// Serviço de autenticação — registro e login de barracas
export const authService = {
    registrar: (dados) => api.post('/auth/registro', dados),
    login:     (dados) => api.post('/auth/login', dados),
};

// Serviço de produtos — CRUD completo
export const produtoService = {
    getProdutos:   ()        => api.get('/produtos'),
    saveProduto:   (produto) => api.post('/produtos', produto),
    deleteProduto: (id)      => api.delete(`/produtos/${id}`),
};

// Serviço de estoque — atualizar preço e quantidade
export const estoqueService = {
    atualizarPreco:      (id, novoPreco) => api.patch(`/estoque/${id}/preco`,      { preco: novoPreco }),
    atualizarQuantidade: (id, novaQtd)   => api.patch(`/estoque/${id}/quantidade`, { quantidade: novaQtd }),
};

export const pedidoService = {
    getPedidos: () => api.get('/pedidos'),

    criarPedido: (pedido) => api.post('/pedidos', pedido),

    avancarStatus: (id) => api.patch(`/pedidos/${id}/avancar`),

    cancelarPedido: (id) => api.patch(`/pedidos/${id}/cancelar`),
};

export const relatorioService = {
    getDadosGerais: () => api.get('/relatorio'),
// Serviço de pedidos — criar, listar e atualizar status
export const pedidoService = {
    getPedidos:      ()         => api.get('/pedidos'),
    criarPedido:     (pedido)   => api.post('/pedidos', pedido),
    atualizarStatus: (id, status) => api.patch(`/pedidos/${id}/status`, { status }),
};

// Serviço de relatório — dados consolidados do dia
export const relatorioService = {
    getRelatorio: () => api.get('/relatorio'),
};

export default api;