# Digital-Design-1-Project-1
 Program Design: 

Validation :
The inputted function is required to be a Sum of Products (SoP) accepting ‘+’ as OR between terms and not at the start  and ‘ \’ ’ for variable negation after the corresponding variable. Violations of SoP formatting are detected by the function validate_alpha . Violations result in the re-entry of the input through the function reading_func where the validation process is repeated until the input is valid.
A maximum of 10 variables is accepted where each variable is represented as a single distinct Latin letter starting from ‘a’. Any upper-case letters are automatically converted to lower case. Inputting more than 10 variables results in the re-entry of the input. 
Spaces between terms are handled by the Remove_Spaces function and so they do not impact program functionality. For example “a b + c” would be stored as “ab+c”.
Repeated literals within the same term are reduced to one distinct literal per term by Remove_Repetitions and so they do not impact program functionality. For example  “abbcc” would stored as  “abc”.
Terms equivalent to zero  (always false in the truth table)  are handled by the program and omitted from the calculations. For example  “ab+aa’ “ would be stored and manipulated as “ab”
Double negations are also handled by the program and stored and manipulated without the negations. For example “ a ’ ’ ” would be stored and manipulated as “a”



To obtain the truth table required for canonical SoP and PoS generation, we constructed several building blocks as follows:
Further Manipulation and Minterm Generation
After validation and before manipulation the inputted function is passed as a string through the variables function which returns an ordered set of characters containing all of our variables. This corresponds later to the number of columns in the truth table.
After initial manipulation of the inputted function such that we have an SoP without spaces, repeated literals within the same term and, without zeroed terms, the new function is stored as a string and passed passed to the function Get_Minterms which takes in the new manipulated function and the set of variables. Get_Minterms utilizes another  function called input_fix_up which takes in the function as a string and returns a vector of strings referred to as terms_vec  where each element in the vector corresponds to the function terms (products) resembled by all the literals variables. For example “a+b” would return a vector containing “ ab ”  “ ab’ ”  “ a’b ”  “ab”. This eases truth table generation moving forward.
After obtaining our vector of terms, we calculate the minterms through terms_vec and the set of variables. Since we have an alphabetically ordered set reflecting each literal from MSB to LSB we utilize the position of each literal to calculate the minterm value. Through a nested loop, for every term in terms_vec we sum the decimal value of every non negated literal and obtain the sum which resembles the minterm. This process is similar to binary to decimal conversion where a non negated literal is ‘1’ and the negated is ‘0’. The integer position of the literal in the set of variables is obtained through the built in distance function by calculating the positions from the beginning of the set to the required literal as follows

y = distance(variables_list.begin(), find(variables_list.begin(), variables_list.end(), x));

Where x is the current literal in the term,  and variables_list is the set of variables.

The value added for each literal is obtained through the equation

sum += pow(2, (num_of_variables - int(y) - 1));

After completing all literals in a single product term the summation is pushed into a vector<int> of minterms. This is repeated for all product terms in terms_vec and the vector of minterms is returned

Truth Table Generation

The truth table is constructed and returned as a vector<vector<char>>  by the function generate_TT.  The function takes in the following: number of variables, initial string inputted by the user (to account for omitted always-zero terms), the new manipulated function string, and the ordered set of variables.
The function first calls the previously explained Get_Minterms function and stores the integer minterms in a vector. The number of truth table columns is the number of variables present in the user’s initial input +1 for the output. It is important to note that any omitted literals entered by the user are still designated a column. For example a function  “ ab+cc’ ” has a column for “ a”, another for “b” and another for “c” despite “ cc’ ” being omitted from calculations. The number of rows in the truth table is 2^number of variables inputted by user. 
The combinations of 1’s and 0’s in each row excluding the final output column are done by converting the row number from integer to binary through a function called dec_to_binary. 
For the final function output column the row number is searched for in the integer vector of minterms if present then the function output is set to 1 else to 0.
if (find(M.begin(), M.end(), i) != M.end())
TT[i].push_back('1');
 else
TT[i].push_back('0');

Here M is the vector of minterms and TT is the truth table as a vector<vector<char>> with i being the row index

Binary Minterms and Maxterms Generation 
After truth table generation, we use the truth table to obtain all the minterms and maxterms through the function Get_Binary_Min_Max
This function takes the vector of minterms and the truth table and returns a vector<string> containing either the minterms or maxterms in binary. The user specifies that they want to generate minterm by passing in a numerical parameter ‘0’ or that they want to generate maxterms by passing in ‘1’
For minterms, the function simply inputs the binary representation of the row (excluding the final column of output) with an output of 1 into the minterms vector
For maxterms, the function simply inputs the binary representation of the row (excluding the final column of output) with an output of 0 into the maxterms vector
The vector is then returned

