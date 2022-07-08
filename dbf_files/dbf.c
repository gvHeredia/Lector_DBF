/*
 *        File: dbf.c
 *       Fecha:
 *       Autor: Gabriel Vasqez H.
 * Descripcion:
 *      En este modulo implementamos las funciones para porder leer
 *      y manipular los datos en los archisovs dbf
*/
#include "dbf.h"
/*
  TODO:
  * Etapa 1
      -[] Funcion que me de la cantidad total de resgistros
      -[x] Agregar funciones que me permitar leer de a un registro a la vez
      -[x] Leer de a un registro por vez en cualquier posicion, pero indexado.
      -[x] Imprimir los datos del registro completo.
  * Etapa 2
      -[] Crear exportacion a un archivo cvs.
      -[] Crear archivo para crear la misma tabla en sql.
      -[] Crear arcivo que genere la injeccion de los datos en SQL(Ojo con los tamanos).
*/
// Declaracionde  variables locales al modulo

// stDescriptor



//La funcion PrintDBFDescriptorFile imprime los capos del encabezado del archivo para informarlos**/
void PrintDBFDescriptorFile(stDBF_DescritorFile DBFDescriptor)
{
// Imprimimos el tipo de archivo
  printf("\nel tipo de archivo es:\t");

  switch(DBFDescriptor.DBFType)
  {
    case 0x02:   printf("FoxBASE                                           "); break;
    case 0x03:   printf("FoxBASE+/Dbase III plus, no memo                  "); break;
    case 0x30:   printf("Visual FoxPro                                     "); break;
    case 0x31:   printf("Visual FoxPro, autoincrement enabled              "); break;
    case 0x32:   printf("Visual FoxPro with field type Varchar or Varbinary"); break;
    case 0x43:   printf("dBASE IV SQL table files, no memo                 "); break;
    case 0x63:   printf("dBASE IV SQL system files, no memo                "); break;
    case 0x83:   printf("FoxBASE+/dBASE III PLUS, with memo                "); break;
    case 0x8B:   printf("dBASE IV with memo                                "); break;
    case 0xCB:   printf("dBASE IV SQL table files, with memo               "); break;
    case 0xF5:   printf("FoxPro 2.x (or earlier) with memo                 "); break;
    case 0xE5:   printf("HiPer-Six format with SMT memo file               "); break;
    case 0xFB:   printf("FoxBASE                                           "); break;
  }
  printf("\nLa fecha de la ultima actualizacione es:\t %d/%d/%d",DBFDescriptor.lastUpdate[2] ,DBFDescriptor.lastUpdate[1] ,DBFDescriptor.lastUpdate[0] );
  printf("\nCantidad de registros:\t%d", DBFDescriptor.NumberRecords);
  printf("\nOffset del primer dato:\t%d", DBFDescriptor.PostFisrtDataRecord);
  printf("\nlongitud del registro de dato:\t%d", DBFDescriptor.LengthDataRecord);

  printf("\nFlag de la tabla:\t%x", DBFDescriptor.TableFlag);
  printf("\nMarca dle codigo de pagina:\t%x", DBFDescriptor.CodePageMark);



}

//GetFieldTypeDescription funcion auxiliar para PrintSubRecordInfo
char* GetFieldTypeDescription(char FieldType)
{
  switch(FieldType)
  {
//    case 'C':   return("Character                    "); break;
    case 'C':   return("Character /Character (binary)"); break;
    case 'Y':   return("Currency (Visual Foxpro)     "); break;
    case 'N':   return("Numeric                      "); break;
    case 'F':   return("Float                        "); break;
    case 'D':   return("Date                         "); break;
    case 'T':   return("DateTime (Visual Foxpro)     "); break;
    case 'B':   return("Double (Visual Foxpro)       "); break;
    case 'I':   return("Integer (Visual Foxpro)      "); break;
    case 'L':   return("Logical                      "); break;
    //case 'M':   return("Memo                         "); break;
    case 'M':   return("Memo/Memo (binary)           "); break;
    case 'G':   return("General                      "); break;
    case 'P':   return("Picture                      "); break;
    case '+':   return("Autoincrement (dBase Level 7)"); break;
    case 'O':   return("Double (dBase Level 7)       "); break;
    case '@':   return("Timestamp (dBase Level 7)    "); break;
    case 'V':   return("Varchar type (Visual Foxpro) "); break;
  }
  return("---UKNONE TYPE---");
}

