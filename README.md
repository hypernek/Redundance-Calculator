# Redundance-Calculator
A calculator of the function *R(n)* that counts the number of representations of a natural number *n* in a given linear numeration system. 

## Usage Instructions

Two programs are included – the first calculates *R(n)* on bounds *n_min* and *n_max* entered by the user and the other calculates the maxima of *R(n)* on lengths entered by the user. The output of both programs is saved as a .csv file to the directory where the program was run.

Both programs are console applications that on initialisation ask the user to enter coefficients of the basis of the *B*-system. After entering these coefficients, the basis of the *B*-system is initialised. Afterwards, each of the two programs behaves differently.

The first program (Rn_calculator.exe) asks the user r to enter the lower and upper bound *n_min* and *n_max* of the values *n_min <= n <= n_max* on which the function *R(n)* is to be calculated. Alternatively, the user may enter an asterisk * and then enter the bounds *l_min* and *l_max* of the range of lengths on which they desire *R(n)* to be calculated. In effect, this sets *n_min* and *n_max* equal to *B_{l_{min}-1}* (the smalest n whose representation has length *l_min*) and *B_{l_{min}}-1* respectively (the largest *n* whose representation has length *l_max*). Hence, the user does not have to know the values of the elements of the basis sequence. After the bounds are set, the calculation commences and the console displays progress information. The values *R(n_min), R(n_min + 1), ..., R(n_max)* are first stored into the computer's RAM, and after they are all calculated, the program starts writing them to disc. Each row of the resultant .csv file is the triplet *(n,\<n\>_B,R(n))* – i.e. for every *n_min <= n <= n_max* the greedy representation of *n* is stored along with the value of *R(n)*. After writing out all the calculated values of *R(n)*, the program writes the time needed for calculation and writing to disc (as the last row of the .csv file).

The second program (Maxima_of_Rn_calculator.exe) asks the user to input the bounds *l_min* and *l_max* of the range of lengths on which the values *psi(l)* and *#Psi(l)* are to be determined. Then the program asks the user to enter a number *M*, which will be the maximum number of elements of *Psi(l)* they desire to save for a given length *l*. The size of *Psi(l)* can be very large and the user does not necessarily need to save all members of *Psi(l)*. For every *l* from the range *l_min <= l <= l_max* the program outputs *psi(l)*, *#Psi(l)*, and the first *M* elements of *Psi(l)* (as one row of the resulting .csv file). The algorithm by which the values *psi(l)* and *#Psi(l)* are determined is as follows.

## Algorithm for Determining the Maxima of *R(n)*:

Initialisation: Set number_of_maximal_representations:=0.Then for every *l* from *l_min <= l <= l_max* do:

1. Calculate R(n) for every n from the range *B_{l-1} <= n <= B_l-1* and store the values into memory in the array Rn_array.

2. Find the maximal value in Rn_array and for every n such that *R(n)* is maximal (i.e. *R(n)=psi(l)*), increment by one the counter number_of_maximal_representations and store its greedy representation *<n>_B* into the list representation_list (but only if it contains less than *M* representations).

3. Write the triplet (max(Rn_array), number_of_maximal_representations, representation_list) into the output .csv file (I.e. write the triplet *psi(l)*, *#Psi(l)*, *Psi(l)*).

4. Empty Rn_array, representation_list and set number_of_maximal_representations equal to zero.

The program also creates a second file recording the time needed for calculation and memory usage for each *l*.

Lastly, in both programs, whenever the user is asked for input, they can enter a percent sign (%) to reset the basis and enter new coefficients, and then running the calculation of *R(n)* in a different *B*-system.

## Note on Systems not Possessing the (F) Property: 

When entering the basis coefficients, any sequence of integers separated by commas is a valid input, thus the program is not limited only to (F) systems. However, the correctness of the values of *R(n)* is not guaranteed for non-(F) systems, because we do not a priori know the size of the canonical alphabet. In the case that a non-(F) system is entered, the program sets the largest digit of the canonical alphabet to the recurrence coefficient that is maximal. For example, in the *(1,5)*-system, it would set the canonical alphabet to *{0,1,2,3,4,5}*, which is too large, as in this system greedy representations contain only digits *{0,1,2,3}*. Hence the resultant values of *R(n)* will be incorrect. For (F) systems however, the correctness of the calculated values of *R(n)* is always guaranteed. 
