#define BUFFERSIZE 256
int currentlineNumber = 0;

typedef struct tokenInfo tokenInfo;
typedef struct twinBuffer twinBuffer;

struct tokenInfo
{
    char tokenName[100];
    char lexeme[20];
    int lineNumber;
};

struct twinBuffer
{
    char first[256];
    char second[256];
};