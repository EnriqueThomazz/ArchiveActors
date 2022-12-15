#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "filmes.h"

/*Variaveis globais com os paths de cada arquivo binario*/
char ID_PATH[] = {"./config.bin"};
char ATOR_PATH[] = {"./ator.bin"};
char FILME_PATH[] = {"./filme.bin"};
char RELACAO_PATH[] = {"./relacoes.bin"};

/* Usado para retornar e incrementar um valor que serve de id de autoincremento.
Existem dois no arquivo config: um para os atores(index 0) e um para os filmes(index 1)*/
int le_id_atual(int index){
    FILE* arq_id = fopen(ID_PATH, "rb+");

    if (fseek(arq_id, index * sizeof(int), SEEK_SET) != 0) return -1;

    int id;
    fread(&id, sizeof(int), 1, arq_id);

    fseek(arq_id, index*sizeof(int), SEEK_SET);
    int novo_id = id + 1;
    fwrite(&novo_id, sizeof(int), 1, arq_id);

    fclose(arq_id);
    
    return id;
}

/*Recebe os parametros para um ator e aloca espaço na memoria para o mesmo*/
TAtor* cria_ator(char nome[], char genero, int idade, float altura){
    int id_ator = le_id_atual(0);

    TAtor* novo = (TAtor*) malloc(sizeof(TAtor));

    novo->id = id_ator;
    strcpy(novo->nome, nome);
    novo->genero = genero;
    novo->idade = idade;
    novo->altura = altura;

    return novo;
}

/*Recebe os parametros para um filme e aloca espaco na memoria para o mesmo*/
TFilme* cria_filme(char titulo[], int ano_lancamento, int bilheteria_milhoes){
    int id_filme = le_id_atual(1);

    TFilme* novo = (TFilme*) malloc(sizeof(TFilme));

    novo->id = id_filme;
    strcpy(novo->titulo, titulo);
    novo->ano_lancamento = ano_lancamento;
    novo->bilheteria_milhoes = bilheteria_milhoes;

    return novo;
}

/*Recebe os parametros para uma relacao e aloca espaco na memoria para a mesma*/
TAtuou* cria_relacao(TFilme* filme, TAtor* ator, char personagem[], float salario){
    TAtuou* novo = (TAtuou*) malloc(sizeof(TAtuou));

    novo->id_filme = filme->id;
    novo->id_ator = ator->id;
    strcpy(novo->personagem, personagem);
    novo->salario_milhoes = salario;

    return novo;
}

/*Recebe um id para um ator e busca ele no arquivo binário. Retorna NULL caso não encontre*/
TAtor* busca_ator(int id){
    FILE* arq = fopen(ATOR_PATH, "rb");
    if(!arq) return NULL;

    TAtor* ator = (TAtor*) malloc(sizeof(TAtor));

    while(1){
        fread(&ator->id, sizeof(int), 1, arq);
        fread(ator->nome, sizeof(char), sizeof(ator->nome), arq);
        fread(&ator->genero, sizeof(char), 1, arq);
        fread(&ator->idade, sizeof(int), 1, arq);
        fread(&ator->altura, sizeof(float), 1, arq);

        if(ator->id == id) break;

        if(feof(arq)){
            ator = NULL;
            break;
        };         
    }

    fclose(arq);
    return ator;
}

/*Recebe um id para um filme e busca ele no arquivo binário. Retorna NULL caso não encontre*/
TFilme* busca_filme(int id){
    FILE* arq = fopen(FILME_PATH, "rb");
    if(!arq) return NULL;

    TFilme* filme = (TFilme*) malloc(sizeof(TFilme));

    while(1){
        fread(&filme->id, sizeof(int), 1, arq);
        fread(filme->titulo, sizeof(char), sizeof(filme->titulo), arq);
        fread(&filme->ano_lancamento, sizeof(int), 1, arq);
        fread(&filme->bilheteria_milhoes, sizeof(float), 1, arq);

        if(filme->id == id) break;
        
        if(feof(arq)){
            filme = NULL;
            break;
        }   
    }

    fclose(arq);
    return filme;    
}

