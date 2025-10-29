#!/bin/bash

# Define o número de repetições para cada polígono para calcular a média
N_REPETITIONS=10
TIMEOUT_LIMIT="600s"

# Array de arquivos de polígonos a serem testados
TEST_FILES=(
    "data/convex_n1000.poly"
    "data/convex_n10000.poly"
    "data/convex_n100000.poly"
    "data/convex_n1000000.poly"
    "data/star_n1000.poly"
    "data/star_n10000.poly"
    "data/star_n100000.poly"
    "data/star_n1000000.poly"
    "data/spiral_n1000.poly"
    "data/spiral_n10000.poly"
    "data/spiral_n100000.poly"
    "data/spiral_n1000000.poly"
)

for FILE in "${TEST_FILES[@]}"; do
    echo "Iniciando $N_REPETITIONS repetições para o arquivo: $FILE"
    
    # Loop de repetições
    for i in $(seq 1 $N_REPETITIONS); do
        echo "  Repetição $i/$N_REPETITIONS..."
        
        timeout $TIMEOUT_LIMIT ./build/parallel_clipper serial $FILE /dev/null
        
        EXIT_CODE=$?
        if [ $EXIT_CODE -eq 124 ]; then
            echo "ERRO CRÍTICO: Execução excedeu o tempo limite ($TIMEOUT_LIMIT). Abortando teste para $FILE."
            # Se o log de tempo não foi escrito, o CSV terá um buraco, o que é um indicador de falha.
            break 
    done
    
    echo "--------------------------------------------------------"
done

echo "Coleta de dados concluída. Verifique os tempos em baseline_results.csv."