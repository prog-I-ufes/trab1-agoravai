#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

int main(){

FILE *arq;

                                                              //usado somente para fazer a contagem de LINHAS
char strAux[50];

int qntLinTeste,i,j;
qntLinTeste=0;

arq = fopen("iris_teste.csv","r");

if (!(arq = fopen("iris_teste.csv","r"))){
  printf("\nErro na abertura do arquivo. fechando programa...\n" );
  exit(1);
}

                                                            //CONTA A QUANTIDADE DE LIHAS QUE TEM NO ARQUIVO TESTE TXT
while(!feof(arq)){
  qntLinTeste++;
  fgets(strAux,50,arq);
}
printf("\nQuantidade de  linhas teste: %d\n",qntLinTeste );
//---------------------------------------------------
fclose(arq);
//---------------------------------------------------




arq = fopen("iris_teste.csv","r");

if (!(arq = fopen("iris_teste.csv","r"))){
  printf("\nErro na abertura do arquivo. fechando programa...\n" );
  exit(1);
}

//-----------------------------
char c;
int qntColuna =0;


                                                            //conta a qauntidade de virgulas para saber quantas colunas tem.
while(c!='\n'){
  if(c==','){
    qntColuna++;
  }

                                                            //imprime o aqrquivo inteiro

for(i=0;i<qntLinTeste-1;i++){
  fscanf(arq,"%c",&c);
  //printf("%c",c );
}

}
fclose(arq);                                                //pois se eu quiser ler de novo ele tera q ser aberto para ler do inicio
//-----------------------------
printf("\nColunas= %d\n",qntColuna );

//-----------------------------


//13/11/2018------------------------------------------------------------------------------------------------------------------------------------
int qntLinTreino = 0;

arq = fopen("iris_treino.csv","r");

if (!(arq = fopen("iris_treino.csv","r"))){
  printf("\nErro na abertura do arquivo. fechando programa...\n" );
  exit(1);
}

                                                            //CONTA A QUANTIDADE DE LIHAS QUE TEM NO ARQUIVO TREINO TXT
while(!feof(arq)){
  qntLinTreino++;
  fgets(strAux,50,arq);
}

//---------------------------------------------------
fclose(arq);
//---------------------------------------------------
printf("\nQuantidade de  linhas treino: %d\n",qntLinTreino );


char matTeste[qntLinTeste];
char matTreino[qntLinTreino];

arq = fopen("iris_teste.csv","r");
if (!(arq = fopen("iris_treino.csv","r"))){
  printf("\nErro na abertura do arquivo. fechando programa...\n" );
  exit(1);
}

/*for(i=0;i<qntLinTeste;i++){
  fscanf(arq,"%s")
}*/





  return 0;
}
