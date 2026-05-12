#include "../include/routes.h"
#include "../include/Produto.h"
#include <iostream>
#include <string>

using namespace std;

//Define todas as portas de entrada do sistema

//CORS
//Libera o acesso ao servidor para qualquer origem
static void setCors(httplib::Response& res) {
    res.set_header("Access-Control-Allow-Origin",  "*");
    res.set_header("Access-Control-Allow-Methods", "GET, POST, PATCH, DELETE, OPTIONS");
    res.set_header("Access-Control-Allow-Headers", "Content-Type, Authorization");
}

//JSON é o formato que os sistemas usam para trocar informações
//Essa função extrai um texto de dentro de uma mensagem JSON
static string getJsonString(const string& json, const string& chave) {
    string busca = "\"" + chave + "\":\""; // monta o padrão que está procurando ex: "nome":"
    size_t pos = json.find(busca); // procura esse padrão no texto
    if (pos == string::npos) return "";
    pos += busca.size();
    size_t fim = json.find("\"", pos);
    return json.substr(pos, fim - pos);
}

//Extrai um número de dentro de uma mensagem JSON
//Funciona igual à função anterior, mas para números (preco, quantidade)
static double getJsonNumber(const string& json, const string& chave) {
    string busca = "\"" + chave + "\":";
    size_t pos = json.find(busca);
    if (pos == string::npos) return 0;
    pos += busca.size();
    return stod(json.substr(pos));
}

//Extrai um valor verdadeiro ou falso de um JSON.
static bool getJsonBool(const string& json, const string& chave) {
    string busca = "\"" + chave + "\":";
    size_t pos = json.find(busca);
    if (pos == string::npos) return false;
    pos += busca.size();
    return json.substr(pos, 4) == "true";
}

// Transforma a senha em um código embaralhado
// Por segurança não guardamos a senha real no banco de dados
// Passamos ela por uma fórmula matemática que gera um número único 
// Hash simples:
static string hashSenha(const string& senha) {
    unsigned long h = 5381; // valor inicial padrão do algoritmo djb2
    for (char c : senha)
        h = ((h << 5) + h) + static_cast<unsigned char>(c);
    return to_string(h);
}

// MIDDLEWARE DE AUTENTICAÇÃO
// Verifica se quem está fazendo um pedido tem permissão
static int autenticar(const httplib::Request& req, httplib::Response& res, Database& db) {
    auto it = req.headers.find("Authorization");
    if (it == req.headers.end()) {
        res.status = 401;
        res.set_content("{\"erro\":\"Token não fornecido\"}", "application/json");
        return -1;
    }

    string auth = it->second;
    string prefix = "Bearer ";
    string token = (auth.substr(0, prefix.size()) == prefix)
                   ? auth.substr(prefix.size())
                   : auth;

    int id = db.getBarracaIdPorToken(token);
    if (id == -1) {
        res.status = 401;
        res.set_content("{\"erro\":\"Token inválido\"}", "application/json");
    }
    return id;
}

