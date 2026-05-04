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



### iii) Geração de código


