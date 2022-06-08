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
      -[] Agregar funciones que me permitar leer de a un registro a la vez
      -[] Leer de a un registro por vez en cualquier posicion, pero indexado.
      -[] Imprimir los datos del registro completo.
  * Etapa 2
      -[] Crear esportacion a un archivo cvs.
      -[] Crear archivo para crear la misma tabla en sql.
      -[] Crear arcivo que genere una injeccion de los datos en SQL(Ajonco los tamanos).
*/

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

//Abrimos el archivo y guardamos la informacion encesaria para liberarla cunado cerramos todo
// Al abrir un archivo debo generar un resumen until de la informacion del mismo para realizar todas las operaciones
// El problema es que debo tener en memoria el formato de para interpretar cada campo del registro

stDBF_DescritorFile DBFOpen(char*)
{

}

ReadRecordByNum(unsigned int Index)
{

}

char DBFClose(char*)
{

}


