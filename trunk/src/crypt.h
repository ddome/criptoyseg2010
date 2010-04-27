#ifndef _CRYPT_H_
#define _CRYPT_H_

#include <stdio.h>
#include <string.h>
#include <openssl/des.h>
#include <openssl/aes.h>

#include "imageUtils.h"

int Start(int encrypt,int decrypt,char * key,char * method,char * alg,char * password,char * vi,FILE * srcFile,FILE * dstFile,imageInfo info);

#endif
