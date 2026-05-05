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


int temporary;
static int label_counter = 1;
static int current_is_main = 0;
static const char *current_locals[4096];
static int current_local_count = 0;
static int current_logical = 0;


static int is_local(const char *name) {
    for (int i = 0; i < current_local_count; i++) {
        if (strcmp(current_locals[i], name) == 0) return 1;
    }
    return 0;
}

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


static int is_global(const char *name) {
    extern class_table *gtable;
    if (!gtable) return 0;
    for (symbol_entry *s = gtable->symbols; s; s = s->next)
        if (!s->is_method && strcmp(s->name, name) == 0)
            return 1;
    return 0;
}

#define MAX_STRLITS 2048
static char *strlit_table[MAX_STRLITS];
static int   strlit_count = 0;

static int get_strlit_index(const char *token) {
    for (int i = 0; i < strlit_count; i++)
        if (strcmp(strlit_table[i], token) == 0) return i;
    if (strlit_count < MAX_STRLITS) {
        strlit_table[strlit_count] = strdup(token);
        return strlit_count++;
    }
    return 0;
}

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

static void collect_strlits(struct node *node) {
    if (!node) return;
    if (node->category == StrLit) get_strlit_index(node->token);
    struct node_list *child = node->children;
    while ((child = child->next) != NULL)
        collect_strlits(child->node);
}

static void emit_strlits(void) {
    for (int i = 0; i < strlit_count; i++) {
        int   len  = strlit_llvm_len(strlit_table[i]);
        char *llvm = strlit_to_llvm(strlit_table[i]);
        printf("@.strlit.%d = private unnamed_addr constant [%d x i8] c\"%s\"\n", i, len, llvm);
        free(llvm);
    }
    if (strlit_count > 0) printf("\n");
}

int  codegen_expression(struct node *expr, const char *args_name);
void codegen_statement(struct node *stmt, enum category ret_cat, const char *args_name);

