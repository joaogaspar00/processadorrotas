/******************************************************************************
* Data: 21 de março de 2021     Ano letivo: 2020/2021   LEE - IST *
*																			  
* Projeto: 1º Trabalho de programação 	Professor: Bertinho D'Andrade         
* 																			  
* Autor: João Pedro Sousa Gaspar											  
*																			  
* Âmbito: A resolução do trabalho foi feita com o objetivo de ajudar os meus  
* 	mentorandos na intrepretação e desenvolvimento do seu programa, sem   
* 	mostrar a codificação.												  
*																			  
* Descrição: Desenvolver um program em C, norma ansi, com a finalidade de     
*   calcular distâncias de rotas. As rotas são definidas através de uma   
* 	sequência de nomes de localidades. Todas as possíveis localidades 	  
* 	estão guardadas num ficheiro de texto (base de dados) cujo nome é 	  
* 	localidades.txt. As rotas são especificadas num ficheiro de texto     
* 	(rotas.txt) que o programa deverá processar. 						  
*																		  
*******************************************************************************
* VERSÕES:																	  
*	- 1º Versão: 21-03-2021 - programa a funcionar sem qualquer tipo de erros,
*	e a cumprir todos os paramêtros pedidos no trabalho                       
*******************************************************************************
* MELHORIAS POSSÍVEIS                                                         
*	- Melhoramento da estrutura de dados: definir uma lista duplamente ligada,
*	ordenada alfabeticamente para mais rápida procura.                        
*	Função: "LISTA * insercao_lista_localidades(..)"                          
*                                                                              
*	- Aprefeiçoamento do cálculo das distâncias                               
*******************************************************************************/

/**************************
*        INCLUDES         *
***************************/

#include <stdio.h>      /* Babeçalho da biblioteca padrão do C */
#include <stdlib.h>     /* Biblioteca de propósito geral padrão da linguagem de programação C */
#include <string.h>     /* Biblioteca com várias funções para manipulação de arrays com caracteres - "strings" */
#include <stdbool.h>    /* Definie um novo tipo de váriaval - "bool". Contém recursos para manipular variáveis lógicas, verdadeiro e falso*/
#include <math.h>       /* Biblioteca com funções matemáticas necessárias: funções seno e cosseno */

/**************************
*         DEFINES         *
***************************/

#define R 6371.0            /* Contante - Raio da terra */
#define PI 3.14159265359    /* Contante - Valor do pi */
#define DEBUG               /* Remover para não aparecem elementos de DEBBUG na linha de comendados*/

/**************************
*    ESTRUTURA DE DADOS   *
***************************/

typedef struct localidade_info{     /* Estrutura do dado utíl do problema */
    char nome[20];                  /* Guarda o nome da localidade */
    float longitude;                /* Guarda a longitude da localidade */
    float latitude;                 /* Guarda a latitude da localidade */
} LOCAL;                            /* Define um tipo de "variável" para o dado útil */

typedef struct lista_ligada{        /* Estrutura para lista */
    LOCAL localidade;               /* Dado útil */
    struct lista_ligada * proximo;  /* Pointer para o proximo elemento na lista - essencial para criação de lista simplesmente ligada */
} LISTA;                            /* Define um tipo de "variável" para a lista */

/**************************
*  PROTÓTIPOS DE FUNÇÕES  *
***************************/
void heading();
bool validacao_abertura_ficheiro(FILE * f_entrada, char nome_f[]);
void processamento_rota(LISTA * dados_uteis);
float processamento_distancia(LISTA * dados_uteis, char referencia[], char leitura[]);
float calculo_distancia(LISTA * dados_uteis, char referencia[], char leitura[]);
void conversao_radianos(float * angulo);
void encontrar_coordenadas(LISTA * dados_uteis, char cidade[], float * longitude, float * latitude);
void print_string(FILE * f, char leitura[]);
void retirar_paragrafo(char leitura[]);
LISTA * leitura_dados_uteis();
bool validacao_dadosuteis(int leituras, float longitude, float latitude, int linha);
LISTA * insercao_lista_localidades(LISTA * localidades_DADOS, char nome[], float longitude, float latitude);
LISTA * criacao_dado(char nome[], float longitude, float latitude);
void print_dados_uteis(LISTA * dados_uteis);

