#/bin/sh
#los parametros se guardan como parametros_<cant_rescatistas>_<cant_personas_por_pos>

for cant_rescatistas in {1, 10, 100}
	do
	for cant_personas_por_pos in {10, 100}
		do
			#mas de 244 paises rompe el tester
			for cant_clientes in {10, 20, 30, 50, 100, 244}
			do
				printf "Testeando $cant_clientes clientes con $cant_rescatistas rescatistas en un aula ($dim_alto * $dim_ancho) con $cant_personas_por_pos personas por posicion..."
				cp parametros_"$cant_rescatistas"_"$cant_personas_por_pos".h ../codigo/parametros.h
				pushd ../codigo/
				make clean all 
	#hay que buscar forma de matar al server para correr mas de un test
				valgrind ./servidor_multi > result_"$cant_rescatistas"_"$cant_personas_por_pos"_"$cant_clientes".outserver
				popd
				python server_tester.py "$cant_clientes" > result_"$cant_rescatistas"_"$cant_personas_por_pos"_"$cant_clientes".outtester
				echo "Ok"

				
				printf "Corriendo ../tests/test$dim.$powerup.testcase ; stdout > ../testsout/test$dim.$powerup.testout ; stderr > ../timingout/test$dim.$powerup.timingout..."
				./app.runnable --take-time 100 < ../tests/test$dim.$powerup.testcase > ../testsout/test$dim.$powerup.testout	2> ../timingout/test$dim.$powerup.timingout
				echo "Ok"				
			done
		done	
	done