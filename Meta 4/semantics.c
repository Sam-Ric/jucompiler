/*
  João Tomás Correia Ferreira (2023217920)
  Samuel Marques Riça (2023206471)
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "ast.h"
#include "semantics.h"


// TYPE UTILITIES

static const char *type_names[] = {
    "int", "double", "boolean", "void", "String[]", "undef"};

const char *type_to_string(enum type t)
{
    if (t == type_none)
        return "";
    return type_names[t];
}

enum type category_to_type(enum category c)
{
    switch (c)
    {
    case Int:
        return type_int;
    case Double:
        return type_double;
    case Bool:
        return type_boolean;
    case Void:
        return type_void;
    case StringArray:
        return type_string_array;
    default:
        return type_undef;
    }
}

static enum type string_to_type(const char *s)
{
    if (!s)
        return type_undef;
    if (strcmp(s, "int") == 0)
        return type_int;
    if (strcmp(s, "double") == 0)
        return type_double;
    if (strcmp(s, "boolean") == 0)
        return type_boolean;
    if (strcmp(s, "String[]") == 0)
        return type_string_array;
    if (strcmp(s, "void") == 0)
        return type_void;
    return type_undef;
}

static int is_numeric(enum type t)
{
    return t == type_int || t == type_double;
}

static int is_compatible(enum type from, enum type to)
{
    if (from == type_undef || to == type_undef)
        return 0;
    if (from == to)
        return 1;
    if (from == type_int && to == type_double)
        return 1;
    return 0;
}

static int is_reserved_underscore(const char *name)
{
    return (strcmp(name, "_") == 0);
}

static char *clean_num_literal(const char *s)
{
    char *cleaned = malloc(strlen(s) + 1);
    int j = 0;
    for (int i = 0; s[i] != '\0'; i++)
    {
        if (s[i] != '_')
        {
            cleaned[j++] = s[i];
        }
    }
    cleaned[j] = '\0';
    return cleaned;
}

static int param_name_exists_in_node(struct node *params_node, const char *name, int limit)
{
    for (int i = 0; i < limit; i++)
    {
        struct node *pd = getchild(params_node, i);
        struct node *pid = getchild(pd, 1);
        if (strcmp(pid->token, name) == 0)
            return 1;
    }
    return 0;
}

// GLOBAL STATE

class_table *gtable = NULL;
static int sem_errors = 0;

// PARAM-LIST HELPERS

static void add_param(param_entry **list, const char *type_str)
{
    param_entry *p = malloc(sizeof(param_entry));
    p->type_str = strdup(type_str);
    p->next = NULL;
    if (!*list)
    {
        *list = p;
        return;
    }
    param_entry *cur = *list;
    while (cur->next)
        cur = cur->next;
    cur->next = p;
}

static void free_params(param_entry *p)
{
    while (p)
    {
        param_entry *nxt = p->next;
        free(p->type_str);
        free(p);
        p = nxt;
    }
}

// Cria uma string do tipo "nome_metodo(type1,type2)" -> caller must free the result"
static char *make_signature(const char *name, param_entry *params)
{
    int len = strlen(name) + 3;
    for (param_entry *p = params; p; p = p->next)
        len += strlen(p->type_str) + 1;
    char *sig = malloc(len);
    strcpy(sig, name);
    strcat(sig, "(");
    for (param_entry *p = params; p; p = p->next)
    {
        strcat(sig, p->type_str);
        if (p->next)
            strcat(sig, ",");
    }
    strcat(sig, ")");
    return sig;
}

// SYMBOL-TABLE HELPERS

// Adiciona o simbolo à lista de simbolos.
static symbol_entry *add_symbol(symbol_entry **list, const char *name,
                                enum type t, int is_method, int is_param,
                                int line, int col)
{
    symbol_entry *s = malloc(sizeof(symbol_entry));
    s->name = strdup(name);
    s->type_str = (char *)type_to_string(t);
    s->is_method = is_method;
    s->is_param = is_param;
    s->params = NULL;
    s->line = line;
    s->col = col;
    s->next = NULL;
    if (!*list)
    {
        *list = s;
        return s;
    }
    symbol_entry *cur = *list;
    while (cur->next)
        cur = cur->next;
    cur->next = s;
    return s;
}

// Função auxiliar para encontrar o simbolo (fieldDecl ou MethodDecl) através do nome
static symbol_entry *find_symbol(symbol_entry *list, const char *name)
{
    for (symbol_entry *cur = list; cur; cur = cur->next)
        if (strcmp(cur->name, name) == 0)
            return cur;
    return NULL;
}

//Função auxiliar para encontrar métodos por assinatura
// Ou seja, recebe uma signature, tipo fatorial(int, int) e vê se está na lista de simbolos
static symbol_entry *find_method_by_sig(symbol_entry *list,
                                        const char *name, param_entry *params)
{
    char *target = make_signature(name, params);
    for (symbol_entry *cur = list; cur; cur = cur->next)
    {
        if (!cur->is_method)
            continue;
        char *sig = make_signature(cur->name, cur->params);
        int match = strcmp(sig, target) == 0;
        free(sig);
        if (match)
        {
            free(target);
            return cur;
        }
    }
    free(target);
    return NULL;
}

// Função auxiliar para procura de variáveis em tabelas de métodos
static symbol_entry *find_variable(method_table *mt, const char *name)
{
    symbol_entry *s = find_symbol(mt->symbols, name);
    if (s && !s->is_method)
        return s;

    for (s = gtable->symbols; s; s = s->next)
    {
        if (strcmp(s->name, name) == 0 && !s->is_method)
        {
            return s;
        }
    }

    return NULL;
}

// Função que ajuda a adicionar elementos à tabela de métodos
static method_table *add_method_table(const char *name, const char *sig)
{
    method_table *mt = malloc(sizeof(method_table));
    mt->name = strdup(name);
    mt->signature = strdup(sig);
    mt->symbols = NULL;
    mt->next = NULL;
    if (!gtable->methods)
    {
        gtable->methods = mt;
        return mt;
    }
    method_table *cur = gtable->methods;
    while (cur->next)
        cur = cur->next;
    cur->next = mt;
    return mt;
}

// PASSO 1 - Primeira passagem para consruir a tabela global

static void process_field_decl(struct node *member) {
    struct node *type_node = getchild(member, 0);
    struct node *id_node = getchild(member, 1);
    enum type v_type = category_to_type(type_node->category);

    int already_defined = 0;
    symbol_entry *s = gtable->symbols;
    while (s)
    {
        // Procuramos os conflitos com variaveis globais e ignoramos os métodos
        if (!s->is_method && strcmp(s->name, id_node->token) == 0)
        {
            already_defined = 1;
            break;
        }
        s = s->next;
    }

    if (is_reserved_underscore(id_node->token))
    {
        printf("Line %d, col %d: Symbol _ is reserved\n", id_node->token_line, id_node->token_column);
        sem_errors++;
        member->type = type_undef;
    }
    else if (already_defined)
    {
        printf("Line %d, col %d: Symbol %s already defined\n", id_node->token_line, id_node->token_column, id_node->token);
        sem_errors++;
        member->type = type_undef;
    }
    else
    {
        add_symbol(&gtable->symbols, id_node->token, v_type, 0, 0, id_node->token_line, id_node->token_column);
    }
}

static void process_method_decl(struct node * member) {
    struct node *header = getchild(member, 0);
    struct node *type_node = getchild(header, 0);
    struct node *id_node = getchild(header, 1);
    struct node *params_node = getchild(header, 2);
    enum type ret = category_to_type(type_node->category);

    /* Build parameter type list from MethodParams children */
    param_entry *method_params = NULL;
    int pidx = 0;
    struct node *pd;
    while ((pd = getchild(params_node, pidx++)) != NULL)
    {
        struct node *pt = getchild(pd, 0); /* type child of ParamDecl */
        struct node *pid = getchild(pd, 1);
        if (is_reserved_underscore(pid->token))
        {
            printf("Line %d, col %d: Symbol _ is reserved\n",
                    pid->token_line, pid->token_column);
            sem_errors++;
        }
        else if (param_name_exists_in_node(params_node, pid->token, pidx - 1))
        {
            printf("Line %d, col %d: Symbol %s already defined\n",
                    pid->token_line, pid->token_column, pid->token);
            sem_errors++;
        }
        add_param(&method_params,
                    type_to_string(category_to_type(pt->category)));
    }

    if (is_reserved_underscore(id_node->token))
    {
        printf("Line %d, col %d: Symbol _ is reserved\n",
                id_node->token_line, id_node->token_column);
        sem_errors++;
    }

    else if (find_method_by_sig(gtable->symbols, id_node->token, method_params))
    {
        char *sig = make_signature(id_node->token, method_params);
        printf("Line %d, col %d: Symbol %s already defined\n",
                id_node->token_line, id_node->token_column, sig);
        sem_errors++;
        free(sig);
        free_params(method_params);
        member->type = type_undef;
    }
    else
    {
        symbol_entry *ms = add_symbol(&gtable->symbols, id_node->token, ret,
                                        1, 0,
                                        id_node->token_line, id_node->token_column);
        ms->params = method_params;
        char *sig = make_signature(id_node->token, method_params);
        add_method_table(id_node->token, sig);
        free(sig);
    }
}

