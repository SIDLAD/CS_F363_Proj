#define BUFFERSIZE 256
int currentlineNumber = 0;
char twinBuffer[2][256];

typedef struct tokenInfo tokenInfo;
struct tokenInfo
{
    char tokenName[100];
    char lexeme[20];
    int lineNumber;
};