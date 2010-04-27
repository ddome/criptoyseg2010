#ifndef _CRYPT_H_
#define _CRYPT_H_

#include <stdio.h>
#include <string.h>
#include <openssl/des.h>
#include <openssl/aes.h>

#include "imageUtils.h"

/* Funcion principal del programa.
 * encrypt:	Vale 1 si se desea encriptar. 0 en caso contrario.
 * decrypt: Vale 1 si se desea desencriptar. 0 en caso contrario.
 * key:	Vale NULL si no especifico un password. Es decir que
 *	password !=NULL
 * method:	Especifica el metodo de encriptacion. Valores posibles
 *	cbc, ebc, ofb o cfb.
 * alg:	Especifica el algoritmo de encriptacion. Valores
 *	posibles aes o des.
 * password:Vale NULL si se especico key y vi. Es decir si key!=NULL
 *	vi!=NULL.
 * vi:	Vale NULL si se especifico password. Esdecir que
 *	password!=NULL.
 * srcFile:	Puntero al archivo original que contiene la imagen a
 *	encriptar.
 * dstFile:	Puntero al archivo donde almacenar la imagen ya encriptada
 * info:	Estructura que contiene la informacion extraida del
 *	header de la imagen.
 *
 * La funcion devuelve 1 si todo salio bien. 0 en caso contrario
 */
int Start(int encrypt,int decrypt,char * key,char * method,char * alg,char * password,char * vi,FILE * srcFile,FILE * dstFile,imageInfo info);

#endif