// A primeira passagem envolve logo a construção da tabela global
// Ele guarda logo as variaveis globais existentes e os metodos no gtable, pq o java não necessita de declaraçao antes da chamada.
// Para isto, basta percorrer a AST à procura de declaraçoes de métodos e fieldDecl.
static void build_global_table(struct node *program)
{
    struct node *class_id = getchild(program, 0);
    gtable = malloc(sizeof(class_table));
    gtable->name = strdup(class_id->token);
    gtable->symbols = NULL;
    gtable->methods = NULL;

    int idx = 1;
    struct node *member;
    while ((member = getchild(program, idx++)) != NULL)
    {

        if (member->category == FieldDecl)
        {
            process_field_decl(member);
        }
        else if (member->category == MethodDecl)
        {
            process_method_decl(member);
        }
    }
}

// PASSO 2 - Popular as tabelas de métodos locais, com a sua propria informação tipo:
// 

static void populate_method_tables(struct node *program)
{
    // Ponteiro responsável por ir buscar métodos à lista ligada da informaçao dos métodos
    method_table *cur_mt = gtable->methods;

    int idx = 1; // Começa pelo idx 1 pq o idx 0 é o nome da classe
    struct node *member; // Nó da arvore que vai ser analisado
    while ((member = getchild(program, idx++)) != NULL) // Vai percorrendo sempre a AST pelos filhos
    {
        if (member->category != MethodDecl || member->type == type_undef) // Se não for um metodo ou nao tiver tipo definido, salta para o proximo
            continue;

        struct node *header = getchild(member, 0); // Pega no cabeçalho da funçao para fazer extração de elementos
        struct node *type_node = getchild(header, 0);
        struct node *params_node = getchild(header, 2);

        method_table *mt = cur_mt; // Guarda uma cópia da info do método que estamos a utilizar e o cur_mt salta logo para o proximo elemento
        if (!mt)
            break;
        cur_mt = cur_mt->next;

        // Esta parte vai tratar do retorno da função.
        enum type ret = category_to_type(type_node->category); // O type_node->category tem tipo "Int" e vai passar pra type_int, por ex.
        add_symbol(&mt->symbols, "return", ret, 0, 0, type_node->token_line, type_node->token_column); // Adiciona à tabela local do método, o return

        // Processamento dos parâmetros
        int pidx = 0;
        struct node *pd; // Este nó vai corresponder aos parâmetros
        while ((pd = getchild(params_node, pidx++)) != NULL) // Iteramos no a no pela lista de parametros
        {
            struct node *pt = getchild(pd, 0); // Parameter type
            struct node *pid = getchild(pd, 1); // Parameter name
            enum type p_type = category_to_type(pt->category); // Categoria do no. Mais uma vez Int -> type_int.

            if (!is_reserved_underscore(pid->token) && !find_symbol(mt->symbols, pid->token)) // Se não for nem um underscore nem estiver já na tabela local, adiciona à tabela local
            {
                add_symbol(&mt->symbols, pid->token, p_type, 0, 1,
                           pid->token_line, pid->token_column);
            }
        }
    }
}

