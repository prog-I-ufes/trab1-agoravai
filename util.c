#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct{
  FILE *treino;
  FILE *teste;
  char *predicao;
}tCaminhos;
////------------------------
typedef struct{
  int k;
  char tipo;
  float r;
}tdados;

typedef struct{
  float dist;
  float rotulotrein;
  float rotulotest;
}tOrdenar;
////------------------------
int comparator(const void *a,const void *b){
  tOrdenar* a1 = (tOrdenar *) a;
  tOrdenar* b1 = (tOrdenar *) b;
  return a1->dist-b1->dist;
}

int maior(int *le, int tamanho)
{
  int maior = 0;
  int maiorPos = 0;
  for (int i = 0; i < tamanho; i++) {
    if(le[i]>maior){
    maior = le[i];
    maiorPos = i;
    }
  }
  return maiorPos;
}


//------------------------------------------------------------------------------
void destroi_tCaminhos(tCaminhos c){
  free(c.predicao);
  fclose(c.treino);
  fclose(c.teste);

  // free(c);
}
//------------------------------------------------------------------------------
FILE *abre_arquivo(char *str){
  FILE *arq = fopen(str, "r");

  if(!arq){
    printf("Falha ao abrir arquivo\n");
    exit(1);
  }
  return arq;
}
//------------------------------------------------------------------------------
float euclidiana(float px, float py){
  return ((px - py) * (px - py));
}
//------------------------------------------------------------------------------
float minkowsky(float px, float py,float r){
  return ( pow((fabs(px-py)),r) );
}

