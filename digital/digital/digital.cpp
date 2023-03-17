// digital.cpp : This file contains the 'main' function. Program execution begins and ends there.
// DigitaL Design 1 Project 1 : Quine-McCluskey Logic Minimization
// Nour Kasaby 900211955
// Nadine Safwat 900212508
// Mariam Elghobary 900211608

#include <iostream>
#include <vector>
#include <string>
#include <ctype.h>
#include <algorithm> 
#include <set>
#include <map>
using namespace std;


bool validate_alpha(string& str, int n);
string Remove_Spaces(string str);
string reading_func();
set<char> variables(string str);
void print_variable_set(string str);
vector<char> dec_to_binary(int n, string str);
vector<string>input_fix_up(string str, set<char> variable_list);
vector<string> Get_Binary_Min_Max(vector<int> M, vector<vector<char>>TT, int type);
void Print_Sop_Pos(set<char> variables_list, vector<int> M, vector<vector<char>>TT);
vector<vector<char>> generate_TT(int num, string str, set<char> variables_list);
template<typename T>
void swap(vector<T>& v, int a, int b);
template<typename T>
void print(vector<T> v);
set<string> QMStep1(vector<string> minterms, set<char> variables_list, vector<string> not_combined, set<string> PI_set);
set<string> translateCombined(set<string> combined, set<char> vars);
void Part4(vector<vector<int>> Mcombinations, vector<string> Bcombinations, set<char> vars);


bool validate_alpha(string& str, int n)                    // validating the SoP format only (makes sure function entry contains letters or ' or +) ASSUMING NO SPACES BETWEEN CHARACTERS
{
    bool SOP = true;                                     // ** need to make sure that func doesnt have ' before a letter or end with a +
    for (int i = 0; i < n; i++) {
        if (!isalpha(str[0])) {
            SOP = false;
            break;
        }

        if (isalpha(str[i]))
        {
            if (variables(str).size() > 10) {
                SOP = false;

                cout << "Invalid. Maximum number of vairables is 10. Please enter a new function." << endl;
                return SOP;
            }

        }
        else if (str[i] != '+' && str[i] != '\'' && str[i] != ' ') {
            SOP = false;
            break;
        }


    }
    if (SOP == true) {
        cout << "SoP format" << endl;
        return SOP;
    }
    else
    {
        cout << "Invalid. Not in SoP format, please re-enter your function" << endl;
        return SOP;
    }
}

string Remove_Spaces(string str) {
    string new_str = "";
    for (int i = 0; i < str.size(); i++) {
        if (!isspace(str[i]))
            new_str += str[i];
    }
    return new_str;
}

string Remove_Repetitions(string str, set<char> variable_list) {
    //Handling of repetitions
    string final = "";
    bool valid = true;
    vector<string> terms_vec;
    vector<string> temp;
    string term = "";
    for (int i = 0; i < str.size(); i++) {
        if (str[i] != '+')
            term += str[i];
        else {
            terms_vec.push_back(term);
            term = "";
        }

    }
    terms_vec.push_back(term);
    map<char, int >frequency;
    set<char>::iterator it = variable_list.begin();

    for (int i = 0; i < terms_vec.size(); i++) {
        valid = true;
        for (it = variable_list.begin(); it != variable_list.end(); it++) {
            frequency[*it] = 0;
        }
        for (int j = 0; j < terms_vec[i].size(); j++) {
            if (frequency[terms_vec[i][j]] == 0) {
                final += terms_vec[i][j];
                frequency[terms_vec[i][j]]++;
            }
            else {
                if (terms_vec[i][j + 1] == '\'')
                    valid = false;
            }
            //end of first for loop
            if (!valid)
                final = final.substr(0, final.length() - j); //-1??
        }
        if (valid)
            temp.push_back(final);
        final = "";

    }
    final = "";
    /*
    cout << "printing" << endl;
    for (int i = 0; i<temp.size(); i++) {
        cout << temp[i] << endl;
    }
    */
    for (int i = 0; i < temp.size(); i++) {
        for (int j = 0; j < temp[i].size(); j++) {
            final += temp[i][j];
        }
        final += '+';
    }
    final = final.substr(0, final.length() - 1);

    return final;
}

string reading_func()                            //very basic function that takes SOP function from user and validates it ONLY. (will be improved later)
{
    string func;
    bool valid = false;
    while (!valid) {
        cout << "Please enter a function as SoP with a maximum of 10 variables." << endl;
        getline(cin, func);
        int n = func.size();
        valid = validate_alpha(func, n);
    }

    func = Remove_Spaces(func);
    func = Remove_Repetitions(func, variables(func));
    transform(func.begin(), func.end(), func.begin(), ::tolower);
    return func;
}

