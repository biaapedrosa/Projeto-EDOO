#pragma once
#include <string>

// Classe que representa uma barraca de praia na orla de Recife
// Cada barraca tem um dono (usuario) e é identificada por um id único
// O token é gerado no login e usado para autenticar as requisições
class Barraca {
    private:
        int id;
        std::string nome;
        std::string usuario;
        std::string senhaHash;
        std::string token;

    public:
        // Construtor usado ao carregar do banco
        Barraca(int id = 0,
                const std::string& nome = "",
                const std::string& usuario = "",
                const std::string& senhaHash = "",
                const std::string& token = "");

        // Getters
        int getId() const;
        const std::string& getNome() const;
        const std::string& getUsuario() const;
        const std::string& getSenhaHash() const;
        const std::string& getToken() const;

        // Setters
        void setToken(const std::string& novoToken);

        // Serialização
        std::string toJson() const;
        std::string toString() const;
};