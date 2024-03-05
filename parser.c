#include "parserDef.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

parseTree _parseTree = NULL;                //to be initialized via function call
grammar _grammar = NULL;                    //to be initialized via function call
table _table = NULL;                        //to be initialized via function call
FirstAndFollow _firstAndFollow = NULL;      //to be initialized via function call
char* predictiveParsingTableCache = "predictiveParsingTableCache";

// function prototypes//
void insertGrammarSymbolIntoLLNode(LinkedList node, Vocabulary v);
void insertTokenInfoIntoTreeNode(TreeNode node, tokenInfo tkinf);
tokenInfo fetchTokenInfoFromTreeNode(TreeNode node);
TreeNode getNextTreeNode(TreeNode current);     //returns NULL means that the "abstract stack" is now empty, ie this is the root node
// void pushGrammarSymbolOntoStack(Stack _stack, Vocabulary v);
// Vocabulary popGrammarSymbolOntostack(Stack _stack);       //will return TOTAL_VOCAB_SIZE if _stack is empty

void populateFirstAndFollowText(FirstAndFollow F);
FirstAndFollow computeFirstAndFollowSets(grammar G);
TerminalBucketSet createSet();
FirstAndFollow initializeFirstAndFollow(FirstAndFollow _firstAndFollow, grammar G);
Vocabulary encodeStr(const char *str);
void enumToStr(Vocabulary v, char *dest);
grammar initializeGrammar(grammar g);
LinkedList *increaseProductionRHSSetSize(LinkedList *rhsSet, int *currentSize); // &ruleCount[i] will be passed as currentSize
grammar populateGrammar(grammar g, char *grammarFileName);
int getNTID(Vocabulary NT);
table initializeTable(table T);
table createParseTable(FirstAndFollow F, table T);
void initializeParseTree();
bool parseInputSourceCode(char *testcaseFile, table T);
void printParseTree(parseTree PT, char *outfile);       //lexeme CurrentNode lineno tokenName valueIfNumber parentNodeSymbol isLeafNode(yes/no) NodeSymbol
void freeParseTree();
void calculateFirstOfRule(TerminalBucketSet _firstOfRule, LinkedList RHSNode, bool *flag, FirstAndFollow F);
TerminalBucketSet firstOfNT(int NT_ID, FirstAndFollow F);
void appendSetUnion(TerminalBucketSet Dest, TerminalBucketSet Src, bool *flag);
bool isNTNode(LinkedList node);
int getTerminalIDFromLLNode(LinkedList node);
int getNTIDFromLLNode(LinkedList node);
int getTerminalIDFromTreeNode(TreeNode node);
int getNTIDFromTreeNode(TreeNode node);
TerminalBucketSet calculateFollowFromNTRule(TerminalBucketSet _followOfNT, LinkedList RHSNode, bool *flag, FirstAndFollow F);
//------------------//

// function definitions

LinkedList createLinkedListNode()
{
    LinkedList list = (LinkedList)malloc(sizeof(struct linkedList));
    list->next = NULL;
    list->data = NULL;
    return list;
}

TreeNode createTreeNode(TreeNode parent,void* data)
{
    TreeNode treeNode = (TreeNode)malloc(sizeof(struct treeNode));
    treeNode->parent=parent;
    treeNode->firstChild=NULL;
    treeNode->nextBrother=NULL;
    treeNode->data=data;
    return treeNode;
}

void createBrotherTreeNode(TreeNode current,void* data)
{
    current->nextBrother = createTreeNode(current->parent,data);
}

void createChildTreeNode(TreeNode current, void* data)
{
    if(current->firstChild == NULL)                     //our use case will be limited to this if-clause
    {
        TreeNode treeNode=createTreeNode(current,data);
        current->firstChild=treeNode;
        return;
    }

    current = current->firstChild;
    while(current->nextBrother != NULL)current = current->nextBrother;
    current->nextBrother = createTreeNode(current->parent,data);
}

void freeTreeNodeRecursive(TreeNode treeNode)
{
    if (treeNode == NULL) {
        return;
    }

    // Free all its children:
    TreeNode current = treeNode->firstChild;
    while(current != NULL)
    {
        TreeNode next = current->nextBrother;
        freeTreeNodeRecursive(current);
        current = next;
    }

    // Free the current treeNode itself
    free(treeNode->data);
    free(treeNode);
}

// Stack createStack()
// {
//     Stack stack = (Stack)malloc(sizeof(struct stack));
//     if (stack != NULL)
//     {
//         stack->top = NULL;
//     }
//     return stack;
// }

// bool isEmpty(Stack stack)
// {
//     return stack->top==NULL;
// }

// void push(Stack stack, void* data)
// {
//     StackNode newNode = (StackNode)malloc(sizeof(struct stackNode));
//     if (newNode != NULL)
//     {
//         newNode->data = data;
//         newNode->next = stack->top;
//         stack->top = newNode;
//     } 
//     else
//     {
//         printf("Error: Memory allocation failed.\n");
//     }
// }

// void* pop(Stack stack)
// {
//     if (stack->top == NULL)
//     {
//         // printf("Error: Stack is empty.\n");
//         return NULL;
//     }

//     StackNode topNode = stack->top;
//     void* data = topNode->data;
//     stack->top = topNode->next;
//     free(topNode);
//     return data;
// }

