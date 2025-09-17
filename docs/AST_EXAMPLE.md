**Execution Tree Example**

Example input:

`cat < input.txt | grep "foo" > out1.txt | wc -l >> out2.txt | echo -n hello`

**AST Structure:**

                         [PIPE]
                        /     \
                   [PIPE]     [COMMAND]
                  /     \            |
             [PIPE]     [COMMAND]   argv=["echo","-n","hello"]
            /     \          |
     [COMMAND]   [COMMAND]   argv=["wc","-l"]
        |            |       redirs: [ >> out2.txt ]
argv=["cat"]   argv=["grep","foo"]
redirs: [ < input.txt ]   redirs: [ > out1.txt ]
