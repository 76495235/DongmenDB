//
// Created by Sam on 2018/1/17.
//

#include <dongmensql/column.h>
#include "dongmensql/expression.h"
#include <cstdlib>

using namespace std;

/**
 * @brief 从后序表达式产生中序表达式字符串
 * @param exprs 表达式字符串
 * @param expr 表达式
 * @return 需要继续处理的后序表达式
 */
int Expression::printTermExpression(char *exprs, TermExpr *term) {
    if (term != NULL) {
        if (term->t == TERM_ID) {
            strcat(exprs, term->id);
        } else if (term->t == TERM_COLREF) {
            strcat(exprs, term->ref->allName);
        } else if (term->t == TERM_LITERAL){
            Literal *val = term->val;
            if (val->t == DATA_TYPE_CHAR || val->t == DATA_TYPE_TEXT) {

                strcat(exprs, val->original_value);
            }else if (val->t == DATA_TYPE_INT || val->t == DATA_TYPE_DOUBLE){
                char *ival = (char *)calloc(20,1);
                sprintf(ival, "%d", ((IntLiteral *) val)->value);
                strcat(exprs, ival);
            }
        } else if(term->t == TERM_FUNC){

        }
        return 1;
    }
    return 0;
}

const char *Expression::getOperatorDesc(TokenType type) {
    switch (type) {
        case TOKEN_NOT:
            return " NOT ";
        case TOKEN_AND:
            return " AND ";
        case TOKEN_OR:
            return " OR ";
        case TOKEN_PLUS:
            return " + ";
        case TOKEN_MINUS:
            return " - ";
        case TOKEN_MULTIPLY:
            return " * ";
        case TOKEN_DIVIDE:
            return " / ";
        case TOKEN_GT:
            return " > ";
        case TOKEN_LT:
            return " < ";
        case TOKEN_LE:
            return " <= ";
        case TOKEN_GE:
            return " >= ";
        case TOKEN_EQ:
            return " = ";
        case TOKEN_NOT_EQUAL:
            return " != ";
        case TOKEN_COMMA:
            return ", ";
        case TOKEN_ASSIGNMENT:
            return " = ";
        case TOKEN_LIKE:
            return " LIKE ";
        case TOKEN_IN:
            return " IN ";
        default:
            return " UNKNOWN ";
    }

};

Expression::Expression(TokenType type, Expression *nextexpr) {
    this->opType = type;
    this->nextexpr = nextexpr;
    this->term = NULL;
    this->alias = NULL;
}

TermExpr *Expression::newTermExpr() {
    TermExpr *expr = (TermExpr *) calloc(sizeof(TermExpr), 1);
    expr->t = TERM_UNKNOWN;
    return expr;
}

int Expression::expression_free() {};

Expression *Expression::expression_print(Expression *expr, char *desc) {
    if (!expr) return NULL;
    Expression *result;
    if (expr->term) {
        if (desc) {
            printTermExpression(desc, expr->term);
        } else {
            char *t = new_id_name();
            printTermExpression(t, expr->term);
            printf(t);
        }
        result = expr->nextexpr;
    } else {
        if (desc) {
            strcat(desc, "(");
        } else {
            printf("(");
        }
        Operator op = operators[expr->opType];

        if (op.numbers == 1) {
            if (desc) {
                strcat(desc, getOperatorDesc(expr->opType));
            } else {
                printf(getOperatorDesc(expr->opType));
            }
            result = expression_print(expr->nextexpr, desc);

        } else if (op.numbers == 2) {
            result = expression_print(expr->nextexpr, desc);
            if (desc) {
                strcat(desc, getOperatorDesc(expr->opType));
            } else {
                printf(getOperatorDesc(expr->opType));
            }
            result = expression_print(result, desc);
        }
        if (desc) {
            strcat(desc, ")");
        } else {
            printf(")");
        }
    }

    if (expr->alias) {
        if (desc) {
            strcat(desc, " as ");
            strcat(desc, expr->alias);
        } else {
            printf(" as %s", expr->alias);
        }
    }
    return result;
};


char *getExpressionNamesTitle(vector<Expression*> exprs){
    char *title = (char *)calloc(1024,1);

    for (int i = 0; i < exprs.size(); i++){
        Expression* expr = exprs.at(i);
        expr->expression_print(expr, title);
        strcat(title, "\t");
    }
    return title;
};

int expression_free_list(vector<Expression*> expr) {};

int expression_print_list(vector<Expression*> exprlist) {

    Expression *expr1 = exprlist.at(0);
    printf("[");
    expr1->expression_print(expr1, NULL);

    for (int i = 1; i < exprlist.size(); i++) {
        expr1 = exprlist.at(i);
        printf(", ");
        expr1->expression_print(expr1, NULL);
    }
    printf("]");
};
