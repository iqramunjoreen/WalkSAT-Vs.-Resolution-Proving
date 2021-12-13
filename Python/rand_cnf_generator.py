import random

literals = 3
numberofclauses = 9000
symbols = 4500
outputfile = "CSP2.txt"


def rand_cnf_generator(p_literals, p_n_clauses, range_symbol):
    clauses = []
    finalclos = set()
    symbols = [i for i in range(1, range_symbol+1)]

    while p_n_clauses > 0:
        random.shuffle(symbols)
        clause = [symbol if random.random() < 0.5 else -1*symbol for symbol in symbols[:p_literals]]
        clause.sort()
        clause_str=" ".join([str(literal) for literal in clause])
        
        if clause_str in finalclos:
          continue;

        if clause_str not in finalclos:
            clauses.append(clause)
            finalclos.add(clause_str)
            p_n_clauses-= 1

    return clauses, symbols


def writer(clauses, symbols, file_path):
    len_symbol = len(symbols)
    len_clauses = len(clauses)
    file = open(file_path, 'w+')
    file.write("c " + "minisat_input_format\n")

    file.write("p " + "cnf " + str(len_symbol) + " " + str(len_clauses) + "\n")
    for clause in clauses:
        clause = [str(literal) for literal in clause]
        file.write(" ".join(clause) + " 0\n")
    file.close()

if __name__ == "__main__":
    clauses, symbols = rand_cnf_generator(literals, numberofclauses , symbols)
    writer(clauses, symbols, outputfile)