// void freeStack(Stack stack)
// {
//     while(!isEmpty(stack))pop(stack);
//     free(stack);
// }

//ADT functions defined above//

void insertGrammarSymbolIntoLLNode(LinkedList node, Vocabulary v)
{
    Vocabulary *tmp = (Vocabulary *)malloc(sizeof(Vocabulary));
    *tmp = v;
    node->data = tmp;
}

void insertTokenInfoIntoTreeNode(TreeNode node, tokenInfo tkinf)
{
    node->data = (void *)tkinf;
}

tokenInfo fetchTokenInfoFromTreeNode(TreeNode node)
{
    if(node == NULL)return NULL;

    return (tokenInfo)node->data;
}

TreeNode getNextTreeNode(TreeNode current)     //returns NULL means that the "abstract stack" is now empty, ie this is the root node
{
    while(current->nextBrother == NULL && current->parent != NULL)current = current->parent;
    return current->nextBrother;
}

// void pushGrammarSymbolOntoStack(Stack _stack, Vocabulary v)
// {
//     Vocabulary *tmp = (Vocabulary *)malloc(sizeof(Vocabulary));
//     *tmp = v;
//     push(_stack,tmp);
// }

// Vocabulary popGrammarSymbolOntostack(Stack _stack)       //will return TOTAL_VOCAB_SIZE if _stack is empty
// {
//     if(isEmpty(_stack))return TOTAL_VOCAB_SIZE;
//     return *(Vocabulary*)_stack->top;
// }

            //ADT Overlay functions defined above//
//--------------------------------------------------------------//

void populateFirstAndFollowText(FirstAndFollow F)
{
    char curNT[VOCAB_STRLEN_CAP];
    char tmpStr[VOCAB_STRLEN_CAP];
    TerminalBucketSet tmp = NULL;

    FILE *fp = fopen("first.txt", "w");

    // first.txt
    for (int i = 0; i < N_TERMINALS_COUNT; i++)
    {
        // i corresponds to non-terminal for which we are writing first and follow
        //  F->follow[i][]; //this is follow for given non-terminal
        enumToStr(i + TERMINALS_COUNT + 1, curNT);
        fprintf(fp, "%s : ", curNT);

        tmp = firstOfNT(i, F);
        for (int j = 0; j < TERMINALS_COUNT; j++)
            if (tmp->val[j])
            {
                enumToStr(j, tmpStr);
                fprintf(fp, " %s, ", tmpStr);
            }

        free(tmp);
        fprintf(fp, "\n\n");
    }
    fclose(fp);

    fp = fopen("follow.txt", "w");
    for (int i = 0; i < N_TERMINALS_COUNT; i++)
    {
        enumToStr(i + TERMINALS_COUNT + 1,curNT);
        fprintf(fp, "%s : ",curNT);

        tmp = F->follow[i];
        for(int j = 0; j < TERMINALS_COUNT; j ++)
            if(tmp->val[j])
            {
                if(j == ENDOFFILE)
                {
                    fprintf(fp," ENDOFFILE, ");  //do not free temp, needed since we are pointing to something we are using
                    
                }
                else
                {
                    enumToStr(j,tmpStr);
                    fprintf(fp," %s, ",tmpStr);
                }
            }
        fprintf(fp, "\n\n");
    }
    fclose(fp);
}

FirstAndFollow computeFirstAndFollowSets(grammar G) // G cannot be NULL
{
    // TODO: _firstAndFollow needs to be computed. First And Follow Files need to be generated as output

    _firstAndFollow = initializeFirstAndFollow(_firstAndFollow, G);
    bool flag = true;

    while (flag)
    {
        flag = false;
        for (int i = 0; i < N_TERMINALS_COUNT; i++)
        {
            for (int j = 0; j < _firstAndFollow->ruleCount[i]; j++)
            {
                calculateFirstOfRule(_firstAndFollow->first[i][j], G->NT[i][j], &flag, _firstAndFollow);
            }
        }
    }

    flag = true;

    while (flag)
    {
        flag = false;
        for (int i = 0; i < N_TERMINALS_COUNT; i++)
        {
            for (int j = 0; j < _firstAndFollow->ruleCount[i]; j++)
            {
                TerminalBucketSet tmp = calculateFollowFromNTRule(_firstAndFollow->follow[i], G->NT[i][j], &flag, _firstAndFollow);
                free(tmp);
            }
        }
    }
    populateFirstAndFollowText(_firstAndFollow);
}

TerminalBucketSet calculateFollowFromNTRule(TerminalBucketSet _followOfNT, LinkedList RHSNode, bool *flag, FirstAndFollow F)    //free up the returned pointer after recursive calls are done, where the first call was made.
{
    TerminalBucketSet tmp = NULL;
    if (isNTNode(RHSNode))
    {
        if (RHSNode->next == NULL)
        {
            appendSetUnion(F->follow[getNTIDFromLLNode(RHSNode)], _followOfNT, flag);
            tmp = firstOfNT(getNTIDFromLLNode(RHSNode),F);
            if(tmp->val[(int)EPS])
            {
                tmp->val[(int)EPS] = false;
                appendSetUnion(tmp, _followOfNT, NULL);
            }
            return tmp;
        }
        tmp = calculateFollowFromNTRule(_followOfNT, RHSNode->next, flag, F);
        appendSetUnion(F->follow[getNTIDFromLLNode(RHSNode)],tmp,flag);
        
        TerminalBucketSet tmp2 = firstOfNT(getNTIDFromLLNode(RHSNode),F);
        if(tmp2->val[(int)EPS])
        {
            tmp2->val[(int)EPS] = false;
            appendSetUnion(tmp2,tmp,NULL);
        }

        free(tmp);
        return tmp2;
    }

    if(RHSNode->next != NULL)   //if the current node is EPS, then this if block cannot execute, as next will definitely be NULL
    {
        tmp = calculateFollowFromNTRule(_followOfNT,RHSNode->next,flag,F);
    }

    if(getTerminalIDFromLLNode(RHSNode) == (int)EPS)
    {
        return tmp; // == NULL
    }

    tmp = createSet();
    tmp->val[getTerminalIDFromLLNode(RHSNode)] = true;
    return tmp;
}