// Passo 3 - Type Checking

// Funçao auxiliar que recebe o tipo de operaçao e retorna uma string da operação
static const char *get_op_string(enum category c)
{
    switch (c)
    {
    case Assign:
        return "=";
    case Add:
    case Plus:
        return "+";
    case Sub:
    case Minus:
        return "-";
    case Mul:
        return "*";
    case Div:
        return "/";
    case Mod:
        return "%";
    case Eq:
        return "==";
    case Ne:
        return "!=";
    case Lt:
        return "<";
    case Gt:
        return ">";
    case Le:
        return "<=";
    case Ge:
        return ">=";
    case And:
        return "&&";
    case Or:
        return "||";
    case Xor:
        return "^";
    case Lshift:
        return "<<";
    case Rshift:
        return ">>";
    case Not:
        return "!";
    default:
        return "?";
    }
}

static void check_arithmetic_operation(struct node *expr) {

    // Vê se as operações aritméticas estão bem "feitas" digamos assim
    struct node *l = getchild(expr, 0); // Peg ano lado esquerdo da operação
    struct node *r = getchild(expr, 1); // Pega no lado direito da operação
    if (!is_numeric(l->type) || !is_numeric(r->type)) // Se os tipos não forem ambos numéricos, dá logo erro
    {
        printf("Line %d, col %d: Operator %s cannot be applied to types %s, %s\n",
                expr->token_line, expr->token_column,
                get_op_string(expr->category),
                type_to_string(l->type), type_to_string(r->type));
        sem_errors++;
        expr->type = type_undef;
    }
    else
    {
        expr->type = (l->type == type_double || r->type == type_double) // Se um deles for double, então fica double. Se não forem, n
                            ? type_double
                            : type_int;
    }
}

