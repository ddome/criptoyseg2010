#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <string.h>

#include "crypt.h"
#include "imageUtils.h"

typedef struct {
    const char *name;
    int         has_arg;
    int        *flag;
    int         val;
} option;

struct option * InitOptionStruct(void);

int IsValid(int encrypt,int decrypt,char * key,char * method,char * alg,char * password,char * vi,int inOut,int out);

imageInfo ValidateImageFormat(imageInfo info);

void PrintHelp(void);

int
main(int argc,char * argv[])
{
    int encrypt=0;
    int decrypt=0;
    int opt=0;
    int indexptr=0;
    int in=0;
    int out=0;
    int help=0;
    int error=0;
    
    char * method=NULL;
    char * key=NULL;
    char * srcPath=NULL;
    char * dstPath=NULL;
    char * vi=NULL;
    char * password=NULL;
    
    char * alg=NULL;
    FILE * srcFile;
    FILE * dstFile;
    
    imageInfo info=NULL;
    
    static struct option options[11] =
             {
	    {"m",required_argument,0,'m'},
	    {"a",required_argument,0,'a'},
	    {"K",required_argument,0,'K'},
	    {"d",no_argument,0,'d'},
	    {"e",no_argument,0,'e'},
	    {"in",required_argument,0,'i'},
	    {"out",required_argument,0, 'o'},
	    {"password",required_argument,0,'p'},
	    {"vi",required_argument,0,'v'},
	    {"help",no_argument,0,'h'},
	    {0,0,0,0}
             };

    /*Uso getopt para parsear los argumentos pasados por lines de comando.*/
    while(!error && !help && (opt = getopt_long_only(argc, argv, "edK:a:m:h",options,&indexptr)) != -1)
    {
        switch(opt)
        {
	case 'e':
	    encrypt=1;
	    break;
	case 'd':
	    decrypt=1;
	    break;
	case 'K':
	    key=optarg;
	    break;
	case 'm':
	    method=optarg;
	    break;
	case 'i':
	    in=1;
	    srcPath=optarg;
	    break;
	case 'o':
	    out=1;
	    dstPath=optarg;
	    break;
	case 'a':
	    alg=optarg;
	    break;
	case 'p':
	    password=optarg;
	    break;
	case 'v':
	    vi=optarg;
	    break;
	case 'h':
	    help=1;
	    break;
	case -1 :
	case ':':
	case '?':
	    printf("La opcion %s requiere que ingrese un parametro.\n",options[optind].name);
	    error=1;
	    break;
        }
    }
    
    if(error || help)
    {
        PrintHelp();
    }
    else
    {
        /*Verifico que si la combinacion de parametros ingrasados es correcta. Si lo es proceso la imagen. En caso contrario
         *muestro el Help y devuelvo error.
         */
        if(IsValid(encrypt,decrypt,key,method,alg,password,vi,in,out))
        {
	srcFile=fopen(srcPath,"rb");
	if(srcFile==NULL)
	{
	    printf("ERROR: No se puedo abrir el archivo de origen. Verifique que exista y tenga los permisos adecuados.\n");
	    exit(EXIT_FAILURE);
	}
	dstFile=fopen(dstPath,"w");
	if(dstFile==NULL)
	{
	    printf("ERROR: No se puedo abrir el archivo de destino. Verifique tener los permisos adecuados.\n");
	    fclose(srcFile);
	    exit(EXIT_FAILURE);
	}
	info=GetImageInfo(srcFile); /*Obtengo la informacion del header de la imagen*/
	info=ValidateImageFormat(info);
	if(info==NULL)
	{
	    printf("ERROR: El formato de la imagen es incorrecto.\n");
	    fclose(srcFile);
	    fclose(dstFile);
	    free(info);
	    exit(EXIT_FAILURE);
	}
	PrintImageInfo(info);
	printf("Encriptando... ");
	/*Aca se procesa la imagen en funcion de los parametros pasados.*/
	if(!Start(encrypt,decrypt,key,method,alg,password,vi,srcFile,dstFile,info))
	{
	    printf("Fallo!\n");
	    printf("ERROR: Se produjo un error inesperado.\n");
	    fclose(srcFile);
	    fclose(dstFile);
	    free(info);
	    exit(EXIT_FAILURE);
	}
	printf("Listo!\n");
	fclose(srcFile);
	fclose(dstFile);
	free(info);
        }
        else
        {
	PrintHelp();
	exit(EXIT_FAILURE);
        }
    }
    exit(EXIT_SUCCESS);
}


