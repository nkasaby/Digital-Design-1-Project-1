// digital.cpp : This file contains the 'main' function. Program execution begins and ends there.
// DigitaL Design 1 Project 1 : Quine-McCluskey Logic Minimization
// Nour Kasaby 900211955
// Nadine Safwat 900212508
// Mariam Elghobary 900211608

#include <iostream>
#include <vector>
#include <string>
#include <ctype.h>
#include <unordered_set>
using namespace std;

void validate_alpha(string& str, int n);
int num_of_variables(string str);
string reading_func();
void print_variable_set(string str);
vector<char> dec_to_binary(int n, string str);
void generate_truth_table(int num, string str);


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
            if (num_of_variables(str) > 10) {
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

int num_of_variables(string str)
{
    unordered_set<char> variable;              //using an unordered set to store unique letters, this will help us know how many vairables we have in the function
    for (int i = 0; i < str.size(); i++)
    {
        if (isalpha(str[i]))               //inserting characters into the set only if they are letters (not + or ')
            variable.insert(str[i]);
    }
    return variable.size();
}

void print_variable_set(string str)            //will probably use to print out the truth tables
{
    unordered_set<char> variable;
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
    int rows = pow(2, num_of_variables(str));
    int cols = num_of_variables(str) + 1;
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

vector<int> Get_Minterms(string str, int num_of_variables) {
    string term = "";
    vector<string> terms_vec;
    for (int i = 0; i < str.size(); i++) {
        if (str[i] != '+')
            term += str[i];
        else {
            terms_vec.push_back(term);
            term = "";
        }

    }
    terms_vec.push_back(term);
    /* cout << "each term: " << endl;
     for (int i = 0; i < terms_vec.size(); i++) {
         cout << terms_vec[i] << endl;
     }
     */
    vector<int> Minterms;
    int sum, count;
    for (int i = 0; i < terms_vec.size(); i++) {
        sum = 0;
        count = 0;
        for (int j = 0; j < terms_vec[i].size(); j++) {

            if (terms_vec[i][j + 1] == '\'')
                j++;
            else
                sum += pow(2, num_of_variables - count - 1);
            count++;


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



void generate_TT(int num, string str)
{
    print_variable_set(str);
    cout << str;
    int rows = pow(2, num_of_variables(str));
    int cols = num_of_variables(str) + 1;
    vector < vector<char>> TT(rows);
    vector<char> bin;

    vector<int>M;
    cout << endl;
    //Obtaining Minterms 
    M = Get_Minterms(str, num);


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
}


int main()
{
    string func;
    func = reading_func();      //testing function
    int num = num_of_variables(func);     // testing function

    cout << endl;

    generate_TT(num, func);     //test
    Get_Minterms(func, num);

}


