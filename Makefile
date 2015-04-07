SRC  = main.cpp hex.cpp montecarlo.cpp
HEAD = hex.h
OTHER = Makefile README
EXEC = hex

${EXEC}: ${SRC} ${HEAD} 
	g++ -std=c++11 -o $@ ${SRC}
    
clean:
	rm ${EXEC}

zip:
	zip h6_cmps109 ${SRC} ${HEAD} ${OTHER}
