/*
 João Tomás Correia Ferreira (2023217920)
 Samuel Marques Riça (2023206471)
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "semantics.h"
#include "codegen.h"

#define MAX_STRLITS 2048 

int temporary; // Variável q controla os registos temporários
static int label_counter = 1; // Variavel qcontrola os blocos de codigo (labels)
static int current_is_main = 0; // Flag para controlo de main. Se a flag estiver a 1, retorna um int em vez de um void em caso de ausencia de return.
static const char *current_locals[4096]; // Armazena o nome das variaveis que são locais à função que está a ser gerada
static int current_local_count = 0; // Indice q indica quantas variaveis locais ja foram guardadas
static int current_logical = 0; // Serve para condiçoes do tipo && ou ||, para armazenar valores de um dos lados da operaçao
static char *strlit_table[MAX_STRLITS]; // Guarda todas as strings presentes no codigo
static int   strlit_count = 0; // Indica quantas strings ja foram utilizadas

// Procura, na função, quantas condiçoes com && ou || existem para alocação previa de memória
static int count_logical(struct node *n) {
    if (!n) return 0;
    int sum = (n->category == And || n->category == Or) ? 1 : 0;
    
    int i = 0;
    struct node *c;
    while ((c = getchild(n, i++)) != NULL) {
        sum += count_logical(c);
    }
    return sum;
}

static int new_label(void) { 
    return label_counter++;
}

// Função para transferir tipo da semântica para o tipo da llvm
static const char *type_to_llvm(enum type t) {
    switch (t) {
        case type_int:          return "i32";
        case type_double:       return "double";
        case type_boolean:      return "i1";
        case type_void:         return "void";
        case type_string_array: return "i8**";
        default:                return "i32";
    }
}

// Faz o mesmo q a anterior, mas o tipo é o q está na AST.
static const char *get_llvm_type(enum category c) {
    switch (c) {
        case Int:         return "i32";
        case Double:      return "double";
        case Bool:        return "i1";
        case Void:        return "void";
        case StringArray: return "i8**";
        default:          return "i32";
    }
}

// Vai procurar na lista de variáveis locais se já existe o nome da variável ou nao.
// Serve para, por ex, distinguir variáveis locais de globais.
static int is_local(const char *name) {
    for (int i = 0; i < current_local_count; i++) {
        if (strcmp(current_locals[i], name) == 0) return 1;
    }
    return 0;
}

// Praticamente igual à is_local, mas procura na tabela global.
static int is_global(const char *name) {
    extern class_table *gtable;
    if (!gtable) return 0;
    for (symbol_entry *s = gtable->symbols; s; s = s->next)
        if (!s->is_method && strcmp(s->name, name) == 0)
            return 1;
    return 0;
}

// Adiciona string à lista de strings
static int get_strlit_index(const char *token) {
    for (int i = 0; i < strlit_count; i++)
        if (strcmp(strlit_table[i], token) == 0) return i;
    if (strlit_count < MAX_STRLITS) {
        strlit_table[strlit_count] = strdup(token);
        return strlit_count++;
    }
    return 0;
}

// Calcula o tamanho real da string, sem as "", contabilizando sequencias de escape e um caracter a mais para o \0
static int strlit_llvm_len(const char *token) {
    int len = 0;
    const char *p = token + 1;
    while (*p && *p != '"') {
        if (*p == '\\') p++;
        len++;
        p++;
    }
    return len + 1;
}

// Converte caracteres de escape tipo \n, \t, etc para formato hexadecimal q é entendido pelo LLVM
static char *strlit_to_llvm(const char *token) {
    int max_len = (strlen(token) + 1) * 3;
    char *out = malloc(max_len);
    char *dst = out;
    const char *p = token + 1;
    while (*p && *p != '"') {
        if (*p == '\\') {
            p++;
            switch (*p) {
                case 'n':  dst += sprintf(dst, "\\0A"); break;
                case 't':  dst += sprintf(dst, "\\09"); break;
                case 'r':  dst += sprintf(dst, "\\0D"); break;
                case 'f':  dst += sprintf(dst, "\\0C"); break;
                case '\\': dst += sprintf(dst, "\\5C"); break;
                case '"':  dst += sprintf(dst, "\\22"); break;
                default:   dst += sprintf(dst, "\\%02X", (unsigned char)*p); break;
            }
        } else {
            *dst++ = *p;
        }
        p++;
    }
    dst += sprintf(dst, "\\00");
    *dst = '\0';
    return out;
}

// Percorre a AST à procura de nós do tipo StrLit
static void collect_strlits(struct node *node) {
    if (!node) return;
    if (node->category == StrLit)  // Condição de paragem, i.e, se for StrLit, chama a funçao para adicionar strings à lista
        get_strlit_index(node->token);
    struct node_list *child = node->children;
    while ((child = child->next) != NULL) // Percorre a AST recursivamente
        collect_strlits(child->node);
}

// Funçao q percorre a lista de strings, sendo q para cada uma, gera uma linha de codigo LLVM
static void emit_strlits(void) {
    for (int i = 0; i < strlit_count; i++) {
        int   len  = strlit_llvm_len(strlit_table[i]);
        char *llvm = strlit_to_llvm(strlit_table[i]);
        // Tipo: @.strlit.0 = private unnamed_addr constant [11 x i8] c"Ola\0A\00"
        // @.strlit.0 é o ID da string.
        // [11 x i8] indica o tamanho do array
        // c"Ola\0A\00" Indica os caracteres
        printf("@.strlit.%d = private unnamed_addr constant [%d x i8] c\"%s\"\n", i, len, llvm);
        free(llvm);
    }
    if (strlit_count > 0) printf("\n");
}

int  codegen_expression(struct node *expr, const char *args_name);
void codegen_statement(struct node *stmt, enum category ret_cat, const char *args_name);

// Funçao q separamos da codegen_expression para simplificar um pouco a funçao
// É responsavel por tratar de operaçoes arimeticas
int arithmetic_expr(struct node * expr, const char *args_name) {
    struct node *l = getchild(expr, 0); // Membro esquerdo
    struct node *r = getchild(expr, 1); // Membro direito
    // Chama recursivamente a codegen_expression, para resolver cenas tipo (2+3) * 4
    int lr = codegen_expression(l, args_name); // Armazena o registo virtual onde os resultados de cada calculo foi armazenado
    int rr = codegen_expression(r, args_name);

    int use_double = (l->type == type_double || r->type == type_double); // Se algum dos lados for double
    if (use_double && l->type == type_int) { // Se o lado esquerdo for um inteiro, mas a conta for pra ser um double
        // O compilador gera uma instrução de conversao e guarda o resultado num novo registo
        int cast = temporary++;
        printf("  %%%d = sitofp i32 %%%d to double\n", cast, lr);
        lr = cast;
    }
    if (use_double && r->type == type_int) { // O mesmo para o lado direito
        int cast = temporary++;
        printf("  %%%d = sitofp i32 %%%d to double\n", cast, rr);
        rr = cast;
    }

    int reg = temporary++; // Registo q vai armazenar o resultado final da operacao
    if (use_double) { // Se for decimal, utiliza operadores com o prefixo f
        const char *op = (expr->category == Add) ? "fadd" : (expr->category == Sub) ? "fsub" :
                            (expr->category == Mul) ? "fmul" : (expr->category == Div) ? "fdiv" : "frem";
        printf("  %%%d = %s double %%%d, %%%d\n", reg, op, lr, rr); // Escreve no ficheiro .ll tipo: $registo = operacao tipo %operando1 %operando2
    } else {
        const char *op = (expr->category == Add) ? "add"  : (expr->category == Sub) ? "sub"  :
                            (expr->category == Mul) ? "mul"  : (expr->category == Div) ? "sdiv" : "srem";
        printf("  %%%d = %s i32 %%%d, %%%d\n", reg, op, lr, rr);
    }
    return reg; // Retorna o número do registo criado
}

int decimal_expression(struct node * expr) {
    char buf[2048]; int j = 0;
    for (int i = 0; expr->token[i]; i++)
        if (expr->token[i] != '_') buf[j++] = expr->token[i]; // Remove tb underscores presentes nos nºs decimais
        // É permitido pelo Juc mas o LLVM n suporta _
    buf[j] = '\0';
    
    char exponent[256] = "";
    char *e_ptr = strpbrk(buf, "eE"); // Se for notacao cientifica
    if (e_ptr) {
        strcpy(exponent, e_ptr); // Armazena a parte do expoente no buffer
        *e_ptr = '\0';
    }   
    
    // Formatação dos nºs decimais para ficarem no formato 5.0 por exemplo.
    char formatted_mantissa[4096];
    int len = strlen(buf);
    
    if (buf[0] == '.') { // Se for um numero .5, tem de adicionar um 0 no inicio para ficar 0.5
        sprintf(formatted_mantissa, "0%s", buf);
    } else if (strchr(buf, '.') == NULL) { // Se não tiver ., tem de adicionar um .0. Exemplo: 5 -> 5.0
        sprintf(formatted_mantissa, "%s.0", buf);
    } else if (buf[len - 1] == '.') { // Se acabar com ponto, transforma em 5.0 por exemplo. Ex: 5. -> 5.0
        sprintf(formatted_mantissa, "%s0", buf);
    } else { // Se estiver correto, mantém
        strcpy(formatted_mantissa, buf);
    }
    
    int reg = temporary++; // Novo registo virtual
    printf("  %%%d = fadd double 0.0, %s%s\n", reg, formatted_mantissa, exponent);
    // Aqui usa fadd pq mexe com doubles
    return reg;
}

// Funçao auxiliar para transformar a assinatura do juc (factorial(int, double)) em algo que o llvm perceba, para resolvermos o method overloading
void call_mangle_names(struct node *id_node, char * mangled_name, int * num_expected, enum type * expected_types, int is_call_real_main ) {
     if (id_node->param_sig && strlen(id_node->param_sig) > 2) {
        char sig_copy[2048];
        strncpy(sig_copy, id_node->param_sig, 2047); // Copia os parâmetros
        sig_copy[2047] = '\0';
        char *p = sig_copy + 1; // Salta o primeiro parêntese e remove o ultimo abaixo
        char *end = strchr(p, ')');
        if (end) *end = '\0';
        
        // Separa os argumentos pela virgula
        char *tok = strtok(p, ",");
        while (tok) {
            if (!is_call_real_main) { // Se não for a função main
                strcat(mangled_name, "."); // adiciona um ponto
                if (strcmp(tok, "String[]") == 0) { // Se o parâmetro for do tipo String[], para o LLVM vai passar a ser StringArray pq ele não gosta muito de []
                    strcat(mangled_name, "StringArray");
                } else {
                    strcat(mangled_name, tok); //Se não só concatena o tipo
                }
            }

            // Analisa qual é o tipo suposto de cada parâmetro e guarda-o no expected_types
            if (strcmp(tok, "int") == 0) 
                expected_types[(*num_expected)++] = type_int;
            else if (strcmp(tok, "double") == 0) 
                expected_types[(*num_expected)++] = type_double;
            else if (strcmp(tok, "boolean") == 0) 
                expected_types[(*num_expected)++] = type_boolean;
            else if (strcmp(tok, "String[]") == 0) 
                expected_types[(*num_expected)++] = type_string_array;
            tok = strtok(NULL, ",");
        }
    }else { // Se não tiver parâmetros, ou seja, se tiver () vazios, adiciona apenas um .empty para manter o formato
        if (!is_call_real_main) {
            strcat(mangled_name, ".empty");
        }
    }
}

// Função responsavel pela chamada de métodos
// É um pouco mais complexa qo normal, pq o Java suporta overloading de métodos e o llvm nao
int call_expression_analysis(struct node *expr, const char *args_name) {
    struct node *id_node = getchild(expr, 0); // Identificador do método
    int num_args = countchildren(expr) - 1; // Nº de argumentos - 1 por causa do nome do método
    enum type ret_type = expr->type;  // Tipo de retorno do método

    char mangled_name[2048]; // Vai armazenar o nome q o LLVM vai receber - vai ficar algo no formato @soma.int.int
    strcpy(mangled_name, id_node->token); // Copia-se logo o nome original do método
    int is_call_real_main = (strcmp(id_node->token, "main") == 0 && id_node->param_sig && strcmp(id_node->param_sig, "(String[])") == 0);
    // a variavel de cima é um bool para ver se é a funçao main(String[] args), pq esta n pode ter nome alterado
    enum type expected_types[512];
    int num_expected = 0;
    
    call_mangle_names(id_node, mangled_name, &num_expected, expected_types, is_call_real_main);

    char args_buf[4096] = ""; // Buffer para construir a string de argumentos
    for (int i = 0; i < num_args; i++) {
        struct node *arg = getchild(expr, i + 1); // Vai buscar cada argumento
        
        if (arg->type == type_string_array) { // Se for do tipo array de strings
            // Basicamente desdobra o array de strings em 2 parâmetros
            int argc_reg = temporary++; 
            printf("  %%%d = load i32, i32* %%%s.argc\n", argc_reg, arg->token); // Ou seja, o nº de elementos
            int argv_reg = temporary++;
            printf("  %%%d = load i8**, i8*** %%%s.argv\n", argv_reg, arg->token); // O ponteiro para o primeiro argumento
            
            char arg_str[2048];
                if (i > 0) strcat(args_buf, ", "); // Se não for o primeiro argumento, adiciona virgula para separar do arg anterior
            sprintf(arg_str, "i32 %%%d, i8** %%%d", argc_reg, argv_reg); // Escreve logo dois argumentos de uma vez
            strcat(args_buf, arg_str); // Adiciona ao buffer global
        } else {
            int arg_reg = codegen_expression(arg, args_name); // Calcula o valor do argumento
            enum type actual_type = arg->type; // Tipo real do argumento
            int final_reg = arg_reg; // Registo onde o valor do arg esta guardado
            
            // Se a função espera double mas recebeu int, é preciso converter de int pra double, atualizando o tipo para a formatação da chamada
            if (i < num_expected && expected_types[i] == type_double && actual_type == type_int) {
                final_reg = temporary++;
                printf("  %%%d = sitofp i32 %%%d to double\n", final_reg, arg_reg);
                actual_type = type_double;
            }

            char arg_str[2048];
            if (i > 0) strcat(args_buf, ", "); // Virgula de separaçao se n for o primeiro argumento
            sprintf(arg_str, "%s %%%d", type_to_llvm(actual_type), final_reg);
            strcat(args_buf, arg_str); // Adiciona ao buffer de argumentos
        }
    }

    const char *llvm_ret = type_to_llvm(ret_type);
    if (ret_type == type_void) { // Se o tipo de retorno for void retorna isso, se nao retorna o q tem a retornar
        printf("  call void @%s(%s)\n", mangled_name, args_buf);
        return -1;
    } else {
        int reg = temporary++;
        printf("  %%%d = call %s @%s(%s)\n", reg, llvm_ret, mangled_name, args_buf);
        return reg;
    }
}

// Traduz operações, literais ou variaveis para a linguagem LLVM
int codegen_expression(struct node *expr, const char *args_name) {
    if (!expr) return -1;

    switch (expr->category) {
        case Natural: {
            char buf[256];
            int j = 0;
            for (int i = 0; expr->token[i]; i++)
                if (expr->token[i] != '_')
                    buf[j++] = expr->token[i]; // serve para remover os _, copiando os nºs para um buffer
            buf[j] = '\0';
            int reg = temporary++; // Incrementa o registo temporario
            printf("  %%%d = add i32 0, %s\n", reg, buf); // %%%d -> Nº de registo; i32 -> Inteiro de 32 bits; %s: Numero limpo sem underscores
            return reg;
        }

        case Decimal: {
            return decimal_expression(expr);
        }

        case BoolLit: {
            int val = (strcmp(expr->token, "true") == 0) ? 1 : 0;
            int reg = temporary++;
            printf("  %%%d = add i1 0, %d\n", reg, val);
            return reg;
        }

        case Identifier: {
            const char *llvm_type = type_to_llvm(expr->type); // Pega na variavel, tipo int para i32. (formato normal para llvm)
            int reg = temporary++; // Cria um novo registo temporario

            // Se o parametro de argumentos da main. Ou seja, tipo String[] args
            if (args_name && strcmp(expr->token, args_name) == 0) {
                printf("  %%%d = load i8**, i8*** %%%s.argv\n", reg, args_name); //Carrega os argumentos no sitio especifico para isso
            } else if (is_local(expr->token)) { // Se for uma variavel local, gera um acesso com %
                printf("  %%%d = load %s, %s* %%%s\n", reg, llvm_type, llvm_type, expr->token);
            } else if (is_global(expr->token)) { // Se for global, gera sem %
                printf("  %%%d = load %s, %s* @%s\n", reg, llvm_type, llvm_type, expr->token);
            } else {
                printf("  %%%d = load %s, %s* %%%s\n", reg, llvm_type, llvm_type, expr->token);
            }
            return reg;
        }

        case Length: {
            struct node *id = getchild(expr, 0);
            int argc_reg = temporary++;
            printf("  %%%d = load i32, i32* %%%s.argc\n", argc_reg, id->token);
            int len_reg = temporary++;
            printf("  %%%d = sub i32 %%%d, 1\n", len_reg, argc_reg);
            return len_reg;
        }

        case ParseArgs: {
            struct node *id_node  = getchild(expr, 0);
            struct node *idx_node = getchild(expr, 1);
            int idx_reg = codegen_expression(idx_node, args_name);
            int adj_reg = temporary++;
            printf("  %%%d = add i32 %%%d, 1\n", adj_reg, idx_reg);
            int argv_reg = temporary++;
            printf("  %%%d = load i8**, i8*** %%%s.argv\n", argv_reg, id_node->token);
            int ptr_reg = temporary++;
            printf("  %%%d = getelementptr i8*, i8** %%%d, i32 %%%d\n", ptr_reg, argv_reg, adj_reg);
            int str_reg = temporary++;
            printf("  %%%d = load i8*, i8** %%%d\n", str_reg, ptr_reg);
            int result_reg = temporary++;
            printf("  %%%d = call i32 @atoi(i8* %%%d)\n", result_reg, str_reg);
            return result_reg;
        }

        case Minus: {
            struct node *operand = getchild(expr, 0);
            int op_reg = codegen_expression(operand, args_name);
            int reg = temporary++;
            if (operand->type == type_double) printf("  %%%d = fneg double %%%d\n", reg, op_reg);
            else printf("  %%%d = sub i32 0, %%%d\n", reg, op_reg);
            return reg;
        }

        case Plus: return codegen_expression(getchild(expr, 0), args_name);

        case Not: {
            int op_reg = codegen_expression(getchild(expr, 0), args_name);
            int reg = temporary++;
            printf("  %%%d = xor i1 %%%d, 1\n", reg, op_reg);
            return reg;
        }

        case Add: case Sub: case Mul: case Div: case Mod: {
           return arithmetic_expr(expr, args_name);
        }

        case Xor: {
            int lr = codegen_expression(getchild(expr, 0), args_name);
            int rr = codegen_expression(getchild(expr, 1), args_name);
            int reg = temporary++;
            printf("  %%%d = xor %s %%%d, %%%d\n", reg, type_to_llvm(expr->type), lr, rr);
            return reg;
        }

        case Lshift: {
            int lr = codegen_expression(getchild(expr, 0), args_name);
            int rr = codegen_expression(getchild(expr, 1), args_name);
            int reg = temporary++;
            printf("  %%%d = shl i32 %%%d, %%%d\n", reg, lr, rr);
            return reg;
        }

        case Rshift: {
            int lr = codegen_expression(getchild(expr, 0), args_name);
            int rr = codegen_expression(getchild(expr, 1), args_name);
            int reg = temporary++;
            printf("  %%%d = ashr i32 %%%d, %%%d\n", reg, lr, rr);
            return reg;
        }

        case And: {
            // Cria um novo label, ou seja, sera util para os saltos
            int lbl = new_label();
            int logic_idx = current_logical++; // Vai buscar o indice reservado pela count_logical incialmente
            
            // Analisa a expressão da esquerda
            int left_reg = codegen_expression(getchild(expr, 0), args_name);
            printf("  store i1 %%%d, i1* %%logical.tmp.%d\n", left_reg, logic_idx);
            
            // Se for verdadeiro, Salta para and.right para avaliar o resto
            // Se for falso, salta logo para o fim (and.end), ignorando oq está a direita
            printf("  br i1 %%%d, label %%and.right.%d, label %%and.end.%d\n", left_reg, lbl, lbl); 
            
            // Label da direita
            printf("and.right.%d:\n", lbl);
            // Analisa a expressão da direita, em caso de operaçoes completas
            int right_reg = codegen_expression(getchild(expr, 1), args_name);
            // Guarda na memoria o valor
            printf("  store i1 %%%d, i1* %%logical.tmp.%d\n", right_reg, logic_idx);
            printf("  br label %%and.end.%d\n", lbl);
            
            printf("and.end.%d:\n", lbl); // Encontram-se ambos aq
            // Aloca mais um registo temporario
            int final_reg = temporary++;
            printf("  %%%d = load i1, i1* %%logical.tmp.%d\n", final_reg, logic_idx); // Vai buscar à memoria o valor final (o da esquerda se foi falso ou o da direita se a esquerda foi verdadeira)
            return final_reg;
        }

        case Or: {
            // O mesmo do and praticamente
            int lbl = new_label();
            int logic_idx = current_logical++;
            
            
            int left_reg = codegen_expression(getchild(expr, 0), args_name);

            printf("  store i1 %%%d, i1* %%logical.tmp.%d\n", left_reg, logic_idx);
            
            // Só muda aq.
            // Se o valor da esquerda for verdadeiro, a condiçao já é verdadeira, pode saltar logo pro fim
            printf("  br i1 %%%d, label %%or.end.%d, label %%or.right.%d\n", left_reg, lbl, lbl); 
            
            printf("or.right.%d:\n", lbl);
            // Alcançado se o valor da esquerda for falso
            int right_reg = codegen_expression(getchild(expr, 1), args_name);

            printf("  store i1 %%%d, i1* %%logical.tmp.%d\n", right_reg, logic_idx);
            printf("  br label %%or.end.%d\n", lbl);
            
            printf("or.end.%d:\n", lbl);
            int final_reg = temporary++;

            printf("  %%%d = load i1, i1* %%logical.tmp.%d\n", final_reg, logic_idx); // Guarda o resultado da operaçao - O da esquerda se era vdd ao inicio ou o da direita se o primeiro era falso
            return final_reg;
        }

        case Eq: case Ne: case Lt: case Gt: case Le: case Ge: {
            struct node *l = getchild(expr, 0);
            struct node *r = getchild(expr, 1);
            int lr = codegen_expression(l, args_name);
            int rr = codegen_expression(r, args_name);

            int use_double = (l->type == type_double || r->type == type_double);
            if (use_double && l->type == type_int) {
                int cast = temporary++;
                printf("  %%%d = sitofp i32 %%%d to double\n", cast, lr);
                lr = cast;
            }
            if (use_double && r->type == type_int) {
                int cast = temporary++;
                printf("  %%%d = sitofp i32 %%%d to double\n", cast, rr);
                rr = cast;
            }
            // Atenção novamente às operações, para serem sempre do mesmo tipo para ser interpretado pelo llvm
            int reg = temporary++;
            if (use_double) {
                const char *op = (expr->category == Eq) ? "oeq" : (expr->category == Ne) ? "one" :
                                 (expr->category == Lt) ? "olt" : (expr->category == Gt) ? "ogt" :
                                 (expr->category == Le) ? "ole" : "oge";
                printf("  %%%d = fcmp %s double %%%d, %%%d\n", reg, op, lr, rr);
            } else if (l->type == type_boolean) {
                const char *op = (expr->category == Eq) ? "eq" : "ne";
                printf("  %%%d = icmp %s i1 %%%d, %%%d\n", reg, op, lr, rr);
            } else {
                const char *op = (expr->category == Eq) ? "eq"  : (expr->category == Ne) ? "ne"  :
                                 (expr->category == Lt) ? "slt" : (expr->category == Gt) ? "sgt" :
                                 (expr->category == Le) ? "sle" : "sge";
                printf("  %%%d = icmp %s i32 %%%d, %%%d\n", reg, op, lr, rr);
            }
            return reg;
        }

        case Assign: {
            struct node *id_node   = getchild(expr, 0); // Variável de destino
            struct node *expr_node = getchild(expr, 1); // Conteudo à direita do igual
            int expr_reg  = codegen_expression(expr_node, args_name); // Calcula o valor à direita do igual
            int final_reg = expr_reg;

            if (id_node->type == type_double && expr_node->type == type_int) { // Se à direita for int e à esquerda double, então tem de transformar o nº
                final_reg = temporary++;
                printf("  %%%d = sitofp i32 %%%d to double\n", final_reg, expr_reg);
            }

            const char *llvm_type = type_to_llvm(id_node->type);
            
            // Se for a String[] args, vai pra um sitio especifico na memoria
            if (args_name && strcmp(id_node->token, args_name) == 0) {
                printf("  store %s %%%d, %s* %%%s.argv\n", llvm_type, final_reg, llvm_type, id_node->token);
            } else if (is_local(id_node->token)) { // Se variavel local, com %
                printf("  store %s %%%d, %s* %%%s\n", llvm_type, final_reg, llvm_type, id_node->token);
            } else if (is_global(id_node->token)) { // Variavel global é sem %
                printf("  store %s %%%d, %s* @%s\n", llvm_type, final_reg, llvm_type, id_node->token);
            } else {
                printf("  store %s %%%d, %s* %%%s\n", llvm_type, final_reg, llvm_type, id_node->token);
            }

            return final_reg;
        }
        
        case Call: {
           return call_expression_analysis(expr, args_name);
        }

        default:
            return -1;
    }
}


void print_statement_analysis(struct node * stmt, const char *args_name) {
    struct node *expr = getchild(stmt, 0); // Vai buscar a string
    if (expr->category == StrLit) {
        int str_idx = get_strlit_index(expr->token); // Vai ver onde está a string na lista de strings
        int len     = strlit_llvm_len(expr->token); // Vê o tamanho da string
        int prt_reg = temporary++; // Cria-se mais um registo temporário
        // Imprime-se a string para a linguagem LLVM
        printf("  %%%d = call i32 (i8*, ...) @printf("
                "i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.string, i32 0, i32 0), "
                "i8* getelementptr inbounds ([%d x i8], [%d x i8]* @.strlit.%d, i32 0, i32 0))\n",
                prt_reg, len, len, str_idx);
    } else {
        int reg = codegen_expression(expr, args_name); // Análise da expressão a ser imprimida
        // Cria um novo registo e imprime o devido texto q tem a imprimir
        if (expr->type == type_int) {
            int prt_reg = temporary++;
            printf("  %%%d = call i32 (i8*, ...) @printf("
                    "i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.int, i32 0, i32 0), "
                    "i32 %%%d)\n", prt_reg, reg);
        } else if (expr->type == type_double) {
            int prt_reg = temporary++; 
            printf("  %%%d = call i32 (i8*, ...) @printf("
                    "i8* getelementptr inbounds ([6 x i8], [6 x i8]* @.str.double, i32 0, i32 0), "
                    "double %%%d)\n", prt_reg, reg);
        } else if (expr->type == type_boolean) {
            // Nao existem booleans em C, logo é preciso mapear.
            int ptr_reg = temporary++;
            // Se o registo reg for 1, escolhe o primeiro ponteiro para string true
            // Se o registo reg for 0, escolhe o ponteiro para string false.
            printf("  %%%d = select i1 %%%d, "
                    "i8* getelementptr inbounds ([5 x i8], [5 x i8]* @.str.true, i32 0, i32 0), "
                    "i8* getelementptr inbounds ([6 x i8], [6 x i8]* @.str.false, i32 0, i32 0)\n",
                    ptr_reg, reg);
            
            // Imprime-se essa string
            int prt_reg = temporary++;
            printf("  %%%d = call i32 (i8*, ...) @printf("
                    "i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.string, i32 0, i32 0), "
                    "i8* %%%d)\n", prt_reg, ptr_reg);
        }
    }
}

// Responsável pela a ação em si
void codegen_statement(struct node *stmt, enum category ret_cat, const char *args_name) {
    if (!stmt) return;

    switch (stmt->category) {
        case Block: {
            int i = 0;
            struct node *child;
            // Em caso de bloco, manda analisar tudo lá dentro
            while ((child = getchild(stmt, i++)) != NULL)
                codegen_statement(child, ret_cat, args_name);
            break;
        }

        case Assign: {
            struct node *id_node   = getchild(stmt, 0); // Pega no identificador (parte à esquerda do =)
            struct node *expr_node = getchild(stmt, 1); // Parte da direita
            int expr_reg  = codegen_expression(expr_node, args_name); // Faz análise da parte à direita do =
            int final_reg = expr_reg; // Armazenamos esse valor

            if (id_node->type == type_double && expr_node->type == type_int) { // Se a variável onde vai ser guardar for do tipo double e o conteudo à direita for do tipo int, é necessario converter
                final_reg = temporary++;
                printf("  %%%d = sitofp i32 %%%d to double\n", final_reg, expr_reg);
            }

            const char *llvm_type = type_to_llvm(id_node->type);
            // Igual ao do codegen_expression
            if (args_name && strcmp(id_node->token, args_name) == 0) {
                printf("  store %s %%%d, %s* %%%s.argv\n", llvm_type, final_reg, llvm_type, id_node->token);
            } else if (is_local(id_node->token)) {
                printf("  store %s %%%d, %s* %%%s\n", llvm_type, final_reg, llvm_type, id_node->token);
            } else if (is_global(id_node->token)) {
                printf("  store %s %%%d, %s* @%s\n", llvm_type, final_reg, llvm_type, id_node->token);
            } else {
                printf("  store %s %%%d, %s* %%%s\n", llvm_type, final_reg, llvm_type, id_node->token);
            }

            break;
        }

        case Print: {
            print_statement_analysis(stmt, args_name);
            break;
        }

        case Return: {
            struct node *expr = getchild(stmt, 0);
            if (expr) {
                int reg = codegen_expression(expr, args_name);
                if (ret_cat == Double && expr->type == type_int) {
                    int cast_reg = temporary++;
                    printf("  %%%d = sitofp i32 %%%d to double\n", cast_reg, reg);
                    printf("  ret double %%%d\n", cast_reg);
                } else {
                    printf("  ret %s %%%d\n", type_to_llvm(expr->type), reg);
                }
            } else {
                if (current_is_main) printf("  ret i32 0\n");
                else printf("  ret void\n");
            }
            printf("unreachable.%d:\n", new_label());
            break;
        }

        case If: {
            struct node *cond      = getchild(stmt, 0);
            struct node *then_stmt = getchild(stmt, 1);
            struct node *else_stmt = getchild(stmt, 2);

            int lbl      = new_label();
            int cond_reg = codegen_expression(cond, args_name);
            // Mais uma vez, análise da condição do if. Se for verdadeira, vai para o label if.then.X
            // Se falso, vai para if.else.X
            printf("  br i1 %%%d, label %%if.then.%d, label %%if.else.%d\n", cond_reg, lbl, lbl);

            // Bloco do then
            printf("if.then.%d:\n", lbl);
            codegen_statement(then_stmt, ret_cat, args_name);
            printf("  br label %%if.end.%d\n", lbl); // Salta para o fim do if, para não entrar no else

            // Bloco do else
            printf("if.else.%d:\n", lbl);
            if (else_stmt) codegen_statement(else_stmt, ret_cat, args_name);
            printf("  br label %%if.end.%d\n", lbl);
            // Vai para o fim do if.
            
            printf("if.end.%d:\n", lbl);
            break;
        }

        case While: {
            struct node *cond      = getchild(stmt, 0);
            struct node *body_stmt = getchild(stmt, 1);

            int lbl = new_label();

            // Salta para o label de verificação do ciclo sempre
            printf("  br label %%while.cond.%d\n", lbl);
            // Label do while
            printf("while.cond.%d:\n", lbl);
            int cond_reg = codegen_expression(cond, args_name); // analise da expressão booleana
            printf("  br i1 %%%d, label %%while.body.%d, label %%while.end.%d\n", cond_reg, lbl, lbl); // Se for verdadeiro, entra no label while.body
            // Se for falso, vai para o label while.end

            // Corpo do while
            printf("while.body.%d:\n", lbl);
            codegen_statement(body_stmt, ret_cat, args_name);
            printf("  br label %%while.cond.%d\n", lbl); // Salta para o label da condição para nova análise

            printf("while.end.%d:\n", lbl); // Término do while
            break;
        }

        case Call:
        case ParseArgs:
            codegen_expression(stmt, args_name);
            break;

        default:
            break;
    }
}

static void get_function_mangled_name(struct node *header, char *mangled_name, int has_string_array) {
    struct node *id_node = getchild(header, 1);
    struct node *params_node = getchild(header, 2);
    
    strcpy(mangled_name, id_node->token);
    
    // Se for o main real, não alteramos o nome
    if (strcmp(id_node->token, "main") == 0 && has_string_array) return;

    // Este mangle segue praticamente o mesmo lá de cima da Call expression
    int pidx = 0;
    struct node *param;
    int param_count = 0;
    while ((param = getchild(params_node, pidx++)) != NULL) {
        struct node *p_type = getchild(param, 0);
        strcat(mangled_name, ".");
        if (p_type->category == Int) strcat(mangled_name, "int");
        else if (p_type->category == Double) strcat(mangled_name, "double");
        else if (p_type->category == Bool) strcat(mangled_name, "boolean");
        else if (p_type->category == StringArray) strcat(mangled_name, "StringArray");
        param_count++;
    }
    if (param_count == 0) strcat(mangled_name, ".empty");
}

static void allocate_function_resources(struct node *method_decl, char *string_array_param_name, int is_real_main) {
    // Obter os nos aqui novamente, pra n ter de passar tantos parametros
    struct node *header = getchild(method_decl, 0);
    struct node *body   = getchild(method_decl, 1);
    struct node *params_node = getchild(header, 2);

    // Regista os parametros na tabela de variaveis locais da função
    current_local_count = 0;
    int search_p_idx = 0;
    struct node *p_node;
    while ((p_node = getchild(params_node, search_p_idx++)) != NULL) {
        current_locals[current_local_count++] = getchild(p_node, 1)->token;
    }

    // Regista as variaveis na tabela de variaveis locais da função
    int search_s_idx = 0;
    struct node *s_node;
    while ((s_node = getchild(body, search_s_idx++)) != NULL) {
        if (s_node->category == VarDecl) {
            current_locals[current_local_count++] = getchild(s_node, 1)->token;
        }
    }

    // Alocação e inicialização dos Parâmetros
    if (is_real_main) {
        // Se for a main, aloca o argc e argv
        printf("  %%%s.argc = alloca i32\n", string_array_param_name);
        printf("  store i32 %%argc, i32* %%%s.argc\n", string_array_param_name);
        printf("  %%%s.argv = alloca i8**\n", string_array_param_name);
        printf("  store i8** %%argv, i8*** %%%s.argv\n", string_array_param_name);
    } else { // Coloca os parâmetros em memória, alocando o espaço necessário para cada um
        int pidx = 0;
        struct node *param;
        while ((param = getchild(params_node, pidx)) != NULL) {
            struct node *p_type = getchild(param, 0);
            char *p_name = getchild(param, 1)->token;
            
            if (p_type->category == StringArray) {
                printf("  %%%s.argc = alloca i32\n", p_name);
                printf("  store i32 %%%s.argc_arg, i32* %%%s.argc\n", p_name, p_name);
                printf("  %%%s.argv = alloca i8**\n", p_name);
                printf("  store i8** %%%s_arg, i8*** %%%s.argv\n", p_name, p_name);
            } else {
                const char *p_llvm = get_llvm_type(p_type->category);
                printf("  %%%s = alloca %s\n", p_name, p_llvm);
                printf("  store %s %%%s_arg, %s* %%%s\n", p_llvm, p_name, p_llvm, p_name);
            }
            pidx++;
        }
    }

    // alocação das variaveis locais da função
    search_s_idx = 0;
    while ((s_node = getchild(body, search_s_idx++)) != NULL) {
        if (s_node->category == VarDecl) {
            struct node *v_type = getchild(s_node, 0);
            struct node *v_id   = getchild(s_node, 1);
            printf("  %%%s = alloca %s\n", v_id->token, get_llvm_type(v_type->category));
        }
    }

    // alocação do espaço necessario para as operaçõos booleanas (&& e ||)
    current_logical = 0;
    int num_logicals = count_logical(body);
    for (int i = 0; i < num_logicals; i++) {
        printf("  %%logical.tmp.%d = alloca i1\n", i);
    }
}

static void codegen_function(struct node *method_decl) {
    struct node *header        = getchild(method_decl, 0); // Header da funçao
    struct node *body          = getchild(method_decl, 1); // Conteudo da funçao
    struct node *ret_type_node = getchild(header, 0); // Return da funçao
    struct node *id_node       = getchild(header, 1); // Nome da funçao
    struct node *params_node   = getchild(header, 2); // parametros
    int sidx;
    struct node *stmt;
    const char *llvm_ret = get_llvm_type(ret_type_node->category);
    
    temporary = 1;
    label_counter = 1; 

    int has_string_array = 0;
    char *string_array_param_name = NULL;
    int search_idx = 0;
    struct node *search_param;
    
    while ((search_param = getchild(params_node, search_idx++)) != NULL) {
        if (getchild(search_param, 0)->category == StringArray) {
            has_string_array = 1;
            string_array_param_name = getchild(search_param, 1)->token;
            break;
        }
    }

    int is_real_main = (strcmp(id_node->token, "main") == 0 && has_string_array);
    current_is_main = is_real_main;
    
    // Transformar o nome da função para o mangled
    char mangled_name[2048];
    get_function_mangled_name(header, mangled_name, has_string_array);

    // Começa a definir a parte inicial da função
    if (is_real_main) {
        printf("define i32 @main(i32 %%argc, i8** %%argv) {\n");
    } else {
        printf("define %s @%s(", llvm_ret, mangled_name);
        int pidx = 0;
        struct node *param;
        while ((param = getchild(params_node, pidx)) != NULL) {
            if (pidx > 0) printf(", ");
            struct node *p_type = getchild(param, 0);
            if (p_type->category == StringArray) {
                printf("i32 %%%s.argc_arg, i8** %%%s_arg", getchild(param, 1)->token, getchild(param, 1)->token);
            } else {
                printf("%s %%%s_arg", get_llvm_type(p_type->category), getchild(param, 1)->token);
            }
            pidx++;
        }
        printf(") {\n");
    }
    printf("entry:\n");

    allocate_function_resources(method_decl, string_array_param_name, is_real_main);

    // Percorre tudo o que não é declaração de variaveis e analisa os statements
    sidx = 0;
    while ((stmt = getchild(body, sidx++)) != NULL) {
        if (stmt->category != VarDecl)
            codegen_statement(stmt, ret_type_node->category, string_array_param_name);
    }

    // Garante que a função termina sempre com uma instrução de retorno válida
    if (is_real_main) printf("  ret i32 0\n");
    else if (ret_type_node->category == Void) printf("  ret void\n");
    else if (ret_type_node->category == Double) printf("  ret double 0.0\n");
    else if (ret_type_node->category == Bool) printf("  ret i1 0\n");
    else printf("  ret i32 0\n");

    printf("}\n\n");
}

void codegen_program(struct node *program) {
    if (!program) return;

    // colecta todas as strings do programa
    collect_strlits(program);

    // Declaração inicial da classe
    printf("declare i32 @printf(i8*, ...)\n");
    printf("declare i32 @atoi(i8*)\n\n");

    // Definição das strings de formato usadas pelo printf
    printf("@.str.int    = private unnamed_addr constant [3 x i8] c\"%%d\\00\"\n");
    printf("@.str.double = private unnamed_addr constant [6 x i8] c\"%%.16e\\00\"\n");
    printf("@.str.string = private unnamed_addr constant [3 x i8] c\"%%s\\00\"\n");
    printf("@.str.true   = private unnamed_addr constant [5 x i8] c\"true\\00\"\n");
    printf("@.str.false  = private unnamed_addr constant [6 x i8] c\"false\\00\"\n\n");

    // Imprime logo todas as strings do programa, definindo-as no .ll
    emit_strlits();

    int idx = 1;
    struct node *member;
    int has_fields = 0;
    // Trata das declarações das funções, tratando como se fossem variaveis globais, percorrendo toda a AST
    while ((member = getchild(program, idx++)) != NULL) {
        if (member->category == FieldDecl) {
            struct node *type_node = getchild(member, 0);
            struct node *id_node   = getchild(member, 1);
            if (type_node->category == Int) printf("@%s = global i32 0\n", id_node->token);
            else if (type_node->category == Double) printf("@%s = global double 0.0\n", id_node->token);
            else if (type_node->category == Bool) printf("@%s = global i1 0\n", id_node->token);
            has_fields = 1;
        }
    }
    if (has_fields) printf("\n");

    idx = 1;
    // percorre novamente a AST, gerando código para cada método da classe
    while ((member = getchild(program, idx++)) != NULL) {
        if (member->category == MethodDecl)
            codegen_function(member);
    }
}