//REGISTRO DE ROTAS
//Cadastra todas as rotas/endereços 
void registrarRotas(httplib::Server& svr, Database& db) {

    // Preflight CORS
    // Antes de fazer um pedido real, o navegador manda uma pergunta perguntando se pode.
    svr.Options(".*", [](const httplib::Request&, httplib::Response& res) {
        setCors(res);
        res.status = 204;
    });

    // AUTENTICAÇÃO 
    // Cadastra uma nova barraca no sistema
    // Quem usa: a tela de cadastro do frontend
    // O que recebe: nome da barraca, nome de usuário e senha
    svr.Post("/auth/registro", [&db](const httplib::Request& req, httplib::Response& res) {
        setCors(res);
        // extrai os dados enviados pelo formulário de cadastro
        try {
            string nome    = getJsonString(req.body, "nome");
            string usuario = getJsonString(req.body, "usuario");
            string senha   = getJsonString(req.body, "senha");

            // verifica se algum campo obrigatório ficou em branco
            if (nome.empty() || usuario.empty() || senha.empty()) {
                res.status = 400;
                res.set_content("{\"erro\":\"nome, usuario e senha são obrigatórios\"}", "application/json");
                return;
            }

            // tenta registrar no banco, se o usuário já existir retorna conflito (409)
            if (db.registrarBarraca(nome, usuario, hashSenha(senha))) {
                res.status = 201;
                res.set_content("{\"mensagem\":\"Barraca registrada com sucesso\"}", "application/json");
            } else {
                res.status = 409;
                res.set_content("{\"erro\":\"Usuário já existe\"}", "application/json");
            }
        } catch (const exception& e) {
            res.status = 500;
            res.set_content("{\"erro\":\"" + string(e.what()) + "\"}", "application/json");
        }
    });

    // Faz o login da barraca no sistema
    // Se usuário e senha estiverem corretos devolve um token que será usado nas próximas ações
    svr.Post("/auth/login", [&db](const httplib::Request& req, httplib::Response& res) {
        setCors(res);
        try {
            string usuario = getJsonString(req.body, "usuario");
            string senha   = getJsonString(req.body, "senha");

            string token = db.loginBarraca(usuario, hashSenha(senha));
            if (token.empty()) {
                res.status = 401;
                res.set_content("{\"erro\":\"Usuário ou senha inválidos\"}", "application/json");
            } else {
                res.set_content("{\"token\":\"" + token + "\"}", "application/json");
            }
        } catch (const exception& e) {
            res.status = 500;
            res.set_content("{\"erro\":\"" + string(e.what()) + "\"}", "application/json");
        }
    });

    // PRODUTOS
    // Cadastra um novo produto para a barraca logada criando objetos diferentes para cada tipo
    svr.Get("/produtos", [&db](const httplib::Request& req, httplib::Response& res) {
        setCors(res);
        int bid = autenticar(req, res, db);
        if (bid == -1) return;
        try {
            auto produtos = db.listarProdutosJson(bid);
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

    svr.Post("/produtos", [&db](const httplib::Request& req, httplib::Response& res) {
        setCors(res);
        int bid = autenticar(req, res, db);
        if (bid == -1) return;
        try {
            string body     = req.body;
            string tipo     = getJsonString(body, "tipo");
            string nome     = getJsonString(body, "nome");
            string categoria = getJsonString(body, "categoria");
            double preco    = getJsonNumber(body, "preco");
            int qtd         = static_cast<int>(getJsonNumber(body, "quantidade"));
            int id          = static_cast<int>(time(nullptr));

            if (tipo == "Bebida Alcoolica" || tipo == "Bebida Nao Alcoolica") {
                int vol         = static_cast<int>(getJsonNumber(body, "volume_ml"));
                string tipoBeb  = getJsonString(body, "tipo_bebida");
                bool alcoolica  = getJsonBool(body, "alcoolica");
                Bebida b(id, nome, preco, categoria, qtd, vol, tipoBeb, alcoolica);
                db.inserirProduto(b, tipo, vol, tipoBeb, alcoolica, false, "", bid);
            } else {
                bool vegana    = getJsonBool(body, "vegana");
                string porcao  = getJsonString(body, "porcao");
                Comida c(id, nome, preco, categoria, qtd, vegana, porcao);
                db.inserirProduto(c, "Comida", 0, "", false, vegana, porcao, bid);
            }

            res.status = 201;
            res.set_content("{\"mensagem\":\"Produto cadastrado com sucesso\"}", "application/json");
        } catch (const exception& e) {
            res.status = 400;
            res.set_content("{\"erro\":\"" + string(e.what()) + "\"}", "application/json");
        }
    });

    // Remove um produto pelo seu ID
    svr.Delete("/produtos/:id", [&db](const httplib::Request& req, httplib::Response& res) {
        setCors(res);
        int bid = autenticar(req, res, db);
        if (bid == -1) return;
        try {
            int id = stoi(req.path_params.at("id"));
            if (db.removerProduto(id, bid)) {
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

    // ESTOQUE
    // Atualiza apenas o preço de um produto
    // PATCH é usado quando queremos alterar só uma parte de um registro sem reescrever tudo
    svr.Patch("/estoque/:id/preco", [&db](const httplib::Request& req, httplib::Response& res) {
        setCors(res);
        int bid = autenticar(req, res, db);
        if (bid == -1) return;
        try {
            int id     = stoi(req.path_params.at("id"));
            double preco = getJsonNumber(req.body, "preco");
            if (db.atualizarPrecoProduto(id, preco, bid)) {
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

    // Atualiza apenas a quantidade em estoque de um produto
    svr.Patch("/estoque/:id/quantidade", [&db](const httplib::Request& req, httplib::Response& res) {
        setCors(res);
        int bid = autenticar(req, res, db);
        if (bid == -1) return;
        try {
            int id  = stoi(req.path_params.at("id"));
            int qtd = static_cast<int>(getJsonNumber(req.body, "quantidade"));
            if (db.atualizarQuantidadeProduto(id, qtd, bid)) {
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

    // PEDIDOS
    // Lista todos os pedidos da barraca logada
    svr.Get("/pedidos", [&db](const httplib::Request& req, httplib::Response& res) {
        setCors(res);
        int bid = autenticar(req, res, db);
        if (bid == -1) return;
        try {
            auto pedidos = db.listarPedidosJson(bid);
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

    // Cria um novo pedido com seus itens
    // Recebe o número da mesa e uma lista de produtos com suas quantidades e subtotais
    svr.Post("/pedidos", [&db](const httplib::Request& req, httplib::Response& res) {
        setCors(res);
        int bid = autenticar(req, res, db);
        if (bid == -1) return;
        try {
            int mesa     = static_cast<int>(getJsonNumber(req.body, "numeroMesa"));
            int pedidoId = db.inserirPedido(mesa, bid);

            string body = req.body;
            size_t pos = body.find("\"itens\":[");
            if (pos != string::npos) {
                pos += 9;
                while (pos < body.size() && body[pos] != ']') {
                    size_t inicio = body.find("{", pos);
                    size_t fim    = body.find("}", inicio);
                    if (inicio == string::npos || fim == string::npos) break;
                    
                    string item = body.substr(inicio, fim - inicio + 1);
                    int prodId  = static_cast<int>(getJsonNumber(item, "produtoId"));
                    int qtd     = static_cast<int>(getJsonNumber(item, "quantidade"));

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

    // Atualiza o status de um pedido
    // Os estados possíveis são: ABERTO → EM_PREPARO → ENTREGUE (ou CANCELADO)
    svr.Patch("/pedidos/:id/status", [&db](const httplib::Request& req, httplib::Response& res) {
        setCors(res);
        int bid = autenticar(req, res, db);
        if (bid == -1) return;
        try {
            int    id     = stoi(req.path_params.at("id"));
            string status = getJsonString(req.body, "status");

            if (status.empty()) status = "EM_PREPARO"; 

            if (db.atualizarStatusPedido(id, status, bid)) {
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

    // RELATÓRIO
    // Retorna um resumo financeiro/operacional da barraca (total vendido, pedidos, etc)
    svr.Get("/relatorio", [&db](const httplib::Request& req, httplib::Response& res) {
        setCors(res);
        int bid = autenticar(req, res, db);
        if (bid == -1) return;
        try {
            res.set_content(db.getRelatorioJson(bid), "application/json");
        } catch (const exception& e) {
            res.status = 500;
            res.set_content("{\"erro\":\"" + string(e.what()) + "\"}", "application/json");
        }
    });
}