/**********************************************************************************
* Função: int main();                                                             *
**********************************************************************************/
int main(){

    LISTA * dados_uteis;                                /* Guadar a estrutura que vai dar suporte para leitura dos dados */

    #ifdef DEBUG
        heading();
    #endif

    dados_uteis = leitura_dados_uteis();                    /* Leitura dos dados do ficheiro "localidades.txt" */

    if(dados_uteis == NULL){
        exit(0);
    }
    #ifdef DEBUG
        print_dados_uteis(dados_uteis);                     /* Debbug: mostrar o dados lidos */
        printf("\n");
    #endif

    processamento_rota(dados_uteis);                        /* Função principal, invoca a função para processar as rotas */

    #ifdef DEBUG
        printf("\n\nPrimir uma tecla para sair...");        /* Para não fechar a linha de comandos automáticamentes */
        getchar();
    #endif

    printf("\n>> Exit application (0) - success\n");        /* Aviso de execução sem erros */
    return 0;
}
/**********************************************************************************/

/*********************************************
* Função: void heading()                     *
* Descrição: Cabeçalho                       *
*********************************************/
void heading(){
    printf("********************************************\n");
    printf("* Data: 21 de marco de 2021                *\n");
    printf("* Ano letivo: 2020/2021                    *\n");
    printf("* Curso: LEE - IST                         *\n");
    printf("* Projeto: 1o Trabalho de programacao      *\n");
    printf("* Professor: Bertinho D'Andrade            *\n");
    printf("* Autor: Joao Pedro Sousa Gaspar           *\n");
    printf("********************************************\n\n");
}
/*********************************************/

/***********************************************************************************
* Função: bool validacao_abertura_ficheiro(FILE * f_entrada, char nome_f[])        *
* Descrição: Recebe o ponteiro para o ficheiro e a string com o nome do ficheiro.  *
*       Verifica a nulidade ou a correta abertura do ficheiro.                     *
***********************************************************************************/
bool validacao_abertura_ficheiro(FILE * f_entrada, char nome_f[]){
    if(f_entrada == NULL){                                              /* Verifica a validade da abertura do ficheiro */
        #ifdef DEBUG
            printf(">> Abertura do ficheiro %s // DEBUG com erros\n", nome_f);
        #endif
        return false;                                                   /* Retorna "false" em caso de erro na abertura */
    }
    #ifdef DEBUG
        printf(">> Abertura correta do ficheiro %s\n", nome_f);
     #endif
    return true;                                                        /* Retorna "true" em caso positivo na abertura do ficheiro */
}
/**********************************************************************************/