//------------------------------------------------------------------------------
float chebyshev(float px, float py){
  return (fabs(px-py));
}
//------------------------------------------------------------------------------
// 10 eh o valor do \n e 44 o valor da virgula na tabela ascii
int num_colunas(char *arquivoStr, FILE *f){
  int c;
  int colunas = 0;
  while ((c = fgetc(f)) != EOF){
    if (c == 10) break;
    if (c == 44) colunas++;
  }

  rewind(f);
  return colunas;
}
//------------------------------------------------------------------------------
void corta_str(char *str, char c){
  int tam = strlen(str);
  for (int i = 0; i < tam; i++)
    if (str[i] == c) str[i] = '\0';
}
//------------------------------------------------------------------------------
int pega_tamanho_vetor(char *arquivoStr,FILE *f){
  int linhas = 0;
  int c;

  while ((c = fgetc(f)) != EOF){
    if (c == 10)
      linhas++;
  }

  rewind(f);
  return linhas;
}
//------------------------------------------------------------------------------
float numrotulos = 0;
float** carregarMatriz(FILE *f, int lin, int col, float **rotulosResultado){
  float **matriz;
  float *rotulos;
  char lixo;

  matriz =  (float **) malloc(sizeof (float *) * lin);
  for (int i = 0; i < lin; i++){
    matriz[i] = (float *) malloc(sizeof(float) * col);
  }

  rotulos = (float *) malloc(sizeof(float) * lin);

  for(int i =0; i<lin;i++){
    for(int j = 0; j < col; j++) {
        fscanf(f,"%f%c",&matriz[i][j],&lixo);
        // printf("\t%.2f",matriz[i][j]);
    }
    fscanf(f,"%f%c",&rotulos[i],&lixo);
    if(rotulos[i]>numrotulos)
      numrotulos=rotulos[i];
    // printf("\t%.2f",rotulos[i]);
    // printf("\n");
  }
  // puts("___________________________________");

  rewind(f);
  *rotulosResultado = rotulos;
  return matriz;
}
//------------------------------------------------------------------------------
FILE *treinoF;
FILE *testeF;
char buffer[9002];
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
tdados leConfig(FILE *f, float **matrizTreino, float **matrizTeste, int numCol, int linhaTreino, int linhaTeste,float *rotulosTreino,float *rotulosTeste){
  tdados dados;
  int K;
  int **matrizConfusao;
  float R;
  char tipoDist;
  int pos = 0;

  tOrdenar **vetorDist = malloc(sizeof(tOrdenar *) * linhaTeste);
  for (int i = 0; i < linhaTeste; i++)
    vetorDist[i] = malloc(sizeof(tOrdenar) * linhaTreino);

  float distancia = 0;

printf("Nrotulos: %d\n",(int)numrotulos );
  matrizConfusao =  (int **) malloc(sizeof (int *) * (int)numrotulos);
  for (int i = 0; i < (int)numrotulos; i++){
    matrizConfusao[i] = (int *) calloc((int)numrotulos , sizeof(int));
  }


  while(fscanf(f, "%d, %c",&K, &tipoDist) != EOF){

    if (tipoDist == 'M'){
      fscanf(f, ", %f", &R);
       for (int j = 0; j < linhaTeste; j++){
       for (int k = 0; k < linhaTreino; k++){
           for (int i = 0; i < numCol; i++){
             distancia += minkowsky(matrizTreino[k][i], matrizTeste[j][i],R);
           }
           distancia = pow(distancia,1/R);
           vetorDist[j][k].dist = distancia;
           vetorDist[j][k].rotulotrein = rotulosTreino[k];
           vetorDist[j][k].rotulotest = rotulosTeste[j];
           //printf("linha: %d x %d -- %f\n",j,k,vetorDist[j][k].dist);
           distancia = 0;
         }
       }
       float respRotulos[linhaTeste];
       for (int i = 0; i < linhaTeste; i++) {
         int counts[10] = {0};

         qsort(vetorDist[i],linhaTreino,sizeof(tOrdenar),comparator);
         for (int j = 0; j < K ; j++) {
           counts[(int)vetorDist[i][j].rotulotrein]++;
         }
         respRotulos[i] = maior(counts,10);
         // printf("Rot Mink : %d\n", (int)respRotulos[i] - 1);
       }


         for(int j = 0; j < linhaTeste;j++){
           // printf("A:%d  B:%d\n",(int)respRotulos[j],(int)rotulosTeste[j] );
          matrizConfusao[(int)respRotulos[j] - 1][(int)rotulosTeste[j] - 1]++;

         }




       float cont = 0;
       float accuracy;
       for(int i = 0;i < linhaTeste; i++){
         if(respRotulos[i] == rotulosTeste[i]){
           cont++;
         }
       }
       accuracy=cont/linhaTeste;


       printf("%.2f\n",accuracy);
       printf("\n");
       for(int i= 0 ;i<(int)numrotulos;i++){
         for(int j=0; j<(int)numrotulos;j++){
           printf("%d ",matrizConfusao[i][j]);
         }
         printf("\n" );
       }
       printf("\n");
       for (int i = 0; i < linhaTeste; i++){
         printf("Rot Mink : %d\n", (int)respRotulos[i] - 1);
       }
    }

    if (tipoDist == 'C'){
      for (int j = 0; j < linhaTeste; j++){
        for (int k = 0; k < linhaTreino; k++){
          float maior = 0;
          for (int i = 0; i < numCol; i++){
             distancia = chebyshev(matrizTreino[k][i],matrizTeste[j][i]);
             //printf("distanciaCheb: %f\n",distancia );
              if(distancia>maior){
                maior=distancia;
              }
          }

          vetorDist[j][k].dist = maior;
          vetorDist[j][k].rotulotrein = rotulosTreino[k];
          vetorDist[j][k].rotulotest = rotulosTeste[j];
          //printf("linha: %d x %d -- %f\n",j,k,vetorDist[j][k].dist);
          distancia = 0;
        }
      }

      float respRotulos[linhaTeste];
      for (int i = 0; i < linhaTeste; i++) {
        int counts[10] = {0};

        qsort(vetorDist[i],linhaTreino,sizeof(tOrdenar),comparator);
        for (int j = 0; j < K ; j++) {
          counts[(int)vetorDist[i][j].rotulotrein]++;
        }
        respRotulos[i] = maior(counts,10);
       // printf("Rot Chibi : %d\n", (int)respRotulos[i] - 1);
      }
      float cont = 0;
      float accuracy;
      for(int i = 0;i < linhaTeste; i++){
        if(respRotulos[i] == rotulosTeste[i]){
          cont++;
        }
      }
      accuracy=cont/linhaTeste;
      printf("%.2f\n",accuracy);
      printf("\n");
      for(int i= 0 ;i<(int)numrotulos;i++){
        for(int j=0; j<(int)numrotulos;j++){
          printf("%d ",matrizConfusao[i][j]);
        }
        printf("\n" );
      }
      printf("\n");
      for (int i = 0; i < linhaTeste; i++){
        printf("Rot Cheby : %d\n", (int)respRotulos[i] - 1);
      }
    }

    if (tipoDist == 'E'){

      for (int j = 0; j < linhaTeste; j++){
      for (int k = 0; k < linhaTreino; k++){
          for (int i = 0; i < numCol; i++){
            distancia += euclidiana(matrizTreino[k][i], matrizTeste[j][i]);
          }
          distancia = sqrt(distancia);
          vetorDist[j][k].dist = distancia;
          vetorDist[j][k].rotulotrein = rotulosTreino[k];
          vetorDist[j][k].rotulotest = rotulosTeste[j];
          //printf("linha: %d x %d -- %f\n",j,k,vetorDist[j][k].dist);
          distancia = 0;
        }
      }
      float respRotulos[linhaTeste];
      for (int i = 0; i < linhaTeste; i++) {
        int counts[10] = {0};

        qsort(vetorDist[i],linhaTreino,sizeof(tOrdenar),comparator);
        for (int j = 0; j < K ; j++) {
          counts[(int)vetorDist[i][j].rotulotrein]++;
        }
        respRotulos[i] = maior(counts,10);
       // printf("Rot Eucli : %d\n", (int)respRotulos[i] - 1);
      }
      float cont = 0;
      float accuracy;
      for(int i = 0;i < linhaTeste; i++){
        if(respRotulos[i] == rotulosTeste[i]){
          cont++;
        }
      }
      accuracy=cont/linhaTeste;
      printf("%.2f\n",accuracy);
      printf("\n");
      for(int i= 0 ;i<(int)numrotulos;i++){
        for(int j=0; j<(int)numrotulos;j++){
          printf("%d ",matrizConfusao[i][j]);
        }
        printf("\n" );
      }
      printf("\n");
      for (int i = 0; i < linhaTeste; i++){
        printf("Rot Euclidin : %d\n", (int)respRotulos[i] - 1);
      }
    }

  for (int i = 0; i < linhaTeste; i++) {
    free(vetorDist[i]);
  }
  free(vetorDist);
  for (int i = 0; i < (int)numrotulos; i++){
    free(matrizConfusao[i]);
  }
  free(matrizConfusao);
  return dados;
  }
}

