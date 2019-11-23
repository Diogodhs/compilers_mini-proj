#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "visitor.h"
 
#define printm(...) printf(__VA_ARGS__)
 
//Tabela de variaveis onde contem o nome, valor 
//e quais variaveis estão sendo utilizados
char varName[5][10];
int varValue[5];
int varCounter = 0;
//Vetor e contador de registradores
char regName[20][10];
int regCounter = 0;
//controle de variáveis dentro ou fora de função
int inside = 0;
 
void visit_file (AST *root) {
    //printm(">>> file\n");
    printm("file has %d declarations\n\n", root->list.num_items);
 
    for (ListNode *ptr = root->list.first; ptr != NULL; ptr = ptr->next) {
        switch (ptr->ast->decl.type) {
        case FUNCTION_DECLARATION:
            visit_function_decl(ptr->ast); break;
        case VARIABLE_DECLARATION:
            visit_var_decl(ptr->ast); break;
        default:
            fprintf(stderr, "UNKNOWN DECLARATION TYPE %c\n", ptr->ast->decl.type);
            break;
        }
    }
    //printm("<<< file\n");
}
 
void visit_function_decl (AST *ast) {
    //printm(">>> function_decl\n");
    AST *params = ast->decl.function.param_decl_list;

    inside = 1;
	printm("\ndefine i32 @%s(", ast->decl.function.id->id.string);    

    if (params != NULL) {
        for (int i = 0; i < params->list.num_items; i++) {
            if(i == 0){
                printm("i32");
            }else{
                printm(", i32");
            }

            //Armazena na tabela de registradores para numero de parametros no máximo 3
            if(i == 0){
                for(int j = 0;j < 10;j++){
                    regName[regCounter][j] = params->list.first->ast->decl.variable.id->id.string[j];
                }
            }else if(i == 1){
                for(int j = 0;j < 10;j++){
                    regName[regCounter][j] = params->list.first->next->ast->decl.variable.id->id.string[j];
                }
            }else if(i == 2){
                for(int j = 0;j < 10;j++){
                    regName[regCounter][j] = params->list.first->next->next->ast->decl.variable.id->id.string[j];
                }
            }
            
            regCounter++;
        }
    }
    printm("){\n");
 
    //Coloca um '-' na tabela de registradores para indicar que não está sendo usado
    regName[regCounter][0] = '-';
    regCounter++;
    if (ast->decl.function.stat_block != NULL) {
        visit_stat_block(ast->decl.function.stat_block, params, ast->decl.function.type);
    } 
    
    //Print da tabela de registradores
    /* printf("\n-------------------------------\n");
    printf("|   Tabela de registradores   |\n");
    printf("-------------------------------\n");
    for(int i = 0;i < regCounter;i++){
        printf("Registrador %d: %s\n", i, regName[i]);
    }
    printf("\n"); */

    //Print da tabela de variáveis
    /* printf("-------------------------------\n");
    printf("| Tabela de variáveis globais |\n");
    printf("-------------------------------\n");
    for(int i = 0;i < varCounter;i++){
        printf("Variavel %d: %s, %d\n", i, varName[i], varValue[i]);
    }
    printf("\n"); */

    inside = 0;
    regCounter = 0;
    printm("\n}\n");

    //Resetamos a tabela de registradores colocando '-' em todos
    for(int i = 0;i < 20;i++){
        regName[i][0] = '-';
    }
}
 
// what is surrounded by { }
ExprResult visit_stat_block (AST *stat_block, AST *params, int return_type) {
    //printm(">>> stat_block\n");
    ExprResult ret = { 0, VOID_CONSTANT };
 
    for (ListNode *ptr = stat_block->list.first; ptr != NULL; ptr = ptr->next) {
        ret = visit_stat(ptr->ast);
    }
    //printm("<<< stat_block\n");
    return  ret;
}
 
 
ExprResult visit_stat (AST *stat) {
    //printm(">>> statement\n");
    ExprResult ret = { 0, TYPE_VOID };
    switch (stat->stat.type) {
    case VARIABLE_DECLARATION:
        visit_var_decl(stat); break;
    case ASSIGN_STATEMENT:
        visit_assign_stat(stat); break;
    case RETURN_STATEMENT:
        ret = visit_return_stat(stat); break;
    case EXPRESSION_STATEMENT:
        visit_expr(stat->stat.expr.expr); break;
        default: fprintf(stderr, "UNKNOWN STATEMENT TYPE %c\n", stat->stat.type); break;
    }
    //printm("<<< statement\n");
    return ret;
}
 
