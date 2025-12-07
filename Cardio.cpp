#include "Cardio.h"
#include <iostream>
#include <iomanip>

// Construtor para novos cadastros
Cardio::Cardio(std::string nome, int duracao, double caloriasPorMinuto)
    : Exercicio(nome)
{
    this->duracao = duracao;
    this->caloriasPorMinuto = caloriasPorMinuto;
}

// Construtor para leitura de arquivo
Cardio::Cardio(int id, std::string nome, bool ativo, int duracao, double caloriasPorMinuto)
    : Exercicio(id, nome, ativo)
{
    this->duracao = duracao;
    this->caloriasPorMinuto = caloriasPorMinuto;
}

// Exibir detalhes do exercício
void Cardio::exibirDetalhes() const
{
    std::string statusStr = (this->isAtivo()) ? "Ativo" : "Inativo";

    std::cout << "Tipo: Cardio"
              << " | ID: " << this->getId()
              << " | Nome: " << this->getNome()
              << " | Status: " << statusStr
              << " | Duracao: " << this->duracao << " min"
              << " | Calorias: " << std::fixed << std::setprecision(2)
              << this->caloriasPorMinuto << " cal/min"
              << std::endl;
}

// Calcular tempo estimado (duração em minutos)
double Cardio::calcularTempoEstimado() const
{
    return this->duracao;
}

// Calcular calorias gastas
double Cardio::calcularCaloriasGastas() const
{
    return this->duracao * this->caloriasPorMinuto;
}

// Retornar tipo do exercício (1 = Cardio)
int Cardio::getTipo() const
{
    return 1;
}

// Getter de duração
int Cardio::getDuracao() const
{
    return this->duracao;
}

// Getter de calorias por minuto
double Cardio::getCaloriasPorMinuto() const
{
    return this->caloriasPorMinuto;
}
