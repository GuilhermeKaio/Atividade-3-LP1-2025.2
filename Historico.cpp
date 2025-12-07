#include "Historico.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>

// Adicionar registro ao histórico
void Historico::adicionarRegistro(const RegistroTreino &registro)
{
    registros.push_back(registro);
}

// Exibir histórico completo
void Historico::exibirHistorico() const
{
    std::cout << "\n--- HISTORICO DE TREINOS REALIZADOS ---\n";

    if (registros.empty())
    {
        std::cout << "Nenhum treino registrado ate o momento.\n";
    }
    else
    {
        std::cout << std::fixed << std::setprecision(2);

        for (const auto &reg : registros)
        {
            std::cout << "Data: " << reg.dataHora
                      << " | Ficha: " << reg.nomeFicha << " (ID: " << reg.idFicha << ")"
                      << " | Tempo: " << reg.tempoTotal << " min"
                      << " | Cal: " << reg.caloriasTotal << " kcal"
                      << std::endl;
        }
    }
    std::cout << "---------------------------------------\n";
}

// Getter de registros
const std::vector<RegistroTreino> &Historico::getRegistros() const
{
    return registros;
}

// Carregar histórico do arquivo
void Historico::carregarDeArquivo()
{
    std::ifstream arquivo("historico.txt");

    // Se o arquivo não existir (primeira execução), apenas retorna sem erro
    if (!arquivo.is_open())
    {
        return;
    }

    std::string linha;
    while (std::getline(arquivo, linha))
    {
        if (linha.empty())
            continue;

        std::stringstream ss(linha);
        std::string segmento;
        RegistroTreino reg;

        try
        {
            // Formato: DATA;ID_FICHA;NOME_FICHA;TEMPO_TOTAL;CALORIAS_TOTAL
            std::getline(ss, reg.dataHora, ';');
            std::getline(ss, segmento, ';');
            reg.idFicha = std::stoi(segmento);
            std::getline(ss, reg.nomeFicha, ';');
            std::getline(ss, segmento, ';');
            reg.tempoTotal = std::stod(segmento);
            std::getline(ss, segmento, ';');
            reg.caloriasTotal = std::stod(segmento);
            this->registros.push_back(reg);
        }
        catch (...)
        {
            continue;
        }
    }

    arquivo.close();
}

// Salvar histórico no arquivo
void Historico::salvarEmArquivo() const
{
    std::ofstream arquivo("historico.txt");

    if (!arquivo.is_open())
    {
        std::cerr << "Erro ao salvar o historico (permissao negada ou disco cheio).\n";
        return;
    }

    for (const auto &reg : registros)
    {
        arquivo << reg.dataHora << ";"
                << reg.idFicha << ";"
                << reg.nomeFicha << ";"
                << reg.tempoTotal << ";"
                << reg.caloriasTotal << "\n";
    }

    arquivo.close();
}
