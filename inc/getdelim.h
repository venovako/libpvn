/* getdelim(3) and getline(3) for Windows */
#ifndef GETDELIM_H
#define GETDELIM_H

#include "pvn_ext.h"

#include <stdio.h>
#include <stdlib.h>

PVN_EXTERN_C ssize_t getdelim(char **restrict linep, size_t *restrict linecapp, int delimiter, FILE *restrict stream);
PVN_EXTERN_C ssize_t getline(char **restrict linep, size_t *restrict linecapp, FILE *restrict stream);

#endif /* !GETDELIM_H */
