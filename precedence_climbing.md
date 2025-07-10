Precedence climbing is a terse way to create a parsing tree from a command line.
It works with a minimum precedence, initially zero:
1. parsing the first command as the left hand side
2. looking at the first operator
3. for every subsequent operator with equal or greater precedence than the minimum precedence:
   a. recurse on the RHS of that operator, setting the new minimum precedence to the precedence of the latter operator if it is right-associative or one greater if it is left-associative.
   b. the result of the recursion is now the right hand side of this node.
   c. this node now becomes the left hand side until the loop ends.
4. return the left hand side.

For minishell, we choose to make the pipe (`|`) right associative, and the other operators (`&&`, `||`, `;`) left-associative.  This has the advantage of
- closing pipe file descriptors earlier in the execution, thus reducing the number of open files, and
- preserving the order of execution of unpiped phrases to maintain the correct order of side effects.
   