// La fucnion PrintSubRecordInfo(stDBFSubRecord Subrecord) imprime los detalles de cada campo que forma el registro
void PrintSubRecordInfo(stDBFSubRecord Subrecord)
{
  printf("\nNombre del campo:\t%s",Subrecord.FieldName);
  printf("\n\tTipo de datos del campo:\t%c",Subrecord.FileType);
    printf(" Descricion: %s",GetFieldTypeDescription(Subrecord.FileType) );

  printf("\n\tDisplacement:\t%x.%x.%x.%x",Subrecord.Displacemente[0],Subrecord.Displacemente[1],Subrecord.Displacemente[2],Subrecord.Displacemente[3]);
  printf("\n\tLongitus del campo en byte:\t%d",Subrecord.LengthFiel);
  printf("\n\tCantidad decimales:\t%d",Subrecord.NumberDecimal);
  printf("\n\tFlag:\t%x",Subrecord.FileFlags );

  printf("\n\tValor del proximo:\t%x-%x-%x-%x",Subrecord.ValueAutNextValue[0],Subrecord.ValueAutNextValue[1],Subrecord.ValueAutNextValue[2],Subrecord.ValueAutNextValue[3] );
  printf("\n\tPaso de incre:\t%x",Subrecord.AutoIncStep );
}

/*
  Abrimos el archivo y carmos la informacion necesaria para realizar las operaciones que se nos pidan
  dentro del archivo y luego poder cerarlo cuanod ya no se use mas (analizar co cuidado que memoria y
  cuando liberarla.
  Al abrir un archivo debo generar un resumen de la informacion del mismo para realizar todas las operaciones
  El problema es que debo tener en memoria el formato de cada campo para interpretarlos cuando pidan
  informacion de o los registros.
  Segun como salio todo retornamos un codigo de error para saber como salio todo.
  * 0 Si todo sale bien
  * -1 si se quedo sin memoria.
*/
char DBFOpen(stDescriptor* Descriptor, char* Nombre)
{
  FILE * pFile;
  int i, NumeroCeldas;
  long  lSize,
        LRegisterSize;    // LRegisterSize es el tamano de cada registro(entrada) de la tabla.
  char * buffer, *RegisterBuf ;
  size_t result;
  stDBF_DescritorFile DBFBuf;
  stDBFSubRecord *ptrSubrecord;

/*
  iniicilaizo los datos de la estructura pero tambien debo deteccta si fu usada antes para no
  perder los punteros d la memoria que esta reservada.
*/
  Descriptor->CampoXRegistro = 0; ///?????
  Descriptor->CeldasXRegistro =0 ;
//  Descriptor->ptrDescriptor = NULL;
  Descriptor->ptTCedasRegistro = NULL;

  //pFile = fopen ( Nombre , "rb" );
  Descriptor->fp = fopen ( Nombre , "rb" );
  // verifico si se puede abrir el archivo sino error!!!
//  if (pFile==NULL)
  if (Descriptor->fp == NULL)
    return (1);//{fputs ("File error",stderr); exit (1);}

  // OK entonces salvo el file poiter
  //Descriptor.fp = pFile;
/*
  // Obtenemos el tamano del archivo. para que??
  fseek (Descriptor.fp , 0 , SEEK_END);
  lSize = ftell (Descriptor.fp);
  rewind (Descriptor.fp);
*/

  //Leemos el encabezado del archivo y lo guardamos en la estructura
  result = fread (&DBFBuf,1,sizeof(stDBF_DescritorFile), Descriptor->fp);
  if (result != sizeof(stDBF_DescritorFile))
    return (2); //  {fputs ("Reading error",stderr); exit (2);}
{
  // Reservo memoria para obtener todos los SubRecords.
  //lSize = DBFBuf.PostFisrtDataRecord ;
  //lSize = lSize - 2 -32;  // 2 byte se deben al 0x0d y al 0x00 que estan
                          // al final del registro
  memcpy(&(Descriptor->Descriptor),&DBFBuf, sizeof(stDBF_DescritorFile));
  Descriptor->TamanoDescripcionReg = (DBFBuf.PostFisrtDataRecord) - 2-sizeof(stDBF_DescritorFile);//32;
  printf("\nByte de subregistro %d", Descriptor->TamanoDescripcionReg);
  Descriptor->CominzoDatos = DBFBuf.PostFisrtDataRecord;
  Descriptor->LongitudRegistro = DBFBuf.LengthDataRecord;

//  buffer = (char*) malloc (sizeof(char)*lSize);
  lSize = sizeof(char)*(Descriptor->TamanoDescripcionReg);
  buffer = (char*) malloc (lSize);
  if (buffer == NULL)
    return (-1); // {fputs ("Memory error",stderr); exit (3);}

  // Reservo memoria para leer la descripcion de los subregistros.
  result = fread (buffer,lSize,1,  Descriptor->fp);
  ptrSubrecord = (stDBFSubRecord*)buffer;
  Descriptor->ptTCedasRegistro = (stDBFSubRecord*)buffer;
}

  //NumeroCeldas = lSize/sizeof(stDBFSubRecord);
  Descriptor->CampoXRegistro = (Descriptor->TamanoDescripcionReg)/sizeof(stDBFSubRecord);

  return (0);
}