static void resolve_method_call(struct node *expr) {
    struct node *id_node = getchild(expr, 0);

    // Vê se o nome da função é um underscore
    if (is_reserved_underscore(id_node->token))
    {
        printf("Line %d, col %d: Symbol _ is reserved\n",
                id_node->token_line, id_node->token_column);
        sem_errors++;
        expr->type = type_undef;
        id_node->type = type_undef;
        return;
    }


    int num_args = countchildren(expr) - 1;

    enum type arg_types[128];
    for (int i = 0; i < num_args && i < 128; i++)
        arg_types[i] = getchild(expr, i + 1)->type;  // Vai buscar o tipo dos parâmetros e guarda-os num array.

    symbol_entry *exact_match = NULL;
    symbol_entry *compatible_match = NULL;
    int compatible_count = 0;

    for (symbol_entry *sym = gtable->symbols; sym; sym = sym->next)
    {
        if (!sym->is_method || strcmp(sym->name, id_node->token) != 0) // Vai percorrer a lista de simbolos, ignorando tudo o que não é metodo e não tenha o nome da função a ser chamada
            continue;

        int num_params = 0;
        for (param_entry *p = sym->params; p; p = p->next) // Verifica o número de parametros da função selecionada na lista de simbolos
            num_params++;
        if (num_params != num_args) // Se a chamada nao tiver o mesmo número de parâmetros da selecionada, passa ao proximo
            continue;

        // Aqui é preciso um match e compatibilidade. Isto porque, a função pode ser tipo fatorial(double, double) e receber int's como parâmetros
        int is_exact = 1, is_comp = 1; // Assumimos que a method call é 100% compativel e um match exato
        param_entry *p = sym->params; // Pegamos nos parametros do simbolo que estamos a analisar
        for (int i = 0; i < num_args; i++, p = p->next)
        {
            enum type pt = string_to_type(p->type_str); // Verificamos o tipo do parâmetro
            if (arg_types[i] != pt) // Se o tipo dos parametros da função chamada for diferente da função comparada, o match falha logo
                is_exact = 0;
            if (!is_compatible(arg_types[i], pt)) // Verificamos compatibilidade, tipo ints com doubles, etc. -> Se não for, zero
                is_comp = 0;
        }
        if (is_exact) // Se for exato, retorna esse mesmo simbolo da lista.
        {
            exact_match = sym;
            break;
        }
        if (is_comp) // Se for compativel, guarda a referencia. Precisa de armazenar o número de compatibilidade, isto pq pode haver metodos ambiguos e é preciso dar erro
        {
            compatible_match = sym;
            compatible_count++;
        }
    }

    if (exact_match) // Se for um exact match, ou seja, os tipos dos parametros exatamente igual aos da call:
    {
        expr->type = string_to_type(exact_match->type_str); // O nó da chamada assume o tipo de retorno do método
        id_node->param_sig = make_signature("", exact_match->params); // Grava a assinatura do método
        id_node->type = type_none; // O identificar fica como type_none, porque em Juc os métodos não têm tipo neles próprios, so no retorno
    }
    else if (compatible_count == 1) // Se for compatível
    {
        expr->type = string_to_type(compatible_match->type_str); // Igual à anterir
        id_node->param_sig = make_signature("", compatible_match->params);
        id_node->type = type_none;
    }
    else if (compatible_count > 1) // Se houver mais que um método que possa servir a chamada
    {
        param_entry *arg_list = NULL; // Cria uma lista para aparecer na mensagem de erro
        for (int i = 0; i < num_args; i++)
        {
            add_param(&arg_list, type_to_string(getchild(expr, i + 1)->type));
        }
        char *sig = make_signature(id_node->token, arg_list); // Cria a assinatura para a mensagem de erro.

        printf("Line %d, col %d: Reference to method %s is ambiguous\n",
                id_node->token_line, id_node->token_column, sig);
        sem_errors++;

        free(sig);
        free_params(arg_list);
        expr->type = type_undef;
        id_node->type = type_undef;
    }
    else // Simbolo nao foi encontrado
    {
        param_entry *attempted = NULL; // Guarda o tipo dos parametros para aparecer no erro e imprimir dps
        for (int i = 0; i < num_args; i++)
            add_param(&attempted, type_to_string(arg_types[i]));

        char *sig = make_signature(id_node->token, attempted);
        printf("Line %d, col %d: Cannot find symbol %s\n",
                id_node->token_line, id_node->token_column, sig);
        free(sig);

        free_params(attempted);
        sem_errors++;
        expr->type = type_undef;
        id_node->type = type_undef;
    }
}

