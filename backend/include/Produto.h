#pragma once
#include <string>

class Produto{
    protected:
        // Atributos
        int id;
        std::string nome_produto;
        double preco;
        std::string categoria;
        int quantidade_em_estoque;

    public:
        // Construtor 
        Produto(int id = 0, 
            const std::string& nome = "", 
            double p = 0.0, 
            const std::string& ctg = "",
            int qtd = 0);
        
        // Destrutor virtual
        virtual ~Produto() = default;
 
        // Métodos
        // Getters
        int getId() const;
        const std::string& getNome() const;
        double getPreco() const;
        const std::string& getCategoria() const;
        int getQuantidadeEmEstoque() const;
        
        // Setters
        void setPreco(double novoPreco);
        void setQuantidadeEmEstoque(int novaQtd);
        
        // Métodos virtuais
        virtual std::string getTipo()     const;
        virtual std::string toJson()      const;
        virtual std::string toString()    const;

};



// Subclasse Bebida
// Herda tudo de Produto e adiciona:
//   - volume em ml (com validação: deve ser positivo)
//   - tipo da bebida (ex: "Suco", "Cerveja", "Agua de Coco")
//   - se é alcoolica
class Bebida : public Produto {
private:
    int volume_ml;
    std::string tipo_bebida;
    bool alcoolica;

public:
    Bebida(int id,
           const std::string& nome,
           double preco,
           const std::string& categoria,
           int qtd,
           int volume_ml,
           const std::string& tipo_bebida,
           bool alcoolica);

    int getVolume() const;
    const std::string& getTipoBebida() const;
    bool isAlcoolica() const;

    std::string getTipo() const override; // retorna "Bebida Alcoolica" ou "Bebida Nao Alcoolica"
    std::string toJson() const override;
    std::string toString() const override;
};



// Subclasse Comida
// Herda tudo de Produto e adiciona:
//   - se tem opção vegana
//   - tamanho da porção
class Comida : public Produto {
private:
    bool vegana;
    std::string porcao; // ex: "individual", "para 2"

public:
    Comida(int id,
           const std::string& nome,
           double preco,
           const std::string& categoria,
           int qtd,
           bool vegana,
           const std::string& porcao);

    bool isVegana() const;
    const std::string& getPorcao() const;

    std::string getTipo() const override;
    std::string toJson() const override;
    std::string toString() const override;
};