/**
 * @brief 
 *       Para poder leer un registro tengo que tener el largo del registro, esto lo obtrengo de la estructura del
 *       archivo que me da el largo de cada registro, luego necesito la posicicon done empieza el regirto, lo que
 *       es facil ya que es N veces el tamano mas el offset de arranque de la seccion delntro del archivo.
 * @param Index 
 *  Numero de resguidstro de deseamos leer.
 * @param Descriptor 
 *  Puntero al descripto utilizado para llevar la informacion del archivo leido.
 * @param RegisterBuf 
 *  Puntero al buffer donde se guardaran los datos leidos.
 * @return char 
 */

char ReadRecordByNum(unsigned int Index, stDescriptor* Descriptor, char** RegisterBuf)
{
  size_t result;
  // Posiciono el filepointer donde comienzan los datos
//  fseek(pFile, DBFBuf.PostFisrtDataRecord, SEEK_SET);
// TODO:  Falta hacer todas las validacione.

  fseek(Descriptor->fp, Descriptor->CominzoDatos, SEEK_SET);

  //RegisterBuf = (char*) malloc (sizeof(char)*LRegisterSize*MULTIPLO);
  *RegisterBuf = (char*) malloc (sizeof(char)*(Descriptor->LongitudRegistro));
//  if (RegisterBuf == NULL) {fputs ("Memory error",stderr); exit (3);}
  if (RegisterBuf == NULL) {return (3);}

//  result = fread (RegisterBuf ,1,LRegisterSize*MULTIPLO, pFile);
  result = fread (*RegisterBuf ,1,Descriptor->LongitudRegistro, Descriptor->fp);
//  if (result != LRegisterSize*MULTIPLO) {fputs ("Reading error",stderr); exit (2);}
//  if (result != LRegisterSize) {fputs ("Reading error",stderr); exit (2);}
  if (result != Descriptor->LongitudRegistro) {return(2);}
 return(0);
}


/**
 * @brief Esta fucnion nos permite leer varios registros de una vez 
 * a fucnion retorna la cantidad de registros que pudo leer, si o pudo leer ningun registro retorna 0 
 * y en caso que se produsca un error retorna numeros negativos.
 * 
 */

char ReadRecordRange(unsigned int StartIdx, unsigned int EndIdx, stDescriptor* Descriptor, char** RegisterBuf)
{
  size_t result;
  unsigned int FileOffset, BufferSize;
  //Primer debemos validar el rango solicitado ya sea que no esten cruzados como que esten dnentro de la cantidad 
  // de registros gaurdados
  if(StartIdx <= EndIdx)  
    return -1;                            // Rango invalido
  if(RegisterBuf == NULL)  
    return -4;                           // Destino invalido
 // TODO Validar con a cantidad de reistros almacenados.
  // Posiciono el filepointer donde comienzan los datos
  FileOffset = Descriptor->CominzoDatos + (StartIdx * sizeof(char)*(Descriptor->LongitudRegistro)) ;
  fseek(Descriptor->fp, FileOffset, SEEK_SET);                  // posciono el puntero del archivo en el registro que deseo leer
  BufferSize = sizeof(char)*(Descriptor->LongitudRegistro);     // Calculo cuanta memoria voy a necesitar para guardar los datos que pidio leer
 
  *RegisterBuf = (char*) malloc ( BufferSize);
  if (RegisterBuf == NULL) 
    return (-3);

  result = fread (*RegisterBuf ,1,Descriptor->LongitudRegistro, Descriptor->fp);
  if (result != Descriptor->LongitudRegistro) 
    return(-2);
 return(0);

}


char DBFClose(stDescriptor Descriptor)
{

  //Descriptor.ptTCedasRegistro;
  //Descriptor.ptTCedasRegistro;
  if(Descriptor.fp != NULL)
    fclose (Descriptor.fp);
  //free (Descriptor.ptrDescriptor); // Liberamos la memoria
  if(Descriptor.ptTCedasRegistro != NULL)
    free(Descriptor.ptTCedasRegistro );
  return 0;
}