void visit_var_decl (AST *ast) {
    AST *id = ast->decl.variable.id;
    ExprResult ret = { 0, TYPE_VOID };
 
    if(inside == 0){
        printm("@%s = ", id->id.string);
        if (ast->decl.variable.expr == NULL){
        	printm("common ");
        }
        printm("global i32");
        if (ast->decl.variable.expr == NULL){
        	printm(" 0");
		}
        for(int i = 0;id->id.string[i] != NULL;i++){
            varName[varCounter][i] = id->id.string[i];
        }
 
        varCounter++;
     }
	else{
        for(int i = 0;id->id.string[i] != NULL;i++){
            regName[regCounter][i] = id->id.string[i];
        }
    }
 
    if (ast->decl.variable.expr != NULL && inside == 0) {
        ExprResult expr = visit_expr(ast->decl.variable.expr);
        printf(" %ld", expr.int_value);
        varValue[varCounter-1] = expr.int_value;
    }
	else if(ast->decl.variable.expr != NULL && inside == 1)
    {
        ExprResult expr = visit_expr(ast->decl.variable.expr);
    }
    
    printf("\n");
}
 
 
ExprResult visit_return_stat (AST *ast) {
    //printm(">>> return stat\n");
    ExprResult ret = { 0, TYPE_VOID };
    if (ast->stat.ret.expr) {
        ret = visit_expr(ast->stat.ret.expr);
		printf("\nret i32 %%%d", regCounter);
    }
	else{
		printf("\nret void");
	}
    return ret;
    //printm("<<< return stat\n");
}
 
void visit_assign_stat (AST *assign) {
    //printm(">>> assign stat\n");
    ExprResult expr = visit_expr(assign->stat.assign.expr);
    //printm("<<< assign stat\n");
}
 
ExprResult visit_expr (AST *expr) {
    //printm(">>> expression\n");
    ExprResult ret = {};
    switch (expr->expr.type) {
    case BINARY_EXPRESSION:
        switch (expr->expr.binary_expr.operation) {
        case '+':
            ret = visit_add(expr); break;
        case '-':
            ret = visit_sub(expr); break;
        case '*':
            ret = visit_mul(expr); break;
        case '/':
            ret = visit_div(expr); break;
        case '%':
            ret = visit_mod(expr); break;
        default:
            fprintf(stderr, "UNKNOWN OPERATOR %c\n", expr->expr.binary_expr.operation); break;
        }
        break;
    case UNARY_MINUS_EXPRESSION:
        ret = visit_unary_minus(expr); break;
    case LITERAL_EXPRESSION:
        ret = visit_literal(expr); break;
    case IDENTIFIER_EXPRESSION:
        ret = visit_id(expr->expr.id.id); break;
    case FUNCTION_CALL_EXPRESSION:
        ret = visit_function_call(expr); break;
    default:
        fprintf(stderr, "UNKNOWN EXPRESSION TYPE %c\n", expr->expr.type);
        break;
    }
    //printm("<<< expression\n");
    return ret;
}
 
// não implementar
ExprResult visit_function_call (AST *ast) {
    //printm(">>> function_call\n");
    ExprResult ret = {}, arg_expr[20]; //instead of alloca
    AST *arg_list = ast->expr.function_call.expr_list;
 
    if (arg_list != NULL) {
        int i = 0;
        for (ListNode *ptr = arg_list->list.first; ptr != NULL; ptr = ptr->next) {
            arg_expr[i++] = visit_expr(ptr->ast);
        }
    }
    //printm("<<< function_call\n");
    return ret;
}
 
