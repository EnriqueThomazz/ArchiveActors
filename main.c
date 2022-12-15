#include <stdio.h>
#include <stdlib.h>
#include "filmes.h"

int main(void){
    //Criando o arquivo de id, se não existir
    FILE* arq_id = fopen("./config.bin", "rb");
    if(!arq_id){
        printf("Arquivos de id's nao existe, criando-o...\n");
        arq_id = fopen("./config.bin", "wb");
        int x = 0;
        fwrite(&x, sizeof(int), 1, arq_id);
        fwrite(&x, sizeof(int), 1, arq_id);
    }
    fclose(arq_id);


    int running = 1;
    int id_ator, id_filme;
    while(running){
        int opt;
        printf("\n\nO que voce deseja fazer?\n");
        printf("1 - Cadastrar um ator\n2 - Cadastrar um filme\n3 - Cadastrar uma relacao\n4 - Listar todos os atores\n5 - Listar todos os filmes\n");
        printf("6 - Buscar se um ator participou em um filme\n7 - Alterar um ator\n8 - Alterar um filme\n9 - Alterar uma relacao\n");
        printf("10 - Listar todos os filmes que um ator fez\n11 - Listar todos os atores de um filme\n0 - Sair\n");

        scanf("%d", &opt);

        switch(opt){
            case 1:
                recebe_ator() ? printf("Ator incluido com sucesso.") : printf("Erro na inclusao do ator.");
                break;

            case 2:
                recebe_filme() ? printf("Filme incluido com sucesso.") : printf("Erro na inclusao do filme.");
                break;

            case 3:
                recebe_relacao() ? printf("Relacao incluida com sucesso.") : printf("Erro na inclusao da relacao.");
                break;

            case 4:
                imprime_atores();
                break;

            case 5:
                imprime_filmes();
                break;

            case 6:                
                printf("Digite o id do ator e o id do filme, separados por ponto e virgula: ");
                scanf("%d;%d", &id_ator, &id_filme);

                TAtor* ator = busca_ator(id_ator);
                TFilme* filme = busca_filme(id_filme);
                if(ator && filme){
                    TAtuou* relacao = busca_relacao(id_ator, id_filme);
                    if(!relacao) printf("O ator %s nao atuou em %s.\n", ator->nome, filme->titulo);
                    else{
                        printf("O ator %s atuou em %s: ele(a) fez a personagem %s e recebeu %.2f milhoes.\n", ator->nome, filme->titulo, relacao->personagem, relacao->salario_milhoes);
                    }
                }

                break;

            case 7:
                printf("Digite o id do ator: ");
                scanf("%d", &id_ator);

                altera_ator(id_ator) ? printf("Alteracoes feitas com sucesso") : printf("Erro na alteracao do ator");

                break;

            case 8:
                printf("Digite o id do filme: ");
                scanf("%d", &id_filme);

                altera_filme(id_filme) ? printf("Alteracoes feitas com sucesso") : printf("Erro na alteracao do filme");

                break;

            case 9:
                printf("Digite o id do filme e do ator, separados por espaco: ");
                scanf("%d %d", &id_filme, &id_ator);

                altera_relacao(id_ator, id_filme) ? printf("Alteracoes feitas com sucesso") : printf("Erro na alteracao da relacao");

                break;

            case 10:
                printf("Digite o id do ator: ");
                scanf("%d", &id_ator);
                busca_filmes_ator(id_ator);

                break;

            case 11:
                printf("Digite o id do filme: ");
                scanf("%d", &id_filme);
                busca_atores_filme(id_filme);

                break;

            case 0:
                running = 0;
                break;

            default:
                printf("Digite uma opcao valida!");
                break;
        }
    }

    return 0;
}