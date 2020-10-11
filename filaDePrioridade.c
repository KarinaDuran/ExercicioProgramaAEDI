/*********************************************************************/
/**   ACH2023 - Algoritmos e Estruturas de Dados I                  **/
/**   EACH-USP - Segundo Semestre de 2020                           **/
/**   Turma 94 - Prof. Luciano Antonio Digiampietri                 **/
/**                                                                 **/
/**   EP1 - Fila de Prioridade                                      **/
/**                                                                 **/
/**   Karina Duran Munhos                   11295911                **/
/**                                                                 **/
/*********************************************************************/

#include "filaDePrioridade.h"
#include <malloc.h>
#define true 1
#define false 0
/*
*Funcao que inicializa uma fila com um tamanho máximo de elementos definido pelo int max.
*Aloca na memoria espaço para uma fila, cria o no cabeça com ID = -1 e prioridade = 1000000.
*Acerta os ponteiros ant e prox do no cabeça para apontarem para ele mesmo.
*Cria um arranjo auxiliar com o tamanho maximo definido por max e todos os elementos recebem NULL.
*Retorna o endereço da fila criada.
*/
PFILA criarFila(int max)
{
    PFILA res = (PFILA) malloc(sizeof(FILADEPRIORIDADE));
    res->maxElementos = max;
    res->arranjo = (PONT*) malloc(sizeof(PONT)*max);
    int i;
    for (i=0; i<max; i++) res->arranjo[i] = NULL;
    PONT cabeca = (PONT) malloc(sizeof(ELEMENTO));
    res->fila = cabeca;
    cabeca->ant = cabeca;
    cabeca->prox = cabeca;
    cabeca->id = -1;
    cabeca->prioridade = 1000000;
    return res;
}

/*Funcao para imprimir o numero de elementos validos (sem o no cabeça) de uma fila de prioridade e para cada elemento
percorrido, incluindo o no cabeça, imprime o endereco do anterior, o ID, a prioridade e o endereco do atual e o endereco
do proximo. Em seguida, imprime os elementos validos com seu ID, prioridade e endereco.
No fim, imprime o arranjo auxiliar com os enderecos dos elementos na posição respectiva de seu ID.*/
void exibirLog(PFILA f)
{
    printf("Log [elementos: %i (alem do no cabeca)]\n", tamanho(f));
    PONT atual = f->fila;
    printf("%p[%i;%f;%p]%p ", atual->ant, atual->id, atual->prioridade, atual, atual->prox);
    atual = atual->prox;
    while (atual != f->fila)
    {
        printf("%p[%i;%f;%p]%p ", atual->ant, atual->id, atual->prioridade, atual, atual->prox);
        atual = atual->prox;
    }
    printf("\nElementos validos: ");
    atual = atual->prox;
    while (atual != f->fila)
    {
        printf("[%i;%f;%p] ", atual->id, atual->prioridade, atual);
        atual = atual->prox;
    }

    printf("\nValores do arrajo:\n\[ ");
    int x;
    for (x=0; x<f->maxElementos; x++) printf("%p ",f->arranjo[x]);
    printf("]\n\n");
}



//Função que recebe o endereco de uma fila de prioridade como parâmetro e retorna o tamanho da fila.
int tamanho(PFILA f)
{
    int tam = 0;
    PONT end = f->fila->prox;
    while (end != f->fila )
    {
        tam ++;
        end = end->prox;
    }
    return tam;
}

/*Funcao auxiliar para procurar se existe um elemento em uma fila de prioridade com um determinado ID.

Retorna false se: o ID for invalido, isto e, menor que 0 ou maior ou igual que o numero maximo de elementos, ou se nao
existir elemento com o ID recebido por parametro.

Caso contrario, retorna true.*/
bool buscaId (PFILA f, int id)
{
    bool resp= false;
//Verifica se o ID e valido
    if(id < f->maxElementos && id >= 0 )
    {
        //Busca no arranjo auxiliar se eh diferente de NULL
        if(f->arranjo[id]!=NULL)
        {
            resp = true;
        }
    }
    return resp;
}

