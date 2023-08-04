
# Calculator
Simple Expression Calculator in C 

### Main File 
**Calculator.c** is the main file for you to run the program 

### Testing file 
**Calculator_test.c** is the same program as the main file but it was testable using C_test bash script 
You can test this program on Linux 

### How to Test
  - You need two text files for that you can give any name to them in this repo i use test and answer.txt
  - test contains two arguments first is always 1 after that give a space and second is the maths expression after the last expression give 2 at new line for exit
  - answer.txt contains the expected answers for the expressions makesure it have three decimal places like 2.000 for 2 at the end give a new line
  - You Execute the **C_test** file with three arguments first :  Calculator_test without .c second : test third : answer.txt
  - Make sure you give the executable permission to C_test if you don't type `chmod +x C_test` in the command prompt 
  - **Example** : At the Command prompt type `./C_test Calculator_test test answer.txt` it will create another file called output.txt
  - If all the test passed it will tell you All test passed
  - If any of the test failed it will give you the output for which is different between two files also which line also on the top 