static void check_natural_exp(struct node *expr) {
    // Faz a verificação para ver se o número natural é ok ou nao
    char buf[1024];
        int j = 0;
        for (int i = 0; expr->token[i] != '\0'; i++)
        {
            if (expr->token[i] != '_')
            {
                buf[j++] = expr->token[i];
            }
        }
        buf[j] = '\0';

        unsigned long long val = strtoull(buf, NULL, 10);

        if (val > 2147483647ULL)
        {
            printf("Line %d, col %d: Number %s out of bounds\n",
                   expr->token_line, expr->token_column, expr->token);
            sem_errors++;
        }
        expr->type = type_int;
}

static void check_decimal_expr(struct node *expr) {
    // Faz a verificação para ver se o número decimal é ok ou nao
    char buf[1024];
    int j = 0;
    for (int i = 0; expr->token[i] != '\0'; i++)
    {
        if (expr->token[i] != '_')
        {
            buf[j++] = expr->token[i];
        }
    }
    buf[j] = '\0';

    double val = strtod(buf, NULL);
    int out_of_bounds = 0;

    if (isinf(val))
    {
        out_of_bounds = 1;
    }

    else if (val == 0.0)
    {
        int is_zero = 1;

        for (int i = 0; buf[i] != '\0'; i++)
        {
            if (buf[i] == 'e' || buf[i] == 'E')
                break;
            if (buf[i] >= '1' && buf[i] <= '9')
            {
                is_zero = 0;
                break;
            }
        }

        if (!is_zero)
        {
            out_of_bounds = 1;
        }
    }

    if (out_of_bounds)
    {
        printf("Line %d, col %d: Number %s out of bounds\n",
                expr->token_line, expr->token_column, expr->token);
        sem_errors++;
    }

    expr->type = type_double;
}

