
all: main

Node.o: Node.cpp Node.h
	g++ -c Node.cpp

NodeNumber.o: NodeNumber.cpp NodeNumber.h
	g++ -c NodeNumber.cpp

NodeVariable.o: NodeVariable.cpp NodeVariable.h
	g++ -c NodeVariable.cpp

NodeOperation.o: NodeOperation.cpp NodeOperation.h
	g++ -c NodeOperation.cpp

AST.o: AST.cpp AST.h
	g++ -c AST.cpp

main: Node.o NodeNumber.o NodeVariable.o NodeOperation.o AST.o main.cpp
	g++ -o main Node.o NodeNumber.o NodeVariable.o NodeOperation.o AST.o main.cpp

testAST: Node.o NodeNumber.o NodeVariable.o NodeOperation.o AST.o testAST.cpp
	g++ -o testAST Node.o NodeNumber.o NodeVariable.o NodeOperation.o AST.o testAST.cpp

testNodeNumber: Node.o NodeNumber.o NodeVariable.o NodeOperation.o AST.o testNodeNumber.cpp
	g++ -o testNodeNumber Node.o NodeNumber.o NodeVariable.o NodeOperation.o AST.o testNodeNumber.cpp

testNodeVariable: Node.o NodeNumber.o NodeVariable.o NodeOperation.o AST.o testNodeVariable.cpp
	g++ -o testNodeVariable Node.o NodeNumber.o NodeVariable.o NodeOperation.o AST.o testNodeVariable.cpp

testNodeOperation: Node.o NodeNumber.o NodeVariable.o NodeOperation.o AST.o testNodeOperation.cpp
	g++ -o testNodeOperation Node.o NodeNumber.o NodeVariable.o NodeOperation.o AST.o testNodeOperation.cpp

tests: testNodeNumber testNodeVariable testNodeOperation testAST

clean:
	rm -rf *.o main testAST testNodeNumber testNodeVariable testNodeOperation

run_test_number:
	./testNodeNumber

run_test_variable:
	./testNodeVariable

run_test_operation:
	./testNodeOperation

run_test_ast:
	./testAST

run_all:
	./main