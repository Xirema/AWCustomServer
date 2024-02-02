includeDir := include
compileCommand := g++ -I$(includeDir) -Wall -pedantic-errors -Werror -O3 -std=c++20
files := Archive.o Data.o Main.o Properties.o RestServer.o SQLUtil.o StateLookup.o ResourcePacks.o RestFunctions.o
server: $(files)
	g++ -o server $(files) -lboost_system -lboost_json -lssl -lcrypto -ldl
Archive.o: src/Archive.cpp
	$(compileCommand) -c src/Archive.cpp -o Archive.o
Data.o: src/db/Data.cpp
	$(compileCommand) -c src/db/Data.cpp -o Data.o
Main.o: src/Main.cpp
	$(compileCommand) -c src/Main.cpp -o Main.o
Properties.o: src/Properties.cpp
	$(compileCommand) -c src/Properties.cpp -o Properties.o
RestServer.o: src/RestServer.cpp
	$(compileCommand) -c src/RestServer.cpp -o RestServer.o
SQLUtil.o: src/SQLUtil.cpp
	$(compileCommand) -c src/SQLUtil.cpp -o SQLUtil.o
StateLookup.o: src/StateLookup.cpp
	$(compileCommand) -c src/StateLookup.cpp -o StateLookup.o
ResourcePacks.o: src/ResourcePacks.cpp
	$(compileCommand) -c src/ResourcePacks.cpp -o ResourcePacks.o
RestFunctions.o:src/RestFunctions.cpp
	$(compileCommand) -c src/RestFunctions.cpp -o RestFunctions.o
clean:
	rm -f *.o
	rm -f server