static void check_expression(struct node *expr, method_table *mt)
// Uma das funçoes mais importantes do analisador semântico, grande responsável pela análise das expressoes (type checking)
// Começa das folhas, i.e, pelos nºs e variaveis, descobrindo os seus tipos pela tabela de simbolos
// Depois sobe para o pai que é o operador e usa os tipos dos filhos para decidir o tipo dos pais.
{
    if (!expr)
        return;

    int cidx = 0;
    struct node *child; 
    // Este loop tem o objetivo de começar sempre pelos filhos do nó da expressão.
    while ((child = getchild(expr, cidx)) != NULL)
    {
        // se for uma chamada, não chegou às folhas ainda, tem de ir mais "afundo" digamos
        if (expr->category == Call && cidx == 0)
        {
            cidx++;
            continue;
        }
        // recursividade para começar sempre pelos filhos
        check_expression(child, mt);
        cidx++;
    }

    // Vai fazer o devido type checking
    switch (expr->category)
    {

    case Natural:
    {
        check_natural_exp(expr);
        break;
    }
    case Decimal:
    {
        check_decimal_expr(expr);
        break;
    }

    case BoolLit:
        expr->type = type_boolean;
        break;

    case StrLit:
        expr->type = type_none;
        break;

    case Identifier:
    {
        if (is_reserved_underscore(expr->token))
        {
            printf("Line %d, col %d: Symbol %s is reserved\n",
                   expr->token_line, expr->token_column, expr->token);
            sem_errors++;
            expr->type = type_undef;
            break;
        }
        symbol_entry *sym = find_variable(mt, expr->token);
        if (!sym)
        {
            printf("Line %d, col %d: Cannot find symbol %s\n",
                   expr->token_line, expr->token_column, expr->token);
            sem_errors++;
            expr->type = type_undef;
        }
        else
        {
            expr->type = string_to_type(sym->type_str);
        }
        break;
    }

    case Plus:
    case Minus:
    {
        struct node *operand = getchild(expr, 0);
        if (!is_numeric(operand->type))
        {
            printf("Line %d, col %d: Operator %s cannot be applied to type %s\n",
                   expr->token_line, expr->token_column,
                   get_op_string(expr->category), type_to_string(operand->type));
            sem_errors++;
            expr->type = type_undef;
        }
        else
        {
            expr->type = operand->type;
        }
        break;
    }

    case Not:
    {
        struct node *operand = getchild(expr, 0);
        if (operand->type != type_boolean)
        {
            printf("Line %d, col %d: Operator ! cannot be applied to type %s\n",
                   expr->token_line, expr->token_column,
                   type_to_string(operand->type));
            sem_errors++;
        }
        expr->type = type_boolean;
        break;
    }

    case Add:
    case Sub:
    case Mul:
    case Div:
    case Mod:
    {
        check_arithmetic_operation(expr);
        break;
    }

    // Levou aqui uma pequena alteraçao neste XOR na Meta 4, para suportar operaçoes logicas de bits em booleans
    case Xor:
    {
        struct node *l = getchild(expr, 0);
        struct node *r = getchild(expr, 1);
        if ((l->type == type_int && r->type == type_int)) {
            expr->type = type_int;
        } else if (l->type == type_boolean && r->type == type_boolean) {
            expr->type = type_boolean;
        } else {
            printf("Line %d, col %d: Operator %s cannot be applied to types %s, %s\n",
                   expr->token_line, expr->token_column,
                   get_op_string(expr->category),
                   type_to_string(l->type), type_to_string(r->type));
            sem_errors++;
            expr->type = type_undef;
        }
        break;
    }
    case Lshift:
    case Rshift:
    {
        struct node *l = getchild(expr, 0);
        struct node *r = getchild(expr, 1);
        if (l->type != type_int || r->type != type_int)
        {
            printf("Line %d, col %d: Operator %s cannot be applied to types %s, %s\n",
                   expr->token_line, expr->token_column,
                   get_op_string(expr->category),
                   type_to_string(l->type), type_to_string(r->type));
            sem_errors++;
        }
        expr->type = type_int;
        break;
    }

    case Lt:
    case Gt:
    case Le:
    case Ge:
    {
        struct node *l = getchild(expr, 0);
        struct node *r = getchild(expr, 1);
        if (!is_numeric(l->type) || !is_numeric(r->type))
        {
            printf("Line %d, col %d: Operator %s cannot be applied to types %s, %s\n",
                   expr->token_line, expr->token_column,
                   get_op_string(expr->category),
                   type_to_string(l->type), type_to_string(r->type));
            sem_errors++;
        }
        expr->type = type_boolean;
        break;
    }

    case Eq:
    case Ne:
    {
        struct node *l = getchild(expr, 0);
        struct node *r = getchild(expr, 1);
        int both_num = is_numeric(l->type) && is_numeric(r->type);
        int both_bool = (l->type == type_boolean && r->type == type_boolean);
        if (!both_num && !both_bool)
        {
            printf("Line %d, col %d: Operator %s cannot be applied to types %s, %s\n",
                   expr->token_line, expr->token_column,
                   get_op_string(expr->category),
                   type_to_string(l->type), type_to_string(r->type));
            sem_errors++;
        }
        expr->type = type_boolean;
        break;
    }

    case And:
    case Or:
    {
        struct node *l = getchild(expr, 0);
        struct node *r = getchild(expr, 1);
        if (l->type != type_boolean || r->type != type_boolean)
        {
            printf("Line %d, col %d: Operator %s cannot be applied to types %s, %s\n",
                   expr->token_line, expr->token_column,
                   get_op_string(expr->category),
                   type_to_string(l->type), type_to_string(r->type));
            sem_errors++;
        }
        expr->type = type_boolean;
        break;
    }

    case Assign:
    {
        struct node *l = getchild(expr, 0);
        struct node *r = getchild(expr, 1);

        if (!is_compatible(r->type, l->type) || l->type == type_string_array)
        {
            printf("Line %d, col %d: Operator = cannot be applied to types %s, %s\n",
                   expr->token_line, expr->token_column,
                   type_to_string(l->type), type_to_string(r->type));
            sem_errors++;
        }
        expr->type = l->type;
        break;
    }

    case Length:
    {
        struct node *target = getchild(expr, 0);
        if (target->type != type_string_array)
        {
            printf("Line %d, col %d: Operator .length cannot be applied to type %s\n",
                   expr->token_line, expr->token_column,
                   type_to_string(target->type));
            sem_errors++;
        }
        expr->type = type_int;
        break;
    }

    case ParseArgs:
    {
        struct node *id_node = getchild(expr, 0);
        struct node *idx_node = getchild(expr, 1);

        if (id_node->type != type_string_array || idx_node->type != type_int)
        {
            printf("Line %d, col %d: Operator Integer.parseInt cannot be applied to types %s, %s\n",
                   expr->token_line, expr->token_column,
                   type_to_string(id_node->type), type_to_string(idx_node->type));
            sem_errors++;
        }
        expr->type = type_int;
        break;
    }

    case Call:
    {
        resolve_method_call(expr);
        break;
    }

    default:
        break;
    }
}

