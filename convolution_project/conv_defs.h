#ifndef CONV_DEFS_H
#define CONV_DEFS_H

#define TEST_LEN 2048
#define CONV_LEN (TEST_LEN*2 - 1)

// yes, both are necessary if you pass a macro to STR
#define PROTOSTR(x) #x
// expands the macro before stringitization
#define STR(x) PROTOSTR(x)

#endif