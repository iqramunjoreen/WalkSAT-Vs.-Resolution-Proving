#include <iostream>
#include <fstream>
#include <assert.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <cstdlib>
#include <utility>
#include <map>
#include "resolution_prover.h"

const std::string START_OF_CLAUSE = "C";
const std::string END_OF_LINE = "0";
const char DELIMITER = ' ';


ResolutionProver::ResolutionProver() {}

ResolutionProver::~ResolutionProver() {}

clause_map ResolutionProver::pl_resolve(const literal_set& ci, const literal_set& cj)
{
    literal_set clause;
    clause_map resolvent;
    literal_set ci_copy;
    
    for (auto di = ci.begin(); di!=ci.end(); ++di)
    {
        for (auto dj = cj.begin(); dj!=cj.end(); ++dj)
        {
            if (((*di) * -1)== *dj)
            {
                //Create a copy & remove literals through resolution
                clause = cj;
                ci_copy = ci;
                ci_copy.erase(*di);
                clause.erase(*dj);

                //Insert the newly clause into resolvent list
                clause.insert(ci_copy.begin(), ci_copy.end());
                resolvent.insert(std::make_pair(clause_to_string(clause), clause));
            }
        }
    }
    return resolvent;
}

///==================================================================================

bool ResolutionProver::pl_resolution()
{
    clause_map new_clauses;
    clause_map resolvent;
    int last_size;
    
    while (true)
    {
        last_size = new_clauses.size();
        for (auto ci = clauses.begin(); ci!=clauses.end(); ++ci)
        {
            for (auto cj = ci; cj!=clauses.end(); ++cj)
            {
                resolvent = pl_resolve(ci->second, cj->second);
                if (resolvent.count(START_OF_CLAUSE) > 0)
                {
                    return false;
                }
                new_clauses.insert(resolvent.begin(), resolvent.end());
            }
        }
        if (new_clauses.size() == last_size)
        {
            return true;
        }
        clauses.insert(new_clauses.begin(), new_clauses.end());
    }
}

///==================================================================================

void ResolutionProver::load_from_file(char* file_path)
{
    std::string line;
    std::ifstream input_file (file_path);
    if (input_file.is_open())
    {
        getline(input_file, line); //extra
        getline(input_file, line); //extra
        std::vector<std::string> tokens = split(line, DELIMITER);

        this->num_clauses = std::stoi (tokens[3]);
        this->num_variables = std::stoi (tokens[2]);

        int len = 0;
        for (int i = 0; i < this->num_clauses; i++)
        {
            getline(input_file, line);

            std::vector<std::string> tokens = split(line, DELIMITER);

            // Remove END_OF_LINE token
            len = tokens.size();
            assert(tokens[len-1] == END_OF_LINE);
            tokens.pop_back();

            literal_set clause = tokens_to_clause(tokens);
            clause_entry clause_pair;

            for (int j = 0; j < len-1; j++)
            {
                clause_pair = std::make_pair(clause_to_string(clause), clause);
                this->clauses.insert(clause_pair);
            }
        }
        input_file.close();
    }
}

///==================================================================================

literal_set ResolutionProver::tokens_to_clause(const std::vector<std::string>& tokens)
{
    literal_set clause;
    for (int i = 0; i < tokens.size(); i++)
    {
        clause.insert( std::stoi(tokens[i]) );
    }
    return clause;
}

///==================================================================================

void ResolutionProver::display_clauses()
{
    for(auto clause = clauses.begin(); clause != clauses.end(); ++clause)
    {
        std::cout << clause->first << std::endl;
    }
}

///==================================================================================

std::string ResolutionProver::clause_to_string(const literal_set& clause)
{
    std::string clause_string = START_OF_CLAUSE;
    for (std::set<int>::iterator it=clause.begin(); it!=clause.end(); ++it)
    {
        clause_string = clause_string + " " + std::to_string (*it);
    }
    return clause_string;
}

///==================================================================================

std::vector<std::string> ResolutionProver::split(const std::string& sentence, char delimiter)
{
    std::vector<std::string> tokens;

    int word_length = 0;
    int start_index = 0;
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
            in_word = false;
            word_length = 0;
        }
        else
        {
            in_word = true;
            word_length ++;
        }
    }
    if (in_word) tokens.push_back(sentence.substr(start_index, word_length));

    return tokens;
}

///==================================================================================
