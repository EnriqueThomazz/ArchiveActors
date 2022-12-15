#include <stdio.h>
#include <stdlib.h>

typedef struct ator{
    int id;

    char nome[30];
    char genero;
    int idade;
    float altura;
}TAtor;

typedef struct filme{
    int id;

    char titulo[30];
    int ano_lancamento;
    float bilheteria_milhoes;
}TFilme;

typedef struct atuou{
    int id_filme;
    int id_ator;

    char personagem[30];
    float salario_milhoes;

}TAtuou;

TAtor* busca_ator(int id);

TFilme* busca_filme(int id);

TAtuou* busca_relacao(int id_ator, int id_filme);

int recebe_ator();

int recebe_filme();

int recebe_relacao();

void imprime_atores();

void imprime_filmes();

int altera_ator(int id_ator);

int altera_filme(int id_filme);

int altera_relacao(int id_ator, int id_filme);

void busca_filmes_ator(int id_ator);

void busca_atores_filme(int id_filme);