set<char> variables(string str)
{
    set<char> variable;              //using an unordered set to store unique letters, this will help us know how many vairables we have in the function
    for (int i = 0; i < str.size(); i++)
    {
        if (isalpha(str[i]))               //inserting characters into the set only if they are letters (not + or ')
            variable.insert(str[i]);
    }
    return variable;
}

void print_variable_set(string str)            //will probably use to print out the truth tables
{
    set<char> variable;
    for (int i = 0; i < str.size(); i++)
    {
        if (isalpha(str[i]))
            variable.insert(str[i]);
    }
    for (auto it = variable.begin(); it != variable.end(); ++it)
        cout << *it << ' ';

}

vector<char> dec_to_binary(int n, string str)
{

    vector <char> binary;
    int rows = pow(2, variables(str).size());
    int cols = variables(str).size() + 1;
    for (int i = cols - 2; i >= 0; i--) {
        int k = n >> i;
        if (k & 1)
        {
            binary.push_back('1');
        }
        else
        {
            binary.push_back('0');
        }

    }
    return binary;
}

vector<string>input_fix_up(string str, set<char> variable_list) { //ensures each term is represented with all literals
    vector<string> terms_vec;
    string term = "";
    for (int i = 0; i < str.size(); i++) {
        if (str[i] != '+')
            term += str[i];
        else {
            terms_vec.push_back(term);
            term = "";
        }

    }
    terms_vec.push_back(term);
    set<char>::iterator it = variable_list.begin();
    for (it = variable_list.begin(); it != variable_list.end(); it++) {
        for (int j = 0; j < terms_vec.size(); j++) {

            if (terms_vec[j].find(*it) == string::npos) {

                terms_vec[j] = (terms_vec[j] + (*it));
                terms_vec.push_back(terms_vec[j] + "'");
            }
        }
    }
    //Testing
    /*
    for (int i = 0; i < terms_vec.size(); i++) {
        cout << terms_vec[i] << endl;
    }
    cout << terms_vec.size();
    */



    return terms_vec;

}

vector<int> Get_Minterms(string str, int num_of_variables, set<char> variables_list) { //returns vector of int resembling minterms

    vector<string> terms_vec;
    terms_vec = input_fix_up(str, variables_list);

    /* cout << "each term: " << endl;
     for (int i = 0; i < terms_vec.size(); i++) {
         cout << terms_vec[i] << endl;
     }
     */
    set<char>::iterator it = variables_list.begin();
    int y;
    vector<int> Minterms;
    int sum, count;
    for (int i = 0; i < terms_vec.size(); i++) {
        sum = 0;


        for (int j = 0; j < terms_vec[i].size(); j++) {

            if (terms_vec[i][j + 1] == '\'')
                j++;
            else
            {
                char x = terms_vec[i][j];

                y = distance(variables_list.begin(), find(variables_list.begin(), variables_list.end(), x));


                sum += pow(2, (num_of_variables - int(y) - 1));
            }   //mariam did this


        }
        Minterms.push_back(sum);
    }
    /*
    cout << "Minterms: " << endl;
  for (int i = 0; i < Minterms.size(); i++) {
      cout << Minterms[i] << endl;
  }
  */

    return Minterms;
}

vector<string> Get_Binary_Min_Max(vector<int> M, vector<vector<char>>TT, int type) { //type 0 returns binary vector of minterms and type 1 of maxterms
    string term = "";
    vector<string>Binary_Terms;
    if (type == 0) {
        for (int i = 0; i < TT.size(); i++) {
            for (int j = 0; j < TT[i].size() - 1; j++) {
                if (find(M.begin(), M.end(), i) != M.end())
                    term += TT[i][j];
            }
            if(term != "")
                Binary_Terms.push_back(term);
            term = "";
        }
    }
    else if (type == 1) {
        for (int i = 0; i < TT.size(); i++) {
            for (int j = 0; j < TT[i].size() - 1; j++) {
                if (find(M.begin(), M.end(), i) != M.end() == false)
                    term += TT[i][j];
            }
            if (term != "")
                Binary_Terms.push_back(term);
            term = "";
        }
    }
    /*
    for (int i = 0; i < Binary_Terms.size(); i++) {
        cout << Binary_Terms[i] << endl;
    }
    */
    return Binary_Terms;

}

