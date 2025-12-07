#include "Sistema.h"
#include "Cardio.h"
#include "Forca.h"
#include "Utils.h"
#include <iostream>
#include <fstream>
#include <limits>
#include <sstream>

Sistema::Sistema()
{
    carregarDados();
}

Sistema::~Sistema()
{
    salvarDados();
    for (auto *ex : exercicios)
    {
        delete ex;
    }
    exercicios.clear();
    for (auto *ficha : fichas)
    {
        delete ficha;
    }
    fichas.clear();
}

// Carregar dados dos arquivos
void Sistema::carregarDados()
{
    std::cout << "Carregando dados do sistema...\n";
    std::ifstream arqEx("exercicios.txt");
    if (arqEx.is_open())
    {
        std::string linha;
        while (std::getline(arqEx, linha))
        {
            if (linha.empty())
                continue;
            std::stringstream ss(linha);
            std::string segmento;

            try
            {
                std::getline(ss, segmento, ';');
                int tipo = std::stoi(segmento);

                std::getline(ss, segmento, ';');
                int id = std::stoi(segmento);

                std::string nome;
                std::getline(ss, nome, ';');

                if (tipo == 1)
                {
                    std::getline(ss, segmento, ';');
                    int duracao = std::stoi(segmento);

                    std::getline(ss, segmento, ';');
                    double calorias = std::stod(segmento);

                    std::getline(ss, segmento, ';');
                    bool ativo = (std::stoi(segmento) == 1);
                    Cardio *novo = new Cardio(id, nome, ativo, duracao, calorias);
                    exercicios.push_back(novo);
                }
                else if (tipo == 2) // Força
                {
                    std::getline(ss, segmento, ';');
                    double carga = std::stod(segmento);

                    std::getline(ss, segmento, ';');
                    int series = std::stoi(segmento);

                    std::getline(ss, segmento, ';');
                    int reps = std::stoi(segmento);

                    std::getline(ss, segmento, ';');
                    int descanso = std::stoi(segmento);

                    std::getline(ss, segmento, ';');
                    bool ativo = (std::stoi(segmento) == 1);
                    Forca *novo = new Forca(id, nome, ativo, carga, series, reps, descanso);
                    exercicios.push_back(novo);
                }
            }
            catch (...)
            {
                continue;
            }
        }
        arqEx.close();
    }
    std::ifstream arqFichas("fichas.txt");
    if (arqFichas.is_open())
    {
        std::string linha;
        int maiorIdFicha = 0;

        while (std::getline(arqFichas, linha))
        {
            if (linha.empty())
                continue;
            std::stringstream ss(linha);
            std::string segmento;

            try
            {
                std::getline(ss, segmento, ';');
                int idFicha = std::stoi(segmento);
                if (idFicha > maiorIdFicha)
                    maiorIdFicha = idFicha;

                std::string nomeFicha;
                std::getline(ss, nomeFicha, ';');
                Ficha *novaFicha = new Ficha(idFicha, nomeFicha);

                std::getline(ss, segmento, ';');
                int qtdExercicios = std::stoi(segmento);
                for (int i = 0; i < qtdExercicios; i++)
                {
                    if (std::getline(ss, segmento, ';'))
                    {
                        int idEx = std::stoi(segmento);
                        Exercicio *ex = buscarExercicioPorId(idEx);
                        if (ex != nullptr)
                        {
                            novaFicha->adicionarExercicio(ex);
                        }
                    }
                }
                fichas.push_back(novaFicha);
            }
            catch (...)
            {
                continue;
            }
        }
        arqFichas.close();
        Ficha::atualizarProximoId(maiorIdFicha);
    }
    historico.carregarDeArquivo();
}

