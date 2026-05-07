# Relatório - jucompiler

    João Tomás Correia Ferreira (2023217920)
    Samuel Marques Riça (2023206471)

---


Este relatório explica conforme pedido o processo de desenvolvimento de
alguns elementos críticos para a criação do compilador *jucompiler*,
nomeadamente:


### i) Gramática

Como mencionado no enunciado, a gramática original era ambígua e escrita em
notação EBNF, portanto para adicionar este componente ao compilador tivemos
de reescrevê-la no YACC.

O primeiro passo foi expandir construções da notação EBNF como `[...]` (opcional)
e `{...}` (zero ou mais repetições) para regras recursivas no YACC. Por exemplo, o
corpo de uma classe (`{MethodDecl | FieldDecl | SEMICOLON }`) foi transformado numa
regra auxiliar `MemberList`. Esta abordagem foi aplicada nas situações semelhantes
da gramática original tais, como podemos ver nas regras `StatementOrVarDecl`,
`StatementList`, `ParamList`, etc. 

É importante salientar também que estas novas regras foram criadas usando
recursão à esquerda, de forma a manter a coerência com a análise LALR(1).


Como mencionado no enunciado, a gramática original era ambígua, visto que não
especificava precedências e associatividade entre os operadores. Para resolver
este problema, recorremos às declarações `%left`, `%right` e `%nonassoc` do YACC.

A ordem de precedência estabelecida, foi a seguinte, com nível de precedência
crescente:
- OR, XOR e AND
- EQ, NE, LT, LE, GT e GE (comparações)
- LSHIFT e RSHIFT (shifts)
- PLUS e MINUS
- STAR, DIV, MOD
- UNARY (operadores unários)

No nível mais alto de precedência - `%nonassoc UNARY` - estão incluídos os
operadores unários (PLUS, MINUS, NOT), sendo utilizada a expressão `%prec
UNIARY` nas respetivas produções.


Nas produções IF-THAN-ELSE foi necessário também resolver o questão da
associatividade dos tokens ELSE aos respetivos IF. Para tal, introduzimos
a regra de precedência `LOWER_THAN_ELSE` com a keyword `%nonassoc`. Assim,
as produções sem ELSE recebem este nível de precedência, garantindo que o
ELSE têm sempre um nível superior e é associado ao IF mais interno.


Na gramática original, `Assignment` aparecia tanto como `Statement` como
dentro de `Expr`, o que fazia com que uma sequência como `IDENTIFIER
ASSIGN Expr` pudesse ser traduzida tanto como `Assignment` como `Expr`.
Para resolver esta ambiguidade, introduzimos o símbolo `ExprOrAssign`, que
unifica os dos casos e é usado em todos os contextos onde ambas as situações
são sintáticamente válidas, nomeadamente: condições IF e WHILE, operações
RETURN e PRINT e argumentos do lado direito das próprias atribuições.


Por fim, incluímos também produções de erro, tanto as necessárias na gramática
original como algumas adicionais após as alterações à mesma, por exemplo `IF
LPAR error RPAR Statement` e `WHILE LPAR error RPAR Statement`, após a adição
do símbolo `ExprOrAssign` descrito anteriormente.


### ii) AST e tabela de símbolos

- **Árvore de Sintaxe Abstrata (AST)**

    Para a geração das ASTs, recorremos às estruturas e funções fornecidas
    originalmente, efetuando algumas modificações.

    As árvores são formadas por nós com a seguinte estrutura:
    ```c
        struct node {
          enum category category;           // Define o tipo do nó (Add, If, MethodDecl, etc)
          char *token;                      // Valor do token (ou NULL)
          int token_line, token_column;     // Posição do token atual
          enum type type;                   // Tipo de dados armazenado pelo nó
          char *param_sig;                  // Assinatura dos parâmetros em chamadas de métodos
          struct node_list *children;       // Lista ligada de nós descendentes do nó atual
        }
    ```

    Comparativamente com a estrutura original dos nós, adicionámos os campos
    `token_line` e `token_column`, utilizados para identificar erros durante
    a análise semântica e imprimir as respetivas mensagens de erro corretamente,
    e o campo `param_sig`, que armazena as assinaturas dos parâmetros dos métodos,
    o que permite adicionar essas informações adicionais à AST anotada após a
    análise semântica.

    Em relação à gestão da AST, recorremos aos métodos fornecidos, por exemplo
    `newnode` e `addchild`, para adicionar os nós à AST à medida que as regras vão
    sendo percorridas. Para além disso, funções como `newlist`, `append` e
    `addchildren` gerem as listas de declarações, parâmetros, statements, etc,
    interagindo com as listas ligadas `node_list`. 

    Estas funções permitem implementar a análise sintática de forma ascendente, após
    a leitura do ficheiro de entrada pelo Lexer.


- **Tabela de símbolos**

    As tabelas de símbolos geradas durante a análise semântica dividem-se em dois
    tipos - tabela global e tabelas locais (por método).

    A tabela global é representada pela estrutura `class_table`:
    ```c
        typedef struct {
          char *name;               // Nome da classe
          symbol_entry *symbols;    // Lista de símbolos
          method_table *methods;    // Lista de tabelas de métodos
        } class_table;
    ```

    enquanto as tabelas locais têm a seguinte estrutura:
    ```c
        typedef struct {
          char *name;                   // Nome do método
          char *signature;              // Assinatura do método (e.g. factorial(int))
          symbol_entry *symbols;        // Lista de símbolos
          struct method_table *next;    // Ponteiro para a próxima tabela de métodos
        } method_table;
    ```

    Em ambas as tabelas são utilizadas listas ligadas `symbol_entry` para
    armazenar os símbolos no scope de cada uma das tabelas, sendo estes
    símbolos relativos a atributos e métodos, no caso de classes; e parâmetros,
    variáveis locais e o return value relativos aos métodos.

    Para distinguir estes diferentes tipos de símbolos, são utilizadas flags
    como `is_method` e `is_param` e identificadores como `name` e `type_str`:
    ```c
        typedef struct {
          char *name;                   // Nome do símbol (variável/atributo/método)
          char *type_str;               // Tipo do símbolo em string (e.g. int, double, etc)
          int is_method;                // Flag que indica se o símbolo é um método
          int is_param;                 // Flag que indica se o símbolo é um parâmetro
          param_entry *params;          // Lista de tipos dos parâmetros (ou NULL)
          int line, col;                // Posição da declaração do símbolo
          struct symbol_entry *next;    // Ponteiro para o próximo símbolo
        } symbol_entry;
    ```

    A construção das tabelas é feita em três passagens pela AST.
    1) `build_global_table`
        A tabela da classe é preenchida com os campos e assinaturas dos
        métodos, analisando assim o scope global.
    2) `populate_method_tables`
        As tabelas de métodos são preenchidas com os símbolos relativos às
        variáveis locais e ao return value.
    3) `check_methods`
        É percorrido o corpo de cada método, verificando os tipos das variáveis
        e garantindo que as declarações das mesmas acontecem antes das suas
        utilizações.

### iii) Geração de código