void Print_Sop_Pos(set<char> variables_list, vector<int> M, vector<vector<char>>TT) {

    vector<string> Minterms = Get_Binary_Min_Max(M, TT, 0);
    vector<string> Maxterms = Get_Binary_Min_Max(M, TT, 1);
    /* for (int i = 0; i < Minterms.size(); i++) {
         cout << Minterms[i] << endl;
     }
     */
    string canonical = "";
    //Sop
    char literal;
    set<char>::iterator it = variables_list.begin();
    bool added = false;

    for (int i = 0; i < Minterms.size(); i++) {
        for (int j = 0; j < Minterms[i].size(); j++) {

            advance(it, j);
            literal = *it;
            canonical += literal;
            added = true;
            if (Minterms[i][j] == '0')
                canonical += '\'';
            it = variables_list.begin();
        }
        if (added)
            canonical += '+';
        added = false;
    }

    canonical = canonical.substr(0, canonical.length() - 1);
    cout << endl << "Canonical SoP form: " << canonical << endl;
    it = variables_list.begin();
    canonical = "";
    canonical += '(';

    for (int i = 0; i < Maxterms.size(); i++) {
        for (int j = 0; j < Maxterms[i].size(); j++) {

            advance(it, j);
            literal = *it;
            canonical += literal;
            added = true;
            if (Maxterms[i][j] == '1')
                canonical += '\'';
            it = variables_list.begin();
            if (j != Maxterms[i].size() - 1)
                canonical += '+';
        }
        if (added)
            canonical += ")(";
        added = false;
    }
    canonical = canonical.substr(0, canonical.length() - 1);
    cout << "Canonical Pos form: " << canonical << endl;


}

vector<vector<char>> generate_TT(int num, string str, set<char> variables_list)
{
    print_variable_set(str);
    cout << str;
    int rows = pow(2, variables(str).size());
    int cols = variables(str).size() + 1;
    vector < vector<char>> TT(rows);
    vector<char> bin;

    vector<int>M;
    cout << endl;
    //Obtaining Minterms 
    M = Get_Minterms(str, num, variables_list);


    for (int i = 0; i < rows; i++)
    {
        bin = dec_to_binary(i, str);
        for (int j = 0; j < cols; j++)

        {
            if (j == cols - 1) {

                if (find(M.begin(), M.end(), i) != M.end()) {
                    TT[i].push_back('1');
                }

                else
                    TT[i].push_back('0');
            }
            else
                TT[i].push_back(bin[j]);

            cout << TT[i][j] << " ";
        }
        cout << endl;
    }

    Print_Sop_Pos(variables_list, M, TT);

    return TT;
}

template<typename T>
void swap(vector<T>& v, int a, int b) {
    T temp = v[a];
    v[a] = v[b];
    v[b] = temp;
}

template<typename T>

void print(vector<T> v) {
    for (int i = 0; i < v.size(); i++) {
        cout << v[i] << endl;
    }
}

vector<string> compare_vec(vector<string> v, vector<string> w, set<char> variables_list, vector<string>& combined, vector<string>& notcombined, vector<string>& joined, set<string> not_comb_set)
{
    int numvar = variables_list.size();
    int difference = 0;
    string new_term = "";

    for (int i = 0; i < v.size(); i++)
    {
        for (int j = 0; j < w.size(); j++)
        {
            for (int k = 0; k < numvar; k++)
            {
                if (v[i].at(k) != w[j].at(k))
                {
                    difference++;
                }
            }
            for (int k = 0; k < numvar; k++)
            {
                if (difference == 1 && v[i].at(k) != w[j].at(k))
                {
                    new_term += "_";
                }
                else if (difference == 1 && v[i].at(k) == w[j].at(k))
                {
                    new_term += v[i].at(k);
                }

            }

            if (difference != 1)
            {
                notcombined.push_back(v[i]);;
                notcombined.push_back(w[j]);
            }

            if (difference == 1)
            {
                combined.push_back(new_term);
                joined.push_back(v[i]);
                joined.push_back(w[j]);
            }

            new_term = "";
            difference = 0;
        }

    }

    for (int i = 0; i < notcombined.size(); i++)
    {
        for (int j = 0; j < joined.size(); j++)
        {
            if (notcombined[i] == joined[j])
            {
                notcombined[i].erase();
            }
        }
    }

    return combined;
}