// Salvar dados nos arquivos
void Sistema::salvarDados()
{
    std::cout << "Salvando dados no disco...\n";
    std::ofstream arqEx("exercicios.txt");
    if (arqEx.is_open())
    {
        for (const auto *ex : exercicios)
        {
            if (ex == nullptr)
                continue;

            int tipo = ex->getTipo();
            arqEx << tipo << ";"
                  << ex->getId() << ";"
                  << ex->getNome() << ";";

            if (tipo == 1)
            {
                const Cardio *card = dynamic_cast<const Cardio *>(ex);
                if (card)
                {
                    arqEx << card->getDuracao() << ";"
                          << card->getCaloriasPorMinuto() << ";"
                          << (card->isAtivo() ? 1 : 0) << "\n";
                }
            }
            else if (tipo == 2)
            {
                const Forca *forc = dynamic_cast<const Forca *>(ex);
                if (forc)
                {
                    arqEx << forc->getCarga() << ";"
                          << forc->getSeries() << ";"
                          << forc->getRepeticoes() << ";"
                          << forc->getTempoDescanso() << ";"
                          << (forc->isAtivo() ? 1 : 0) << "\n";
                }
            }
        }
        arqEx.close();
    }
    else
    {
        std::cerr << "Erro ao salvar exercicios.txt\n";
    }
    std::ofstream arqFichas("fichas.txt");
    if (arqFichas.is_open())
    {
        for (const auto *ficha : fichas)
        {
            if (ficha == nullptr)
                continue;
            arqFichas << ficha->getId() << ";"
                      << ficha->getNome() << ";";

            const std::vector<Exercicio *> &listaEx = ficha->getExercicios();

            arqFichas << listaEx.size();
            for (const auto *ex : listaEx)
            {
                if (ex != nullptr)
                {
                    arqFichas << ";" << ex->getId();
                }
            }
            arqFichas << "\n";
        }
        arqFichas.close();
    }
    else
    {
        std::cerr << "Erro ao salvar fichas.txt\n";
    }
    historico.salvarEmArquivo();
}

// Buscar exercício por ID
Exercicio *Sistema::buscarExercicioPorId(int id)
{
    for (auto *ex : exercicios)
    {
        if (ex->getId() == id)
        {
            return ex;
        }
    }
    return nullptr;
}

// Buscar ficha por ID
Ficha *Sistema::buscarFichaPorId(int id)
{
    for (auto *f : fichas)
    {
        if (f->getId() == id)
        {
            return f;
        }
    }
    return nullptr;
}

// Cadastrar novo exercício
void Sistema::cadastrarExercicio()
{
    int tipo;
    std::cout << "--- CADASTRAR NOVO EXERCICIO ---" << std::endl;
    std::cout << "1. Cardio\n";
    std::cout << "2. Força\n";

    std::cout << "Escolha o tipo: ";
    std::cin >> tipo;

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::string nome;
    std::cout << "Nome do exercicio: ";
    std::getline(std::cin, nome);

    if (tipo == 1)
    {
        int duracao;
        double calorias;

        std::cout << "Duracao (em minutos): ";
        std::cin >> duracao;

        std::cout << "Calorias por minuto: ";
        std::cin >> calorias;

        Cardio *novoCardio = new Cardio(nome, duracao, calorias);

        this->exercicios.push_back(novoCardio);

        std::cout << "Exercicio de Cardio cadastrado com sucesso! (ID: " << novoCardio->getId() << ")\n";
        pausar();
    }
    else if (tipo == 2)
    {
        double carga;
        int series, repeticoes, tempoDescanso;

        std::cout << "Carga: ";
        std::cin >> carga;

        std::cout << "Series: ";
        std::cin >> series;

        std::cout << "Repetições: ";
        std::cin >> repeticoes;

        std::cout << "Tempo de descanso: ";
        std::cin >> tempoDescanso;

        Forca *novoForca = new Forca(nome, carga, series, repeticoes, tempoDescanso);

        this->exercicios.push_back(novoForca);

        std::cout << "Exercicio de Força cadastrado com sucesso! (ID: " << novoForca->getId() << ")\n";
        pausar();
    }
    else
    {
        std::cout << "Tipo invalido!\n";
        pausar();
    }
}

// Listar exercícios ativos
void Sistema::listarExercicios()
{
    std::cout << "\n--- LISTA DE EXERCICIOS DISPONIVEIS ---\n";

    if (exercicios.empty())
    {
        std::cout << "Nenhum exercicio cadastrado no sistema.\n";
        pausar();
        return;
    }

    bool encontrouAtivo = false;

    for (const auto &ex : exercicios)
    {
        if (ex != nullptr)
        {
            if (ex->isAtivo())
            {
                std::cout << "------------------------------------------------\n";
                std::cout << "ID: " << ex->getId()
                          << " | Nome: " << ex->getNome()
                          << " | Tipo: " << (ex->getTipo() == 1 ? "Cardio" : "Forca")
                          << "\n";
                ex->exibirDetalhes();

                encontrouAtivo = true;
            }
        }
    }

    if (!encontrouAtivo)
    {
        std::cout << "Nenhum exercicio ativo encontrado.\n";
    }

    std::cout << "------------------------------------------------\n";
    pausar();
}

