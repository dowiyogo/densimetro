#ifndef __TESTER_H
#define __TESTER_H

#include <stdio.h>

/********************************************************** API PARAMETERS ****/

#define __SPACES_TO_OKBAD   60

/******************************************************************** API *****/


#define TESTER_NAME(text)           \
    printf("%s\n", text);           \
    {                               \
        int __text_index=0;         \
        while(text[__text_index++]){\
            putchar('=');           \
        }                           \
        printf("\n\n");             \
    }

#define TESTER_DESCRIPTION(text)    \
    printf("\t%s\n\n", text);       \

#define TESTER_TEST(fooName, foo)   \
    {                               \
        int __fooResult = foo();    \
        int __text_index=0;         \
        char __fooNameIsNULL=fooName[0];\
        while(__text_index <__SPACES_TO_OKBAD){\
            if(__fooNameIsNULL){    \
                putchar(fooName[__text_index++]);\
                __fooNameIsNULL = fooName[__text_index];\
            }else{                  \
                putchar('.');       \
                __text_index++;     \
            }                       \
        }                           \
        if(!__fooResult){printf("[%s]\n","OK");}\
        else{printf("[%s][%d]\n","BAD",__fooResult);}\
    }

#endif // __TESTER_H
//Test OK....................
