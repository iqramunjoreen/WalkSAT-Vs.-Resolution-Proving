import random
import time

class SAT:
    def __init__(self, clauses=[], variables=set()):
        self.clauses = clauses
        self.variables = variables
    def assign_validate(self, model):
        sclauses = []
        n_clauses = []
        for clause in self.clauses:
          if self.validateclause(clause,model)==True:
            sclauses.append(clause)
          else:
            n_clauses.append(clause)

        return sclauses, n_clauses

    def filereader(self, file_path):
        count=2;
        while True:
          try:
            file = open(file_path, "r")
          except:
            print("Error! invalid file name")
          else:
            break;
        _ = file.readline()
        lines = file.readline().split()
        numberofclauses=int(lines[-1])
        numberofvariables=int(lines[-2])
        if(numberofclauses<=0):
          print("Invalid miniSat format");
          exit();
        if(numberofvariables<=0):
          print("Invalid miniSat format");
          exit();

        for i in range(numberofclauses):
            count=count+1;
            splits = [int(token) for token in file.readline().split()]
            split_length=len(splits)

            if splits[split_length-1] != 0:
                print("Wrong clause format, line",count, "has an error");

            self.clauses.append(splits[:-1])
            for var in splits[:-1]:
                self.variables.add(abs(var))

    def walk_sat(self, p, max_flips):
        # 1. Generate a random model
        model = {var: random.choice([True, False]) for var in self.variables}
        sclauses, n_clauses = self.assign_validate(model)

        for i in range(max_flips):
            if not n_clauses:
                return model

            rand_clause = random.choice(n_clauses)
            if random.uniform(0, 1) < p:
                chosen_var = abs(random.choice(rand_clause))
            else:
                def sat_count(var):
                    model[var] = not var
                    count = len([clause for clause in self.clauses if self.validateclause(clause, model)])
                    model[var] = not var
                    return count

                max_sat = 0
                for var in rand_clause:
                    num_sat = sat_count(abs(var))
                    if num_sat > max_sat:
                        chosen_var = abs(var)
                        max_sat = num_sat

            model[chosen_var] = not model[chosen_var]
            sclauses, n_clauses = self.assign_validate(model)
        return None

    def validateclause(self, clause, model):
        for token in clause:
            if token > 0 and model[token] == True:
                return True
            if token < 0 and model[abs(token)] == False:
                return True
        return False


sat_initializer = SAT()
sat_initializer.filereader("2-queens.txt")
start_time = time.time()
answer = sat_initializer.walk_sat(0.75, 200000)
end_time = time.time()
print(f"Python Walk-SAT took {end_time-start_time} seconds to run")