// Desativar exercício
void Sistema::excluirExercicio()
{
    std::cout << "\n--- EXCLUIR EXERCICIO (Desativar) ---\n";

    int id;
    std::cout << "Digite o ID do exercicio: ";
    if (!(std::cin >> id))
    {
        std::cout << "Entrada invalida!\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        pausar();
        return;
    }

    bool encontrado = false;

    for (auto *ex : exercicios)
    {
        if (ex->getId() == id)
        {
            if (!ex->isAtivo())
            {
                std::cout << "Este exercicio ja esta inativo.\n";
                encontrado = true;
                break;
            }

            ex->setAtivo(false);

            std::cout << "Exercicio '" << ex->getNome() << "' foi desativado com sucesso!\n";
            std::cout << "Ele nao aparecera mais em novas listagens, mas permanece nos historicos antigos.\n";

            encontrado = true;
            break;
        }
    }

    if (!encontrado)
    {
        std::cout << "Exercicio com ID " << id << " nao encontrado.\n";
    }

    pausar();
}

// Criar nova ficha
void Sistema::criarFicha()
{
    std::cout << "\n--- CRIAR NOVA FICHA ---\n";

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::string nome;
    std::cout << "Nome da ficha (ex: Treino A - Peito): ";
    std::getline(std::cin, nome);

    Ficha *novaFicha = new Ficha(nome);

    this->fichas.push_back(novaFicha);

    std::cout << "Ficha '" << novaFicha->getNome() << "' criada com sucesso! (ID: " << novaFicha->getId() << ")\n";

    pausar();
}

// Adicionar exercício à ficha
void Sistema::adicionarExercicioFicha()
{
    std::cout << "\n--- ADICIONAR EXERCICIO NA FICHA ---\n";

    int idFicha;
    std::cout << "Digite o ID da Ficha de destino: ";
    if (!(std::cin >> idFicha))
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "ID invalido.\n";
        pausar();
        return;
    }

    Ficha *ficha = buscarFichaPorId(idFicha);

    if (ficha == nullptr)
    {
        std::cout << "Ficha nao encontrada!\n";
        pausar();
        return;
    }

    std::cout << "Ficha selecionada: " << ficha->getNome() << "\n";

    int idExercicio;
    std::cout << "Digite o ID do Exercicio a adicionar: ";
    if (!(std::cin >> idExercicio))
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "ID invalido.\n";
        pausar();
        return;
    }

    Exercicio *ex = buscarExercicioPorId(idExercicio);

    if (ex == nullptr)
    {
        std::cout << "Exercicio nao encontrado!\n";
        pausar();
        return;
    }

    if (!ex->isAtivo())
    {
        std::cout << "Este exercicio esta inativo e nao pode ser adicionado.\n";
        pausar();
        return;
    }

    ficha->adicionarExercicio(ex);

    std::cout << "Exercicio '" << ex->getNome() << "' adicionado a ficha '" << ficha->getNome() << "' com sucesso!\n";
    pausar();
}

// Listar todas as fichas
void Sistema::listarFichas()
{
    std::cout << "\n--- LISTA DE FICHAS DE TREINO ---\n";

    if (fichas.empty())
    {
        std::cout << "Nenhuma ficha cadastrada no sistema.\n";
    }
    else
    {
        for (const auto *ficha : fichas)
        {
            if (ficha != nullptr)
            {
                ficha->exibirFicha();
                std::cout << std::endl;
            }
        }
    }

    pausar();
}

// Registrar treino realizado
void Sistema::registrarTreino()
{
    std::cout << "\n--- REGISTRAR EXECUCAO DE TREINO ---\n";
    int idFicha;
    std::cout << "Digite o ID da Ficha realizada: ";

    if (!(std::cin >> idFicha))
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Entrada invalida.\n";
        pausar();
        return;
    }

    // 2. Busca a ficha
    Ficha *ficha = buscarFichaPorId(idFicha);

    if (ficha == nullptr)
    {
        std::cout << "Ficha nao encontrada!\n";
        pausar();
        return;
    }

    std::string dataHoraAtual = getDataHoraAtual();

    // 4. Calcula os totais do momento
    // Importante calcular agora, pois a ficha pode mudar no futuro
    double tempoTotal = ficha->calcularTempoTotal();
    double caloriasTotal = ficha->calcularCaloriasTotais();

    // 5. Cria o registro
    RegistroTreino registro;
    registro.dataHora = dataHoraAtual;
    registro.idFicha = ficha->getId();
    registro.nomeFicha = ficha->getNome();
    registro.tempoTotal = tempoTotal;
    registro.caloriasTotal = caloriasTotal;

    // 6. Envia para o histórico
    historico.adicionarRegistro(registro);

    std::cout << "Treino registrado com sucesso!\n";
    std::cout << "Data: " << dataHoraAtual << "\n";
    std::cout << "Tempo: " << tempoTotal << " min | Calorias: " << caloriasTotal << " kcal\n";

    pausar();
}

// Exibir histórico de treinos
void Sistema::exibirHistorico()
{
    historico.exibirHistorico();

    pausar();
}