***The core functionality***
**Task 1**
Bitwise operations:
    step 1: Obtain the number that contains the permisions of the and (edx), and the number that shows what rooms she wishes to book (eax)
    step 2: Logic AND on bits and store it in edx. If edx == eax => she is allowed. If edx != eax => she isnt allowed

**Task 2**
*Subtask 1*
Implementation: I used a bubble sort to sort the struct array. Then I checked for every element the following characteristings in this order:      1. Admin status (if equal check 2.)
            2. Priority of the request (if equal check 3)
            3. Alphabetical order of name

*Subtask 2*
Imprementation: A loop goes trough every request, and selects the KEY, then performs this operations :
    1. If the 16th bit is 0, it is not a hacker, if its 1 goest to 2.
    2. Checks the first bit. If 0 , it is not a hacker. If 1, checks the lower 7 remaining bits
    3. If there is an odd number of bits, it is not a hacker. Otherwise check the upper 7 remaining bits
    4. If there is an odd number of bits this is a **HACKER**. If not it is a **LEGITIMATE REQUEST**

**Task 3**
I will reffer to the bytes number from 0 to 7
*Encode*: In the loop one_to_seve i encoded the bytes from 1 to 7. Outside the loop i encoded the remaining byte ( the byte 0 ).
*Decode*: I Decoded the 0 byte first. Then in the loop 7 to 1 i decoded the bytes from 7 to 1, in this order.
In rest i just applied the pseudo-code.

**Task 4**
I used eax, and ebx to store the cursor. Eax will store the index of the current row, and ebx for the current collumn.
Firstly it will check wheter it can go at the (1,0) or (0,1) position in matrix.
Then the code will loop until it found an exit. In each loop it will check:
    1. If it reached the exit, it will stop, and save the data to the desired pointers
    2. If it didnt reached an exit it will mark the current possition, in order not to go trough it again
    3. Now will check wheter the current position is on the left end (COL_NULL), top edge (ROW_NUL), or inside the matrix (INSIDE)
    4. Then will check accordingly if it can go ABOVE, BELLOW, LEFT or RIGHT. Once the path was found will actualize eax and ebx accordingly