/*Funcao auxiliar para encontrar o elemento anterior(contando com o no cabeca) da prioridade recebida por parametro
e retorna-lo.*/
PONT buscaAnt(PFILA f, float prioridade)
{
    PONT ant, atual;
    ant = f->fila;
    atual = f->fila->prox;
//Percorre a fila ate encontrar o elemento desejado ou retornar ao no cabeca
    while ((atual->prioridade > prioridade)&&(atual!= f->fila))
    {
        ant = atual;
        atual = atual->prox;
    }
    return ant;
}

/*
Funcao para inserir um novo elemento em uma fila de prioridade e recebe como parametro o endereco da fila,o ID
e a prioridade do elemento.

Retorna false se: ID for invalido, isto e, menor que 0 ou maior ou igual que o numero maximo de elementos, a
prioridade for maior que a do no cabeça ou se ja existir um elemento na fila que possua o mesmo ID recebido como
parametro.

Caso contrario, o elemento e inserido na posicao correta seguindo a ordem decrescente de prioridades e acerta-se
os ponteiros ant e prox do novo elemento e dos elementos envolvidos (anterior e proximo).
*/
bool inserirElemento(PFILA f, int id, float prioridade)
{
    bool resposta = false;
    if(id >= f->maxElementos || id < 0 || buscaId(f, id) == true || prioridade >= f->fila->prioridade)
    {
        return resposta;
    }
    else
    {
        //Aloca espaço na memória para o novo elemento
        PONT novo = (PONT) malloc (sizeof(ELEMENTO));
        //Atribui ID e prioridade ao novo elemento
        novo->id = id;
        novo->prioridade = prioridade;
        //Chama o metodo auxiliar para encontrar o anterior
        PONT ant;
        ant = buscaAnt(f, prioridade);
        //Acerta os ponteiros ant e prox do novo elemento na posicao certa
        novo->prox = ant->prox;
        novo->ant = ant;
        //Acerta os ponteiros ant do posterior e prox do anterior
        novo->prox->ant = novo;
        ant -> prox = novo;
        //Atribui o endereco a respectiva posicao do ID no arranjo auxiliar
        f->arranjo[id] = novo;
        resposta = true;
    }
    return resposta;
}

/*Funcao para aumentar a prioridade de um elemento existente.
Retorna false se: o ID for invalido, ou seja, menor que 0 ou maior ou igual que o numero maximo de elementos, se
nao existir um elemento valido com o ID recebido por parametro, se a prioridade for maior que a prioridade do no
cabeca ou se a prioridade do elemento com o ID recebido ja for maior que o valor do float novaPrioridade recebido
por parametro.

Caso contrario, altera-se a prioridade e reposiciona o elemento na fila, acertando os ponteiros ant e prox dos
elementos envolvidos na troca de posicao.
*/
bool aumentarPrioridade(PFILA f, int id, float novaPrioridade)
{
    bool resposta = false;
    float priori;
    bool x = consultarPrioridade(f, id, &priori);
    if(id >= f->maxElementos || id < 0 || x == false || novaPrioridade >= f->fila->prioridade)
    {
        return resposta;
    }
    else
    {
        //Verifica se a nova prioridade e maior que a atual
        if(priori<novaPrioridade)
        {
            resposta = true;
            //Redefine os ponteiros ant do elemento posterior e prox do elemento anterior
            f->arranjo[id]->ant->prox = f->arranjo[id]->prox;
            f->arranjo[id]->prox->ant = f->arranjo[id]->ant;
            //Atribuii o novo valor de prioridade
            f->arranjo[id]->prioridade = novaPrioridade;
            //Utiliza a funcao auxiliar para encontrar a posição correta da nova prioridade
            PONT ant = buscaAnt(f, novaPrioridade);
            //Acerta os ponteiros ant e prox do elemento com a nova prioridade
            f->arranjo[id]->ant = ant;
            f->arranjo[id]->prox=ant->prox;
            //Acerta os ponteiros ant do posterior e prox do anterior
            ant->prox->ant = f->arranjo[id];
            ant->prox = f->arranjo[id];
        }
        return resposta;
    }

}

