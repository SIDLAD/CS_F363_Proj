#include "parserDef.h"
#include <stdio.h>
#include <string.h>

// function prototypes//
FirstAndFollow computeFirstAndFollowSets(grammar G);
TerminalBucketSet createSet();
FirstAndFollow initializeFirstAndFollow(FirstAndFollow _firstAndFollow, grammar G);
void freeParseTree(parseTree PT);
Vocabulary encodeStr(const char *str);
grammar initializeGrammar(grammar g);
LinkedList *increaseProductionRHSSetSize(LinkedList *rhsSet, int *currentSize); // &ruleCount[i] will be passed as currentSize
grammar populateGrammar(grammar g, char *grammarFileName);
int getNTID(Vocabulary NT);
void insertGrammarSymbol(LinkedList node, Vocabulary v);
table createParseTable(FirstAndFollow F, table T);
void calculateFirstOfRule(TerminalBucketSet _firstOfRule, LinkedList RHSNode, bool *flag, FirstAndFollow F);
TerminalBucketSet firstOfNT(int NT_ID, FirstAndFollow F);
void appendSetUnion(TerminalBucketSet Dest, TerminalBucketSet Src, bool *flag);
bool isNTNode(LinkedList node);
int getTerminalIDFromNode(LinkedList node);
int getNTIDFromNode(LinkedList node);
void calculateFollowFromNTRule(TerminalBucketSet _followOfNT, LinkedList RHSNode, bool* flag, FirstAndFollow F);
//------------------//

// function definitions
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
        for(int i=0;i<N_TERMINALS_COUNT;i++)
        {
            for (int j = 0; j < _firstAndFollow->ruleCount[i]; j++)
            {
                calculateFollowFromNTRule(_firstAndFollow->follow[i],G->NT[i][j],&flag,_firstAndFollow);
            }
        }
    }
}

void calculateFollowFromNTRule(TerminalBucketSet _followOfNT, LinkedList RHSNode, bool* flag, FirstAndFollow F)
{
    if(isNTNode(RHSNode))
    {
        if(RHSNode->next == NULL)
        {
            appendSetUnion(F->follow[getNTIDFromNode(RHSNode)],_followOfNT,flag);
            return;
        }
        calculateFollowFromNTRule(_followOfNT,RHSNode->next,flag,F);

        if(! isNTNode(RHSNode->next))
        {
            //Note that the next Terminal in the rule can not be EPS, as any rule that contains EPS is essentially only EPS
            if(! (F->follow[getNTIDFromNode(RHSNode)]->val[getTerminalIDFromNode(RHSNode->next)]))
            {
                F->follow[getNTIDFromNode(RHSNode)]->val[getTerminalIDFromNode(RHSNode->next)] = true;
                *flag = true;
            }
            return;
        }

        TerminalBucketSet tmp = firstOfNT(getNTIDFromNode(RHSNode->next),F);

        if(! tmp->val[(int)EPS])
        {
            appendSetUnion(F->follow[getNTIDFromNode(RHSNode)],tmp,flag);
            return;
        }

        tmp->val[(int)EPS] = false;
        appendSetUnion(F->follow[getNTIDFromNode(RHSNode)],tmp,flag);
        appendSetUnion(F->follow[getNTIDFromNode(RHSNode)],F->follow[getNTIDFromNode(RHSNode->next)],flag);
        
        return;
    }

    if(RHSNode->next == NULL)
    {
        return;
    }

    calculateFollowFromNTRule(_followOfNT,RHSNode->next,flag,F);
}

void calculateFirstOfRule(TerminalBucketSet _firstOfRule, LinkedList RHSNode, bool *flag, FirstAndFollow F) // firstAndFollow F is being passed here because firstOfNT requires it.
{
    if (!isNTNode(RHSNode)) // if it is a terminal node. Note that if it the rule can derive EPS, then first set of rule should contain EPS
    {
        if (!_firstOfRule->val[getTerminalIDFromNode(RHSNode)])
        {
            _firstOfRule->val[getTerminalIDFromNode(RHSNode)] = true;
            *flag = true;
        }
        return;
    }

    TerminalBucketSet tmp = firstOfNT(getNTIDFromNode(RHSNode), F);

    if (! tmp->val[(int)EPS])
    {
        appendSetUnion(_firstOfRule, tmp, flag);
        return;
    }

    tmp->val[(int)EPS] = false;
    appendSetUnion(_firstOfRule, tmp, flag);

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

TerminalBucketSet firstOfNT(int NT_ID, FirstAndFollow F)
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
            *flag = true;
        }
}

