#!/bin/bash

TEST_PATH="tests"
JUCOMPILER="./jucompiler"

GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m'

SUCCESS=0
TOTAL=0

# Garante que os ficheiros temporários gerados são apagados no fim
trap 'rm -f "$TEST_PATH"/*.actual "$TEST_PATH"/*.ll "$TEST_PATH"/*.s "$TEST_PATH"/*.exe' EXIT

for java_file in "$TEST_PATH"/*.java; do
    test_name=$(basename "$java_file" .java)
    expected_out="$TEST_PATH/$test_name.out"
    actual_out="$TEST_PATH/$test_name.actual"
    ll_file="$TEST_PATH/$test_name.ll"
    s_file="$TEST_PATH/$test_name.s"
    exe_file="$TEST_PATH/$test_name.exe"
    args_file="$TEST_PATH/$test_name.in"

    TOTAL=$((TOTAL+1))
    
    # Se não houver ficheiro .out esperado, salta este teste
    [ ! -f "$expected_out" ] && continue

    # 1. Compilar Juc para LLVM IR (Sem flags, para ativar a Meta 4)
    "$JUCOMPILER" < "$java_file" > "$ll_file" 2>/dev/null

    # Verifica se o ficheiro .ll foi gerado (se está vazio, houve erro semântico/sintático)
    if [ ! -s "$ll_file" ]; then
        echo -e "${RED}FAIL: $test_name (Falha ao gerar LLVM IR)${NC}"
        continue
    fi

    # 2. Compilar LLVM IR para Assembly nativo
    llc "$ll_file" -o "$s_file" 2>/dev/null
    
    # 3. Ligar o Assembly e criar o executável
    cc "$s_file" -no-pie -o "$exe_file" 2>/dev/null

    # Verifica se o executável foi criado com sucesso
    if [ ! -f "$exe_file" ]; then
        echo -e "${RED}FAIL: $test_name (Falha no llc ou cc)${NC}"
        continue
    fi

    # Lidar com argumentos de linha de comandos (se o teste precisar)
    args=""
    if [ -f "$args_file" ]; then
        args=$(cat "$args_file")
    fi

    # 4. Executar o binário gerado e guardar o output real
    ./"$exe_file" $args > "$actual_out" 2>&1

    # 5. Comparar o output da execução com o output esperado
    if diff -q "$actual_out" "$expected_out" > /dev/null; then
        echo -e "${GREEN}PASS: $test_name${NC}"
        SUCCESS=$((SUCCESS+1))
    else
        echo -e "${RED}FAIL: $test_name${NC}"
        # Podes descomentar a linha abaixo se quiseres ver logo qual foi a diferença
        # diff -y "$actual_out" "$expected_out"
    fi
done

echo "Results: ${SUCCESS}/${TOTAL}"