ExprResult visit_id (AST *ast) {
    //printm(">>> identifier\n");
    ExprResult ret = { }; // armazenar aqui

    if (ast->id.type == TYPE_INT) {
        ret.int_value = ast->id.int_value;
        ret.type = TYPE_INT;
    } else if (ast->id.type == TYPE_FLOAT) {
        ret.float_value = ast->id.float_value;
        ret.type = TYPE_FLOAT;
    }/*else{
        ret.ssa_register = ast->id.ssa_register;
        ret.type = TYPE_
    }*/
    
    //printm("<<< identifier\n");
    return ret;
}
 
ExprResult visit_literal (AST *ast) {
    //printm(">>> literal\n");
    ExprResult ret = {};
    ret.int_value = ast->expr.literal.int_value;
    ret.type = INTEGER_CONSTANT;
    //printm("<<< literal\n");
    return ret;
}
 
ExprResult visit_unary_minus (AST *ast) {
    //printm(">>> unary_minus\n");
    ExprResult expr, ret = {};
    expr = visit_expr(ast->expr.unary_minus.expr);
    //printm("<<< unary_minus\n");
    return ret;
}
 
ExprResult visit_add (AST *ast) {
    regCounter++;
	printm("\n%%%d = ", regCounter);
	printm("add i32 ");
    ExprResult left, right, ret = {};
    left  = visit_expr(ast->expr.binary_expr.left_expr);
    right = visit_expr(ast->expr.binary_expr.right_expr);
    if (left.type == INTEGER_CONSTANT && right.type == INTEGER_CONSTANT){
		printm("%ld, %ld", left.int_value, right.int_value);
		ast->id.ssa_register = left.int_value + right.int_value;
	}
	if (left.type == INTEGER_CONSTANT && right.type == LLIR_REGISTER){
		printm("%ld, %ld", left.int_value, right.ssa_register);
		ast->id.ssa_register = left.int_value + right.ssa_register;
	}
	if (left.type == LLIR_REGISTER && right.type == INTEGER_CONSTANT){
		printm("%ld, %ld", left.ssa_register, right.int_value);
		ast->id.ssa_register = left.ssa_register + right.int_value;
	}
	if (left.type == LLIR_REGISTER && right.type == LLIR_REGISTER ){
		printm("%ld, %ld", left.ssa_register, right.ssa_register);
		ast->id.ssa_register = left.ssa_register + right.ssa_register;
	}
    ret.int_value = left.int_value + right.int_value;
    ret.type = INTEGER_CONSTANT;
    //printm("\n");
    return ret;
}
 
ExprResult visit_sub (AST *ast) {
    regCounter++;
	printm("\n%%%d = ", regCounter);
	printm("sub i32 ");
    ExprResult left, right, ret = {};
    left  = visit_expr(ast->expr.binary_expr.left_expr);
    right = visit_expr(ast->expr.binary_expr.right_expr);
    if (left.type == INTEGER_CONSTANT && right.type == INTEGER_CONSTANT){
		printm("%ld, %ld", left.int_value, right.int_value);
		ast->id.ssa_register = left.int_value - right.int_value;
	}
	if (left.type == INTEGER_CONSTANT && right.type == LLIR_REGISTER){
		printm("%ld, %ld", left.int_value, right.ssa_register);
		ast->id.ssa_register = left.int_value - right.ssa_register;
	}
	if (left.type == LLIR_REGISTER && right.type == INTEGER_CONSTANT){
		printm("%ld, %ld", left.ssa_register, right.int_value);
		ast->id.ssa_register = left.ssa_register - right.int_value;
	}
	if (left.type == LLIR_REGISTER && right.type == LLIR_REGISTER ){
		printm("%ld, %ld", left.ssa_register, right.ssa_register);
		ast->id.ssa_register = left.ssa_register - right.ssa_register;
	}
    ret.int_value = left.int_value - right.int_value;
    ret.type = INTEGER_CONSTANT;
    //printm("\n");
    return ret;
}
 