// Em vez de ser para verificar os tipos das operações, etc, esta valida as regras dos ifs, returns, etc...
static void check_statement(struct node *stmt, method_table *mt)
{
    if (!stmt)
        return;

    switch (stmt->category)
    {
    
    // Se a statement for do tipo block, tem de percorrer tudo lá dentro
    case Block:
    {
        int i = 0;
        struct node *ch;
        while ((ch = getchild(stmt, i++)) != NULL)
            check_statement(ch, mt);
        break;
    }


    case If:
    {
        struct node *cond = getchild(stmt, 0);
        struct node *then_stmt = getchild(stmt, 1);
        struct node *else_stmt = getchild(stmt, 2);
        // Faz uma analise primeiro da condição do if, ver se os tipos batem, etc...
        check_expression(cond, mt);
        // Se a condição resultante n for boolean, dá erro
        if (cond->type != type_boolean)
        {
            printf("Line %d, col %d: Incompatible type %s in if statement\n",
                   cond->token_line, cond->token_column,
                   type_to_string(cond->type));
            sem_errors++;
        }
        // Verifica a statement do then e do else, por recursividade
        check_statement(then_stmt, mt);
        if (else_stmt)
            check_statement(else_stmt, mt);
        break;
    }

    case While:
    {
        // O mesmo do if
        struct node *cond = getchild(stmt, 0);
        struct node *body = getchild(stmt, 1);
        check_expression(cond, mt);
        if (cond->type != type_boolean)
        {
            printf("Line %d, col %d: Incompatible type %s in while statement\n",
                   cond->token_line, cond->token_column,
                   type_to_string(cond->type));
            sem_errors++;
        }
        check_statement(body, mt);
        break;
    }

    case Return:
    {
        // Vai à tabela de simbolos buscar o return que anotámos e nomeamo-lo expected
        symbol_entry *ret_sym = find_symbol(mt->symbols, "return");
        enum type expected = ret_sym ? string_to_type(ret_sym->type_str) : type_void;
        struct node *ret_expr = getchild(stmt, 0); // oq vem à frente do return

        // Analisa a expressão que o utilizador escreveu no return
        if (ret_expr)
        {
            check_expression(ret_expr, mt);
            enum type actual = ret_expr->type;
            //
            if (expected == type_void) //se tiver algo a frente do return e for void, não funciona
            {
                printf("Line %d, col %d: Incompatible type %s in return statement\n",
                       ret_expr->token_line, ret_expr->token_column,
                       type_to_string(actual));
                sem_errors++;
            }
            else if (!is_compatible(actual, expected)) // Se o valor de retorno atual n for compativel com o expected, imcompativel
            {
                printf("Line %d, col %d: Incompatible type %s in return statement\n",
                       ret_expr->token_line, ret_expr->token_column,
                       type_to_string(actual));
                sem_errors++;
            }
        }
        else
        {
            // se não tiver valor à frente do return mas o expected n for void, então devia retornar algo
            if (expected != type_void)
            {
                printf("Line %d, col %d: Incompatible type void in return statement\n",
                       stmt->token_line, stmt->token_column);
                sem_errors++;
            }
        }
        break;
    }

    case Print:
    {
        struct node *child = getchild(stmt, 0);
        if (child->category != StrLit)
        {
            check_expression(child, mt);
            // Só aceita int, double e boolean. Tudo o resto (void, String[], undef) dá erro
            if (child->type != type_int && child->type != type_double && child->type != type_boolean)
            {
                printf("Line %d, col %d: Incompatible type %s in System.out.print statement\n",
                       child->token_line, child->token_column, type_to_string(child->type));
                sem_errors++;
            }
        }
        break;
    }

    case Call:
    case Assign:
    case ParseArgs:
        check_expression(stmt, mt);
        break;

    case VarDecl:
        break;

    default:
        break;
    }
}