void calculateFirstOfRule(TerminalBucketSet _firstOfRule, LinkedList RHSNode, bool *flag, FirstAndFollow F) // firstAndFollow F is being passed here because firstOfNT requires it.
{
    if (!isNTNode(RHSNode)) // if it is a terminal node. Note that if it the rule can derive EPS, then first set of rule should contain EPS
    {
        if (!_firstOfRule->val[getTerminalIDFromLLNode(RHSNode)])
        {
            _firstOfRule->val[getTerminalIDFromLLNode(RHSNode)] = true;
            *flag = true;
        }
        return;
    }

    TerminalBucketSet tmp = firstOfNT(getNTIDFromLLNode(RHSNode), F);

    if (!tmp->val[(int)EPS])
    {
        appendSetUnion(_firstOfRule, tmp, flag);

        free(tmp);
        return;
    }

    tmp->val[(int)EPS] = false;
    appendSetUnion(_firstOfRule, tmp, flag);
    free(tmp);

    if (RHSNode->next == NULL)
    {
        if (!_firstOfRule->val[(int)EPS])
        {
            _firstOfRule->val[(int)EPS] = true;
            *flag = true;
        }

        return;
    }

    calculateFirstOfRule(_firstOfRule, RHSNode->next, flag, F);
}

TerminalBucketSet firstOfNT(int NT_ID, FirstAndFollow F) // make sure to free the pointer after using it wherever it is called
{
    TerminalBucketSet first = createSet();

    for (int i = 0; i < TERMINALS_COUNT; i++)
    {
        if (first->val[i])
            continue;

        for (int j = 0; j < F->ruleCount[NT_ID]; j++)
        {
            first->val[i] |= F->first[NT_ID][j]->val[i];

            if (first->val[i])
                continue;
        }
    }

    return first;
}

void appendSetUnion(TerminalBucketSet dest, TerminalBucketSet src, bool *flag)
{
    for (int i = 0; i < TERMINALS_COUNT; i++)
        if (src->val[i] && !dest->val[i])
        {
            dest->val[i] = true;
            if(flag != NULL)
                *flag = true;
        }
}

bool isNTNode(LinkedList node) // returns true if the linkedlist node's data item is a non-terminal
{
    return getNTID(*((Vocabulary *)(node->data))) >= 0;
}

int getTerminalIDFromLLNode(LinkedList node)
{
    return (int)(*((Vocabulary *)(node->data)));
}

int getNTIDFromLLNode(LinkedList node)
{
    return getNTID(*((Vocabulary *)(node->data)));
}

int getTerminalIDFromTreeNode(TreeNode node)
{
    return (int)((tokenInfo )(node->data))->tokenName;
}

int getNTIDFromTreeNode(TreeNode node)
{
    return getNTID(((tokenInfo )(node->data))->tokenName);
}

TerminalBucketSet createSet()
{
    TerminalBucketSet set = (TerminalBucketSet)calloc(1, sizeof(struct terminalBucketSet)); // calloc used instead of malloc so that all bits are initialized to zero
    return set;
}

FirstAndFollow initializeFirstAndFollow(FirstAndFollow F, grammar G)
{
    F = (FirstAndFollow)malloc(sizeof(struct firstAndFollow));
    for (int i = 0; i < N_TERMINALS_COUNT; i++)
    {
        F->ruleCount[i] = G->ruleCount[i];
        F->follow[i] = createSet();
        F->first[i] = (TerminalBucketSet *)calloc(F->ruleCount[i], sizeof(TerminalBucketSet));
        for (int j = 0; j < F->ruleCount[i]; j++)
        {
            F->first[i][j] = createSet();
        }
    }

    F->follow[getNTID(STARTSYMBOL)]->val[ENDOFFILE] = true;
    return F;
}

grammar initializeGrammar(grammar g)
{
    if (g != NULL)  // in case g is not NULL, then that means that it is already initialized
        return g;

    g = (grammar)malloc(sizeof(struct grammar));
    for (int i = 0; i < N_TERMINALS_COUNT; i++)
    {
        g->NT[i] = NULL;
        g->ruleCount[i] = 0;
    }

    return g;
}

LinkedList *increaseProductionRHSSetSize(LinkedList *rhsSet, int *currentSize) // &ruleCount[i] will be passed as currentSize
{
    rhsSet = (LinkedList *)realloc(rhsSet, ((*currentSize)++ + 1) * sizeof(LinkedList));
    rhsSet[*currentSize - 1] = createLinkedListNode();
    return rhsSet;
}

