// digital.cpp : This file contains the 'main' function. Program execution begins and ends there.
// DigitaL Design 1 Project 1 : Quine-McCluskey Logic Minimization
// Nour Kasaby 900211955
// Nadine Safwat 900212508
// Mariam Elghobary 900211608

#include <iostream>
#include <vector>
#include <string>
#include <ctype.h>
#include <set>
#include <map>
using namespace std;

void validate_alpha(string& str, int n);
set<char> variables(string str);
string reading_func();
void print_variable_set(string str);
vector<char> dec_to_binary(int n, string str);
vector<string>input_fix_up(string str, set<char> variable_list);
vector<string> Get_Binary_Min_Max(vector<int> M, vector<vector<char>>TT, int type);
void Print_Sop_Pos(set<char> variables_list, vector<int> M, vector<vector<char>>TT);
vector<vector<char>> generate_TT(int num, string str, set<char> variables_list);


void validate_alpha(string& str, int n)                    // validating the SoP format only (makes sure function entry contains letters or ' or +) ASSUMING NO SPACES BETWEEN CHARACTERS
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
                reading_func();
            }

        }
        else if (str[i] != '+' && str[i] != '\'') {
            SOP = false;
            break;
        }


    }
    if (SOP == true)
        cout << "SoP format" << endl;
    else
    {
        cout << "Invalid. Not in SoP format, please re-enter your function" << endl;
        reading_func();
    }
}


string reading_func()                            //very basic function that takes SOP function from user and validates it ONLY. (will be improved later)
{
    string func;
    cout << "Please enter a function as SoP with a maximum of 10 variables." << endl;
    cin >> func;
    int n = func.size();
    validate_alpha(func, n);
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

vector<string>input_fix_up(string str, set<char> variable_list ) { //ensures each term is represented with all literals
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
    for (it = variable_list.begin();it != variable_list.end(); it++) {
        for (int j = 0; j < terms_vec.size();j++) {

            if (terms_vec[j].find(*it) == string::npos) {

                terms_vec[j]=(terms_vec[j] + (*it));
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

vector<int> Get_Minterms(string str, int num_of_variables,set<char> variables_list) { //returns vector of int resembling minterms
    
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
            if (Maxterms[i][j] == '0')
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
        cout << v[i] << "   ";
    }
    cout << endl;
}


void QMStep1(vector<string> minterms) {

    cout << "QM STEP 1 TEST: \n";

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

    cout << endl;

    for (int k = 0; k < numOfOnes.size() - 1; k++)
    {
        for (int l = 0; l < numOfOnes.size() - k - 1; l++) {
            if (numOfOnes[l] > numOfOnes[l + 1]) {
                swap(numOfOnes, l, l + 1);
                swap(minterms, l, l + 1);
            }
        }
    }

    for (int a = 0; a < minterms.size(); a++) {
        cout << minterms[a] << "   ";
    }

    cout << "\n QM STEP 1 TEST END \n";



    for (int i = 0; i < numOfOnes.size(); i++)
    {
        groups[numOfOnes[i]].push_back(minterms[i]);
    }

    for (auto i : groups)
    {
        cout << i.first << "->";
        for (auto j : i.second)
            cout << j << " ";
        cout << endl;
    }



    // cout << minterms[1] << endl << minterms[2] << endl;
}

int main()
{
    //string func;
    //func = reading_func();      //testing function
    //int num = variables(func).size();     // testing function

    //cout << endl;

    //generate_TT(num, func, variables(func));     //test
    //Get_Minterms(func, num);

    vector<string> m = {
        "111", "011", "100", "000", "101", "110"
    };

    QMStep1(m);

}
