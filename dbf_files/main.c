/*
 *
 *  Autor: Gabriel Vasquez H.

 *
*/


#include <stdio.h>
#include <stdlib.h>
#include "dbf.h"

#define MULTIPLO  10


int main()
{
//  FILE *fp;
  FILE * pFile;
  int i, NumeroCeldas;
  long lSize,
    LRegisterSize;    // LRegisterSize es el tamano de cada registro(entrada) de la tabla.
  char * buffer, *RegisterBuf ;
  size_t result;
  stDBF_DescritorFile DBFBuf;
  stDBFSubRecord *ptrSubrecord;

  pFile = fopen ( "STOCK.DBF" , "rb" );
  if (pFile==NULL) {fputs ("File error",stderr); exit (1);}

  // Obtenemos el tamano del archivo.
  fseek (pFile , 0 , SEEK_END);
  lSize = ftell (pFile);
  rewind (pFile);

  // allocate memory to contain the file's descriptor :
  //buffer = (char*) malloc (sizeof(char)*lSize);
  //if (buffer == NULL) {fputs ("Memory error",stderr); exit (2);}

  //Leemos el encabezado del archivo y lo guardamos en la estructura
  result = fread (&DBFBuf,1,sizeof(stDBF_DescritorFile), pFile);
  if (result != sizeof(stDBF_DescritorFile)) {fputs ("Reading error",stderr); exit (2);}

  // Reservo memoria para obtener todos los SubRecords.
  lSize = DBFBuf.PostFisrtDataRecord ;
  lSize = lSize - 2 -32; // 2 byte se deben al 0x0d y al 0x00 que estan al final del registro
  buffer = (char*) malloc (sizeof(char)*lSize);
  if (buffer == NULL) {fputs ("Memory error",stderr); exit (3);}

  // Reservo memoria para leer la descripcion de los subregistros.
  result = fread (buffer,lSize,1, pFile);
  ptrSubrecord = (stDBFSubRecord*)buffer;
  printf("\nByte de subregistro %d: Longitud estruc%d", (int)lSize, (int)sizeof(stDBFSubRecord));
  NumeroCeldas = lSize/sizeof(stDBFSubRecord);
  printf("\nLa cantidad de celdas por registro es: %d", NumeroCeldas);

  printf("\nImprimimos el concepto de cada celda del registro");
  LRegisterSize =0;
  for(i=0; i< NumeroCeldas ;i++){
    printf("\n %s", ptrSubrecord[i].FieldName);
    LRegisterSize += ptrSubrecord[i].LengthFiel; // utilizo para contar el tamano total de cada entrada de la tabla
  }
  /* the whole file is now loaded in the memory buffer. */
  PrintDBFDescriptorFile(DBFBuf);
  printf("\n Longitud de cada registro(calc sum) es:%d",(int)LRegisterSize);
  printf("\n descripcion de cada celda del registro:");
  for(i=0; i< NumeroCeldas ;i++)
  {
    //printf("\n %s", ptrSubrecord[i].FieldName);
    PrintSubRecordInfo(ptrSubrecord[i]);
  }
  //imprimimos un ar de registros

  /*
    debemos reservar memoria e ir leyendo los registros de forma incremental
  */
  // nos posiscionamos donde empiezan los registros.



lSize = ftell (pFile);
  fseek(pFile, DBFBuf.PostFisrtDataRecord, SEEK_SET);
lSize = ftell (pFile);

  RegisterBuf = (char*) malloc (sizeof(char)*LRegisterSize*MULTIPLO);
  if (RegisterBuf == NULL) {fputs ("Memory error",stderr); exit (3);}

  result = fread (RegisterBuf ,1,LRegisterSize*MULTIPLO, pFile);
  if (result != LRegisterSize*MULTIPLO) {fputs ("Reading error",stderr); exit (2);}
  printf("\n0 1 2 3 4 5 6 7 8 9 A B C D E F\n");
  for(i=0;i<LRegisterSize*MULTIPLO;i++ ){
    printf("%c ",RegisterBuf[i]);
    if((i%16==0) && (i>0))
      printf("\n");
    if((i%LRegisterSize==0) && (i>0))
    {
      printf("\n===============================\n");
      printf("\n0 1 2 3 4 5 6 7 8 9 A B C D E F\n");
    }
  }

  // ya termien entonces libero la memoria que pedi.
  fclose (pFile);
  free (buffer); // Liberamos la memoria
  free(RegisterBuf);
  //fclose(fp);




  printf("Hello world!\n");

  return 0;
}
