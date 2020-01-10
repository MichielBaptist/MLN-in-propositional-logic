# About
This project contains the source code of work done for a bachelor's thesis about Markov Logic
Networks in propositional logic. This bachelor's thesis was conducted by Jorik Jooken and 
Michiel Baptist in the 3rd year bachelor computer science at the KU Leuven in Belgium. We would like to 
thank our promoters for this bachelors thesis: Prof. Dr. Luc De Raedt and Anton Dries.

In this bachelor's we conducted research in the field of Markov Logic Networks. More specifically
Markov Logic Networks in propositional logic. We explored MARG inference using Weighted Model Counting(WMC),
MAP inferenc using Weighted MaxSat and parameter learning using gradient descent. More about our research can be 
found in the included paper (in Dutch).

# Structure of an MLN file
This readme contains detailed instructions on how to use MARG exact inference, MAP inference 
and parameter learning of Markov Logic Networks in propositional logic.

MARG, MAP use an mln file as input and weight learning produces one as output. The general
format of an mln file is the following:

	-------------------------------------------------
	|	<number of literals> <number of clauses> |
	|	weight1 <literals> 0			 |
	|	weight2 <literals> 0			 |
	|	...					 |
	-------------------------------------------------
    
    * The file containing the mln must be named "mln".
	
	* The weight of the clauses may be any real number, and may be negative.

	* The list of literals (<literals>) is a disjuntion of the literals.
	  	-> e.g. a v b v c ...

	* A literal is assumed to be an integer between 1 and the number of literals. 
	  A literal may be negative.
	  
  	* A clause must always end in a 0.

examples of MLN's can be found in the mln_samples folder.

# 1. MARG inference

Important: Our implementation of finding the MARG uses a Weighted Model Counter.
           This solver calls the binary "miniC2D". This should not be removed
           or the MAP solver will not work properly.

MARG will calulate the probability of a literal of collection of literals, given a MLN file.
Marg can be calculated as follows:
	1) place an mln file in the bin/ folder. The mln file format is defined above and must be named "mln".
	2) place a query file in the bin/ folder. This file must be named "query_atoms".
		-> The query file format is as follows: <amount of query literals> <list of literals>
		   The list of query literals is a conjunction of these literals.
		   e.g. a ^ b ^ c ...
	3) optional: place an evidence file in the bin/ folder. This file must be named "evidence".
		-> The evidence file format is as follows: <amount of evidence literals> <list of literals>
		   The list of literals is a conjunction.
	4) run the solver by calling the "evidence_solver" or "no_evidence_solver".
	5) a file named "marg_output" will contain the desired probability.
  
# 2. MAP inference

Important: Our implementation of finding the MAP uses a Weighted MaxSat solver.
           This solver calls the binaries "CCLS_to_maxsat", "CCLS2015" and "akmaxsat". Thes should not be removed
           or the MAP solver will not work properly.

MAP will calulate the most probable world, this means it will return a list of all unique literals
of the mln and whether they are false (indicated by the negation of the corresponding integer) or 
true (indicated by the corresponding integer). 

Example of the output of MAP:
    -> 1 2 3 -4 5 -6
    This is the most probable world according to the input mln. In this world 1,2,3 and 5 are true and
    4 and 6 are false. 
    
MAP can be calculated as follows:
    1) place an mln in the bin/ folder. The mln file format is defined above and must be named "mln".
    2) run the solver by calling the "MLN_MAP".
    3) a file named "MAP" will contain the most probable world of given mln.

# 3. Parameter learning

Parameter learning will learn the weights of an mln given unweighted clauses and a collection of 
possible worlds. There are two inputs for the solvers:

    input 1) A collection of unweighted clauses.
             This file must be named "clauses" and has the following format:
             
                 <number of unweighted clauses>
                 <unweighted clause 1>
                 <unweighted clause 2>
                 ...
                 <unweighted clause n>
                 
             The format of an unweighted clause is as follows:
                
                 <number of literals> <list of literals>
                 
            The <list of literals> are space seperated and are a disjunction of literals.
            
    input 2) A collection of observed worlds.
             This file must be named "observed worlds" and has the following format:
             
                <number of worlds> <Number of literals per world: m>
                <world 1>
                <world 2>
                ...
                <world n>
                
            The format of a world is as follows:
            
                <list of m integers>
                
            * The <list of m integers> must be m integers big, any input after the m'th integer will 
              not be read/considered.
            
            * The values of <list of m integers> is assumed to be the truth value of the literal corresponding
              to the position of the integer. e.g. the integer at position 5 is the truth value of the 5th literal.
              
            * The value of this integer may be any integer.
                -> A value of 1 indicates that the corresponding literal is true.
                -> A value of 0 indicates that the corresponding literal is false.
                -> A value other than 0 or 1 indicates that the truth value of the corresponding literal is unknown.
    
An example of these inputs can be found in the samples.
            
Learning can be calculated as follows:
	1) place a collection of clauses bin/ folder. This file format is defined above and must be named "clauses".
	2) place a collection of observed worlds bin/ folder. This file format is defined above and must be named "observed_worlds".
	3) run the solver by calling the "weight_learning" or "weight_learning_incomplete_data".
	4) a file named "mln" will contain the calculated mln, i.e. an mln containing the given clauses with calculated weights.
             
