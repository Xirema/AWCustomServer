includeDir := include
compileCommand := g++ -I$(includeDir) -Wall -pedantic-errors -Werror -Wno-unused-function -Wno-reorder -O3 -std=c++20
files := Archive.o DataLookup.o DataUpload.o Main.o Properties.o RestServer.o SQLUtil.o StateLookup.o
server: $(files)
	g++ -o server $(files) -lboost_system -lboost_json -lssl -lcrypto -ldl
Archive.o: src/Archive.cpp
	$(compileCommand) -c src/Archive.cpp -o Archive.o
DataLookup.o: src/DataLookup.cpp
	$(compileCommand) -c src/DataLookup.cpp -o DataLookup.o
DataUpload.o: src/DataUpload.cpp
	$(compileCommand) -c src/DataUpload.cpp -o DataUpload.o
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
clean:
	rm -f *.o
	rm -f server