/*Recebe um id para uma relacao e busca ela no arquivo binário. Retorna NULL caso não encontre*/
TAtuou* busca_relacao(int id_ator, int id_filme){
    FILE* arq = fopen(RELACAO_PATH, "rb");
    if(!arq) return NULL;

    TAtuou* relacao = (TAtuou*) malloc(sizeof(TAtuou));

    while(1){
        fread(&relacao->id_filme, sizeof(int), 1, arq);
        fread(&relacao->id_ator, sizeof(int), 1, arq);
        fread(relacao->personagem, sizeof(char), sizeof(relacao->personagem), arq);
        fread(&relacao->salario_milhoes, sizeof(float), 1, arq);

        if(relacao->id_filme == id_filme && relacao->id_ator == id_ator) break;
        
        if(feof(arq)){
            relacao = NULL;
            break;
        }   
    }

    fclose(arq);
    return relacao;
}

/*Recebe um ator e executa sua "serializacao" em disco*/
int salva_ator(TAtor* ator){
    FILE* arq = fopen(ATOR_PATH, "ab");
    if(!arq){
        printf("Erro na abertura do arquivo de atores.");
        return 0;
    }
    // "Serializando o objeto", a ordem é importante
    fwrite(&ator->id, sizeof(int), 1, arq);
    fwrite(ator->nome, sizeof(char), sizeof(ator->nome), arq);
    fwrite(&ator->genero, sizeof(char), 1, arq);
    fwrite(&ator->idade, sizeof(int), 1, arq);
    fwrite(&ator->altura, sizeof(float), 1, arq);

    fclose(arq);

    return 1;
}

/*Recebe um filme e executa sua "serializacao" em disco*/
int salva_filme(TFilme* filme){
    FILE* arq = fopen(FILME_PATH, "ab");
    if(!arq){
        printf("Erro na abertura do arquivo de filmes.");
        return 0;
    }
    // "Serializando o objeto", a ordem é importante
    fwrite(&filme->id, sizeof(int), 1, arq);
    fwrite(filme->titulo, sizeof(char), sizeof(filme->titulo), arq);
    fwrite(&filme->ano_lancamento, sizeof(int), 1, arq);
    fwrite(&filme->bilheteria_milhoes, sizeof(float), 1, arq);

    fclose(arq);

    return 1;
}

/*Recebe uma relacao e executa sua "serializacao" em disco*/
int salva_relacao(TAtuou* relacao){
    FILE* arq = fopen(RELACAO_PATH, "ab");
    if(!arq){
        printf("Erro na abertura do arquivo de relacoes.");
        return -1;
    }
    // "Serializando o objeto", a ordem é importante
    fwrite(&relacao->id_filme, sizeof(int), 1, arq);
    fwrite(&relacao->id_ator, sizeof(int), 1, arq);
    fwrite(relacao->personagem, sizeof(char), sizeof(relacao->personagem), arq);
    fwrite(&relacao->salario_milhoes, sizeof(float), 1, arq);

    fclose(arq);

    return 1;
}

/*Recebe os inputs do usuário para a criacao de um novo ator. O ator é alocado e salvo em disco.*/
int recebe_ator(){
    char nome[30];
    char genero;
    int idade;
    float altura;

    printf("Digite o nome, idade, genero e altura do ator, separados por ponto e virgula: ");
    scanf("\n%[^;];%d;%c;%f", nome, &idade, &genero, &altura);

    TAtor* ator = cria_ator(nome, genero, idade, altura);

    return salva_ator(ator);
}

/*Exibe na console todos os atores cadastrados.*/
void imprime_atores(){
    FILE* arq = fopen(ATOR_PATH, "rb");
    TAtor* ator = (TAtor*) malloc(sizeof(TAtor));

    while(1){
        fread(&ator->id, sizeof(int), 1, arq);
        fread(ator->nome, sizeof(char), sizeof(ator->nome), arq);
        fread(&ator->genero, sizeof(char), 1, arq);
        fread(&ator->idade, sizeof(int), 1, arq);
        fread(&ator->altura, sizeof(float), 1, arq);

        if(feof(arq)) break; // O loop tava rodando 1x mais do q o necessario, coloquei isso e resolveu

        printf("\n====================================================================\n");
        printf("ID: %d, Nome: %s, Genero: %c, Idade: %d, Altura: %.2f", ator->id, ator->nome, ator->genero, ator->idade, ator->altura);           
    }

    free(ator);
    fclose(arq);
}

/*Recebe os inputs do usuário para a criacao de um novo filme. O filme é alocado e salvo em disco.*/
int recebe_filme(){
    char titulo[30];
    int ano_lancamento;
    float bilheteria_milhoes;

    printf("Digite o titulo, ano de lancamento e bilheteria(em milhoes) do filme, separados por ponto e virgula: ");
    scanf("\n%[^;];%d;%f", titulo, &ano_lancamento, &bilheteria_milhoes);

    TFilme* filme = cria_filme(titulo, ano_lancamento, bilheteria_milhoes);

    return salva_filme(filme);
}

