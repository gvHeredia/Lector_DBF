/*
 *  Fecha: 7/6/2022
 *  Autor: Gabriel Vasquez H.
 *  Objetivo:
 *    Este projecto nace a raiz de una nececida en mi trabajo de poder trabajar con
 *    sitemas que tienen esta base de datos en maquinas viejas de pocos recursos.
 *    A si que mientras desarrollo una herramienta para poder extraer y recuperar los
 *    datos, me parecio uutil poder hacer un ejemplo simple para no olvidarme
 *    la estructura y dejarlo para que otros puedan seguir con un ejeplo modificable
 *    sin tener que empezar de cero.
 *  Datos de Ana mary
 *  27959636422
 *  Anariky2022
 *
*/


#include <stdio.h>
#include <stdlib.h>
#include "dbf.h"

#define MULTIPLO  10
#define NEW       1

int main(int argCnt, char** Arg )
{
//  FILE *fp;
  FILE * pFile;
  int i, NumeroCeldas;
  long  lSize,
        LRegisterSize;    // LRegisterSize es el tamano de cada registro(entrada) de la tabla.
  char * bu0ffer, *RegisterBuf ;
  size_t result;
  stDBF_DescritorFile DBFBuf;
  stDBFSubRecord *ptrSubrecord;

  stDescriptor Descriptor;

  if (DBFOpen(&Descriptor, "STOCK.DBF") != 0) {
    fputs ("File error",stderr); exit (1);
  }

  // allocate memory to contain the file's descriptor :
  //buffer = (char*) malloc (sizeof(char)*lSize);
  //if (buffer == NULL) {fputs ("Memory error",stderr); exit (2);}

  //Leemos el encabezado del archivo y lo guardamos en la estructura
/*
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
  // the whole file is now loaded in the memory buffer.
  PrintDBFDescriptorFile(DBFBuf);
k  printf("\n Longitud de cada registro(calc sum) es:%d",(int)LRegisterSize);
  printf("\n descripcion de cada celda del registro:");

*/

  printf("\nByte de subregistro %d", Descriptor.TamanoDescripcionReg);
  printf("\nLa cantidad de celdas por registro es: %d", Descriptor.CampoXRegistro);

  printf("\nImprimimos el concepto de cada celda del registro");
  LRegisterSize =0;
//  for(i=0; i< NumeroCeldas ;i++){
  for(i=0; i< Descriptor.CampoXRegistro ;i++){
    printf("\n %s", Descriptor.ptTCedasRegistro[i].FieldName);
    LRegisterSize += Descriptor.ptTCedasRegistro[i].LengthFiel; // utilizo para contar el tamano total de cada entrada de la tabla
//    printf("\n %s", ptrSubrecord[i].FieldName);
//    LRegisterSize += ptrSubrecord[i].LengthFiel; // utilizo para contar el tamano total de cada entrada de la tabla
  }
  /* the whole file is now loaded in the memory buffer. */
  PrintDBFDescriptorFile(DBFBuf);
  PrintDBFDescriptorFile( Descriptor.Descriptor );
  printf("\n Longitud de cada registro(calc sum) es:%d",(int)LRegisterSize);
  printf("\n descripcion de cada celda del registro:");

//  for(i=0; i< NumeroCeldas ;i++)
  for(i=0; i< Descriptor.CampoXRegistro ;i++)
  {
    //printf("\n %s", ptrSubrecord[i].FieldName);
    //PrintSubRecordInfo(ptrSubrecord[i]);
    PrintSubRecordInfo(Descriptor.ptTCedasRegistro[i]);

  }

  //imprimimos un par de registros a modo ejemplo
  //debemos reservar memoria e ir leyendo los registros de forma incremental
  // nos posiscionamos donde empiezan los registros.


//aun no arreglado
/*
  lSize = ftell (pFile);
  fseek(pFile, DBFBuf.PostFisrtDataRecord, SEEK_SET);
  lSize = ftell (pFile);

  RegisterBuf = (char*) malloc (sizeof(char)*LRegisterSize*MULTIPLO);
  if (RegisterBuf == NULL) {fputs ("Memory error",stderr); exit (3);}

  result = fread (RegisterBuf ,1,LRegisterSize*MULTIPLO, pFile);
  if (result != LRegisterSize*MULTIPLO) {fputs ("Reading error",stderr); exit (2);}
*/
  if(ReadRecordByNum(4, &Descriptor, &RegisterBuf) )
  { exit (3);} // debe ser un switch

  printf("\n0 1 2 3 4 5 6 7 8 9 A B C D E F\n");
//  for(i=0;i<LRegisterSize*MULTIPLO;i++ ){
  for(i=0;i<Descriptor.LongitudRegistro;i++ ){
    printf("%c ",RegisterBuf[i]);
    if((i%16==0) && (i>0))
      printf("\n");
    if((i%LRegisterSize==0) && (i>0))
    {
      printf("\n===============================\n");
      printf("\n0 1 2 3 4 5 6 7 8 9 A B C D E F\n");
    }
  }

  free(RegisterBuf);
  // ya termien entonces libero la memoria que pedi.
  DBFClose(Descriptor);



  printf("\n Fin de lectura del archivo DBF!\n");

  return 0;
}