tCaminhos leCaminhos(FILE *f,char **caminhoTeste,char **caminhoTreino, char **caminhoPredicao){
  int pos;
  tCaminhos caminho;

  fgets(buffer,9002,f);
  pos = strlen(buffer);

  *caminhoTreino = malloc(sizeof(char) * (pos + 1));
  strcpy(*caminhoTreino, buffer);
  corta_str(*caminhoTreino, '\n');

  // printf("%s\n", *caminhoTreino); //TESTE
  caminho.treino = abre_arquivo(*caminhoTreino);

  fgets(buffer,9002,f);
  pos = strlen(buffer);

  *caminhoTeste = malloc(sizeof(char) * (pos + 1));
  strcpy(*caminhoTeste, buffer);
  corta_str(*caminhoTeste, '\n');
  printf("%s\n", *caminhoTeste); //TESTE
  caminho.teste = abre_arquivo(*caminhoTeste);

  fgets(buffer,9002,f);
  pos = strlen(buffer);

  *caminhoPredicao = malloc(sizeof(char) * (pos + 1));
  strcpy(*caminhoPredicao, buffer);
  corta_str(*caminhoPredicao, '\n');
  // printf("%s\n", *caminhoPredicao); //TESTE

  caminho.predicao = (char *) malloc(sizeof(char) * 20);              //20 pra caber ate 3 digitos de execuçao
  strcpy(caminho.predicao,*caminhoPredicao);
  free(*caminhoPredicao);

  free(*caminhoTreino);
  free(*caminhoTeste);

  return caminho;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------


int main(){
  FILE *config=fopen("config.txt","r");
  char *caminhoTeste, *caminhoTreino, *caminhoPredicao;

  //
  FILE *teste;

  tCaminhos caminhos = leCaminhos(config,&caminhoTeste,&caminhoTreino,&caminhoPredicao);

  int a;
  float b;
  char c;


  float **matrizTeste;
  float **matrizTreino;
  float *rotulosTeste;
  float *rotulosTreino;
  char *arquivoStr = "iris_treino.csv";//teste

  // f = fopen(arquivoStr, "r");
  int colunas = num_colunas(arquivoStr,caminhos.treino);
  int linhas_treino = pega_tamanho_vetor(arquivoStr,caminhos.treino);
  int linhas_teste = pega_tamanho_vetor(arquivoStr, caminhos.teste);

  matrizTeste = carregarMatriz(caminhos.teste,linhas_teste,colunas,&rotulosTeste);
  matrizTreino = carregarMatriz(caminhos.treino,linhas_treino,colunas,&rotulosTreino);

  leConfig(config, matrizTreino, matrizTeste, colunas, linhas_treino, linhas_teste,rotulosTreino,rotulosTeste);
  // printf("%f\n",matrizTeste[1][1] );
  // printf("%f\n",matrizTreino[1][1] );

  printf("Colunas: %d, Tam de iris_teste: %d\n", colunas, linhas_teste);
  //----------------------------------------------------------------------------
//------------------------------------------------------------------------------
//EUCLIDES FULL PRONTA
// o  valor de i smp sera menor que o valor de j
float distEaux[linhas_teste][linhas_teste];
float distE[linhas_teste][linhas_teste];
int i,j,k;


// inicializa os vetorescomo zeros
for(i=0;i<linhas_teste;i++){
  for(j=0;j<linhas_teste;j++){
    distEaux[i][j]=0;
  }
}

for(i=0;i<(linhas_teste-1);i++){
  for(j=i+1;j<linhas_teste;j++){
    for(k=0;k<colunas;k++){
    distEaux[i][j]=euclidiana(matrizTeste[i][k],matrizTeste[j][k])+distEaux[i][j];
    }
  }
}


//distE[i][j] -> distancia da linha i com a linha j
//lembrar de colocar o -lm
for(i=0;i<linhas_teste;i++){
  for(j=1;j<linhas_teste;j++){
    distE[i][j]=sqrt(distEaux[i][j]);
  }
}



//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// o valor de R ainda tem q ser lido do arquivo
// minkowsky2 retorna as distancias de cada linha para as demais linhas;
//minkowsky2(linhas_teste,colunas,r,&matrizTeste)
//se tiver na main provavel que ira funcionar,mas como esta em funcao n sei se funciona(como a euclidiana)

//------------------------------------------------------------------------------
  for (int i = 0; i < linhas_teste; i++){
    free(matrizTeste[i]);
  }

  for (size_t i = 0; i < linhas_treino; i++) {
    free(matrizTreino[i]);
  }

    free(matrizTeste);
    free(matrizTreino);

    free(rotulosTeste);
    free(rotulosTreino);

   fclose(config);
   destroi_tCaminhos(caminhos);



  return 0;
}