/*Exibe na console todos os filmes cadastrados.*/
void imprime_filmes(){
    FILE* arq = fopen(FILME_PATH, "rb");
    TFilme* filme = (TFilme*) malloc(sizeof(TFilme));

    while(1){
        fread(&filme->id, sizeof(int), 1, arq);
        fread(filme->titulo, sizeof(char), sizeof(filme->titulo), arq);
        fread(&filme->ano_lancamento, sizeof(int), 1, arq);
        fread(&filme->bilheteria_milhoes, sizeof(float), 1, arq);
        
        if(feof(arq)) break; // O loop tava rodando 1x mais do q o necessario, coloquei isso e resolveu

        printf("\n====================================================================\n");
        printf("ID: %d, Titulo: %s, Ano de Lancamento: %d, Bilheteria(milhoes): %.2f", filme->id, filme->titulo, filme->ano_lancamento, filme->bilheteria_milhoes);           
    }

    free(filme);
    fclose(arq);
}

/*Recebe os inputs do usuário para a criacao de uma nova relacao. A funcao busca o ator e o filme para garantir que ambos existem.
A relacao é alocada e salva em disco.*/
int recebe_relacao(){
    int id_filme;
    int id_ator;

    printf("Digite o id do filme e o id do ator, separados por ponto e virgula: ");
    scanf("%d;%d", &id_filme, &id_ator);

    TAtor* ator = busca_ator(id_ator);
    if(!ator) return -1;

    TFilme* filme = busca_filme(id_filme);
    if(!filme) return 0;

    printf("\nConfirma que %s atuou em %s? (S=1/N=0)", ator->nome, filme->titulo);
    int opt;
    scanf("%d", &opt);

    if(opt == 1){
        char personagem[30];
        float salario_milhoes;

        printf("\nDigite o nome do personagem e o salario(em milhoes), separados por ponto e virgula: ");
        scanf("\n%[^;];%f", personagem, &salario_milhoes);

        TAtuou* relacao = cria_relacao(filme, ator, personagem, salario_milhoes);

        return salva_relacao(relacao);        
    }

    return 0;
}

/*Recebe um id de ator e realiza a busca para fazer a alteracao de um dos campos (impossivel alterar id)*/
int altera_ator(int id_ator){
    FILE* arq = fopen(ATOR_PATH, "rb+");
    if(!arq) return 0;

    TAtor* ator = (TAtor*) malloc(sizeof(TAtor));

    while(1){
        fread(&ator->id, sizeof(int), 1, arq);
        fread(ator->nome, sizeof(char), sizeof(ator->nome), arq);
        fread(&ator->genero, sizeof(char), 1, arq);
        fread(&ator->idade, sizeof(int), 1, arq);
        fread(&ator->altura, sizeof(float), 1, arq);

        if(ator->id == id_ator) break;

        if(feof(arq)){
            ator = NULL;
            break;
        };         
    }

    if(!ator) return 0;

    printf("ID: %d, Nome: %s, Genero: %c, Idade: %d, Altura: %.2f\n", ator->id, ator->nome, ator->genero, ator->idade, ator->altura);
    printf("O que voce gostaria de modificar?");

    char opcao[30];
    scanf("%s", opcao);

    for (int i = 0; opcao[i] != '\0'; i++) { // Convertendo pra lowercase, pra poder comparar as strings
        opcao[i] = tolower(opcao[i]);
    }

    // Verificando a escolha
    if(strcmp(opcao, "nome") == 0){
        printf("Digite o novo nome: ");
        char novo_nome[30];
        scanf("%s", novo_nome);

        strcpy(ator->nome, novo_nome);   

    }else if(strcmp(opcao, "genero") == 0){
        printf("Digite o novo genero: ");
        char novo_genero;
        scanf("%c", &novo_genero);

        ator->genero = novo_genero;

    }else if(strcmp(opcao, "idade") == 0){
        printf("Digite a nova idade: ");
        int nova_idade;
        scanf("%d", &nova_idade);

        ator->idade = nova_idade;
        
    }else if(strcmp(opcao, "altura") == 0){
        printf("Digite a nova altura: ");
        float nova_altura;
        scanf("%f", &nova_altura);

        ator->altura = nova_altura;
    }else{
        printf("Escolha inválida.");
        fclose(arq);
        return 0;
    }

    fseek(arq, ftell(arq) - sizeof(TAtor) + 1, SEEK_SET); // Tive que hardcodar o +1 pq o sizeof(TAtor) n tava retornando certo

    fwrite(&ator->id, sizeof(int), 1, arq);
    fwrite(ator->nome, sizeof(char), sizeof(ator->nome), arq);
    fwrite(&ator->genero, sizeof(char), 1, arq);
    fwrite(&ator->idade, sizeof(int), 1, arq);
    fwrite(&ator->altura, sizeof(float), 1, arq);

    fclose(arq);
    return 1;
}