grammar populateGrammar(grammar g, char *grammarFileName) // g cannot be NULL. If grammarFileName == NULL then grammarFileName := grammar.txt
{
    if (grammarFileName == NULL)
    {
        grammarFileName = (char *)malloc(MAX_FILENAME_LENGTH * (sizeof(char)));
        strcpy(grammarFileName, grammarFile);
    }

    char line[MAX_LINE_SIZE_GRAMMAR];
    FILE *fp = fopen(grammarFileName, "r");
    char *symbolString;
    Vocabulary symbol;
    int NT_ID;

    while (fgets(line, sizeof(line), fp) != NULL)
    {
        symbolString = strtok(line, " "); // this is our LHS

        symbol = encodeStr(symbolString);
        NT_ID = getNTID(symbol);

        strtok(NULL, " \n"); // skipping ===>

        symbolString = strtok(NULL, " \n"); // reading first symbolString of RHS
        symbol = encodeStr(symbolString);

        g->NT[NT_ID] = increaseProductionRHSSetSize(g->NT[NT_ID], &(g->ruleCount[NT_ID]));
        LinkedList currentNode = g->NT[NT_ID][g->ruleCount[NT_ID] - 1];
        insertGrammarSymbolIntoLLNode(currentNode, symbol);

        while (symbolString = strtok(NULL, " \n")) // RHS of rules onwards
        {
            if (strcmp(symbolString, "|") == 0)
            { // create a new head and populate for the new rule after |

                symbolString = strtok(NULL, " \n");
                symbol = encodeStr(symbolString);

                g->NT[NT_ID] = increaseProductionRHSSetSize(g->NT[NT_ID], &(g->ruleCount[NT_ID]));
                currentNode = g->NT[NT_ID][g->ruleCount[NT_ID] - 1];
                insertGrammarSymbolIntoLLNode(currentNode, symbol);
            }
            else
            {
                symbol = encodeStr(symbolString);
                // continue on the same head
                currentNode->next = createLinkedListNode();
                currentNode = currentNode->next;
                insertGrammarSymbolIntoLLNode(currentNode, symbol);
            }
        }
    }
    return g;
}

int getNTID(Vocabulary NT) // returns a negative value if not a non-terminal
{
    return (int)NT - TERMINALS_COUNT - 1;
}

table initializeTable(table T)
{
    if (T != NULL)  // in case T is not NULL, then that means that it is already initialized
        return T;

    T = (table)malloc(sizeof(struct table));

    for(int i=0;i<N_TERMINALS_COUNT;i++)
    {
        for(int j=0;j<TERMINALS_COUNT;j++)
        {
            T->cells[i][j] = NULL;
            T->isErrorCell[i][j] = 1;       //keeping them by default as error not sync cells.
        }
    }

    return T;
}


int cache_table(table T, char* predictiveParsingTableCache)
{
    //open the file in write mode, assuming it is a binary file
    FILE* file = fopen(predictiveParsingTableCache, "wb");
    if(file == NULL)
    {
        printf("Error: File could not be opened for writing.\n");
        return -1;
    }
    //write the table to the file
    //first write N_TERMINALS_COUNT and TERMINALS_COUNT to the file
    int temp = N_TERMINALS_COUNT;
    fwrite(&temp, sizeof(int), 1, file);
    temp = TERMINALS_COUNT;
    fwrite(&temp, sizeof(int), 1, file);
    //now write the table to the file
    for(int i = 0; i < N_TERMINALS_COUNT; i++)
    {
        for(int j = 0; j < TERMINALS_COUNT; j++)
        {
            fwrite(&T->isErrorCell[i][j], sizeof(int), 1, file);
            // traverse the linked list and write the number of nodes in the linked list to the file
            LinkedList node = T->cells[i][j];
            int count = 0;
            while(node != NULL)
            {
                count++;
                node = node->next;
            }
            fwrite(&count, sizeof(int), 1, file);
            // now traverse the linked list again and write the data to the file
            node = T->cells[i][j];
            while(node != NULL)
            {
                fwrite(node->data, sizeof(Vocabulary), 1, file);
                node = node->next;
            }
        }
    }
}

table retrieve_table(char* predictiveParsingTableCache)
{
    //open the file in read mode, assuming it is a binary file
    FILE* file = fopen(predictiveParsingTableCache, "rb");
    if(file == NULL)
    {
        printf("Error: File could not be opened for reading.\n");
        return NULL;
    }
    //read N_TERMINALS_COUNT and TERMINALS_COUNT from the file
    int temp = 0;
    fread(&temp, sizeof(int), 1, file);
    if(temp != N_TERMINALS_COUNT)
    {
        printf("Error: N_TERMINALS_COUNT does not match.\n");
        return NULL;
    }
    fread(&temp, sizeof(int), 1, file);
    if(temp != TERMINALS_COUNT)
    {
        printf("Error: TERMINALS_COUNT does not match.\n");
        return NULL;
    }
    //create a new table
    table T = initializeTable(NULL);
    //read the table from the file
    for(int i = 0; i < N_TERMINALS_COUNT; i++)
    {
        for(int j = 0; j < TERMINALS_COUNT; j++)
        {
            fread(&T->isErrorCell[i][j], sizeof(int), 1, file);
            // read the number of nodes in the linked list from the file
            int count;
            fread(&count, sizeof(int), 1, file);
            // now read the data from the file and create the linked list
            for(int k = 0; k < count; k++)
            {
                Vocabulary data;
                fread(&data, sizeof(Vocabulary), 1, file);
                if(k == 0)
                {
                    T->cells[i][j] = createLinkedListNode();
                    insertGrammarSymbolIntoLLNode(T->cells[i][j], data);
                }
                else
                {
                    LinkedList node = T->cells[i][j];
                    while(node->next != NULL)
                    {
                        node = node->next;
                    }
                    node->next = createLinkedListNode();
                    insertGrammarSymbolIntoLLNode(node->next, data);
                }
            }
        }
    }
    return T;
}

