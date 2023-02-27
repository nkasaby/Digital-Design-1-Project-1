// digital.cpp : This file contains the 'main' function. Program execution begins and ends there.
// DigitaL Design 1 Project 1 : Quine-McCluskey Logic Minimization
// Nour Kasaby 900211955
// Nadine Safwat 900
// Mariam Elghobary 900

#include <iostream>
#include <vector>
#include <string>
#include <ctype.h>
#include <unordered_set>
using namespace std;

void validate_alpha(string& str, int n);
int num_of_variables(string str);
string reading_func();



void validate_alpha(string &str, int n)                    // validating the SoP format only (makes sure function entry contains letters or ' or +) ASSUMING NO SPACES BETWEEN CHARACTERS
{                                                         
    bool SOP = false;                                     // ** need to make sure that func doesnt have ' before a letter or end with a +
    for (int i = 0; i < n; i++) {
        if (isalpha(str[0]))
        {
            if (isalpha(str[i + 1]))
            {
                if (num_of_variables(str) <=10)
                     SOP = true;
                else
                {
                    cout << "Invalid. Maximum number of vairables is 10. Please enter a new function." << endl;
                    reading_func();
                }
            }
            else if (str[i + 1] == '+')
                SOP = true;
            else if (str[i + 1] == '\'')
                SOP = true;
          
            else
            {
                SOP;
                break;
            }
        }
        else
            SOP;
    }
    if (SOP==true)
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
        cout << *it<<' ';
}


void generate_truth_table(int num)
{

}


int main()
{
    string func;
    func = reading_func();      //testing function
    cout << num_of_variables(func);     // testing function
    cout << endl;
    print_variable_set(func);         //testung function
}

