#include "../include/Barraca.h"
#include <sstream>
#include <iomanip>

// Inicializando todos os atributos
Barraca::Barraca(int id, const std::string& nome, const std::string& usuario,
                 const std::string& senhaHash, const std::string& token)
    : id(id), nome(nome), usuario(usuario), senhaHash(senhaHash), token(token)
{}

// Getters
int Barraca::getId() const {return id;}
const std::string& Barraca::getNome() const {return nome;}
const std::string& Barraca::getUsuario() const {return usuario;}
const std::string& Barraca::getSenhaHash() const {return senhaHash;}
const std::string& Barraca::getToken() const {return token;}

// Setter (token é atualizado a cada login)
void Barraca::setToken(const std::string& novoToken) {
    token = novoToken;
}

// Serializa para JSON (não inclui senha nem token por segurança)
std::string Barraca::toJson() const {
    std::ostringstream oss;
    oss << "{"
        << "\"id\":" << id << ","
        << "\"nome\":\"" << nome << "\","
        << "\"usuario\":\"" << usuario << "\""
        << "}";
    return oss.str();
}

// Representa a barraca como string — usado para debug
std::string Barraca::toString() const {
    return "Barraca[" + std::to_string(id) + "]: "
        + nome + " (@" + usuario + ")";
}