#include <stdio.h>
#include "lexerDef.h"

twinBuffer initialiseBuffer(twinBuffer buffer)
{
    buffer = (twinBuffer)malloc(sizeof(struct twinBuffer));
    return buffer;
}