/*Recebe um id de filme e realiza a busca para fazer a alteracao de um dos campos (impossivel alterar id)*/
int altera_filme(int id_filme){
    FILE* arq = fopen(FILME_PATH, "rb+");
    if(!arq) return 0;

    TFilme* filme = (TFilme*) malloc(sizeof(TFilme));

    while(1){
        fread(&filme->id, sizeof(int), 1, arq);
        fread(filme->titulo, sizeof(char), sizeof(filme->titulo), arq);
        fread(&filme->ano_lancamento, sizeof(int), 1, arq);
        fread(&filme->bilheteria_milhoes, sizeof(float), 1, arq);

        if(filme->id == id_filme) break;
        
        if(feof(arq)){
            filme = NULL;
            break;
        }   
    }

    if(!filme) return 0;

    printf("ID: %d, Titulo: %s, Ano de Lancamento(escreva apenas 'ano'): %d, Bilheteria: %.2f\n", filme->id, filme->titulo, filme->ano_lancamento, filme->bilheteria_milhoes);
    printf("O que voce gostaria de modificar?");

    char opcao[30];
    scanf("%s", opcao);

    for (int i = 0; opcao[i] != '\0'; i++) { // Convertendo pra lowercase, pra poder comparar as strings
        opcao[i] = tolower(opcao[i]);
    }

    if(strcmp(opcao, "titulo") == 0){
        printf("Digite o novo titulo: ");
        char novo_titulo[30];
        scanf("%s", novo_titulo);

        strcpy(filme->titulo, novo_titulo);  

    }else if(strcmp(opcao, "ano") == 0){
        printf("Digite o novo ano de lancamento: ");
        int novo_ano;
        scanf("%d", &novo_ano);

        filme->ano_lancamento = novo_ano;

    }else if(strcmp(opcao, "bilheteria") == 0){
        printf("Digite a nova bilheteria: ");
        float nova_bilheteria;
        scanf("%f", &nova_bilheteria);

        filme->bilheteria_milhoes = nova_bilheteria;

    }else{
        printf("Escolha inválida.");
        fclose(arq);
        return 0;
    }
;
    fseek(arq, ftell(arq) - sizeof(TFilme) + 2, SEEK_SET); // Tive que hardcodar o +2 pq o sizeof(TFilme) n tava retornando certo

    fwrite(&filme->id, sizeof(int), 1, arq);
    fwrite(filme->titulo, sizeof(char), sizeof(filme->titulo), arq);
    fwrite(&filme->ano_lancamento, sizeof(int), 1, arq);
    fwrite(&filme->bilheteria_milhoes, sizeof(float), 1, arq);

    fclose(arq);

    return 1;
}