/***********************************************************************************
* Função: void processamento_rota(LISTA * dados_uteis)                             *
* Descrição: Recebe a lista onde estão guardados os dados uteis do programa e      *
*       executa o processamento do calculo das distancias:                         *
*                                                                                  *
*       1º- Abertura dos ficheiro de entrada ("rotas.txt") e de sáida              *
*        ("rotas_processadas.txt")                                                 *
*                                                                                  *
*       2º- Leitura dos dados do ficheiro de entrada para processamenteo. Existem  *
*       três tipos de leitura diferentes:                                          *
*           1- "#ROTA X" deve-se ler e imprimir diretamente no ficheiro de saída   *
*           2- Uma linha em branco, deve-se tb imprimir a linha no ficheiro de     *
*           saída e em seguida repor as informações que indicam uma nova rota      *
*           3- Leitura de uma localidade. Aqui é um pouco mais sensivel. Deve-se   *
*           retirar o '\n', e seguidamente verificar se existe referência, caso    *
*           nao exista definir a leitura como referencia                           *
*                                                                                  *
*       3º- processar a distancia em função da leitura e do ponto de referencia    *
*       Adicionar ao total da rota (considera-se o "trajeto2 a distancia entre     *
*       dois pontos e "distancia" toda a distancia percorrida na rota)             *
*                                                                                  *
*       4º- Definir o novo ponto de referencia                                     *
************************************************************************************/
void processamento_rota(LISTA * dados_uteis){

    FILE * f_entrada, * f_saida;
    char nome_f1[] = "rotas.txt", nome_f2[] = "rotas_processadas.txt", leitura[30], referencia[30] = "\0";
    bool validacao = true;
    float distancia = 0, trajeto = 0;

    #ifdef DEBUG
        printf("********************************************\n");
        printf("* Segundo passo - processamento das rotas  *\n");
        printf("********************************************\n\n");
    #endif

    f_entrada = fopen(nome_f1, "r");                                        /* Aberutra do ficheiro de entrada */
    f_saida = fopen(nome_f2, "w");                                          /* Aberutra do ficheiro de saída */

    validacao = validacao_abertura_ficheiro(f_entrada, nome_f1);            /* validação da aberutra do ficheiro de entrada */
    if(validacao == false){
        return;
    }
    validacao = validacao_abertura_ficheiro(f_saida, nome_f2);              /* validação da aberutra do ficheiro de saida */
    if(validacao == false){
        return;
    }
    #ifdef DEBUG
        printf(">> Inicio do processamento de rotas");
    #endif
    while(1){                                                               /* Leitura do ficheiro linha a linha infinitamene*/
        fgets(leitura, 30, f_entrada);                                      /* Leitura de uma linha ficheiro para processamento */

        if(feof(f_entrada)){                                                /* Verificação do fim do ficheiro */
            break;                                                          /* Caso seja verdade interrompe o ciclo */
        }
        else if(leitura[0] == '#'){                                              /* Primeiro tipo de leitura */
            print_string(f_saida, leitura);                                 /* Escrita no ficheiro de saída*/
            #ifdef DEBUG
                printf("\n\n\t->-> %s", leitura);                           /* Debbuging*/
            #endif
            continue;                                                       /* dar a instrução de "continue" para ignorar o que está em baixo" */
        }
        else if(leitura[0] == '\n'){                                        /* Segundo tipo de leitura */
            print_string(f_saida, leitura);                                 /* Meter um linha em branco no ficheiro de saída */
            distancia = 0;                                                  /* Reset à distancia para processamento de nova rota */
            referencia[0] = '\0';                                           /* Remoção da referencia de rota */
            continue;
        }
        else{                                                               /* Segundo tipo de leitura */
            #ifdef DEBUG
                printf("\t\t Localidade: %s", leitura);                     /* Debbuging*/
            #endif
            retirar_paragrafo(leitura);                                     /* remoção do caracter '\n' */
            if(referencia[0] == '\0'){                                      /* verifica se já existe referencia */
                strcpy(referencia, leitura);                                /* caso não exista, define a referencia com a localidade lida */
            }
            #ifdef DEBUG
                printf("\t\t\t>> Distancia a calcular - Localidade [%s] - Loc. Referencia [%s]\n", leitura, referencia);     /* Debbuging*/
            #endif

            trajeto = processamento_distancia(dados_uteis, referencia, leitura);                    /* Calculo do valor da distancia entre a referencia e a localidade lida */
            distancia = distancia + trajeto;                                                        /* Adicionar à distancia total da rota */

            #ifdef DEBUG
                printf("\t\t\t>> distancia total %f km\n\n", distancia);                            /* Debbuging*/
            #endif

            fprintf(f_saida, "%s %.2f km\n", leitura, distancia);                                   /* Imprimir no ficheiro de saida a localidade e a distancia à referencia*/

            strcpy(referencia, leitura);                                                            /* Definir uma nova referencia com o ultimo valor lido */
        }
    }
    fclose(f_entrada);              /* Fecho do ficheiro de entrada */
    fclose(f_saida);                /* Fecho do ficheiro de entrada */
}
/**********************************************************************************/

