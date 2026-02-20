# README


## How to run
- Run the 3.12.cpp file first to generate "First-Follow.txt" file.
- Then, run the 3.3.cpp file and give input string to check acceptance.


## Input format
- Write your input grammar in "GrammarLL.txt" file. Make sure to add an extra newline at the end of input grammar.
- All UPPERcase characters will be treated as non-terminals.
- Other than that all other characters will be treated as terminals.
- Use 0 to represent epsilon (ε).
- All terminals/non-terminals should be single character.
- When running 3.3.cpp file provide only a single line of input.


## Output format
- If given string is "Accepted" steps are given on how to generate the given string and the productions involved.
- String is rejected, then partial reason of error is outputted.


## Examples:
- **Input 1**: <br>
GrammarLL.txt <br>
    E -> T G <br>
    G -> +T G | 0 <br>
    T -> F U <br>
    U -> \*F U | 0 <br>
    F -> (E) | d <br>
<br>
Input string: d+d\*d<br>

- **Input 2**: <br>
GrammarLL.txt <br>
S -> UVW <br>
U -> (S) | aSb | d <br>
V -> aV | 0 <br>
W -> cW | 0 <br>
<br>
Input string: aa(d)bbc <br>

- **Input 3**: <br>
GrammarLL.txt <br>
S -> UVW <br>
U -> (S) | aSb | d <br>
V -> aV | 0 <br>
W -> cW | 0 <br>
<br>
Input string: ab) <br>
