import random
import time
#import copy

filename = "2-queens.txt"

def clause_to_string(clause):
    clause.sort()
    return " ".join([str(literal) for literal in clause])



class ResolutionProver:
    def __init__(self, clauses=[]):
        self.clauses = clauses

    def readfile(self, file_path):
        count=2;  
        looper=True;
        while looper:
          try:
            file = open(file_path, "r")
            
          except:
            print("Error! File not found");
            exit();
          else:
            break;
        _ = file.readline()
        lines = file.readline().split()
        num_var  = int(lines[-2])
        num_clauses= int(lines[-1])  
        if(num_var<=0):
          print("Please enter a miniSat formatted file")
          exit();
        if(num_clauses<=0):
          print("Please enter a miniSat formatted file")
          exit();

        for i in range(num_clauses):
            count=count+1;
            splits = [int(token) for token in file.readline().split()]
            length_split=len(splits)
            #print("length is",length_split)

            if splits[length_split-1] != 0:
              print("Not a valid format of clause, line",count,"has an error")
              
              exit();
              
            self.clauses.append(clause_to_string(splits[:-1]))

    
    def pl_resolution(self):
        new = set()
        while True:
            n = len(self.clauses)
            pairs = [(self.clauses[i], self.clauses[j])
                for i in range(n) for j in range(i + 1, n)]
            for (first, second) in pairs:
                resolvents = self.pl_resolve(first, second)
                if "" in resolvents:
                    return True
                new = new.union(set(resolvents))
            if new.issubset(set(self.clauses)):
                return False
            for c in new:
                if c not in self.clauses:
                    self.clauses.append(c)


    def pl_resolve(self, first, second):

        clauses = []
        first = set(first.split())
        second = set(second.split())
        for i in first:
            for j in second:
                if -1*int(i) == int(j):
                    #print("");
                    remover_first = first.copy()
                    remover_first.remove(i)
                    remover_second = second.copy()
                    remover_second.remove(j)
                    clauses.append(clause_to_string(list(remover_first.union(remover_second))))
        return clauses

initialize_prover = ResolutionProver()
initialize_prover.readfile(filename)
start_time = time.time()
answer = initialize_prover.pl_resolution()
end_time = time.time()
print(f"Elapsed time {end_time-start_time},MiniSat problem is {not answer}");                         
          
