We don't need to use traverse twice because we can do `wait_child(left)` after the
traverse recursion that runs `exec_terminal(right)`

Consider the pipeline:
```
cmd0 | cmd1 | cmd2
```
This results in the left-shallow tree:
```
PIPE---PIPE---cmd2
|      | 
cmd0   cmd1
```
We can traverse the tree once, forking in `exec_terminal()` as we descend the right
branch and then `wait_pid()` on the way back up after the recursion step.  This is the 
associated stack:
```
traverse(tree):
    tree->type == PIPE
    pipe(pipefd); // pipefd = {3,4}
    traverse(left, fd = {0, 3, 4}, &pid_left):
        left->type == TERMINAL
        *pid_left = exec_terminal(left->data->tokens, ...)
        return (0);
    ret = traverse(right, fd = {4, -1, -1}):
        rigth->type = PIPE
        pipe(pipefd) // pipefd = {5, 6}
        traverse(left, fd = {4, 5, 6}, &pid_left)
            left->type == TERMINAL
            *pid_left = exec_terminal(left->data->tokens, ...)
            return(0)
        traverse(right, fd = {6, 1, -1}, &pid_right)
            right->type == TERMINAL
            *pid_right = exec_termminal(right->data->tokens, ...)
            return(0)
        ret = wait_child(pid_right)
        wait_child(pid_left)
        return(ret)
    wait_child(pid_left)
    return(ret);
``` 
1. We run `traverse()` on the root of the tree, which is a PIPE node.  
2. We create a `pipe()` which gives us the file descriptors 3 and 4.
3. We run `traverse(left)` and pass it fd's 0, 3 and 4.
4. left is a TERMINAL node, so we run `exec_terminal()` on its tokens and assign the 
    resulting pid to `pid_left`
5. returning to the previous `traverse()`, we run `traverse()` again on the right branch
    of `tree`, this time passing only file descriptor 4.
6. since `right->type` is `PIPE`, we create a new `pipe()` with file descriptors 5 and 6
7. we run traverse on `left` with fds 4, 5 and 6.
8. since `left->type` is TERMINAL, we run `exec_terminal()` on its tokens and assign the
    resulting pid to `pid_left`
9. returning to the depth-2 traverse, we run `traverse()` again on the right branch, 
    passing it file descriptors 6 and 1.
10. since `pid_right` (cmd2) was assigned, run `wait_child()` on it and set `ret` to the 
    return value
11. run `wait_child()` on `pid_left` (cmd1) and return `ret` to the previous level.
12. run `wait_child()` on `pid_left` (cmd0) and return `ret` to the first `traverse`
    invocation.

