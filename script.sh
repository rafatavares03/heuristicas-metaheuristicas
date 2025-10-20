#!/bin/bash

metaheuristicasComAleatoriedade=("GRASP" "SA")
metaheuristicasSemAleatoriedade=("buscaTabu")
problemas=("mochila" "TSP")

echo "metaheuristica,problema,solucao,funcao_objetivo,tempo" > resultados.csv

for problema in "${problemas[@]}"; do 
  for metaheuristica in "${metaheuristicasComAleatoriedade[@]}"; do
    g++ TP1/"$metaheuristica"/"$problema".cpp -o ./teste
    for i in $(seq 1 10); do 
      echo -n "$metaheuristica","$problema", >> resultados.csv
      ./teste < "$problema".txt >> resultados.csv
    done
    echo "$metaheuristica" para o problema "$problema" foi executado!
  done
  for metaheuristica in "${metaheuristicasSemAleatoriedade[@]}"; do 
    g++ TP1/"$metaheuristica"/"$problema".cpp -o ./teste
    echo -n "$metaheuristica","$problema", >> resultados.csv
    ./teste < "$problema".txt >> resultados.csv
    echo "$metaheuristica" para o problema "$problema" foi executado!
  done
done
echo "Resultados gerados!"