Canonical SoP/PoS Generation
To generate the canonical SoP and Pos, the function Print_Sop_Pos takes in the truth table, vector if integer minterms and set of variables. The function calls Get_Binary_Min_Max to receive a vector<strings> containing all the minterms in binary. This vector is then iterated through using a nested loop where for every binary term each binary digit is translated to its literal equivalent by corresponding the digit position and the literal position in the set of variables. If the digit is 1 then it is translated to the literal, if the digit is 0 then it is translated to the literal followed by a ‘\’’ for negation. Corresponding the digit position and the literal position in the set of variables is eased through the built in function advance which is used as follows
advance(it, j)
where it is an iterator pointing to the beginning of the set of variables and  j  is the index of the digit. Advance simply sets the iterator to point to the position  j  in the set of variables i.e. the iterator it now points to the digit’s corresponding literal.
To maintain SoP format every literal in the same term are put consecutively to resemble the product and between every two terms  a ‘+’ is inserted
The same procedure is repeated for the maxterms with the only differences being the parameters of the function Get_Binary_Min_Max to obtain the maxterms and the manipulation of the literals. For PoS, if the digit is 1 then it is translated to the literal followed by a ‘\’’ for negation, if the digit is 0 then it is translated to the literal . Every literal in the same term is separated from the next by a ‘+’ and every term (sum) is bracketed to resemble the product of sums.
The strings of SoP and Pos are then outputted.


In this step, we created the Quine-McCluskey algorithm to generate the prime implicants of a given function. We used 2 main functions to generate the prime implicants and show the minterms they cover. 
Generating and printing the prime implicants:

QMStep1 function: set<string> QMStep1(vector<string> minterms, set<char> variables_list, vector<string> not_combined, set<string> PI_set)

This function takes in 2 vectors of strings  (minterms and not combined), a set of characters (variables_list, we need the size of it), and a set of strings (PI_set).
The function will sort the minterms in the binary representation according to the number of ones in the binary number, then we will create a map where the key is the number of ones and the value of the key is the vector containing all the binary minterms that contain the corresponding number of ones.
We will then compare 2 consecutive rows of the map values (provided that key1 = key2-1). This will happen in a separate function called compare_vec which will return a vector or strings called combined. 
We insert the vector of strings that stored the prime implicants in their binary form in a set to get rid of the duplicates then we print out the set of primary implicants in binary form.
We then call this function again for the algorithm to run recursively for each column of the QM table. The base cases for this recursive function is of the map size is 1, meaning that we only have binary numbers with the same number of ones and when there are binary numbers that can no longer be combined (the difference between their characters is more than 1); code-wise, this happens when the size of the vector os strings that stores the terms can be combined is = 0.

Compare_vec function: vector<string> compare_vec(vector<string> v, vector<string> w, set<char> variables_list, vector<string>& combined, vector<string>& notcombined, vector<string>& joined, set<string> not_comb_set)

This function takes in several inputs which are:
2 vectors of strings(v and w) which are the two vectors in the value entries of the map.
A set of characters (variables_list) which contains the variables. We need its size to loop over each character in the elements of the 2 above vectors.
3 vectors of strings by reference (combined, not combined, joined). In these vectors we will be storing the binary minterms after they combined (resulting term including dashes), the binary minterms that could not be combined and binary minterms that combined respectively. These vectors update with every iteration. We passed them by reference to avoid loss of data with every iteration.
A set of strings (not_comb_set). We enter the elements of the vector ‘notcombined’ in this set to remove duplicates. 
Firstly, we declared an integer called ‘difference’ and initialize it with 0 to could the number of different characters between the strings of the binary numbers. We also declared a string called ‘new_term’ and initialized it as an empty string. We also declared a vector of strings called mins to store the minterms in binary form.
We then compare each character in every string of the two rows in the map through a nestedt for loop containing 3 loops and we increment the difference by 1 whenever we find a different character.
Then we check if the difference between characters is 1 and the characters in the same position are different, then we add ‘_’ to the string new_term in that specific position of the different character and if the the difference is still 1 but the characters in the same position are the same, then we copy the characters of the string in the first vector (note: copying characters of the string in either vector would not matter!) to the string new_term in the same positions.
If difference is not 1, then we will push the strings of both vectors in the notcombined vector.
If difference is 1, then we will push the strings of both vectors in the joined vector and the mins vector; and the new_term in the combined vector.
Then we will reset difference to 0 and new_term to “” for the next iterations.
We then compare the elements of notcombined and joined to see if a term that couldn’t combine in a previous iteration combined later on. If the element in notcombined is the same as the element in joined then it has combined in later iterations and should be removed from the notcombined vector.
Then we turn notcombined, combined and joined into sets to remove duplicates then turn them back into vectors. This step is crucial to avoid extremely long running times as we ran into this problem when we tested the program with 6+ variables.
Then we return the vector combined.
Showing the minterms the prime implicants cover:

In order to print out the minterms covered by the prime implicants we used the functions described below.