/************************************************************************************************************************
* Função: float processamento_distancia(LISTA * dados_uteis, char referencia[], char leitura[]){                        *
* Descrição: Pré-processamento antes de calcular o valor da distancia.                                                  *
*        Verifica se "existe" distancia.                                                                                *
*                                                                                                                       *
************************************************************************************************************************/
float processamento_distancia(LISTA * dados_uteis, char referencia[], char leitura[]){

    float trajeto;

    if(strcmp(referencia, leitura) == 0){                               /* Verificação se a referencia é igual à localidade lida*/
        return 0;                                                       /* Retorna zero caso sejam iguais */
    }
    trajeto = calculo_distancia(dados_uteis, referencia, leitura);      /* Chama afunção para calcular a distancia*/
    return trajeto;                                                     /* Retorna o valor calculado */

}
/***********************************************************************************************************************/

/******************************************************************************************
* Função: float calculo_distancia(LISTA * dados_uteis, char referencia[], char leitura[]) *
* Descrição: Cálculo da distancia. Procura na estutura de dados pela longitude e latitude.*
*        Conversão para radianos da longitude e latitude                                  *
*       Calculo da distancia em x e em y                                                  *
******************************************************************************************/
float calculo_distancia(LISTA * dados_uteis, char referencia[], char leitura[]){

    float lon, lon_0, lat, lat_0, x=2, y=2;

    encontrar_coordenadas(dados_uteis, referencia, &lon_0, &lat_0);             /* Procura os valores de longitude e latitude para a referencia */
    encontrar_coordenadas(dados_uteis, leitura, &lon, &lat);                    /* Procura os valores de longitude e latitude para a localidade lida */

    #ifdef DEBUG
        printf("\t\t\t>> Em graus:    [%s %.2f %.2f] >-< ref [%s %.2f %.2f]\n", leitura, lat, lon, referencia,  lat_0, lon_0);
    #endif

    conversao_radianos(&lon_0);     /* Conversão para radianos */
    conversao_radianos(&lon);       /* Conversão para radianos */
    conversao_radianos(&lat_0);     /* Conversão para radianos */
    conversao_radianos(&lat);       /* Conversão para radianos */

    #ifdef DEBUG
        printf("\t\t\t>> Em radianos: [%s %.2f %.2f] >-< ref [%s %.2f %.2f]\n", leitura, lat, lon, referencia,  lat_0, lon_0);    /* Debbuging*/
    #endif


    x = R * cos(lat) * sin(lon - lon_0);                /* Calculo da distancia em x */

    #ifdef DEBUG
        printf("\t\t\t\t x = %f\n", x);                     /* Debbuging*/
    #endif

    y = R * (sin(lat - lat_0) / cos(lat - lat_0));      /* Calculo da distancia em y */

    #ifdef DEBUG
        printf("\t\t\t\t y = %f\n", y);                     /* Debbuging*/
    #endif

    return sqrt(x * x + y * y);                         /* distancia dada pela raiz quadrada dos quadrados de x e y */
}
/******************************************************************************************/

/**************************************************************
* Função: void conversao_radianos(float * angulo)             *
* Descrição: formula para conversão em radianos, passagem por *
*        endereço.                                            *
**************************************************************/
void conversao_radianos(float * angulo){
    *angulo = *angulo * PI / 180;           /* Fórmula para conversão em radianos, escrita diretamente na variável passada por endereço */
}
/**************************************************************/

