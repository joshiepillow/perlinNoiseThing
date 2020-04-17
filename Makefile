.DEFAULT_GOAL := test	

test :
	g++ main.cpp -o test -lsfml-graphics -lsfml-window -lsfml-system