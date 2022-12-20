# Student Information
Name: **Ankush Madharha** \
ID: **1172859** \
Assignment Number: **#3** 

# Execution
The program uses a single makefile to compile question 1 into executable files.
## Run the `make` command:
```
make
```
Expected Output:
```
gcc  -Wall -std=c99 -pedantic q1.c -o q1
```

## Execute Q1:
```
./q1 "((2.00*(x1-1.00))+(x2/3.00))"
```

**Note:** To ensure correct execution, surround the command line arguments in quotations shown above. 

# Sample Output
### Input:
```
./q1 "((2.00*(x1-1.00))+(x2/3.00))"
```
## Q1:
### Menu:
```
Please select from the following options:
1. Preorder
2. Postorder
3. Exit
3
```

### Pre-order:
```
+   *   2.00   -   x1[0.00]   1.00   /   x2[0.00]   3.00  
```

### Postorder
```
2.00   x1[0.00]   1.00   -   *   x2[0.00]   3.00   /   +   
```