int codegen_expression(struct node *expr, const char *args_name) {
    if (!expr) return -1;

    switch (expr->category) {
        case Natural: {
            char buf[256]; int j = 0;
            for (int i = 0; expr->token[i]; i++)
                if (expr->token[i] != '_') buf[j++] = expr->token[i];
            buf[j] = '\0';
            int reg = temporary++;
            printf("  %%%d = add i32 0, %s\n", reg, buf);
            return reg;
        }

        case Decimal: {
            char buf[2048]; int j = 0;
            for (int i = 0; expr->token[i]; i++)
                if (expr->token[i] != '_') buf[j++] = expr->token[i];
            buf[j] = '\0';
            
            char exponent[256] = "";
            char *e_ptr = strpbrk(buf, "eE");
            if (e_ptr) {
                strcpy(exponent, e_ptr);
                *e_ptr = '\0';
            }
            
            char formatted_mantissa[4096];
            int len = strlen(buf);
            
            if (buf[0] == '.') {
                sprintf(formatted_mantissa, "0%s", buf);
            } else if (strchr(buf, '.') == NULL) {
                sprintf(formatted_mantissa, "%s.0", buf);
            } else if (buf[len - 1] == '.') {
                sprintf(formatted_mantissa, "%s0", buf);
            } else {
                strcpy(formatted_mantissa, buf);
            }
            
            int reg = temporary++;
            printf("  %%%d = fadd double 0.0, %s%s\n", reg, formatted_mantissa, exponent);
            return reg;
        }

        case BoolLit: {
            int val = (strcmp(expr->token, "true") == 0) ? 1 : 0;
            int reg = temporary++;
            printf("  %%%d = add i1 0, %d\n", reg, val);
            return reg;
        }

        case Identifier: {
            const char *llvm_type = type_to_llvm(expr->type);
            int reg = temporary++;

            if (args_name && strcmp(expr->token, args_name) == 0) {
                printf("  %%%d = load i8**, i8*** %%%s.argv\n", reg, args_name);
            } else if (is_local(expr->token)) {
                printf("  %%%d = load %s, %s* %%%s\n", reg, llvm_type, llvm_type, expr->token);
            } else if (is_global(expr->token)) {
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

            int reg = temporary++;
            if (use_double) {
                const char *op = (expr->category == Add) ? "fadd" : (expr->category == Sub) ? "fsub" :
                                 (expr->category == Mul) ? "fmul" : (expr->category == Div) ? "fdiv" : "frem";
                printf("  %%%d = %s double %%%d, %%%d\n", reg, op, lr, rr);
            } else {
                const char *op = (expr->category == Add) ? "add"  : (expr->category == Sub) ? "sub"  :
                                 (expr->category == Mul) ? "mul"  : (expr->category == Div) ? "sdiv" : "srem";
                printf("  %%%d = %s i32 %%%d, %%%d\n", reg, op, lr, rr);
            }
            return reg;
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
            int lbl = new_label();
            int logic_idx = current_logical++; // Usa um índice global
            
            int left_reg = codegen_expression(getchild(expr, 0), args_name);
            printf("  store i1 %%%d, i1* %%logical.tmp.%d\n", left_reg, logic_idx);
            
            printf("  br i1 %%%d, label %%and.right.%d, label %%and.end.%d\n", left_reg, lbl, lbl);
            
            printf("and.right.%d:\n", lbl);
            int right_reg = codegen_expression(getchild(expr, 1), args_name);
            printf("  store i1 %%%d, i1* %%logical.tmp.%d\n", right_reg, logic_idx);
            printf("  br label %%and.end.%d\n", lbl);
            
            printf("and.end.%d:\n", lbl);
            int final_reg = temporary++;
            printf("  %%%d = load i1, i1* %%logical.tmp.%d\n", final_reg, logic_idx);
            return final_reg;
        }

        case Or: {
            int lbl = new_label();
            int logic_idx = current_logical++; // Captura o índice global
            
            
            int left_reg = codegen_expression(getchild(expr, 0), args_name);

            printf("  store i1 %%%d, i1* %%logical.tmp.%d\n", left_reg, logic_idx);
            
            printf("  br i1 %%%d, label %%or.end.%d, label %%or.right.%d\n", left_reg, lbl, lbl);
            
            printf("or.right.%d:\n", lbl);
            int right_reg = codegen_expression(getchild(expr, 1), args_name);

            printf("  store i1 %%%d, i1* %%logical.tmp.%d\n", right_reg, logic_idx);
            printf("  br label %%or.end.%d\n", lbl);
            
            printf("or.end.%d:\n", lbl);
            int final_reg = temporary++;

            printf("  %%%d = load i1, i1* %%logical.tmp.%d\n", final_reg, logic_idx);
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
            // Usa getchild(expr, 0) na codegen_expression ou getchild(stmt, 0) na codegen_statement
            struct node *id_node   = getchild(expr, 0); 
            struct node *expr_node = getchild(expr, 1);
            int expr_reg  = codegen_expression(expr_node, args_name);
            int final_reg = expr_reg;

            if (id_node->type == type_double && expr_node->type == type_int) {
                final_reg = temporary++;
                printf("  %%%d = sitofp i32 %%%d to double\n", final_reg, expr_reg);
            }

            const char *llvm_type = type_to_llvm(id_node->type);
                        
            if (args_name && strcmp(id_node->token, args_name) == 0) {
                printf("  store %s %%%d, %s* %%%s.argv\n", llvm_type, final_reg, llvm_type, id_node->token);
            } else if (is_local(id_node->token)) {
                printf("  store %s %%%d, %s* %%%s\n", llvm_type, final_reg, llvm_type, id_node->token);
            } else if (is_global(id_node->token)) {
                printf("  store %s %%%d, %s* @%s\n", llvm_type, final_reg, llvm_type, id_node->token);
            } else {
                printf("  store %s %%%d, %s* %%%s\n", llvm_type, final_reg, llvm_type, id_node->token);
            }

            return final_reg;
        }
        
        case Call: {
            struct node *id_node = getchild(expr, 0);
            int num_args = countchildren(expr) - 1;
            enum type ret_type = expr->type;

            char mangled_name[2048];
            strcpy(mangled_name, id_node->token);
            int is_call_real_main = (strcmp(id_node->token, "main") == 0 && id_node->param_sig && strcmp(id_node->param_sig, "(String[])") == 0);

            enum type expected_types[512];
            int num_expected = 0;
            
            if (id_node->param_sig && strlen(id_node->param_sig) > 2) {
                char sig_copy[2048];
                strncpy(sig_copy, id_node->param_sig, 2047);
                sig_copy[2047] = '\0';
                char *p = sig_copy + 1; /* skip '(' */
                char *end = strchr(p, ')');
                if (end) *end = '\0';
                
                char *tok = strtok(p, ",");
                while (tok) {
                    if (!is_call_real_main) {
                        strcat(mangled_name, ".");
                        if (strcmp(tok, "String[]") == 0) {
                            strcat(mangled_name, "StringArray");
                        } else {
                            strcat(mangled_name, tok);
                        }
                    }

                    if (strcmp(tok, "int") == 0) expected_types[num_expected++] = type_int;
                    else if (strcmp(tok, "double") == 0) expected_types[num_expected++] = type_double;
                    else if (strcmp(tok, "boolean") == 0) expected_types[num_expected++] = type_boolean;
                    else if (strcmp(tok, "String[]") == 0) expected_types[num_expected++] = type_string_array;
                    tok = strtok(NULL, ",");
                }
            }else {
                if (!is_call_real_main) {
                    strcat(mangled_name, ".empty");
                }
            }

            char args_buf[4096] = "";
            for (int i = 0; i < num_args; i++) {
                struct node *arg = getchild(expr, i + 1);
                
                if (arg->type == type_string_array) {
                    int argc_reg = temporary++;
                    printf("  %%%d = load i32, i32* %%%s.argc\n", argc_reg, arg->token);
                    int argv_reg = temporary++;
                    printf("  %%%d = load i8**, i8*** %%%s.argv\n", argv_reg, arg->token);
                    
                    char arg_str[2048];
                    if (i > 0) strcat(args_buf, ", ");
                    sprintf(arg_str, "i32 %%%d, i8** %%%d", argc_reg, argv_reg);
                    strcat(args_buf, arg_str);
                } else {
                    int arg_reg = codegen_expression(arg, args_name);
                    enum type actual_type = arg->type;
                    int final_reg = arg_reg;
                    
                    if (i < num_expected && expected_types[i] == type_double && actual_type == type_int) {
                        final_reg = temporary++;
                        printf("  %%%d = sitofp i32 %%%d to double\n", final_reg, arg_reg);
                        actual_type = type_double;
                    }

                    char arg_str[2048];
                    if (i > 0) strcat(args_buf, ", ");
                    sprintf(arg_str, "%s %%%d", type_to_llvm(actual_type), final_reg);
                    strcat(args_buf, arg_str);
                }
            }

            const char *llvm_ret = type_to_llvm(ret_type);
            if (ret_type == type_void) {
                printf("  call void @%s(%s)\n", mangled_name, args_buf);
                return -1;
            } else {
                int reg = temporary++;
                printf("  %%%d = call %s @%s(%s)\n", reg, llvm_ret, mangled_name, args_buf);
                return reg;
            }
        }

        default:
            return -1;
    }
}

void codegen_statement(struct node *stmt, enum category ret_cat, const char *args_name) {
    if (!stmt) return;

    switch (stmt->category) {
        case Block: {
            int i = 0;
            struct node *child;
            while ((child = getchild(stmt, i++)) != NULL)
                codegen_statement(child, ret_cat, args_name);
            break;
        }

        case Assign: {
            struct node *id_node   = getchild(stmt, 0);
            struct node *expr_node = getchild(stmt, 1);
            int expr_reg  = codegen_expression(expr_node, args_name);
            int final_reg = expr_reg;

            if (id_node->type == type_double && expr_node->type == type_int) {
                final_reg = temporary++;
                printf("  %%%d = sitofp i32 %%%d to double\n", final_reg, expr_reg);
            }

            const char *llvm_type = type_to_llvm(id_node->type);
            
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
            struct node *expr = getchild(stmt, 0);
            if (expr->category == StrLit) {
                int str_idx = get_strlit_index(expr->token);
                int len     = strlit_llvm_len(expr->token);
                int prt_reg = temporary++; // Captura o i32 do printf
                printf("  %%%d = call i32 (i8*, ...) @printf("
                       "i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.string, i32 0, i32 0), "
                       "i8* getelementptr inbounds ([%d x i8], [%d x i8]* @.strlit.%d, i32 0, i32 0))\n",
                       prt_reg, len, len, str_idx);
            } else {
                int reg = codegen_expression(expr, args_name);
                if (expr->type == type_int) {
                    int prt_reg = temporary++; // Captura o i32 do printf
                    printf("  %%%d = call i32 (i8*, ...) @printf("
                           "i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.int, i32 0, i32 0), "
                           "i32 %%%d)\n", prt_reg, reg);
                } else if (expr->type == type_double) {
                    int prt_reg = temporary++; // Captura o i32 do printf
                    printf("  %%%d = call i32 (i8*, ...) @printf("
                           "i8* getelementptr inbounds ([6 x i8], [6 x i8]* @.str.double, i32 0, i32 0), "
                           "double %%%d)\n", prt_reg, reg);
                } else if (expr->type == type_boolean) {
                    int ptr_reg = temporary++;
                    printf("  %%%d = select i1 %%%d, "
                           "i8* getelementptr inbounds ([5 x i8], [5 x i8]* @.str.true, i32 0, i32 0), "
                           "i8* getelementptr inbounds ([6 x i8], [6 x i8]* @.str.false, i32 0, i32 0)\n",
                           ptr_reg, reg);
                    int prt_reg = temporary++; // Captura o i32 do printf
                    printf("  %%%d = call i32 (i8*, ...) @printf("
                           "i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.string, i32 0, i32 0), "
                           "i8* %%%d)\n", prt_reg, ptr_reg);
                }
            }
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

            printf("  br i1 %%%d, label %%if.then.%d, label %%if.else.%d\n", cond_reg, lbl, lbl);

            printf("if.then.%d:\n", lbl);
            codegen_statement(then_stmt, ret_cat, args_name);
            printf("  br label %%if.end.%d\n", lbl);

            printf("if.else.%d:\n", lbl);
            if (else_stmt) codegen_statement(else_stmt, ret_cat, args_name);
            printf("  br label %%if.end.%d\n", lbl);

            printf("if.end.%d:\n", lbl);
            break;
        }

        case While: {
            struct node *cond      = getchild(stmt, 0);
            struct node *body_stmt = getchild(stmt, 1);

            int lbl = new_label();

            printf("  br label %%while.cond.%d\n", lbl);
            printf("while.cond.%d:\n", lbl);
            int cond_reg = codegen_expression(cond, args_name);
            printf("  br i1 %%%d, label %%while.body.%d, label %%while.end.%d\n", cond_reg, lbl, lbl);

            printf("while.body.%d:\n", lbl);
            codegen_statement(body_stmt, ret_cat, args_name);
            printf("  br label %%while.cond.%d\n", lbl);

            printf("while.end.%d:\n", lbl);
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

static void codegen_function(struct node *method_decl) {
    struct node *header        = getchild(method_decl, 0);
    struct node *body          = getchild(method_decl, 1);
    struct node *ret_type_node = getchild(header, 0);
    struct node *id_node       = getchild(header, 1);
    struct node *params_node   = getchild(header, 2);

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
    
    char mangled_name[2048];
    strcpy(mangled_name, id_node->token);

    if (!is_real_main) {
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
        if (param_count == 0) {
            strcat(mangled_name, ".empty");
        }
    }

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

    current_local_count = 0;
    int search_p_idx = 0;
    struct node *p_node;
    while ((p_node = getchild(params_node, search_p_idx++)) != NULL) {
        current_locals[current_local_count++] = getchild(p_node, 1)->token;
    }
    int search_s_idx = 0;
    struct node *s_node;
    while ((s_node = getchild(body, search_s_idx++)) != NULL) {
        if (s_node->category == VarDecl) {
            current_locals[current_local_count++] = getchild(s_node, 1)->token;
        }
    }

    if (is_real_main) {
        printf("  %%%s.argc = alloca i32\n", string_array_param_name);
        printf("  store i32 %%argc, i32* %%%s.argc\n", string_array_param_name);
        printf("  %%%s.argv = alloca i8**\n", string_array_param_name);
        printf("  store i8** %%argv, i8*** %%%s.argv\n", string_array_param_name);
    } else {
        int pidx = 0;
        struct node *param;
        while ((param = getchild(params_node, pidx)) != NULL) {
            struct node *p_type = getchild(param, 0);
            const char *p_llvm  = get_llvm_type(p_type->category);
            char       *p_name  = getchild(param, 1)->token;
            
            if (p_type->category == StringArray) {
                printf("  %%%s.argc = alloca i32\n", p_name);
                printf("  store i32 %%%s.argc_arg, i32* %%%s.argc\n", p_name, p_name);
                printf("  %%%s.argv = alloca i8**\n", p_name);
                printf("  store i8** %%%s_arg, i8*** %%%s.argv\n", p_name, p_name);
            } else {
                printf("  %%%s = alloca %s\n", p_name, p_llvm);
                printf("  store %s %%%s_arg, %s* %%%s\n", p_llvm, p_name, p_llvm, p_name);
            }
            pidx++;
        }
    }

    current_logical = 0;
    int num_logicals = count_logical(body);
    for (int i = 0; i < num_logicals; i++) {
        printf("  %%logical.tmp.%d = alloca i1\n", i);
    }

    int sidx = 0;
    struct node *stmt;
    while ((stmt = getchild(body, sidx++)) != NULL) {
        if (stmt->category == VarDecl) {
            struct node *v_type = getchild(stmt, 0);
            struct node *v_id   = getchild(stmt, 1);
            printf("  %%%s = alloca %s\n", v_id->token, get_llvm_type(v_type->category));
        }
    }

    sidx = 0;
    while ((stmt = getchild(body, sidx++)) != NULL) {
        if (stmt->category != VarDecl)
            codegen_statement(stmt, ret_type_node->category, string_array_param_name);
    }

    if (is_real_main) printf("  ret i32 0\n");
    else if (ret_type_node->category == Void) printf("  ret void\n");
    else if (ret_type_node->category == Double) printf("  ret double 0.0\n");
    else if (ret_type_node->category == Bool) printf("  ret i1 0\n");
    else printf("  ret i32 0\n");

    printf("}\n\n");
}

void codegen_program(struct node *program) {
    if (!program) return;

    collect_strlits(program);

    printf("declare i32 @printf(i8*, ...)\n");
    printf("declare i32 @atoi(i8*)\n\n");

    printf("@.str.int    = private unnamed_addr constant [3 x i8] c\"%%d\\00\"\n");
    printf("@.str.double = private unnamed_addr constant [6 x i8] c\"%%.16e\\00\"\n");
    printf("@.str.string = private unnamed_addr constant [3 x i8] c\"%%s\\00\"\n");
    printf("@.str.true   = private unnamed_addr constant [5 x i8] c\"true\\00\"\n");
    printf("@.str.false  = private unnamed_addr constant [6 x i8] c\"false\\00\"\n\n");

    emit_strlits();

    int idx = 1;
    struct node *member;
    int has_fields = 0;
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
    while ((member = getchild(program, idx++)) != NULL) {
        if (member->category == MethodDecl)
            codegen_function(member);
    }
}