/*Recebe um id de ator e um de filme e realiza a busca para fazer a alteracao de um dos campos.
Ao alterar um id, a função verifica se existe um filme ou ator com aquele novo id*/
int altera_relacao(int id_ator, int id_filme){
    FILE* arq = fopen(RELACAO_PATH, "rb+");
    if(!arq) return 0;

    TAtuou* relacao = (TAtuou*) malloc(sizeof(TAtuou));

    while(1){
        fread(&relacao->id_filme, sizeof(int), 1, arq);
        fread(&relacao->id_ator, sizeof(int), 1, arq);
        fread(relacao->personagem, sizeof(char), sizeof(relacao->personagem), arq);
        fread(&relacao->salario_milhoes, sizeof(float), 1, arq);

        if(relacao->id_filme == id_filme && relacao->id_ator == id_ator) break;
        
        if(feof(arq)){
            relacao = NULL;
            break;
        }   
    }

    if(!relacao) return 0;

    printf("Ator: %d, Filme: %d, Personagem: %s, Salario: %.2f\n", relacao->id_ator, relacao->id_filme, relacao->personagem, relacao->salario_milhoes);
    printf("O que voce gostaria de modificar?");

    char opcao[30];
    scanf("%s", opcao);

    for (int i = 0; opcao[i] != '\0'; i++) { // Convertendo pra lowercase, pra poder comparar as strings
        opcao[i] = tolower(opcao[i]);
    }

    if(strcmp(opcao, "ator") == 0){
        printf("Digite o id do novo ator: ");
        int novo_ator;
        scanf("%d", &novo_ator);

        if(!busca_ator(novo_ator)){
            printf("Ator nao encontrado.");
            fclose(arq);
            return 0;
        }

        relacao->id_ator = novo_ator;

    }else if(strcmp(opcao, "filme") == 0){
        printf("Digite o id do novo filme: ");
        int novo_filme;
        scanf("%d", &novo_filme);

        if(!busca_filme(novo_filme)){
            printf("Filme nao encontrado.");
            fclose(arq);
            return 0;
        }

        relacao->id_filme = novo_filme;

    }else if(strcmp(opcao, "personagem") == 0){
        printf("Digite o novo personagem: ");
        char novo_personagem[30];
        scanf("%s", &novo_personagem);

        strcpy(relacao->personagem, novo_personagem); 
        
    }else if(strcmp(opcao, "salario") == 0){
        printf("Digite o novo salario: ");
        int novo_salario;
        scanf("%d", &novo_salario);

        relacao->salario_milhoes = novo_salario;

    }else{
        printf("Escolha inválida.");
        fclose(arq);
        return 0;
    }

    fseek(arq, ftell(arq) - sizeof(TAtuou) + 2, SEEK_SET); // Tive que hardcodar o +2 pq o sizeof(TAtuou) n tava retornando certo

    fwrite(&relacao->id_filme, sizeof(int), 1, arq);
    fwrite(&relacao->id_ator, sizeof(int), 1, arq);
    fwrite(relacao->personagem, sizeof(char), sizeof(relacao->personagem), arq);
    fwrite(&relacao->salario_milhoes, sizeof(float), 1, arq);

    fclose(arq);

    return 1;
}

/*Caminha pelas relacoes salvas em disco e imprime na console todas aquelas que tiverem o id do ator desejado*/
void busca_filmes_ator(int id_ator){
    TAtor* ator = busca_ator(id_ator);
    if(!ator) return;

    printf("O(a) ator(a) %s ja atuou nos seguintes filmes: \n\n", ator->nome);

    FILE* arq = fopen(RELACAO_PATH, "rb");
    if(!arq) return;

    TAtuou* relacao = (TAtuou*) malloc(sizeof(TAtuou));

    while(1){
        fread(&relacao->id_filme, sizeof(int), 1, arq);
        fread(&relacao->id_ator, sizeof(int), 1, arq);
        fread(relacao->personagem, sizeof(char), sizeof(relacao->personagem), arq);
        fread(&relacao->salario_milhoes, sizeof(float), 1, arq);

        if(feof(arq)){
            relacao = NULL;
            break;
        }   

        if(relacao->id_ator == id_ator){
            TFilme* filme = busca_filme(relacao->id_filme);
            printf("%s como %s\n", filme->titulo, relacao->personagem);            
        }
        
    }

    fclose(arq);
}

/*Caminha pelas relacoes salvas em disco e imprime na console todas aquelas que tiverem o id do filme desejado*/
void busca_atores_filme(int id_filme){
    TFilme* filme = busca_filme(id_filme);
    if(!filme) return;

    printf("O filme %s tem o seguinte elenco: \n\n", filme->titulo);

    FILE* arq = fopen(RELACAO_PATH, "rb");
    if(!arq) return;

    TAtuou* relacao = (TAtuou*) malloc(sizeof(TAtuou));

    while(1){
        fread(&relacao->id_filme, sizeof(int), 1, arq);
        fread(&relacao->id_ator, sizeof(int), 1, arq);
        fread(relacao->personagem, sizeof(char), sizeof(relacao->personagem), arq);
        fread(&relacao->salario_milhoes, sizeof(float), 1, arq);
        
        if(feof(arq)){
            relacao = NULL;
            break;
        }   

        if(relacao->id_filme == id_filme){
            TAtor* ator = busca_ator(relacao->id_ator);
            printf("%s como %s\n", ator->nome, relacao->personagem);            
        }
    }

    fclose(arq);
}