table createParseTable(FirstAndFollow F, table T) // F can be passed as NULL or _firstAndFollow. T has to be passed as _table.
{
    if (T == NULL)
    {
        _table = initializeTable(_table);
        T = _table;
    }
    else
    {
        printf("Parse Table has already been created.\n");
        return T;
    }

    if(access(predictiveParsingTableCache,F_OK) == 0)//this means that the cache file EXISTS
    {
        //populate the parsing table T from the cache'd file found (predictiveParsingTableCache is the string that contains the name of the file)
        T = retrieve_table(predictiveParsingTableCache);
        return T;
    }

    if (F == NULL)  //else _firstAndFollow (and _grammar) is already computed
    {
        _grammar = initializeGrammar(_grammar);
        _grammar = populateGrammar(_grammar, NULL);
        computeFirstAndFollowSets(_grammar);
        F = _firstAndFollow;
    }

    //populating the parsing table from _FirstAndFollow 

    int EPSDerivingRuleNumber[N_TERMINALS_COUNT];
    memset(EPSDerivingRuleNumber,-1,sizeof(EPSDerivingRuleNumber));     //setting all values of the array to -1

    for(int i=0; i<N_TERMINALS_COUNT; i++)
    {
        for(int j=0; j<_firstAndFollow->ruleCount[i]; j++)
        {
            for(int k=0;k<TERMINALS_COUNT;k++)
            {
                if(k == (int)EPS)
                {
                    if(! _firstAndFollow->first[i][j]->val[k])  // if this rule cannot derive EPS, then skip
                        continue;

                    if(EPSDerivingRuleNumber[i] == -1)
                    {
                        EPSDerivingRuleNumber[i] = j;   //EPS Deriving Rule found for the NT!
                    }
                    else
                    {
                        //more than one rule is deriving EPS for the same non-terminal, so grammar is not LL(1)
                        printf("Grammar is not LL(1). Terminating program.\n");
                        exit(0);
                    }
                }
                else if(_firstAndFollow->first[i][j]->val[k])
                {
                    if(! _table->isErrorCell[i][k])         //if multiple rules correspond to the same cell, then grammar is not LL(1)
                    {
                        printf("Grammar is not LL(1). Terminating program.\n");
                        exit(0);
                    }

                    _table->isErrorCell[i][k] = 0;
                    _table->cells[i][k] = _grammar->NT[i][j];
                }
            }
        }
    }

    for(int i=0; i<N_TERMINALS_COUNT; i++)
    {
        for(int j=0; j<TERMINALS_COUNT; j++)
        {
            bool forceSync = false;
            switch((Vocabulary)j)
            {
                //list of tokens that start a function or any of the statements in the function body:

                //FORCE_SYNC SET:// this set essentially captured anything following tokens such as semicolon and a few keywords such as endrecord, endunion, endif, endwhile, else, ), ] etc
                case TK_FUNID:
                case TK_MAIN:
                case TK_RECORD:
                case TK_UNION:
                case TK_DEFINETYPE:
                case TK_TYPE:
                case TK_ID:
                case TK_WHILE:
                case TK_IF:
                case TK_READ:
                case TK_WRITE:
                case TK_CALL:
                case TK_SQL:
                //these are some forceSync tokens that we would need to add to the syncset so that the programmer's intentions
                //are captured to minimise the number of errors ie changes, to a program with syntax errors.
                    forceSync = true;
            }

            //if the token belongs to the above set, or to the follow of the current Non-terminal, then:
            if(forceSync || _firstAndFollow->follow[i]->val[j])
            {
                if(EPSDerivingRuleNumber[i] != -1 && _firstAndFollow->follow[i]->val[j]) 
                {//there is NO ERROR if the current Non-terminal on stack-top can derive EPS, and a token from its follow-set is read
                    
                    if(! _table->isErrorCell[i][j]) //if the NT can derive EPS, but FIRST(NT)^FOLLOW(NT) is not empty, then grammar is not LL(1)
                    {
                        printf("Grammar is not LL(1).Terminating program.\n");
                        exit(0);
                    }

                    //if NT can derive EPS, and FIRST(NT)^FOLLOW(NT) is empty up till this terminal, then this cell is attached the corresponding EPS Deriving grammar Rule:
                    _table->isErrorCell[i][j] = 0;
                    _table->cells[i][j] = _grammar->NT[i][EPSDerivingRuleNumber[i]];                    
                }

                else if(_table->isErrorCell[i][j])
                {
                    //if the NT cannot derive EPS and terminal is not in FIRST(NT), then this cell is marked as err & sync => 2                    
                    _table->isErrorCell[i][j] = 2;
                }
            }
        }
    }

    //cache the predictive parsing table T in the cache file (predictiveParsingTableCache is the string that contains the name of the file)
    cache_table(T, predictiveParsingTableCache);
    return T;
}