/**************************************************************************************************************
* Função: void encontrar_coordenadas(LISTA * dados_uteis, char cidade[], float * longitude, float * latitude) *
* Descrição: Percorre a lista, ao encontrar a localidade escrever na variável passada por endereço.           *
*        Caso não encontre saí da aplicação                                                                   *
**************************************************************************************************************/
void encontrar_coordenadas(LISTA * dados_uteis, char cidade[], float * longitude, float * latitude){

    for(; dados_uteis != NULL; dados_uteis = dados_uteis->proximo){                                                         /* ciclo que percorrer a lista */
        if(strcmp(cidade, dados_uteis->localidade.nome) == 0 && strlen(cidade) == strlen(dados_uteis->localidade.nome)){    /* verirca se encontrou a localidade */
            *longitude = dados_uteis->localidade.longitude;                                                                 /* Escrita da longitude na variavel passada por endereço */
            *latitude = dados_uteis->localidade.latitude;                                                                   /* Escrita da latitude na variavel passada por endereço */
            return;                                                                                                         /* não precisa de procurar mais, retorna*/
        }
    }
    printf(">> Erro no ficheiro de rotas - Localidade invalida [%s]\n", cidade);                                            /* Caso não encontre a localiade */
    exit(0);                                                                                                                /* Saí de forma bruta do programa */
}
/**************************************************************************************************************/

/********************************************************************
* Função: void print_string(FILE * f, char leitura[])               *
* Descrição: imprime a leitura no ficheiro de saida                 *
********************************************************************/
void print_string(FILE * f, char leitura[]){
    fprintf(f, "%s", leitura);
}
/*******************************************************************/

/********************************************************************
* Função: void retirar_paragrafo(char leitura[])                    *
* Descrição: percorre o array de forma a encontrar o '\n' e         *
        substituir por '\0'                                         *
********************************************************************/
void retirar_paragrafo(char leitura[]){
    int i = 0;
    if(leitura[0] == '\n'){return;}
    while(leitura[i] != '\n'){
        i++;
    }
    leitura[i]='\0';
}
/*******************************************************************/

/********************************************************************
* Função: LISTA * leitura_dados_uteis()                             *
* Descrição: Abre o ficheiro, le os dados e desencolve a estrura    *
*        de suporte para analise dos dados                          *
********************************************************************/
LISTA * leitura_dados_uteis(){

    FILE * ficheiro;
    char nome_ficheiro[] = "localidade.txt";
    int num_localidades, leituras, i;
    float longitude, latitude;
    char nome[20];
    bool validacao;
    LISTA * localidades_DADOS = NULL;

    #ifdef DEBUG
        printf("********************************************\n");
        printf("*   Primeiro passo - leitura dos dados     *\n");
        printf("********************************************\n\n");
    #endif

    ficheiro = fopen(nome_ficheiro, "r");                                       /* Abertura do ficheiro */

    if(ficheiro == NULL){                                                       /* Validação da Abertura do ficheiro */
        #ifdef DEBUG
            printf(">> Abertura do ficheiro %s com erros\n", nome_ficheiro);    /* Debbunging */
        #endif
        return NULL;                                                            /* Retorna NULL para indicar o erro */
    }
    else{
        #ifdef DEBUG
            printf(">> Abertura correta do ficheiro %s\n\n", nome_ficheiro);    /* Debbunging */
        #endif
    }

    fscanf(ficheiro, "%d", &num_localidades);                                   /* Leitura do numero de localidades a processar*/

    if(num_localidades < 1){                                                    /* Valida o número de localidades */
        printf(">> Linha 1: Erro nao positivo\n");                              /* Debbunging */
        return NULL;                                                            /* Retorna NULL para indicar o erro */
    }

    #ifdef DEBUG
        printf(">> Numero de localidades a serem processadas %d\n", num_localidades);                   /* Debbunging */
        printf(">> Inicio da leitura dos dados\n");                                                     /* Debbunging */
    #endif

    for(i = num_localidades; i > 0; i--){
        leituras = fscanf(ficheiro, "%s %f %f", nome, &latitude, &longitude);                           /* Leitura do nome, latiturde e longitude da localidade */

        validacao = validacao_dadosuteis(leituras, longitude, latitude, i);                             /* Validadçaõ dos dados lidos*/
        if(validacao == false){return NULL;}                                                            /* Casos os dados não sejam válidos */

        localidades_DADOS = insercao_lista_localidades(localidades_DADOS, nome, longitude, latitude);   /* inserção na estrutura de dados */

    }
    fclose(ficheiro);                                                                                   /* Fecho do ficheiro*/
    return localidades_DADOS;                                                                           /* Retorna o ponteiro para o inicio da lista */
}
/*******************************************************************/