ExprResult visit_mul (AST *ast) {
    regCounter++;
	printm("\n%%%d = ", regCounter);
	printm("mul i32 ");
    ExprResult left, right, ret = {};
    left  = visit_expr(ast->expr.binary_expr.left_expr);
    right = visit_expr(ast->expr.binary_expr.right_expr);
    if (left.type == INTEGER_CONSTANT && right.type == INTEGER_CONSTANT){
		printm("%ld, %ld", left.int_value, right.int_value);
		ast->id.ssa_register = left.int_value * right.int_value;
	}
	if (left.type == INTEGER_CONSTANT && right.type == LLIR_REGISTER){
		printm("%ld, %ld", left.int_value, right.ssa_register);
		ast->id.ssa_register = left.int_value * right.ssa_register;
	}
	if (left.type == LLIR_REGISTER && right.type == INTEGER_CONSTANT){
		printm("%ld, %ld", left.ssa_register, right.int_value);
		ast->id.ssa_register = left.ssa_register * right.int_value;
	}
	if (left.type == LLIR_REGISTER && right.type == LLIR_REGISTER ){
		printm("%ld, %ld", left.ssa_register, right.ssa_register);
		ast->id.ssa_register = left.ssa_register * right.ssa_register;
	}
    ret.int_value = left.int_value * right.int_value;
    ret.type = INTEGER_CONSTANT;
    //printm("\n");
    return ret;
}
 
ExprResult visit_div (AST *ast) {
	regCounter++;
	printm("\n%%%d = ", regCounter);
	printm("div i32 ");
    ExprResult left, right, ret = {};
    left  = visit_expr(ast->expr.binary_expr.left_expr);
    right = visit_expr(ast->expr.binary_expr.right_expr);
    if (left.type == INTEGER_CONSTANT && right.type == INTEGER_CONSTANT){
		printm("%ld, %ld", left.int_value, right.int_value);
		ast->id.ssa_register = left.int_value / right.int_value;
	}
	if (left.type == INTEGER_CONSTANT && right.type == LLIR_REGISTER){
		printm("%ld, %ld", left.int_value, right.ssa_register);
		ast->id.ssa_register = left.int_value / right.ssa_register;
	}
	if (left.type == LLIR_REGISTER && right.type == INTEGER_CONSTANT){
		printm("%ld, %ld", left.ssa_register, right.int_value);
		ast->id.ssa_register = left.ssa_register / right.int_value;
	}
	if (left.type == LLIR_REGISTER && right.type == LLIR_REGISTER ){
		printm("%ld, %ld", left.ssa_register, right.ssa_register);
		ast->id.ssa_register = left.ssa_register / right.ssa_register;
	}
    /*printm("%ld %ld", left.int_value, right.int_value);
    ret.int_value = left.int_value / right.int_value;
    ret.type = TYPE_INT;*/
    //printm("\n");
    return ret;
}
 
ExprResult visit_mod (AST *ast) {
    regCounter++;
	printm("\n%%%d = ", regCounter);
	printm("srem i32 ");
    ExprResult left, right, ret = {};
    left  = visit_expr(ast->expr.binary_expr.left_expr);
    right = visit_expr(ast->expr.binary_expr.right_expr);
    if (left.type == INTEGER_CONSTANT && right.type == INTEGER_CONSTANT){
		printm("%ld, %ld", left.int_value, right.int_value);
		ast->id.ssa_register = left.int_value % right.int_value;
	}
	if (left.type == INTEGER_CONSTANT && right.type == LLIR_REGISTER){
		printm("%ld, %ld", left.int_value, right.ssa_register);
		ast->id.ssa_register = left.int_value % right.ssa_register;
	}
	if (left.type == LLIR_REGISTER && right.type == INTEGER_CONSTANT){
		printm("%ld, %ld", left.ssa_register, right.int_value);
		ast->id.ssa_register = left.ssa_register % right.int_value;
	}
	if (left.type == LLIR_REGISTER && right.type == LLIR_REGISTER ){
		printm("%ld, %ld", left.ssa_register, right.ssa_register);
		ast->id.ssa_register = left.ssa_register % right.ssa_register;
	}

    return ret;
}