bool isNTToken(tokenInfo tk)
{
    return getNTID(tk->tokenName) >= 0;
}

void initializeParseTree()
{
    _parseTree = (parseTree)malloc(sizeof(struct parseTree));
    _parseTree->root = createTreeNode(NULL,NULL);
}

char* removeAngularBrackets(char* str)
{
    if(str[0] == '<')
    {
        if(str[strlen(str) - 1] == '>')
            str[strlen(str) - 1] = '\0';
        return str + 1;
    }
    return str;
}

void* error_endOfParserReached(int linenumber,FILE* ptrs[],int size){
    printf("Line %d\t Error: End of Parser reached, but token-stream is not exhausted.\n",linenumber);
    for(int i=0; i<size;i++)
    fprintf(ptrs[i],"Line %d\t Error: End of Parser reached, but token-stream is not exhausted.\n",linenumber);  
}

void* error_endOfTokenStream(int linenumber,FILE* ptrs[],int size){
    printf("Line %d\t Error: End of token-stream reached, but parsing is not complete.\n",linenumber);
    for(int i=0; i<size;i++)
    fprintf(ptrs[i],"Line %d\t Error: End of token-stream reached, but parsing is not complete.\n",linenumber); 
}

void* error_invalidToken(tokenInfo tk_inp, tokenInfo tk_exp, FILE* ptrs[],int size){
    char tmp1[VOCAB_STRLEN_CAP],tmp2[VOCAB_STRLEN_CAP];
    enumToStr(tk_inp->tokenName,tmp1),enumToStr(tk_exp->tokenName,tmp2);
    if(isNTToken(tk_exp))
    {
        printf("Line %d\t Error: Invalid token %s encountered with value %s stack top %s\n",tk_inp->lineNumber,tmp1,tk_inp->lexeme,removeAngularBrackets(tmp2));
        for(int i=0; i<size;i++)
        fprintf(ptrs[i],"Line %d\t Error: Invalid token %s encountered with value %s stack top %s\n",tk_inp->lineNumber,tmp1,tk_inp->lexeme,removeAngularBrackets(tmp2));  
        return NULL;
    }
    printf("Line %d\t Error: The token %s for lexeme %s  does not match with the expected token %s\n",tk_inp->lineNumber,tmp1,tk_inp->lexeme,tmp2);
    for(int i=0; i<size;i++)
    fprintf(ptrs[i],"Line %d\t Error: The token %s for lexeme %s  does not match with the expected token %s\n",tk_inp->lineNumber,tmp1,tk_inp->lexeme,tmp2);  

    return NULL;
}

bool match(TreeNode* curNode, tokenInfo tk, table T, bool* noSyntaxErrors)
{    
    if(fetchTokenInfoFromTreeNode(*curNode)->tokenName == tk->tokenName)
    {
        insertTokenInfoIntoTreeNode(*curNode,createTokenInfo(tk->tokenName,tk->lexeme,tk->lineNumber));
        *curNode = getNextTreeNode(*curNode);
    }
    else        //if terminal is on top of the abstract stack, and tk->tokenName is not equal to that terminal, then go with err & syn by default
    {
        error_invalidToken(tk,fetchTokenInfoFromTreeNode(*curNode),fptrs,fptrsLen);
        *curNode = getNextTreeNode(*curNode);
        *noSyntaxErrors = false;
        return false;
    }
    return true;
}

void expandTreeNode(TreeNode* curNode, LinkedList rule)
{
    createChildTreeNode(*curNode,createTokenInfo(*((Vocabulary *)(rule->data)),NULL,-1));
    *curNode = (*curNode)->firstChild;
    rule = rule->next;

    while(rule != NULL)
    {
        createBrotherTreeNode(*curNode,createTokenInfo(*((Vocabulary *)(rule->data)),NULL,-1));
        *curNode = (*curNode)->nextBrother;
        rule = rule->next;
    }
    *curNode = (*curNode)->parent->firstChild;
}

bool parseInputSourceCode(char *testcaseFile, table T)
{
    initializeTwinBuffer();
    initializeSymbolTable();
    initializeParseTree();       //freeParseTree will be called in driver.c after printParseTree

    insertTokenInfoIntoTreeNode(_parseTree->root,createTokenInfo(STARTSYMBOL,NULL,-1));

    TreeNode curNode = _parseTree->root;
    bool noSyntaxErrors = true;

    tokenInfo tk = getNextToken(buffer);
    while(tk != NULL)   //if tk is NULL, that means that the end of file has been reached
    {
        if(tk->tokenName == TK_COMMENT)
        {
            free(tk);
            tk = getNextToken(buffer);
            continue;
        }

        if(curNode == NULL)     //parse tree has expanded completely, but input file is not completely read
        {
            error_endOfParserReached(tk->lineNumber,fptrs,fptrsLen);
            break;
        }

        if(getNTIDFromTreeNode(curNode) < 0)        //i.e. curNode is a Terminal-TreeNode.
        {
            if(match(&curNode,tk,T,&noSyntaxErrors))
            {
                free(tk);
                tk = getNextToken(buffer);
            }
            //else err & syn
        }
        else        //curNode is an NT-TreeNode. Decision is to be made by referring to the predictive parsing table, fetching the required rule, and then expanding the tree accordingly
        {
            switch(T->isErrorCell[getNTIDFromTreeNode(curNode)][tk->tokenName])
            {
                case 0:     //no error

                    fetchTokenInfoFromTreeNode(curNode)->lineNumber = tk->lineNumber; // the line number for a Non-terminal is the line at which the first token belonging to its expansion is.

                    expandTreeNode(&curNode,T->cells[getNTIDFromTreeNode(curNode)][tk->tokenName]);
                    if(getTerminalIDFromTreeNode(curNode) == (int)EPS)      //if the rule expands to EPS
                    {
                        fetchTokenInfoFromTreeNode(curNode)->lineNumber = tk->lineNumber;
                        curNode = getNextTreeNode(curNode);
                    }
                    break;

                case 1:     //error without sync

                    error_invalidToken(tk,fetchTokenInfoFromTreeNode(curNode),fptrs,fptrsLen);
                    noSyntaxErrors = false;
                    free(tk);
                    tk = getNextToken(buffer);
                    break;

                case 2:     //error & sync

                    error_invalidToken(tk,fetchTokenInfoFromTreeNode(curNode),fptrs,fptrsLen);
                    noSyntaxErrors = false;
                    curNode = getNextTreeNode(curNode);
                    break;
            }
        }
    }

    if(curNode != NULL)     //input string is completely read, but the parse tree is not completely expanded
    {
        error_endOfTokenStream(currentLineNumber,fptrs,fptrsLen);
    }

    freeTwinBuffer();
    return noSyntaxErrors;
}