bool isNTNode(LinkedList node) // returns true if the linkedlist node's data item is a non-terminal
{
    return getNTID(*((Vocabulary *)(node->data))) >= 0;
}

int getTerminalIDFromNode(LinkedList node)
{
    return (int)(*((Vocabulary *)(node->data)));
}

int getNTIDFromNode(LinkedList node)
{
    return getNTID(*((Vocabulary *)(node->data)));
}

TerminalBucketSet createSet()
{
    TerminalBucketSet set = (TerminalBucketSet)calloc(1, sizeof(struct terminalBucketSet)); // calloc used instead of malloc so that all bits are initialised to zero
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

void freeParseTree(parseTree PT)
{
    // TODO: Note that this is required to measure the lexing and parsing time in case of multiple executions within while loop of driver code
}

grammar initializeGrammar(grammar g)
{
    if (g != NULL)
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
        insertGrammarSymbol(currentNode, symbol);

        while (symbolString = strtok(NULL, " \n")) // RHS of rules onwards
        {
            if (strcmp(symbolString, "|") == 0)
            { // create a new head and populate for the new rule after |

                symbolString = strtok(NULL, " \n");
                symbol = encodeStr(symbolString);

                g->NT[NT_ID] = increaseProductionRHSSetSize(g->NT[NT_ID], &(g->ruleCount[NT_ID]));
                currentNode = g->NT[NT_ID][g->ruleCount[NT_ID] - 1];
                insertGrammarSymbol(currentNode, symbol);
            }
            else
            {
                symbol = encodeStr(symbolString);
                // continue on the same head
                currentNode->next = createLinkedListNode();
                currentNode = currentNode->next;
                insertGrammarSymbol(currentNode, symbol);
            }
        }
    }
    return g;
}

void insertGrammarSymbol(LinkedList node, Vocabulary v)
{
    Vocabulary *tmp = (Vocabulary *)malloc(sizeof(Vocabulary));
    *tmp = v;
    node->data = tmp;
}

int getNTID(Vocabulary NT) // returns a negative value if not a non-terminal
{
    return (int)NT - TERMINALS_COUNT;
}

table createParseTable(FirstAndFollow F, table T) // F can be passed as NULL or _firstAndFollow.
{
    if (F == NULL)
    {
        _grammar = initializeGrammar(_grammar);
        _grammar = populateGrammar(_grammar, NULL);
        computeFirstAndFollowSets(_grammar);
        F = _firstAndFollow;
    }

    // TODO
    return T;
}

