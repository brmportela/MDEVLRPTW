#!/bin/bash
#Numero de execucoes
e=10
#Numero de iteracoes
i=1000 
#Heuristica
h=4
k=0
x=0
case $1 in
	"a")	nomes=(
		"c101C5" "c103C5" "c206C5" "c208C5" "r104C5" "r105C5" "r202C5" "r203C5" "rc105C5" "rc108C5" "rc204C5" "rc208C5" 
		"c101C10" "c104C10" "c202C10" "c205C10" "r102C10" "r103C10" "r201C10" "r203C10" "rc102C10" "rc108C10" "rc201C10" "rc205C10" 
		"c103C15" "c106C15" "c202C15" "c208C15" "r102C15" "r105C15" "r202C15" "r209C15" "rc103C15" "rc108C15" "rc202C15" "rc204C15"
		"c101C21" "c109C21" "c202C21" "c208C21"
		"r101C21" "r109C21" "r206C21" "r211C21"
		"rc101C21" "rc106C21" "rc203C21" "rc208C21"
		) ;;
	"b")	nomes=(
		"pr03" "pr06" "pr08" "pr10"
		"pr03s" "pr06s" "pr08s" "pr10s"
		) ;;
	"c")	nomes=(	
		"pr13" "pr16" "pr18" "pr20"
		"pr13s" "pr16s" "pr18s" "pr20s"
		) ;;
	"c2")	nomes=(	
		"pr18s"
		) ;;
	"c3")	nomes=(	
		"pr20s"
		) ;;
	"d")	nomes=(	
		"pr11a" "pr11b" "pr11as" "pr11bs" 
		) ;;
	"e")	nomes=(	
		"pr13a" "pr13as" "pr13b" "pr13bs"
		) ;;
	"f")	nomes=(	
		"pr16a" "pr16b" 
		) ;;
	"g")	nomes=(	
		"pr16as" "pr16bs"
		) ;;
	"h")	nomes=(	
		"pr17a" "pr17b" "pr17as" "pr17bs" 
		) ;;
	"i")	nomes=(	
		"pr20a" "pr20b" 
		) ;;
	"j")	nomes=(	
		"pr20as" "pr20bs"
		) ;;
	"k")	nomes=(	
		"pr21a" "pr21b" "pr21as" "pr21bs"
		) ;;
	"k2")	nomes=(	
		"pr21as"
		) ;;
	"k3")	nomes=(	
		"pr21bs"
		) ;;
	"l")	nomes=(	
		"pr24a" "pr24b" 
		) ;;
	"m")	nomes=(	
		"pr24as" "pr24bs"
		) ;;
	*) echo "Entrada incorreta!" ;;
esac
while [ $x -lt ${#nomes[@]} ]; do
	echo "##############################################################################"
	echo "--------------------------------- "${nomes[$x]}" ---------------------------------"
	echo "##############################################################################"
	echo
	
	if [ -e results/${nomes[$x]}/ ]; then
		rm -r results/${nomes[$x]}/
	fi
	mkdir results/${nomes[$x]}/
	if [ -e results/${nomes[$x]}.txt ]; then
		rm results/${nomes[$x]}.txt
	fi
	if [ -e results/R_${nomes[$x]}.txt ]; then
		rm results/R_${nomes[$x]}.txt
	fi
	touch results/R_${nomes[$x]}.txt
#	if [ -e results/k_${nomes[$x]}.txt ]; then
#		rm results/k_${nomes[$x]}.txt
#	fi
#	touch results/k_${nomes[$x]}.txt
	if [ -e results/N_${nomes[$x]}.txt ]; then
		rm results/N_${nomes[$x]}.txt
	fi
	touch results/N_${nomes[$x]}.txt
	k=0
	while [ $k -lt $e ]; do
	  echo "---------------------------------------------------------------------------"
	  echo "Instancia - "${nomes[$x]}" / Iteracao "$k
	  ./projeto_principal run instance/${nomes[$x]}.txt $k $i $h
	  k=`expr $k + 1`
	done
	#gnuplot results/${nomes[$x]}/plot.plt
	x=`expr $x + 1`
done
chmod -R 777 results