/***********************************************************************************************
* Função: bool validacao_dadosuteis(int leituras, float longitude, float latitude, int linha)) *
* Descrição: Valida o número de leituras feitas por linhas e se o valor para a longitude e para*
        a latitude estão dentro do pedido                                                      *
************************************************************************************************/
bool validacao_dadosuteis(int leituras, float longitude, float latitude, int linha){
    if(leituras < 3 || leituras> 3){                                                        /* Valida o núemro de leituras*/
        printf("\n>> Linha %d: Erro no numero de parametros\n", linha+2);                   /* Debbunging */
        printf(">> Exit application (0)\n");                                                /* Debbunging */
        return false;                                                                       /* Retorna "false" a indicar erro */
    }
    else if(longitude > 180 || longitude < -180){                                           /* Verifica se o valor da longitude está entre -180 e 180*/
        printf("\n>> Linha %d: Erro longitude fora dos limites\n", linha+2);                /* Debbunging */
        printf(">> Exit application (0)\n");                                                /* Debbunging */
        return false;                                                                       /* Retorna "false" a indicar erro */
    }
    else if(latitude > 90 || latitude < -90){                                               /* Verifica se o valor da latitude está entre -90 e 90*/
        printf("\n>> Linha %d: : Erro latitude fora dos limites\n", linha+2);               /* Debbunging */
        printf(">> Exit application (0)\n");                                                /* Debbunging */
        return false;                                                                       /* Retorna "false" a indicar erro */
    }
    return true;                                                                            /* Retorna "true" indicando que não houve erro na leitura*/
}
/***********************************************************************************************/

/**********************************************************************************************************************
* Função: LISTA * insercao_lista_localidades(LISTA * localidades_DADOS, char nome[], float longitude, float latitude) *
* Descrição: recebe dados sobre uma localidade, cria um elemento e insere no topo da lista                            *
**********************************************************************************************************************/
LISTA * insercao_lista_localidades(LISTA * localidades_DADOS, char nome[], float longitude, float latitude){

    LISTA * novo;

    novo = criacao_dado(nome, longitude, latitude);         /* Criação de um novo elemento da lista*/
    novo->proximo = localidades_DADOS;                      /* Inserção no topo da lista */

    return novo;
}
/*********************************************************************************************************************/

/******************************************************************************
* Função: LISTA * criacao_dado(char nome[], float longitude, float latitude)  *
* Descrição: alloca memória para um elemento da lista. guarda o nome,         *
*       a latitude e a longitude nessa variável alocada                       *
******************************************************************************/
LISTA * criacao_dado(char nome[], float longitude, float latitude){

    LISTA * novo = (LISTA*) malloc (sizeof(LISTA));                 /* alocação de memória */

    strcpy(novo->localidade.nome,nome);                             /* Cópia do nome da localidade */
    novo->localidade.longitude = longitude;                         /* Guarda a longitude */
    novo->localidade.latitude = latitude;                           /* Guarda a latitude */

    return novo;
}
/*****************************************************************************/

/******************************************************************************
* Função: void print_dados_uteis(LISTA * dados_uteis)                         *
* Descrição: percorre a lista elemento a elemento e imprime o seu conteudo    *
******************************************************************************/
void print_dados_uteis(LISTA * dados_uteis){
    printf(">> Estrutura de Dados\n\n");
    for(;dados_uteis != NULL; dados_uteis = dados_uteis->proximo){                                                                      /* Ciclo para percorrer a lista*/
        printf("\t[%s %f %f]\n", dados_uteis->localidade.nome, dados_uteis->localidade.latitude, dados_uteis->localidade.longitude);    /* Impressão do conteudo da lista*/
    }
}
/*****************************************************************************/