void padUnderscores(string binary, set<string>& pad);
	This is a recursive function that I made because we needed a way to receive a string in the form of  “00__1_” and be able to receive all the different minterms that can be made by replacing the ‘_’ with a ‘1’ or a ‘0’. Initially I thought to do this iteratively but I realized very quickly that the most efficient way to cover all possible combinations is using recursion. The function will first check that the string isn't empty then it will loop through the characters and will check if they are an underscore. If that is true then the function will replace the underscore with a 0 and recursively call, and then replace with a 1 and recursively call, all the final combinations are then stored in a set of strings. I chose a set in order to remove any repeated terms that may have been created. 

vector<vector<int>> fixing(set<string> BPI);
	This function will receive the binary form of the minterms used in the combinations, (received from padUnderscores) in the parameters and will use the function binaryToDecimal to convert the combinations into their integer forms and store them as a vector, it will do this for all the combinations and will return a vector of vector of integers in the form {{1,2,3,4},{5,6,7,8},{9,10}} which will represent the minterms that created certain combinations like “00__1_”

*Note that the example is not a valid one i simply put it for visual purposes

set<string> translateCombined(set<string> combined, set<char> vars);
	This function was heavily inspired by the function Print_Sop_Pos as it translates the binary combined forms to the binary expressions (“00__1_” to a’b’e). It will do this by receiving the set of combined binary forms from QMStep1 and the variables used in the function from reading_func and it will loop through the set and then through every character and it will check whether the character is ‘_’ or  ‘0’. If its ‘_’ it will ignore it, if it's not it will use an iterator to go to the position of the character in the variables and it will add this variable to the term, then it will check if the character is a ‘0’ and, if true, will add a ‘\’’ to the function as well to show that the variable should be inverted. After looping through all the characters it will store the newly created expression in a set of strings that will be returned at the end of the function

void Part4(vector<vector<int>> Mcombinations, set<string> Bcombinations, set<char> vars, string f);
	This is the function that detects if the implicant is essential. First I create three maps, one to store the number of covers a minterm has, then two to store which implicant covers which minterms, in both the binary and integer form. I then use the map that stores the number of covers for each minterm to detect which minterm only has one and then i will map that result to the map that stores the covers as binary form and translate it using translateCombined to eventually output that cover as an essential prime implicant in the binary expression form. Then I will generate all the minterms for the whole function using the GetMinterms function to check which minterms are covered by the essential covers i found and which aren't and this is done using a simple for loop and if statement. Minterms that aren't are stored in a set called Not covered and outputted onto the screen in their integer forms. 

Utility Functions: 

void swap(vector<T>& v, int a, int b); 
	This is used in QMStep1 so that the binary combinations can be ordered by the number of ones they contain. I used template typename T because I wasn't initially sure of which type the vector would be of. 

void print(vector<T> v);
	This is used throughout the code to print out the values inside of the vector, we mostly used it for testing instead of having to type out the for loop every time we needed to see the contents of a vector.

void printset(set<T> s);
This has the same purpose as print but for sets. 

vector<char> dec_to_binary(int n, string str);
Converts vector of binary strings to a vector of decimal integers

Problems in the Program:
	Naturally, many problems came up during the process of this project like, space handline, case handline, the time taken for certain functions to run, duplicate handling, and many more minor setbacks, however, we were able to overcome them by debugging the code or simply brainstorming together on the best ways to change the program to fix the problem. 

Test Cases: 
A'' : This is tested to show the case handling and the double dash handling that was implemented in the validation stage

a'b +        b'a: This is tested to show the space handline and how the order variables are inputted do not affect the output. Also implemented in the validations stage

1000+0001 (ab'c'd'+a'b'c'd): A case to show that the function will not accept boolean inputs and will give the user a chance to re input their expression in the right form and then will show how the algorithm works under normal conditions

a'b'c'd'e'f' + ab'c'd'e'f' + a'bc'd'e'f' + abc'd'e'f' + a'b'cd'e'f' + ab'cd'e'f' + a'bcd'e'f': A normal case to test

[Test 5]: An expression under normal cases but with a large number of variables to see how long the function will take, also to confirm that the function can handle large inputs

ab+cc’: shows how, even though cc’ will be negated, it will still show up in the truth table

BCDE' + A'BC'E + A'CD'E + A'B'DE + ABE' + B'C'E': tests the case handling implemented in validation while having a normal function, also produces non essential minterms so not covered function is one show. 

aa+a+aa+a: displays that the repetition of the same variable will still result in the variable once

aaa': shows that even if the expression is negated, the code will still be able to run

ab2+c, 12+a, a'b+cc'+abcd+ad'+ad'+acdd': shows that the function will continue to ask you for a valid input


*Results of test cases are attached as a separate pdf because its a very large document

Instructions for how to build and run the program:

When prompted for an input, enter the SOP expression you would like to test the algorithm on.
When you want to exit the program, close the terminal.

Member contributions:
This project would not have been possible without consistent collaboration and a lively team spirit. We initially split the project such that each team member is responsible for equal proportions of the project. Initially, Mariam was responsible for parts 1 and 2, Nour for 3 and Nadine for 4. However, as we started working  each team member contributed greatly in all four parts and this was reflected through our commits. Many segments of code were constructed collaboratively on zoom and so it is fair to say that all three of us wrote the entire project.

