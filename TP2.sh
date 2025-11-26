#!/bin/bash

TSP=("AG" "CF")
MOCHILA=("AG")

echo "metaheuristica,problema,configuracao,solucao,funcao_objetivo,segundos" > resultados.csv

for metaheuristica in "${TSP[@]}"; do
  g++ TP2/"$metaheuristica"/TSP.cpp -o ./teste
  for configuracao in $(seq 1 2); do
    for i in $(seq 1 10); do
      echo -n "$metaheuristica",TSP,"$configuracao", >> resultados.csv
      ./teste "$configuracao" 1 < TSP.txt >> resultados.csv
    done
    echo "$metaheuristica" para o TSP com configuração "$configuracao" foi executado!
  done 
done

for metaheuristica in "${MOCHILA[@]}"; do
  g++ TP2/"$metaheuristica"/mochila.cpp -o ./teste
  for configuracao in $(seq 1 2); do
    for i in $(seq 1 10); do
      echo -n "$metaheuristica",mochila,"$configuracao", >> resultados.csv
      ./teste "$configuracao" 1 < mochila.txt >> resultados.csv
    done
    echo "$metaheuristica" para a mochila com configuração "$configuracao" foi executado!
  done 
done

echo Resultados salvos!