int
IsValid(int encrypt,int decrypt,char * key,char * method,char * alg,char * password,char * vi,int in,int out)
{
    /*Valido la utilizacion de in y out*/
    if(!in && !out)
    {
        printf("ERROR: ");
        printf("Debe especificar el parametro -in y -out.\n");
        return 0;
    }
    
    /*Valido la utilizacion de encrypt o decrypt*/
    if(encrypt && decrypt)
    {
        printf("ERROR: ");
        printf("No es posible utilizar los parametros -e y -d en simultaneo.\n");
        return 0;
    }
    if(!encrypt && !decrypt)
    {
        printf("ERROR: ");
        printf("Debe especificar el parametro -e o -d.\n");
        return 0;
    }
    
    /*Valido el algoritmo de encripcion*/
    if(alg==NULL || (strncmp(alg,"aes",3)!=0 && strncmp(alg,"des",3)!=0))
    {
        printf("ERROR: ");
        printf("Debe especificar un algoritmo de incripcion valido -a [aes|des].");
        if(method!=NULL)
        {
	printf("Usted ingreso %s.\n",alg);
        }
        else
        {
	printf("\n");
        }
        return 0;
    }
    
    /*Valido el metodo de encripcion*/
    if(method==NULL || (strncmp(method,"ecb",3)!=0 && strncmp(method,"cfb",3)!=0 && strncmp(method,"ofb",3)!=0 && strncmp(method,"cbc",3)!=0))
    {
        printf("ERROR: ");
        printf("Debe especificar un metodo de encripcion valido -m [ecb|cfb|ofb|cbc].");
        if(method!=NULL)
        {
	printf("Usted ingreso %s.\n",method);
        }
        else
        {
	printf("\n");
        }
        return 0;
    }
    
    /*Valido la autenticacion*/
    if(password==NULL && (key==NULL || vi==NULL))
    {
        if(password==NULL && key==NULL && vi==NULL)
        {
	printf("ERROR: Debe ingresar los datos de autenticacion. Ya sea usando -password o -K en conjunto con -vi.\n");
	return 0;
        }
        else if(password!=NULL && (key!=NULL || vi!=NULL))
        {
	printf("ERROR: ");
	printf("No es posible utilizar los parametros -password y -key o -vi en simultaneo\n");
	return 0;
        }
        else if(password==NULL && ((key!=NULL || vi==NULL) || (key==NULL || vi!=NULL)))
        {
	printf("ERROR: Las opciones -K y -vi deben ser utilizadas en conjunto.\n");
	return 0;
        }
    }
    
    /*valido que para ebc se debe especificar el password*/
    if( strncmp(method,"ebc",3)==0 && (key!=NULL || vi!=NULL || password==NULL))
    {
        printf("ERROR: El metodo de encriptacion ecb solo puede ser utilizado en conjunto con el argumento -pass unicamente\n");
        return 0;
    }
    
    /*Si uso cfb, ofb o cbc debo especificar el password o el key en conjunto con vi*/
    if( (strncmp(method,"cfb",3)!=0 || strncmp(method,"ofb",3)!=0 || strncmp(method,"cbc",3)!=0) && (password==NULL && (key==NULL || vi==NULL) ))
    {
        printf("ERROR: Los metodos de encriptacion ebc ofb y cbc requieren que utilize el parametro -pass o -K y -vi en conjunto.\n");
        return 0;
    }
    
    return 1;
}

/*Valido el formato de la imagen*/
imageInfo
ValidateImageFormat(imageInfo info)
{
    /*Version 3.0 de BMP*/
    if(strncmp(info->identifier,"BM",2)!=0)
    {
        free(info);
        return NULL;
    }
    
    /*Bit per pixel menor a 24*/
    if(info->bpp<=24)
    {
        free(info);
        return NULL;
    }
    
    /*La imagen no esta comprimida*/
    if(info->compression!=0)
    {
        free(info);
        return NULL;
    }
    
    if(info->dataSize%8!=0 || info->dataSize%16!=0)
    {
        free(info);
        return NULL;
    }
    
    return info;
}

void
PrintHelp(void)
{
    printf("TODO completar help\n");
}