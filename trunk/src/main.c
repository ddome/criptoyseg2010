include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <string.h>

#include "imageUtils.h"
typedef struct {
    const char *name;
    int         has_arg;
    int        *flag;
    int         val;
} option;

struct option * InitOptionStruct(void);

int IsValid(int encrypt,int decrypt,char * key,char * method,char * alg,char * password,char * vi,int inOut,int out);

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
    
    imageInfo * info=NULL;
    
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
	case ':':
	    printf("La opcion %s requiere que ingrese un parametro.\n",options[optind].name);/*TODO Arreglar el caso en que no se ingresa un parametro*/
	    error=1;
	    break;
	case '?':
	    printf("La opcion %s requiere que ingrese un parametro.\n",options[optind].name);/*TODO Arreglar el caso en que no se ingresa un parametro*/
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
        if(IsValid(encrypt,decrypt,key,method,alg,password,vi,in,out))
        {
	srcFile=fopen(srcPath,"r");
	if(srcFile==NULL)
	{
	    printf("ERROR: No se puedo abrir el archivo de origen. Verifique que exista y tenga los permisos adecuados.\n");
	    exit(EXIT_FAILURE);
	}
	dstFile=fopen(dstPath,"w");
	if(dstFile==NULL)
	{
	    printf("ERROR: No se puedo abrir el archivo de destino. Verifique tener los permisos adecuados.\n");
	    exit(EXIT_FAILURE);
	}
	info=GetImageInfo(srcFile);
	if(info==NULL)
	{
	    printf("El formato de la imagen es incorrecto.\n");
	    exit(EXIT_FAILURE);
	}
	
        }
        else
        {
	PrintHelp();
        }
    }
    exit(EXIT_SUCCESS);
}

int
IsValid(int encrypt,int decrypt,char * key,char * method,char * alg,char * password,char * vi,int in,int out)
{
    int error=1;
    /*Valido la utilizacion de in y out*/
    if(!in && !out)
    {
        printf("ERROR: ");
        printf("Debe especificar el parametro -in y -out.\n");
        error=0;
    }
    
    /*Valido la utilizacion de encrypt y decrypt*/
    if(encrypt && decrypt)
    {
        printf("ERROR: ");
        printf("No es posible utilizar los parametros -e y -d en simultaneo.\n");
        error=0;
    }
    if(!encrypt && !decrypt)
    {
        printf("ERROR: ");
        printf("Debe especificar el parametro -e o -d.\n");
        error=0;
    }
    
    /*Valido el algoritmo de encripcion*/
    if(alg==NULL || (strncmp(alg,"aes",3)!=0 && strncmp(alg,"des",3)!=0))
    {
        printf("ERROR: ");
        printf("Debe especificar un algoritmo de incripcion valido -m [aes|des].");
        if(method!=NULL)
        {
	printf("Usted ingreso %s.\n",alg);
        }
        else
        {
	printf("\n");
        }
        error=0;
    }
    
    /*Valido el metodo de encripcion*/
    if(method==NULL || (strncmp(method,"ecb",3)!=0 && strncmp(method,"cfb",3)!=0 && strncmp(method,"ofb",3)!=0 && strncmp(method,"cbc",3)!=0))
    {
        printf("ERROR: ");
        printf("Debe especificar un metodo de encripcion valido -a [ecb|cfb|ofb|cbc].");
        if(method!=NULL)
        {
	printf("Usted ingreso %s.\n",method);
        }
        else
        {
	printf("\n");
        }
        error=0;
    }
    
    /*Valido la autenticacion*/
    if(password==NULL && (key==NULL || vi==NULL))
    {
        if(password==NULL && key==NULL && vi==NULL)
        {
	printf("Debe ingresar los datos de autenticacion. Ya sea usando -password o -K en conjunto con -vi.\n");
        }
        else if(password!=NULL && (key!=NULL || vi!=NULL))
        {
	printf("ERROR: ");
	printf("No es posible utilizar los parametros -password y -key o -vi en simultaneo\n");
	error=0;
        }
        else if(password==NULL && ((key!=NULL || vi==NULL) || (key==NULL || vi!=NULL)))
        {
	printf("ERROR: Las opciones -K y -vi deben ser utilizadas en conjunto.\n");
	error=0;
        }
    }
    return error;
}

void
PrintHelp(void)
{
    printf("TODO completar help\n");
}