set<string> QMStep1(vector<string> minterms, set<char> variables_list, vector<string> not_combined, set<string> PI_set) {

    vector <string> str;
    vector<string> joined;
    vector<string> combined;
    set<string> PI_set_final;
    int comp = 0;

    vector<int> numOfOnes(minterms.size());
    map<int, vector<string>> groups;

    for (int i = 0; i < minterms.size(); i++) {
        int count = 0;
        for (int j = 0; j < minterms[i].size(); j++) {
            if (minterms[i][j] == '1')
                count++;
        }
        numOfOnes[i] = count;
    }
    for (int k = 0; k < numOfOnes.size() - 1; k++)
    {
        for (int l = 0; l < numOfOnes.size() - k - 1; l++) {
            if (numOfOnes[l] > numOfOnes[l + 1]) {
                swap(numOfOnes, l, l + 1);
                swap(minterms, l, l + 1);
            }
        }
    }

    for (int i = 0; i < numOfOnes.size(); i++)
    {
        groups[numOfOnes[i]].push_back(minterms[i]);
    }

    if (groups.size() == 1) {
        for (int x = 0; x < groups[numOfOnes[0]].size(); x++) {
            not_combined.push_back(groups[numOfOnes[0]][x]);
        }

        for (int i = 0; i < not_combined.size(); i++)
        {
            PI_set.insert(not_combined[i]);
        }

        return PI_set;
    }
    for (auto it1 = groups.begin(); it1 != groups.end(); ++it1) {                              //loops to access value elements of the map
        for (auto it2 = groups.begin(); it2 != groups.end(); ++it2) {

            {
                if (it1->first == it2->first - 1)
                    str = compare_vec(it1->second, it2->second, variables_list, combined, not_combined, joined, PI_set);
            }
        }
    }
    if (combined.size() == 0) {
        for (int i = 0; i < not_combined.size(); i++)
        {
            PI_set.insert(not_combined[i]);
        }
        return PI_set;
    }

    PI_set_final = QMStep1(str, variables_list, not_combined, PI_set);
    return PI_set_final;
}

set<string> translateCombined(set<string> combined, set<char> vars) {
    string Implicant;
    set<string> translated;
    set<char>::iterator it = vars.begin();
    char letter;
    for (auto i = combined.begin(); i != combined.end(); i++)
    {
        Implicant = "";
        for (int j = 0; j < (*i).size(); j++) {
            if ((*i)[j] != '_')
            {
                advance(it, j);
                letter = *it;
                Implicant += letter;
                if ((*i)[j] == '0')
                    Implicant += '\'';
            }
            it = vars.begin();
        }
        translated.insert(Implicant);
    }

    for (auto i = translated.begin(); i != translated.end(); i++)
    {
        cout << *i << " ";
    }
    cout << endl;

    return translated;
}

void Part4(vector<vector<int>> Mcombinations, vector<string> Bcombinations, set<char> vars) {
    map<int, int> smthn;
    map<int, vector<string>> things;
    for (int i = 0; i < Mcombinations.size(); i++) {
        for (int j = 0; j < Mcombinations[i].size(); j++) {
            smthn[Mcombinations[i][j]]++;
            things[Mcombinations[i][j]].push_back(Bcombinations[i]);
        }
    }

    set<string> essentials;
    set<string> nonEssentials;

    for (auto i : smthn) {
        if (i.second == 1) {
            essentials.insert(things[i.first][0]);
        }
    }

    for (int i = 0; i < Bcombinations.size(); i++) {
        if (essentials.find(Bcombinations[i]) == essentials.end())
            nonEssentials.insert(Bcombinations[i]);
    }

    essentials = translateCombined(essentials, vars);
    nonEssentials = translateCombined(nonEssentials, vars);


    cout << "Essentials: \n";
    for (auto i = essentials.begin(); i != essentials.end(); i++) {
        cout << *i << endl;
    }
    /*cout << "\nNon-Essentials: \n";
    for (auto i = nonEssentials.begin(); i != nonEssentials.end(); i++) {
        cout << *i << endl;
    }*/
}



int main()
{
     string func;
     vector<string> not_combined;
     vector<string> joined;
     set<string> PIset;

      func = reading_func();     
      set<char> vars = variables(func);
      int num = vars.size();
      cout << endl;     //test

      vector<vector<char>> TT = generate_TT(num, func, vars);
      vector<int> M = Get_Minterms(func, num, vars);
      vector<string> B = Get_Binary_Min_Max(M, TT, 0);
      set<string> PI = QMStep1(B, vars, not_combined, PIset);
      translateCombined(PI, vars);
}