/*Funcao para reduzir a prioridade de um elemento existente.
Retorna false se: o ID for invalido, ou seja, menor que 0 ou maior ou igual que o numero maximo de elementos, se
nao existir um elemento valido com o ID recebido por parametro, se a prioridade for maior que a prioridade do no
cabeca ou se a prioridade do elemento com o ID recebido ja for menor que o valor do float novaPrioridade recebido
por parametro.

Caso contrario, altera-se a prioridade e reposiciona o elemento na fila, acertando os ponteiros ant e prox dos
elementos envolvidos na troca de posicao.
*/
bool reduzirPrioridade(PFILA f, int id, float novaPrioridade)
{
    bool resposta = false;
    float priori;
    bool x = consultarPrioridade(f, id, &priori);
    if(id >= f->maxElementos || id < 0 || x == false || novaPrioridade >= f->fila->prioridade)
    {
        return resposta;
    }
    else
    {
        //Verifica se a nova prioridade e menor que a atual
        if(priori>novaPrioridade)
        {
            resposta = true;
            //Redefine os ponteiros ant do elemento posterior e prox do elemento anterior
            f->arranjo[id]->ant->prox = f->arranjo[id]->prox;
            f->arranjo[id]->prox->ant = f->arranjo[id]->ant;
            //Atribuii o novo valor de prioridade
            f->arranjo[id]->prioridade = novaPrioridade;
            //Utiliza a funcao auxiliar para encontrar a posição correta da nova prioridade
            PONT ant = buscaAnt(f, novaPrioridade);
            //Acerta os ponteiros ant e prox do elemento com a nova prioridade
            f->arranjo[id]->ant = ant;
            f->arranjo[id]->prox=ant->prox;
            //Acerta os ponteiros ant do posterior e prox do anterior
            ant->prox->ant = f->arranjo[id];
            ant->prox = f->arranjo[id];
        }
        return resposta;
    }

}

/*Funcao para remover o elemento com maior prioridade de uma fila.
Se a fila nao possuir elementos validos, ou seja, com tamanho = 0, o ponteiro para elemento resposta recebe NULL,
se possuir, o elemento de maior prioridade fica guardado em resposta e os ponteiros ant do elemento posterior e prox
do anterior sao ajustados pos remocao deste elemento da fila. O elemento tambem e excluido do arranjo auxiliar na
posicao do seu ID respectivo.*/
PONT removerElemento(PFILA f)
{
    PONT resposta = NULL;
    if(tamanho(f)!=0)
    {
        PONT p = f->fila;
        resposta = p->prox;
        int id = resposta->id;
        //Exclui o elemento do arranjo auxiliar
        f->arranjo[id] = NULL;
        //Ajusta os ponteiros prox do elemento anterior e ant do elemento posterior
        p->prox = resposta->prox;
        p->prox->ant= p;
    }
    return resposta;
}

/*Funcao que recebe um inteiro ID, um endereco de uma fila e um endereço para armazenar um tipo float, contido em
resposta, que armazenara a prioridade do elemento com o ID recebido.

Retorna false se: o ID for invalido, isto e, menor que zero ou maior ou igual que o numero de elementos maximo,
ou nao existir um elemento valido com o ID recebido por parametro.

Caso contrario, armazena no endereco fornecido pelo parametro a prioridade do elemento com o ID correspondente
e retorna true.*/
bool consultarPrioridade(PFILA f, int id, float* resposta)
{
    bool retorno = false;
    if(id >= 0 && id < f->maxElementos && buscaId(f, id) == true)
    {
        //Armazena a prioridade no endereco rebido por parametro.
        *resposta = f->arranjo[id]->prioridade;
        retorno = true;
    }
    return retorno;
}

