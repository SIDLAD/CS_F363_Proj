#include <stdio.h>
#include "lexerDef.h"

Vocabulary encodeStr (const char *str)
{
    const static struct {
    Vocabulary val;
    const char *str;
    } conversion [] = {
        {TK_ENDUNION, "TK_ENDUNION"},
        {TK_EPS, "TK_EPS"},
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
        {idsList, "<idsList>"},
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
    for (int i = 0;  i < sizeof (conversion) / sizeof (conversion[0]);  i++)
        if (!strcmp (str, conversion[i].str))
            return conversion[i].val;
}

twinBuffer initialiseBuffer(twinBuffer buffer)
{
    buffer = (twinBuffer)malloc(sizeof(struct twinBuffer));
    return buffer;
}