12-9-2021 12:50AM
Current issue words are being truncated when converted to list to get substrings.
Check the split string function and make sure the whole string is being split, then
check the count spaces, whatever the issue is is occurring between those two function 
calls in tandem.

12-9-2021 4:00PM
Fixed the issue with the words, for loop was stopping one pass too early. Had to handle
some formatting issues with the output. Next step is to start the swapping of values in
each instruction. I need to swap the compass of the starting instruction and change the
string. Then I need to work on figuring out a method to swap the left and right instructions
and lastly focus on swapping the addresses first.

12-12-2021 2:32AM
Still trying to figure out the swap streets. I'm thinking the best solution is just to make a
string, then call splitString and store it into an array and maybe return a pointer to that
array so that you can access it when necessary. If not maybe send the string into a stack
and try to change the value of the string as you pop out of the stack and append to a string?
Overall try to swap the address without going out range. Maybe pass the starting indexes into
the swapStreets function to avoid the for-loop?

12-14-2021 12:42AM
The instructions are not being changed when passed through the swapLR() function. Maybe pass
the array as a pointer keep the changes. Then the loop is indexing out of range figure out why
it is not adding the end of the street name, then confirm that that the size of the array is 
being calculated properly. Maybe the way the string is being passed into the function?