static void check_methods(struct node *program)
{
    // pega no conteudo das funçoes e vai analisa-los
    method_table *cur_mt = gtable->methods;

    int idx = 1;
    struct node *member;
    while ((member = getchild(program, idx++)) != NULL) // vai percorrer a AST à procura de declaraçoes de metodos q não tenham problemas
    {
        if (member->category != MethodDecl || member->type == type_undef)
            continue;

        
        struct node *body = getchild(member, 1); // Pega no corpo da funçao
        method_table *mt = cur_mt;
        if (!mt)
            break;
        cur_mt = cur_mt->next;

        int sidx = 0;
        struct node *stmt;
        while ((stmt = getchild(body, sidx++)) != NULL) // Vai percorrer o corpo da funçao
        {
            // Se for uma declaraçao de variavel, vai adicionar à tabela de simbolos/dar erro
            if (stmt->category == VarDecl)
            {
                struct node *vt = getchild(stmt, 0);
                struct node *vid = getchild(stmt, 1);
                enum type v_type = category_to_type(vt->category);
                // Verifica a definiçao de variaveis locais
                if (is_reserved_underscore(vid->token))
                {
                    printf("Line %d, col %d: Symbol _ is reserved\n", vid->token_line, vid->token_column);
                    sem_errors++;
                }
                // Verifica variaveis já declaradas
                else if (find_symbol(mt->symbols, vid->token))
                {
                    printf("Line %d, col %d: Symbol %s already defined\n",
                           vid->token_line, vid->token_column, vid->token);
                    sem_errors++;
                }
                // Se não adiciona à tabela do metodo
                else
                {
                    add_symbol(&mt->symbols, vid->token, v_type, 0, 0,
                               vid->token_line, vid->token_column);
                }
            }
            // Se não for, manda verificar
            else
            {
                check_statement(stmt, mt);
            }
        }
    }
}

// Funções para imprimir as tabelas e etc...


static void print_class_table(void)
{
    printf("===== Class %s Symbol Table =====\n", gtable->name);
    for (symbol_entry *s = gtable->symbols; s; s = s->next)
    {
        if (s->is_method)
        {
            int len = 3;
            for (param_entry *p = s->params; p; p = p->next)
            {
                len += strlen(p->type_str) + 1;
            }
            char *params_str = malloc(len);
            strcpy(params_str, "(");
            for (param_entry *p = s->params; p; p = p->next)
            {
                strcat(params_str, p->type_str);
                if (p->next)
                    strcat(params_str, ",");
            }
            strcat(params_str, ")");

            printf("%s\t%s\t%s\n", s->name, params_str, s->type_str);
            free(params_str);
        }
        else
        {
            printf("%s\t\t%s\n", s->name, s->type_str);
        }
    }
}

static void print_method_table(method_table *mt)
{
    printf("===== Method %s Symbol Table =====\n", mt->signature);
    for (symbol_entry *s = mt->symbols; s; s = s->next)
    {
        if (s->is_param)
            printf("%s\t\t%s\tparam\n", s->name, s->type_str);
        else
            printf("%s\t\t%s\n", s->name, s->type_str);
    }
}

void print_tables(void)
{
    if (!gtable)
        return;
    print_class_table();
    for (method_table *mt = gtable->methods; mt; mt = mt->next)
    {
        printf("\n");
        print_method_table(mt);
    }
}

// Funções para imprimir a AST com algumas informaçoes a mais necessárias

extern char *category_name[];

static int is_expr_node(enum category c)
{
    switch (c)
    {
    case Natural:
    case Decimal:
    case BoolLit:
    case StrLit:
    case Identifier:
    case Plus:
    case Minus:
    case Not:
    case Add:
    case Sub:
    case Mul:
    case Div:
    case Mod:
    case Xor:
    case Lshift:
    case Rshift:
    case Lt:
    case Gt:
    case Le:
    case Ge:
    case Eq:
    case Ne:
    case And:
    case Or:
    case Assign:
    case Length:
    case ParseArgs:
    case Call:
        return 1;
    default:
        return 0;
    }
}

void show_annotated(struct node *node, int depth)
{
    if (!node)
        return;

    for (int i = 0; i < depth; i++)
        printf("..");

    if (node->token == NULL)
        printf("%s", category_name[node->category]);
    else
        printf("%s(%s)", category_name[node->category], node->token);

    if (node->param_sig)
    {
        printf(" - %s", node->param_sig);
    }
    else if (node->category == StrLit)
    {
        printf(" - String");
    }
    else if (is_expr_node(node->category) && node->type != type_none)
    {
        printf(" - %s", type_to_string(node->type));
    }

    printf("\n");

    struct node_list *child = node->children->next;
    while (child)
    {
        show_annotated(child->node, depth + 1);
        child = child->next;
    }
}

// Basicamente é o q o lex chama para iniciar a análise semântica

int semantic_analysis(struct node *program)
{
    sem_errors = 0;
    if (!program)
        return 0;

    build_global_table(program);
    populate_method_tables(program);
    check_methods(program);

    return sem_errors;
}
