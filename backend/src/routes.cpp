#include "../include/routes.h"
#include "../include/Produto.h"
#include <iostream>
#include <string>

using namespace std;

// CORS 
// Adiciona os headers de CORS em todas as respostas
// Sem isso o navegador bloqueia as requisições do React (porta 5173) para o backend C++ (porta 8080) por política de segurança.
static void setCors(httplib::Response& res) {
    res.set_header("Access-Control-Allow-Origin",  "*");
    res.set_header("Access-Control-Allow-Methods", "GET, POST, PATCH, DELETE, OPTIONS");
    res.set_header("Access-Control-Allow-Headers", "Content-Type");
}

// Parser JSON simples
// Extrai o valor de um campo string de um JSON no formato "chave":"valor"
static string getJsonString(const string& json, const string& chave) {
    string busca = "\"" + chave + "\":\"";
    size_t pos = json.find(busca);
    if (pos == string::npos) return "";
    pos += busca.size();
    size_t fim = json.find("\"", pos);
    return json.substr(pos, fim - pos);
}

// Extrai o valor de um campo numérico de um JSON no formato "chave":valor
static double getJsonNumber(const string& json, const string& chave) {
    string busca = "\"" + chave + "\":";
    size_t pos = json.find(busca);
    if (pos == string::npos) return 0;
    pos += busca.size();
    return stod(json.substr(pos));
}

// Extrai o valor de um campo booleano de um JSON no formato "chave":true/false
static bool getJsonBool(const string& json, const string& chave) {
    string busca = "\"" + chave + "\":";
    size_t pos = json.find(busca);
    if (pos == string::npos) return false;
    pos += busca.size();
    return json.substr(pos, 4) == "true";
}

