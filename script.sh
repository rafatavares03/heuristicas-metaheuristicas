#!/bin/bash

metaheuristicasComAleatoriedade=("GRASP" "SA")
metaheuristicasSemAleatoriedade=("buscaTabu")
problemas=("mochila" "TSP")

echo "metaheuristica,problema,configuracao,solucao,funcao_objetivo,tempo" > resultados.csv

for problema in "${problemas[@]}"; do 
  for configuracao in $(seq 1 2); do
    for metaheuristica in "${metaheuristicasComAleatoriedade[@]}"; do
      g++ TP1/"$metaheuristica"/"$problema".cpp -o ./teste
      for i in $(seq 1 10); do 
        echo -n "$metaheuristica","$problema","$configuracao", >> resultados.csv
        ./teste "$configuracao" 1 < "$problema".txt >> resultados.csv
      done
      echo "$metaheuristica" para o problema "$problema" com configuração "$configuracao" foi executado!
    done
    for metaheuristica in "${metaheuristicasSemAleatoriedade[@]}"; do 
      g++ TP1/"$metaheuristica"/"$problema".cpp -o ./teste
      echo -n "$metaheuristica","$problema","$configuracao", >> resultados.csv
      ./teste "$configuracao" 1 < "$problema".txt >> resultados.csv
      echo "$metaheuristica" para o problema "$problema" com configuração "$configuracao" foi executado!
    done
  done
done
echo "Resultados gerados!"
