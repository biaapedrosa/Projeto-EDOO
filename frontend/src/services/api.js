import axios from 'axios';

const api = axios.create({
    baseURL: 'http://localhost:8080',
    headers: {
        'Content-Type': 'application/json',
    },
    timeout: 10000,
});

// Interceptor — injeta o token em TODA requisição automaticamente
// Isso resolve o 401 em GET /produtos, POST /produtos, DELETE /produtos/:id,
// PATCH /estoque/:id/quantidade e PATCH /estoque/:id/preco
api.interceptors.request.use((config) => {
    const token = localStorage.getItem('token');
    if (token) {
        config.headers['Authorization'] = `Bearer ${token}`;
    }
    return config;
});

export const produtoService = {
    getProdutos:   ()        => api.get('/produtos'),
    saveProduto:   (produto) => api.post('/produtos', produto),
    deleteProduto: (id)      => api.delete(`/produtos/${id}`),
};

export const estoqueService = {
    atualizarPreco:      (id, novoPreco) => api.patch(`/estoque/${id}/preco`,      { preco: novoPreco }),
    atualizarQuantidade: (id, novaQtd)   => api.patch(`/estoque/${id}/quantidade`, { quantidade: novaQtd }),
};

export default api;