# 🏖️ Orla360 - Sistema de Gestão de Barracas de Praia

Sistema de informação desenvolvido em C++ com Programação Orientada a Objetos para gerenciar barracas de praia da orla de Recife. O sistema permite que donos de barracas cadastrem seus estabelecimentos e gerenciem produtos, pedidos, estoque e relatórios de vendas.

> Projeto da disciplina **Estruturas de Dados Orientadas a Objetos**  
> Centro de Informática - UFPE  
> **Alunas**: Beatriz Freitas, Beatriz Pandolfi, Luísa Bispo, Marina Cabral

---

## Funcionalidades

- **Cadastro de barracas** com autenticação (registro e login)
- **Gestão de produtos:** cadastro de Bebidas e Comidas com CRUD completo
- **Controle de pedidos:** criação, acompanhamento de status e cancelamento
- **Controle de estoque:** atualização de quantidades e alertas de estoque baixo
- **Relatório de vendas:** faturamento, produto mais vendido e alertas

---

## Tecnologias utilizadas

| Camada | Tecnologia |
|---|---|
| Backend | C++ |
| Servidor HTTP | cpp-httplib |
| Banco de dados | SQLite3 |
| Frontend | React + Vite |
| Estilização | Tailwind CSS |

O **C++** foi utilizado por ser requisito da disciplina e por permitir a aplicação direta dos conceitos de POO. Para expor o backend como uma API REST, escolhemos o **cpp-httplib** por ser uma biblioteca leve de arquivo único, sem necessidade de instalação complexa. O **SQLite** foi escolhido como banco de dados por ser local, sem necessidade de servidor externo, ideal para o contexto acadêmico do projeto. No frontend, optamos pelo **React com Vite** pela facilidade de criar interfaces dinâmicas e reativas, e pelo **Tailwind CSS** para estilização rápida e responsiva.

---

## Uso de Inteligência Artificial

Durante o desenvolvimento do projeto, utilizamos ferramentas de IA como apoio ao aprendizado e à produção de código. A IA foi utilizada para:

- **Consultas e estudos:** esclarecer dúvidas sobre conceitos de POO como herança, polimorfismo e padrões de projeto, além de entender como integrar C++ com SQLite e HTTP
- **Correção de código:** identificar e corrigir bugs e erros de compilação, como problemas de chaves incorretas e conflitos de tipos entre classes
- **Sugestões de arquitetura:** auxiliar na decisão de separar as rotas em `routes.cpp`, implementar o padrão Singleton na classe `Database` e estruturar a hierarquia de classes de `Produto`
- **Revisão e refatoração:** revisar o código produzido, sugerir melhorias de organização e garantir que os requisitos da disciplina fossem atendidos

Vale destacar que todas as decisões de design e implementação foram discutidas e aprovadas pelo grupo, sendo a IA utilizada como ferramenta de apoio e não como substituta do raciocínio e aprendizado.

---

## Estrutura do projeto

```
Projeto-EDOO/
├── backend/
│   ├── include/          # Headers das classes e bibliotecas
│   │   ├── Produto.h     # Classe base + subclasses Bebida e Comida
│   │   ├── Barraca.h     # Classe de barraca
│   │   ├── Pedido.h      # Classe de pedido
│   │   ├── ItemPedido.h  # Classe de item de pedido
│   │   ├── Estoque.h     # Classe de estoque
│   │   ├── Relatorio.h   # Classe de relatório
│   │   ├── Database.h    # Classe de banco de dados (Singleton)
│   │   ├── routes.h      # Declaração das rotas HTTP
│   │   ├── httplib.h     # Biblioteca HTTP
│   │   └── sqlite3.h     # Biblioteca SQLite
│   ├── src/              # Implementações
│   │   ├── Produto.cpp
│   │   ├── Barraca.cpp
│   │   ├── Pedido.cpp
│   │   ├── ItemPedido.cpp
│   │   ├── Estoque.cpp
│   │   ├── Relatorio.cpp
│   │   ├── Database.cpp
│   │   ├── routes.cpp    # Endpoints da API REST
│   │   └── sqlite3.c     # Código fonte do SQLite
│   └── main.cpp          # Inicializa o servidor HTTP
└── frontend/
    └── src/
        ├── pages/        # Páginas do sistema
        ├── components/   # Componentes reutilizáveis (Navbar)
        └── services/
            └── api.js    # Comunicação com o backend
```