Vocabulary encodeStr(const char *str)
{
    const static struct
    {
        Vocabulary val;
        const char *str;
    } conversion[] = {
        {TK_ENDUNION, "TK_ENDUNION"},
        {EPS, "EPS"},
        {TK_COMMA, "TK_COMMA"},
        {TK_READ, "TK_READ"},
        {TK_REAL, "TK_REAL"},
        {TK_DOT, "TK_DOT"},
        {TK_GE, "TK_GE"},
        {TK_NE, "TK_NE"},
        {TK_PLUS, "TK_PLUS"},
        {TK_DEFINETYPE, "TK_DEFINETYPE"},
        {TK_ASSIGNOP, "TK_ASSIGNOP"},
        {TK_AS, "TK_AS"},
        {TK_CL, "TK_CL"},
        {TK_DIV, "TK_DIV"},
        {TK_GT, "TK_GT"},
        {TK_PARAMETERS, "TK_PARAMETERS"},
        {TK_UNION, "TK_UNION"},
        {TK_RUID, "TK_RUID"},
        {TK_SQR, "TK_SQR"},
        {TK_FIELDID, "TK_FIELDID"},
        {TK_MUL, "TK_MUL"},
        {TK_OR, "TK_OR"},
        {TK_END, "TK_END"},
        {TK_LIST, "TK_LIST"},
        {TK_WRITE, "TK_WRITE"},
        {TK_TYPE, "TK_TYPE"},
        {TK_SEM, "TK_SEM"},
        {TK_WITH, "TK_WITH"},
        {TK_AND, "TK_AND"},
        {TK_IF, "TK_IF"},
        {TK_LT, "TK_LT"},
        {TK_MINUS, "TK_MINUS"},
        {TK_EQ, "TK_EQ"},
        {TK_ELSE, "TK_ELSE"},
        {TK_PARAMETER, "TK_PARAMETER"},
        {TK_NUM, "TK_NUM"},
        {TK_ENDIF, "TK_ENDIF"},
        {TK_WHILE, "TK_WHILE"},
        {TK_FUNID, "TK_FUNID"},
        {TK_OP, "TK_OP"},
        {TK_COLON, "TK_COLON"},
        {TK_ID, "TK_ID"},
        {TK_ENDRECORD, "TK_ENDRECORD"},
        {TK_LE, "TK_LE"},
        {TK_NOT, "TK_NOT"},
        {TK_RECORD, "TK_RECORD"},
        {TK_THEN, "TK_THEN"},
        {TK_RNUM, "TK_RNUM"},
        {TK_SQL, "TK_SQL"},
        {TK_OUTPUT, "TK_OUTPUT"},
        {TK_INT, "TK_INT"},
        {TK_CALL, "TK_CALL"},
        {TK_INPUT, "TK_INPUT"},
        {TK_RETURN, "TK_RETURN"},
        {TK_ENDWHILE, "TK_ENDWHILE"},
        {TK_GLOBAL, "TK_GLOBAL"},
        {TK_MAIN, "TK_MAIN"},
        {iterativeStmt, "<iterativeStmt>"},
        {global_or_not, "<global_or_not>"},
        {relationalOp, "<relationalOp>"},
        {A, "<A>"},
        {stmts, "<stmts>"},
        {input_par, "<input_par>"},
        {recId, "<recId>"},
        {declarations, "<declarations>"},
        {arithmeticExpression2Contd, "<arithmeticExpression2Contd>"},
        {otherFunctions, "<otherFunctions>"},
        {definetypestmt, "<definetypestmt>"},
        {var, "<var>"},
        {ioStmt, "<ioStmt>"},
        {more_ids, "<more_ids>"},
        {assignmentStmt, "<assignmentStmt>"},
        {function, "<function>"},
        {returnStmt, "<returnStmt>"},
        {typeDefinitions, "<typeDefinitions>"},
        {otherStmts, "<otherStmts>"},
        {declaration, "<declaration>"},
        {arithmeticExpressionContd, "<arithmeticExpressionContd>"},
        {operator2, "<operator2>"},
        {funCallStmt, "<funCallStmt>"},
        {parameter_list, "<parameter_list>"},
        {primitiveDatatype, "<primitiveDatatype>"},
        {booleanExpression, "<booleanExpression>"},
        {arithmeticExpression2, "<arithmeticExpression2>"},
        {singleOrRecId, "<singleOrRecId>"},
        {output_par, "<output_par>"},
        {arithmeticExpression, "<arithmeticExpression>"},
        {mainFunction, "<mainFunction>"},
        {outputParameters, "<outputParameters>"},
        {arithmeticExpression3, "<arithmeticExpression3>"},
        {optionalReturn, "<optionalReturn>"},
        {operator1, "<operator1>"},
        {conditionalStmt, "<conditionalStmt>"},
        {stmt, "<stmt>"},
        {dataType, "<dataType>"},
        {program, "<program>"},
        {remaining_list, "<remaining_list>"},
        {conditionalStmtContd, "<conditionalStmtContd>"},
        {logicalOp, "<logicalOp>"},
        {fieldDefinitions, "<fieldDefinitions>"},
        {fieldDefinition, "<fieldDefinition>"},
        {typeDefinition, "<typeDefinition>"},
        {inputParameters, "<inputParameters>"},
        {constructedDatatype, "<constructedDatatype>"},
        {moreFields, "<moreFields>"},
        {idList, "<idList>"},
    };
    for (int i = 0; i < sizeof(conversion) / sizeof(conversion[0]); i++)
        if (!strcmp(str, conversion[i].str))
            return conversion[i].val;
    return -1;
}