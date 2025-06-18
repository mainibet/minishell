# Test Cases and Results

## 1. Overview
Brief summary of testing goals and scope.

---

## 2. Test Environment
- OS:
- Compiler:
- Tools used:
  - Valgrind (memory leaks)
  - Norminette (code style)
  - Other testing tools

---

## 3. Test Cases

| ID  | Command / Feature                   | Expected Result                      | Actual Result   | Status      | Notes                  |
|------|-----------------------------------|------------------------------------|-----------------|-------------|------------------------|
| TC1  | echo "Hello $USER"                 | Expands $USER variable              |                 | Not tested / Passed / Failed |                        |
| TC2  | cd .. && pwd                      | Changes directory and prints path  |                 |             |                        |
| TC3  | ls \| grep minishell              | Pipes output correctly              |                 |             |                        |
| TC4  | cat << EOF                       | Reads until EOF delimiter           |                 |             |                        |

---

## 4. Norminette Compliance
- Date of last run:
- Number of files checked:
- Errors found:
- Warnings found:
- Status (Passed / Failed):

## 5. Bug Reports
- Bug ID:
- Description:
- Steps to reproduce:
- Status (fixed / pending):

---

## 6. Notes & Recommendations
- Additional observations, fixes needed, or improvements.

---

## 7. Testing Checklist
- [ ] All built-in commands tested
- [ ] Redirections tested
- [ ] Signal handling tested
- [ ] Memory leak check passed
