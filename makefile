CC = g++
BIN = bin
SRC = src
INF = ${SRC}/inference
MARG = ${INF}/MARG
MAP = ${INF}/MAP
LEARN = ${SRC}/weightlearning
GEN = ${SRC}/generators

ALLSRC = 	${MARG}/marg_inference_no_evidence.cpp\
			${MARG}/marg_inference_with_evidence.cpp\
			${MAP}/MLN_MAP.cpp\
			${LEARN}/weight_learning.cpp\
			${LEARN}/weight_learning_incomplete_data.cpp
	
all:
	$(CC) -g $(MAP)/MLN_MAP.cpp -o $(BIN)/MAP_solver
	$(CC) -g $(MARG)/marg_inference_no_evidence.cpp -o $(BIN)/no_evidence_solver
	$(CC) -g $(MARG)/marg_inference_with_evidence.cpp -o $(BIN)/evidence_solver
	$(CC) -g $(LEARN)/weight_learning.cpp -o $(BIN)/weight_learner
	$(CC) -g $(LEARN)/weight_learning_incomplete_data.cpp -o $(BIN)/weight_learner_incomplete_data

gen:
	$(CC) -g $(GEN)/test_generator.cpp -o $(BIN)/test_generator
	$(CC) -g $(GEN)/test_generator2.cpp -o $(BIN)/test_generator2
	
test:$(ALL)
	$(ALL)
	
clean:	
	rm $(BIN)/MAP_solver
	rm $(BIN)/no_evidence_solver
	rm $(BIN)/evidence_solver
	rm $(BIN)/weight_learner
	rm $(BIN)/weight_learner_incomplete_data
	
