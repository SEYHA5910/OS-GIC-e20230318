# Lab 2 — Linux Navigation and File Management: individual report

- **Student name/ID:** Khim Vyseyha / e20230318
- **Ubuntu username:** khim_vyseyha@DESKTOP-OLR1NFL:~$
- **My values (oslab values lab2):** file1 = [vendor list.txt], file2 = [payroll may.txt], owner = [Sales], reader = [Marketing], third = HR
- **Files:** evidence/paths.txt, evidence/tree.txt, evidence/checkpoint.txt, evidence/plus.txt, evidence/challenge.txt

## What I Did

| Step | My command | Current folder when I ran it |
|---|---|---|
| Move the first file (relative destination) | `mv incoming/[file1] reports/` | `lab2` (or your owner department) |
| Move the second file (absolute paths) | `mv /home/gic-khim-vyseyha/.../incoming/[file2] /home/gic-khim-vyseyha/.../reports/` | `lab2` |
| Read the report from my reader department (relative path) | `cat ../[owner]/reports/[file1]` | `TechCorp/[reader]` |
| The path error I made on purpose, and its message | `cat [file1]` -> "cat: [file1]: No such file or directory" | `TechCorp/[reader]` |
| The relative path that fixed it | `cat ../[owner]/reports/[file1]` | `TechCorp/[reader]` |

## Explanation

A correct file name will still give "No such file or directory" because the Linux shell defaults to looking only inside your current working directory. If the file exists in another folder, you must provide a relative or absolute path so the shell knows how to navigate there. 

The `mv` command moves a file to a new location, deleting it from the original spot (like taking a physical document out of `incoming/` and putting it into `reports/`). The `cp` command duplicates the file, leaving the original intact while creating a clone in the new destination (like copying the text files into `TechCorp/HR/`).

## What My Tests Do Not Prove

The `ls` command only proves that a file exists and has the correct name. It does not prove that the file contains the correct text or data, which is why commands like `cmp` are necessary to verify the actual contents.

## Plus / Challenge

**Plus:** 
Command: `mv "../a/b/lost file.txt" found.txt`
Command: `cp "../../c/found.txt" "../../../TechCorp/HR/"`
Number of `..`: 3

**Challenge:** 
Putting quotes around `"*.txt"` fails because the shell treats the asterisk as a literal character rather than a wildcard, searching for a file explicitly named `*.txt`. The wildcard `*.txt` does not copy `reports/.hidden.txt` because wildcards intentionally ignore hidden files (files starting with a dot) by default.

## AI Note
Used Gemini as a thought partner to map out the directory tree and verify the number of `..` segments needed to build the relative path from `mess/a/b` to `TechCorp/HR`.