---

## Como rodar o projeto

### Pré-requisitos

- **g++** (MinGW-W64 no Windows ou g++ no Linux/Mac)
- **gcc** (para compilar o SQLite)
- **Node.js** e **npm**

### Passo 1: Compilar o SQLite separadamente

O SQLite é código C puro, então precisa ser compilado com `gcc` antes do resto:

```bash
gcc -c backend/src/sqlite3.c -o sqlite3.o
```

### Passo 2: Compilar o backend

**Windows:**
```bash
g++ backend/src/Produto.cpp backend/src/Estoque.cpp backend/src/ItemPedido.cpp backend/src/Pedido.cpp backend/src/Relatorio.cpp backend/src/Database.cpp backend/src/routes.cpp backend/src/Barraca.cpp backend/main.cpp sqlite3.o -I backend/include -o orla360 -lws2_32 -lpthread -D_WIN32_WINNT=0x0A00
```

**Linux/Mac:**
```bash
g++ backend/src/Produto.cpp backend/src/Estoque.cpp backend/src/ItemPedido.cpp backend/src/Pedido.cpp backend/src/Relatorio.cpp backend/src/Database.cpp backend/src/routes.cpp backend/src/Barraca.cpp backend/main.cpp sqlite3.o -I backend/include -o orla360 -lpthread
```

### Passo 3: Rodar o backend

```bash
./orla360
```

O servidor vai iniciar na porta **8080** e criar o arquivo `orla360.db` automaticamente.

### Passo 4: Instalar dependências do frontend

```bash
cd frontend
npm install
```

### Passo 5: Rodar o frontend

```bash
npm run dev
```

O frontend vai estar disponível em **http://localhost:5173**

> ⚠️ **Importante:** O backend deve estar rodando antes de usar o frontend.

---

## Como usar o sistema

1. Acesse **http://localhost:5173** no navegador
2. Vá em **Barracas** e registre sua barraca
3. Faça **login** com o usuário e senha cadastrados
4. Gerencie seus **Produtos**, **Pedidos** e **Estoque**
5. Visualize o **Relatório** de vendas do dia

---

## Endpoints da API

| Método | Rota | Descrição |
|---|---|---|
| POST | `/auth/registro` | Registrar nova barraca |
| POST | `/auth/login` | Login da barraca |
| GET | `/produtos` | Listar produtos |
| POST | `/produtos` | Cadastrar produto |
| DELETE | `/produtos/:id` | Remover produto |
| PATCH | `/estoque/:id/preco` | Atualizar preço |
| PATCH | `/estoque/:id/quantidade` | Atualizar quantidade |
| GET | `/pedidos` | Listar pedidos |
| POST | `/pedidos` | Criar pedido |
| PATCH | `/pedidos/:id/status` | Atualizar status |
| GET | `/relatorio` | Ver relatório |

---

## Conceitos de POO utilizados

- **Encapsulamento:** atributos privados com getters e setters em todas as classes
- **Herança:** `Bebida` e `Comida` herdam de `Produto`
- **Polimorfismo:** métodos virtuais `getTipo()`, `toJson()` e `toString()` sobrescritos nas subclasses
- **Modificadores de acesso:** `private`, `protected` e `public`
- **Ponteiros e referências:** uso de `unique_ptr`, `Produto*` e referências em parâmetros
- **Padrão de projeto Singleton:** classe `Database` garante uma única instância de conexão com o banco
