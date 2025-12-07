#include "Ficha.h"
#include <iostream>
#include <iomanip>

int Ficha::proximoId = 1;

Ficha::Ficha(std::string nome)
{
    this->nome = nome;
    this->id = proximoId++;
}

// Construtor para leitura de arquivo
Ficha::Ficha(int id, std::string nome)
{
    this->id = id;
    this->nome = nome;
}

// Destrutor (não deletar exercícios, apenas limpar vector)
Ficha::~Ficha()
{
    exercicios.clear();
}

// Adicionar exercício à ficha
void Ficha::adicionarExercicio(Exercicio *exercicio)
{
    if (exercicio != nullptr)
    {
        this->exercicios.push_back(exercicio);
    }
}

// Exibir ficha completa com exercícios e totais
void Ficha::exibirFicha() const
{
    std::cout << "================================================\n";
    std::cout << "FICHA " << this->getId() << ": " << this->getNome() << "\n";
    std::cout << "================================================\n";

    if (exercicios.empty())
    {
        std::cout << " (Nenhum exercicio nesta ficha)\n";
    }
    else
    {
        for (const auto *ex : exercicios)
        {
            if (ex != nullptr)
            {
                ex->exibirDetalhes();
            }
        }
    }

    std::cout << "------------------------------------------------\n";
    std::cout << std::fixed << std::setprecision(2);

    std::cout << "RESUMO DO TREINO:\n";
    std::cout << "Tempo Total Estimado: " << this->calcularTempoTotal() << " min\n";
    std::cout << "Gasto Calorico Total: " << this->calcularCaloriasTotais() << " kcal\n";
    std::cout << "================================================\n";
}

// Calcular tempo total da ficha
double Ficha::calcularTempoTotal() const
{
    double total = 0.0;

    for (const auto *ex : exercicios)
    {
        if (ex != nullptr)
        {
            total += ex->calcularTempoEstimado();
        }
    }

    return total;
}

// Calcular calorias totais da ficha
double Ficha::calcularCaloriasTotais() const
{
    double total = 0.0;

    for (const auto *ex : exercicios)
    {
        if (ex != nullptr)
        {
            total += ex->calcularCaloriasGastas();
        }
    }

    return total;
}

// Getters
int Ficha::getId() const
{
    return id;
}

std::string Ficha::getNome() const
{
    return nome;
}

const std::vector<Exercicio *> &Ficha::getExercicios() const
{
    return this->exercicios;
}

// Atualizar próximo ID
void Ficha::atualizarProximoId(int maiorId)
{
    if (maiorId >= proximoId)
    {
        proximoId = maiorId + 1;
    }
}
