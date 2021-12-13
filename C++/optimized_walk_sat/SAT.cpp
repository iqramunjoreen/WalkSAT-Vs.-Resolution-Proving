#include <iostream>
#include <fstream>
#include <assert.h>
#include <stdlib.h>
#include <ctime>
#include <cstdlib>
#include <random>
#include "SAT.hpp"

const char DELIMITER = ' ';
const std::string END_OF_LINE = "0";

SAT::SAT() {}
SAT::~SAT() {}

///=====================================================================================================

std::map<int, bool> SAT::walk_sat(float p, int max_flips)
{
    
    std::map<int, bool> model;

    srand(time_t(0));
    for (int i = 1; i < this->num_variables+1; i++)
    {
        if (rand() % 2 == 0)
        {
            model[i] = true;
        }
        else
        {
            model[i] = false;
        }
    }
    check_assignment(model, 0);

    int num_flips = 0;
    while (num_flips < max_flips)
    {
        if (unsat_clauses.empty())
            return model;

        auto it = unsat_clauses.begin();
        std::advance(it, rand()%unsat_clauses.size());
        std::vector<int> choosen_clause = clauses[*it];

        std::default_random_engine generator;
        std::uniform_real_distribution<double> distribution(0.0,1.0);

        int chosen_var = 0;
        if (distribution(generator) < p)
        {
            chosen_var = abs(choosen_clause[rand()%choosen_clause.size()]);
        }
        else
        {
            int least_unsat_count = num_clauses;
            for (int i = 0; i < choosen_clause.size(); i++)
            {
                int var = abs(choosen_clause[i]);
                int unsat_change = count_unsat_change(model, var);
                
                if (unsat_change < least_unsat_count)
                {
                    least_unsat_count = unsat_change;
                    chosen_var = var;
                }
            }
        }
        
        model[chosen_var] = !model[chosen_var];
        check_assignment(model, chosen_var);

        num_flips = num_flips + 1;
    }

    model.clear();  //If no solution found, return {0: false}
    model[0] = false;
    return model;
}

///=====================================================================================================

bool SAT::symbol_exists(const std::vector<int>& clause, int symbol){
    for (int i = 0; i < clause.size(); i ++)
    {
        if (symbol == abs(clause[i]))
        {
            return true;
        }
    }
    return false;
}

///=====================================================================================================

bool SAT::verify_model(const std::map<int, bool>& model)
{
    for (auto it = clauses.begin(); it != clauses.end(); ++it)
    {
        if (!check_clause(it->second, model))
        {
            return false;
        }
    }
    return true;
}

///=====================================================================================================

void SAT::check_assignment(const std::map<int, bool>& model, int changed_symbol)
{
    // Only on initalization
    if (changed_symbol == 0)
    {
        for (auto it = clauses.begin(); it != clauses.end(); ++it)
        {
            if (!check_clause(it->second, model))
                unsat_clauses.insert(it->first);
        }
    }
    else
    {
        for (auto it = clauses.begin(); it != clauses.end(); ++it)
        {
            if (symbol_exists(it->second, changed_symbol))
            {
                if(check_clause(it->second, model))
                {
                    unsat_clauses.erase(it->first);
                }
                else
                {
                    unsat_clauses.insert(it->first);
                }
            }
        }
    }
}

///=====================================================================================================

int SAT::count_unsat_change(std::map<int, bool>& model, int var)
{
    model[var] = !model[var];
    int change = 0;
    for (auto it = clauses.begin(); it != clauses.end(); ++it)
    {
        if (symbol_exists(it->second, var))
        {
            bool result = check_clause(it->second, model);
            if (result && (unsat_clauses.find(it->first) != unsat_clauses.end()))
            {
                change--;
            }
            else if (!result && (unsat_clauses.find(it->first) == unsat_clauses.end()))
            {
                change++;
            }
        }
    }
    model[var] = !model[var];
    return change;
}

///=====================================================================================================


bool SAT::check_clause(const std::vector<int>& clause, const std::map<int, bool>& model)
{
    for (std::vector<int>::const_iterator it = clause.begin(); it != clause.end(); ++it)
    {
        if ( ( model.at(abs(*it)) && (*it > 0) ) || ( (*it < 0) && !model.at(abs(*it))) )
        {
            return true;
        }
    }
    return false;
}

///=====================================================================================================
//Utility Functions

void SAT::load_from_file(char* file_path) {
  std::string line;
  std::ifstream input_file (file_path);
  if (input_file.is_open()) {
    getline(input_file, line); //ignore comment
    getline(input_file, line); //read the config
    std::vector<std::string> tokens = split(line, DELIMITER);

    this->num_variables = std::stoi (tokens[2]);
    this->num_clauses = std::stoi (tokens[3]);

    for (int i = 0; i < this->num_clauses; i++) {
      getline(input_file, line);
      tokens = split(line, DELIMITER);
      std::vector<int> clause;
      for (int j = 0; j < tokens.size()-1; j++) {
        clause.push_back( std::stoi(tokens[j]) );
      }
      std::pair<std::string, std::vector<int> > entry;
      entry = std::make_pair (clause_to_string(clause), clause);
      clauses.insert(entry);
    }
    input_file.close();
  }
}

///=====================================================================================================


std::vector<std::string> SAT::split(const std::string& sentence, char delimiter)
{
    std::vector<std::string> tokens;

    int start_index = 0;
    int word_length = 0;
    bool in_word = false;

    for (int i = 0; i < sentence.length(); i++)
    {
        if (sentence[i] == delimiter)
        {
            if (in_word)
            {
                std::string token = sentence.substr(start_index, word_length);
                tokens.push_back(token);
            }
      
            start_index = i+1;
            word_length = 0;
            in_word = false;
        }
        else
        {
            word_length ++;
            in_word = true;
        }
    }
    if (in_word)
    {
        tokens.push_back(sentence.substr(start_index, word_length));
    }

    return tokens;
}

///=====================================================================================================

std::string SAT::clause_to_string(const std::vector<int>& clause)
{
    std::string encode = "C";
    for (int i = 0; i < clause.size(); i++)
    {
        encode += " " + std::to_string(clause[i]);
    }
  return encode;
}

///=====================================================================================================

void SAT::display_model(const std::map<int, bool>& model)
{
    for(std::map<int, bool>::const_iterator var = model.begin(); var != model.end(); ++var)
    {
        std::cout << var->first << " " << var->second << std::endl;
    }
}

///=====================================================================================================
