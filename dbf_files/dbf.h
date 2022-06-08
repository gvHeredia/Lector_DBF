#ifndef DBF_H_INCLUDED
#define DBF_H_INCLUDED

/*
# Estructuras dentro del archivo DBF
## DBF File Header

|Byte offset	     |       Description    |
|------------------|:----------------------:|
|0	| DBF File type:|
|   | 0x02   FoxBASE  |
|   | 0x03   FoxBASE+/Dbase III plus, no memo  |
|   | 0x30   Visual FoxPro  |
|   | 0x31   Visual FoxPro, autoincrement enabled  |
|   | 0x32   Visual FoxPro with field type Varchar or Varbinary  |
|   | 0x43   dBASE IV SQL table files, no memo  |
|   | 0x63   dBASE IV SQL system files, no memo  |
|   | 0x83   FoxBASE+/dBASE III PLUS, with memo  |
|   | 0x8B   dBASE IV with memo  |
|   | 0xCB   dBASE IV SQL table files, with memo  |
|   | 0xF5   FoxPro 2.x (or earlier) with memo  |
|   | 0xE5   HiPer-Six format with SMT memo file  |
|   | 0xFB   FoxBASE  |
|1 - 3        | Last update (YYMMDD)  |
|4 - 7        | Number of records in file  |
|8 - 9        | Position of first data record  |
|10 - 11      | Length of one data record, including delete flag  |
|12 - 27      |Reserved  |
|28           |Table flags:  |
|             | 0x01   file has a structural .cdx  |
|             | 0x02   file has a Memo field  |
|             | 0x04   file is a database (.dbc)  |
|             | This byte can contain the sum of any of the above values. For example, the value 0x03 indicates the table has a structural .cdx and a Memo field.  |
|29	          |Code page mark  |
|30 - 31	    |Reserved, contains 0x00  |
|32 - n	      |Field subrecords The number of fields determines the number of field subrecords. One field subrecord exists for each field in the table.  |
|n+1	        |Header record terminator (0x0D)  |
|n+2 to n+264	|Visual Foxpro only: A 263-byte range that contains the backlink, which is the relative path of an associated database (.dbc) file, information. If the first byte is 0x00, the file is not associated with a database. Therefore, database files always contain 0x00.  |


## Field Subrecords Structure

|Byte offset	|Description |
|-------|:-------:|
|0 - 10 | Field name with a maximum of 10 characters. If less than 10, it is padded with null characters (0x00).|
|11	    | Field type:|
|  	    |     C   -   Character|
|  	    |     Y   -   Currency (Visual Foxpro)|
|  	    |     N   -   Numeric|
|  	    |     F   -   Float|
|  	    |     D   -   Date|
|  	    |     T   -   DateTime (Visual Foxpro)|
|  	    |     B   -   Double (Visual Foxpro)|
|  	    |     I   -   Integer (Visual Foxpro)|
|  	    |     L   -   Logical|
|  	    |     M   - Memo|
|  	    |     G   - General|
|  	    |     C   -   Character (binary)|
|  	    |     M   -   Memo (binary)|
|  	    |     P   -   Picture|
|  	    |     +   -   Autoincrement (dBase Level 7)|
|  	    |     O   -   Double (dBase Level 7)|
|  	    |     @   -   Timestamp (dBase Level 7)|
|  	    |     V   -   Varchar type (Visual Foxpro)|
|12 - 15|	Displacement of field in record|
|16     | Length of field (in bytes)|
|17     | Number of decimal places  |
|18     | Field flags:              |
|  	    |     0x01   System Column (not visible to user)|
|  	    |     0x02   Column can store null values|
|  	    |     0x04   Binary column (for CHAR and MEMO only)|
|  	    |     0x06   (0x02+0x04) When a field is NULL and binary (Integer, Currency, and Character/Memo fields)|
|  	    |     0x0C   Column is autoincrementing|
|19 - 22|	Value of autoincrement Next value|
|23     | Value of autoincrement Step value|
|24 - 31| Reserved|

##Cuidado.
Si el formato en dBaseIII tiene los iguientes limites
  Cantidad maxima de campos (fields):128
  Logitud maxima de caracteres permitida en los campos de caracteres: 254

*/


/*
  La etrucura stDBF_DescritorFile nos da la informacion
  general del archivo para poder leerlo,
*/
typedef struct{
  unsigned char DBFType; // 1 byte
  char lastUpdate[3];    // 3 byte YYMMDD
  int  NumberRecords;    // 4-7: 4 byte cantidad de registros guardados
  short PostFisrtDataRecord;  // 8-9 2 byte posicion en el archivo donde comiezan los datos
  short LengthDataRecord;     // 10-11 2 byte
  char  Reserved[16];         // 12-27 16 byte reservados
  char  TableFlag;            // 28 1byte
  char  CodePageMark;         // 29 1byte
  char  Reserved_1[2];         // 30-31 2 byte reservados
}stDBF_DescritorFile;

/*
  Limitations for dBase III format
  Maximum number of fields: 128
  Maximum length of Character field: 254
*/

/*
  Esta estructura es la utilizada para almacenar los datos
  de cada celda que forman el registro de las tabla
*/
typedef struct {
  char FieldName[11];     // 0-10  Nmbr el campo si es menos de 10 se llena con NULL
  char FileType;          // 11
  char Displacemente[4];  // 12-15 12.13.14.15 = 4byte
  char LengthFiel,        // longitud del cammpo
       NumberDecimal,     // 17
       FileFlags;         // 18
   char ValueAutNextValue[4]; // 19-22 19.20.21.22: 4 byte
   char AutoIncStep;               // 23
   char Reserved[8];               //24-31 24.25.26.27.28.29.30.31: 8 byte
}stDBFSubRecord;


// Prototipos

void PrintDBFDescriptorFile(stDBF_DescritorFile DBFDescriptor);
char* GetFieldTypeDescription(char FieldType);
void PrintSubRecordInfo(stDBFSubRecord Subrecord);

#endif // DBF_H_INCLUDED