void printTreeNodeInOrder(TreeNode node, FILE* fp, int* nodeNumber)              //lexeme CurrentNode lineno tokenName valueIfNumber parentNodeSymbol isLeafNode(yes/no) NodeSymbol
{
    if(node->firstChild != NULL)printTreeNodeInOrder(node->firstChild,fp,nodeNumber);

    //------//
    char* str_tmp = malloc(VOCAB_STRLEN_CAP*sizeof(char));
    tokenInfo tk_tmp = fetchTokenInfoFromTreeNode(node);

    fprintf(fp,"Lexeme: %s\tCurrent Node: %d\t",tk_tmp->lexeme,(*nodeNumber));

    if(tk_tmp->lineNumber!=-1)
        fprintf(fp,"Line no: %d\t",tk_tmp->lineNumber);    //the line number for a non-terminal is that line at which its expansion starts
    else
        fprintf(fp,"Line no: (error)\t");

    if(getNTID(tk_tmp->tokenName) < 0)
    {
        enumToStr(tk_tmp->tokenName,str_tmp);
        fprintf(fp,"Token Name: %s\t",str_tmp);
    }
    else fprintf(fp,"Token Name: ----\t");

    if(tk_tmp->tokenName == TK_NUM || tk_tmp->tokenName == TK_RNUM){
        if(tk_tmp->lineNumber == -1)fprintf(fp,"Value If Number: (error)\t");
        else fprintf(fp,"Value If Number: %s\t",tk_tmp->lexeme);
    }
    else fprintf(fp,"Value If Number: ----\t");

    if(node->parent == NULL)fprintf(fp,"Parent Node Symbol: ROOT\t");
    else{
        enumToStr(fetchTokenInfoFromTreeNode(node->parent)->tokenName,str_tmp);
        fprintf(fp,"Parent Node Symbol: %s\t",removeAngularBrackets(str_tmp));
    }

    if(node->firstChild == NULL)fprintf(fp,"Is Leaf Node: yes\t");
    else fprintf(fp,"Is Leaf Node: no\t");

    if(getNTID(tk_tmp->tokenName) >= 0)
    {
        enumToStr(tk_tmp->tokenName,str_tmp);
        fprintf(fp,"Node Symbol: %s\t",removeAngularBrackets(str_tmp));
    }
    else fprintf(fp,"Node Symbol: ----\t");

    fprintf(fp,"\n");
    free(str_tmp);
    (*nodeNumber)++;
    //------//

    if(node->firstChild == NULL)return;

    TreeNode curNode = node->firstChild->nextBrother;
    while(curNode != NULL)
    {
        printTreeNodeInOrder(curNode,fp,nodeNumber);
        curNode = curNode->nextBrother;
    }
}

void printParseTree(parseTree PT, char *outfile)
{
    //TODO: inorder traversal and printing
    FILE* fp = fopen(outfile,"w");
    int nodeNumber = 1;
    
    printTreeNodeInOrder(PT->root,fp,&nodeNumber);

    fclose(fp);
}

void freeParseTree()
{
    // Note that this is required to measure the lexing and parsing time in case of multiple executions within while loop of driver code
    freeTreeNodeRecursive(_parseTree->root);
    free(_parseTree);
    _parseTree = NULL;
}


