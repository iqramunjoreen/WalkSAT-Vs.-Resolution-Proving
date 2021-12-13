#include <vector>
#include <string>
#include <set>
#include <map>

#ifndef __SAT_H__
#define __SAT_H__

class SAT
{
  public:
    
    SAT();
    ~SAT();
    
    std::map<int, bool> walk_sat(float p, int flips);
    void load_from_file(char* file_path);
    bool verify_model(const std::map<int, bool>& model);
    void display_model(const std::map<int, bool>& model);
    
  private:
    
    std::map<std::string, std::vector<int> > clauses;
    std::set<std::string> unsat_clauses;
    int num_clauses;
    int num_variables;

    
    int count_unsat_change(std::map<int, bool>& model, int var);
    bool symbol_exists(const std::vector<int>& clause, int symbol);
    void check_assignment(const std::map<int, bool>& model, int changed_symbol);
    bool check_clause(const std::vector<int>& clause, const std::map<int, bool>& model);
    
    
    //Utilities
    std::string clause_to_string(const std::vector<int>& clause);
    std::vector<std::string> split(const std::string& sentence, char delimiter);
};

#endif