// Registro de rotas
void registrarRotas(httplib::Server& svr, Database& db) {

    // Preflight CORS — o navegador envia OPTIONS antes de POST/PATCH/DELETE para verificar se o servidor aceita CORS
    svr.Options(".*", [](const httplib::Request&, httplib::Response& res) {
        setCors(res);
        res.status = 204;
    });


    // --- PRODUTOS ----------------------------
    // GET /produtos — retorna todos os produtos como array JSON
    svr.Get("/produtos", [&db](const httplib::Request&, httplib::Response& res) {
        setCors(res);
        try {
            auto produtos = db.listarProdutosJson();
            string json = "[";
            for (size_t i = 0; i < produtos.size(); ++i) {
                json += produtos[i];
                if (i + 1 < produtos.size()) json += ",";
            }
            json += "]";
            res.set_content(json, "application/json");
        } catch (const exception& e) {
            res.status = 500;
            res.set_content("{\"erro\":\"" + string(e.what()) + "\"}", "application/json");
        }
    });

    // POST /produtos — cadastra um novo produto
    // Espera JSON com: tipo, nome, preco, categoria, quantidade + campos extras dependendo do tipo (Bebida ou Comida)
    svr.Post("/produtos", [&db](const httplib::Request& req, httplib::Response& res) {
        setCors(res);
        try {
            string body = req.body;
            string tipo = getJsonString(body, "tipo");
            string nome = getJsonString(body, "nome");
            string categoria = getJsonString(body, "categoria");
            double preco = getJsonNumber(body, "preco");
            int qtd  = static_cast<int>(getJsonNumber(body, "quantidade"));

            // Gera um id baseado no timestamp — simples e sem colisão
            int id = static_cast<int>(time(nullptr));

            if (tipo == "Bebida Alcoolica" || tipo == "Bebida Nao Alcoolica") {
                int volume = static_cast<int>(getJsonNumber(body, "volume_ml"));
                string tipoBebida = getJsonString(body, "tipo_bebida");
                bool alcoolica = getJsonBool(body, "alcoolica");
                Bebida b(id, nome, preco, categoria, qtd, volume, tipoBebida, alcoolica);
                db.inserirProduto(b, tipo, volume, tipoBebida, alcoolica, false, "");
            } else {
                bool vegana = getJsonBool (body, "vegana");
                string porcao = getJsonString(body, "porcao");
                Comida c(id, nome, preco, categoria, qtd, vegana, porcao);
                db.inserirProduto(c, "Comida", 0, "", false, vegana, porcao);
            }

            res.status = 201; // 201 Created
            res.set_content("{\"mensagem\":\"Produto cadastrado com sucesso\"}", "application/json");
        } catch (const exception& e) {
            res.status = 400;
            res.set_content("{\"erro\":\"" + string(e.what()) + "\"}", "application/json");
        }
    });

    // DELETE /produtos/:id — remove um produto pelo id
    svr.Delete("/produtos/:id", [&db](const httplib::Request& req, httplib::Response& res) {
        setCors(res);
        try {
            int id = stoi(req.path_params.at("id"));
            if (db.removerProduto(id)) {
                res.set_content("{\"mensagem\":\"Produto removido\"}", "application/json");
            } else {
                res.status = 404;
                res.set_content("{\"erro\":\"Produto não encontrado\"}", "application/json");
            }
        } catch (const exception& e) {
            res.status = 400;
            res.set_content("{\"erro\":\"" + string(e.what()) + "\"}", "application/json");
        }
    });

  
    // --- ESTOQUE ----------------------------
    // PATCH /estoque/:id/preco — atualiza o preço de um produto
    svr.Patch("/estoque/:id/preco", [&db](const httplib::Request& req, httplib::Response& res) {
        setCors(res);
        try {
            int id = stoi(req.path_params.at("id"));
            double preco = getJsonNumber(req.body, "preco");
            if (db.atualizarPrecoProduto(id, preco)) {
                res.set_content("{\"mensagem\":\"Preço atualizado\"}", "application/json");
            } else {
                res.status = 404;
                res.set_content("{\"erro\":\"Produto não encontrado\"}", "application/json");
            }
        } catch (const exception& e) {
            res.status = 400;
            res.set_content("{\"erro\":\"" + string(e.what()) + "\"}", "application/json");
        }
    });

    // PATCH /estoque/:id/quantidade — atualiza a quantidade de um produto
    svr.Patch("/estoque/:id/quantidade", [&db](const httplib::Request& req, httplib::Response& res) {
        setCors(res);
        try {
            int id = stoi(req.path_params.at("id"));
            int qtd = static_cast<int>(getJsonNumber(req.body, "quantidade"));
            if (db.atualizarQuantidadeProduto(id, qtd)) {
                res.set_content("{\"mensagem\":\"Quantidade atualizada\"}", "application/json");
            } else {
                res.status = 404;
                res.set_content("{\"erro\":\"Produto não encontrado\"}", "application/json");
            }
        } catch (const exception& e) {
            res.status = 400;
            res.set_content("{\"erro\":\"" + string(e.what()) + "\"}", "application/json");
        }
    });

    
    // --- PEDIDOS ----------------------------
    // GET /pedidos — retorna todos os pedidos como array JSON
    svr.Get("/pedidos", [&db](const httplib::Request&, httplib::Response& res) {
        setCors(res);
        try {
            auto pedidos = db.listarPedidosJson();
            string json = "[";
            for (size_t i = 0; i < pedidos.size(); ++i) {
                json += pedidos[i];
                if (i + 1 < pedidos.size()) json += ",";
            }
            json += "]";
            res.set_content(json, "application/json");
        } catch (const exception& e) {
            res.status = 500;
            res.set_content("{\"erro\":\"" + string(e.what()) + "\"}", "application/json");
        }
    });

    // POST /pedidos — cria um novo pedido com itens
    // Espera JSON com: mesa (int) e itens (array de {produtoId, quantidade, subtotal})
    svr.Post("/pedidos", [&db](const httplib::Request& req, httplib::Response& res) {
        setCors(res);
        try {
            int mesa = static_cast<int>(getJsonNumber(req.body, "mesa"));
            int pedidoId = db.inserirPedido(mesa);

            // Percorre o array de itens no JSON e insere cada um
            string body = req.body;
            size_t pos = body.find("\"itens\":[");
            if (pos != string::npos) {
                pos += 9;
                while (pos < body.size() && body[pos] != ']') {
                    size_t inicio = body.find("{", pos);
                    size_t fim    = body.find("}", inicio);
                    if (inicio == string::npos || fim == string::npos) break;

                    string item = body.substr(inicio, fim - inicio + 1);
                    int prodId = static_cast<int>(getJsonNumber(item, "produtoId"));
                    int qtd = static_cast<int>(getJsonNumber(item, "quantidade"));
                    double sub = getJsonNumber(item, "subtotal");

                    db.inserirItemPedido(pedidoId, prodId, qtd, sub);
                    pos = fim + 1;
                }
            }

            res.status = 201;
            res.set_content("{\"id\":" + to_string(pedidoId) + ",\"mensagem\":\"Pedido criado\"}", "application/json");
        } catch (const exception& e) {
            res.status = 400;
            res.set_content("{\"erro\":\"" + string(e.what()) + "\"}", "application/json");
        }
    });

    // PATCH /pedidos/:id/status — atualiza o status de um pedido
    // Espera JSON com: status ("EM_PREPARO", "ENTREGUE" ou "CANCELADO")
    svr.Patch("/pedidos/:id/status", [&db](const httplib::Request& req, httplib::Response& res) {
        setCors(res);
        try {
            int    id     = stoi(req.path_params.at("id"));
            string status = getJsonString(req.body, "status");
            if (db.atualizarStatusPedido(id, status)) {
                res.set_content("{\"mensagem\":\"Status atualizado\"}", "application/json");
            } else {
                res.status = 404;
                res.set_content("{\"erro\":\"Pedido não encontrado\"}", "application/json");
            }
        } catch (const exception& e) {
            res.status = 400;
            res.set_content("{\"erro\":\"" + string(e.what()) + "\"}", "application/json");
        }
    });


    // --- RELATÓRIO ----------------------------
    // GET /relatorio — retorna dados consolidados do dia
    svr.Get("/relatorio", [&db](const httplib::Request&, httplib::Response& res) {
        setCors(res);
        try {
            res.set_content(db.getRelatorioJson(), "application/json");
        } catch (const exception& e) {
            res.status = 500;
            res.set_content("{\"erro\":\"" + string(e.what()) + "\"}", "application/json");
        }
    });
}