const static struct
{
    Vocabulary val;
    const char *str;
} conversion[] = {
    {TK_COMMENT, "TK_COMMENT"},
    {TK_THEN, "TK_THEN"},
    {TK_ASSIGNOP, "TK_ASSIGNOP"},
    {TK_INT, "TK_INT"},
    {TK_AS, "TK_AS"},
    {TK_INPUT, "TK_INPUT"},
    {TK_OR, "TK_OR"},
    {TK_ENDRECORD, "TK_ENDRECORD"},
    {TK_MINUS, "TK_MINUS"},
    {TK_EQ, "TK_EQ"},
    {TK_PARAMETER, "TK_PARAMETER"},
    {TK_TYPE, "TK_TYPE"},
    {TK_WHILE, "TK_WHILE"},
    {TK_GLOBAL, "TK_GLOBAL"},
    {TK_SEM, "TK_SEM"},
    {TK_RUID, "TK_RUID"},
    {TK_OP, "TK_OP"},
    {TK_ENDUNION, "TK_ENDUNION"},
    {TK_COMMA, "TK_COMMA"},
    {TK_MUL, "TK_MUL"},
    {TK_RETURN, "TK_RETURN"},
    {TK_MAIN, "TK_MAIN"},
    {TK_IF, "TK_IF"},
    {TK_DOT, "TK_DOT"},
    {TK_FIELDID, "TK_FIELDID"},
    {TK_SQL, "TK_SQL"},
    {TK_WRITE, "TK_WRITE"},
    {TK_DIV, "TK_DIV"},
    {TK_PLUS, "TK_PLUS"},
    {TK_NUM, "TK_NUM"},
    {TK_GT, "TK_GT"},
    {TK_DEFINETYPE, "TK_DEFINETYPE"},
    {TK_OUTPUT, "TK_OUTPUT"},
    {TK_REAL, "TK_REAL"},
    {TK_PARAMETERS, "TK_PARAMETERS"},
    {TK_WITH, "TK_WITH"},
    {TK_GE, "TK_GE"},
    {TK_LT, "TK_LT"},
    {TK_READ, "TK_READ"},
    {TK_AND, "TK_AND"},
    {TK_RECORD, "TK_RECORD"},
    {TK_END, "TK_END"},
    {TK_CALL, "TK_CALL"},
    {TK_ENDWHILE, "TK_ENDWHILE"},
    {TK_ELSE, "TK_ELSE"},
    {TK_SQR, "TK_SQR"},
    {TK_NOT, "TK_NOT"},
    {TK_ENDIF, "TK_ENDIF"},
    {TK_COLON, "TK_COLON"},
    {EPS, "EPS"},
    {TK_ID, "TK_ID"},
    {TK_LE, "TK_LE"},
    {TK_NE, "TK_NE"},
    {TK_FUNID, "TK_FUNID"},
    {TK_UNION, "TK_UNION"},
    {TK_LIST, "TK_LIST"},
    {TK_RNUM, "TK_RNUM"},
    {TK_CL, "TK_CL"},
    {dataType, "<dataType>"},
    {fieldDefinition, "<fieldDefinition>"},
    {function, "<function>"},
    {more_ids, "<more_ids>"},
    {output_par, "<output_par>"},
    {typeDefinitions, "<typeDefinitions>"},
    {stmts, "<stmts>"},
    {constructedDatatype, "<constructedDatatype>"},
    {definetypestmt, "<definetypestmt>"},
    {elsePart, "<elsePart>"},
    {relationalOp, "<relationalOp>"},
    {booleanExpression, "<booleanExpression>"},
    {returnStmt, "<returnStmt>"},
    {var, "<var>"},
    {moreExpansions, "<moreExpansions>"},
    {singleOrRecId, "<singleOrRecId>"},
    {fieldDefinitions, "<fieldDefinitions>"},
    {actualOrRedefined, "<actualOrRedefined>"},
    {parameter_list, "<parameter_list>"},
    {oneExpansion, "<oneExpansion>"},
    {funCallStmt, "<funCallStmt>"},
    {otherFunctions, "<otherFunctions>"},
    {otherStmts, "<otherStmts>"},
    {lowPrecedenceOperators, "<lowPrecedenceOperators>"},
    {inputParameters, "<inputParameters>"},
    {highPrecedenceOperators, "<highPrecedenceOperators>"},
    {declaration, "<declaration>"},
    {moreFields, "<moreFields>"},
    {typeDefinition, "<typeDefinition>"},
    {ioStmt, "<ioStmt>"},
    {stmt, "<stmt>"},
    {termPrime, "<termPrime>"},
    {input_par, "<input_par>"},
    {A, "<A>"},
    {assignmentStmt, "<assignmentStmt>"},
    {option_single_constructed, "<option_single_constructed>"},
    {optionalReturn, "<optionalReturn>"},
    {idList, "<idList>"},
    {global_or_not, "<global_or_not>"},
    {recId, "<recId>"},
    {mainFunction, "<mainFunction>"},
    {remaining_list, "<remaining_list>"},
    {fieldType, "<fieldType>"},
    {iterativeStmt, "<iterativeStmt>"},
    {term, "<term>"},
    {logicalOp, "<logicalOp>"},
    {declarations, "<declarations>"},
    {arithmeticExpression, "<arithmeticExpression>"},
    {expPrime, "<expPrime>"},
    {primitiveDatatype, "<primitiveDatatype>"},
    {outputParameters, "<outputParameters>"},
    {factor, "<factor>"},
    {program, "<program>"},
    {conditionalStmt, "<conditionalStmt>"},
};

Vocabulary encodeStr(const char *str)
{
    for (int i = 0; i < sizeof(conversion) / sizeof(conversion[0]); i++)
        if (!strcmp(str, conversion[i].str))
            return conversion[i].val;
    return -1;
}

void enumToStr(Vocabulary v, char *dest)
{
    for (int i = 0; i < sizeof(conversion) / sizeof(conversion[0]); i++)
        if (conversion[i].val == v)
        {
            strcpy(dest, conversion[i].str);
            return;
        }
}