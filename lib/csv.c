#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct
{
    size_t csz;
    char rgch[0];
} FIELDS;

// Assumes record CRLF has been replaced with null terminator.
FIELDS * CsvToFields(const char * pszRow)
{
    FIELDS * pfields = (FIELDS *) malloc(sizeof(FIELDS) + strlen(pszRow) + 1);
    const char * pchSrc = pszRow;
    char * pchDst = (char *) &pfields->rgch;
    bool fInQuote = false;

    if (!pfields)
        return NULL;

    pfields->csz = 0;
    while (*pchSrc)
    {
        if (*pchSrc == ';' && !fInQuote)
        {
            ++pfields->csz;
            *pchDst++ = '\0';
        }
        /*else if (*pchSrc == '"')
        {
            char chNext = pchSrc[1];

            if (chNext == '"' && fInQuote)
            {
                *pchDst++ = '"';
                ++pchSrc;
            }
            else
            {
                fInQuote = !fInQuote ||                        // enter field
                           !(chNext == ';' || chNext == '\0'); // leave field
            }
        }*/
        else
        {
            *pchDst++ = *pchSrc;
        }
        ++pchSrc;
    }
    *pchDst++ = '\0';
    ++pfields->csz;
    return pfields;
}

void PrintFields(const FIELDS * pfields)
{
    const char * psz = pfields->rgch;
    size_t i;
    for(i = 0; i < pfields->csz; ++i)
    {
        printf("A[%d]: %s\n", i, psz);
        psz += strlen(psz) + 1;
    }
}

