# Lab 4 — Redirection and Pipelines: individual report

- **Name / student ID:** Khim Vyseyha / [e20230318]
- **Linux account:** khim_vyseyha@DESKTOP-OLR1NFL:~$
- **Personal status and other word:** `done` and `clear`

## Commands I Used

Save your actual commands, one per line, in evidence/commands.txt. Do not execute that file as a script.

| Stage | My command | Input received | Output produced |
| :--- | :--- | :--- | :--- |
| Select exact first field | `grep '^done,' data/events.csv > evidence/selected.txt` | `data/events.csv` | `evidence/selected.txt` |
| Extract field 2 | `cut -d, -f2 < evidence/selected.txt > evidence/fields.txt` | `evidence/selected.txt` | `evidence/fields.txt` |
| Count selected records | `wc -l < evidence/selected.txt > evidence/normal.txt` | `evidence/selected.txt` | `evidence/normal.txt` |
| Save stdout and stderr separately | `cat missing.csv > evidence/stdout.txt 2> evidence/stderr.txt` | `missing.csv` | `evidence/stdout.txt`, `evidence/stderr.txt` |
| Save exit status immediately | `echo $? > evidence/status.txt` | `$?` (internal variable) | `evidence/status.txt` |

## What My Tests Show

| Test | Expected | Observed | Mistake it would catch |
| :--- | :--- | :--- | :--- |
| Original records | 3 exact matches | 3 | Extracting values instead of exact rows |
| No match | 0 records | 0 | Failing to filter out the status word completely |
| One added match plus a look-alike | 4 records | 4 | Prefix matching without the comma (matching `done-late`) |
| Missing input | empty stdout, diagnostic on stderr, nonzero status | empty stdout, diagnostic on stderr, nonzero status | Testing pipeline status instead of direct command status |

## Explain the Commands

1. **Why does the comma in `grep '^WORD,'` matter?** 
   It ensures we match only the exact status word (e.g., `done,`). Without the comma, the regular expression would accidentally match look-alike words that share the same prefix (like `done-late,`).
2. **Why does `wc -l` count records here, rather than add quantities?**
   `wc -l` strictly counts the number of newline characters (physical lines) in a text stream. It has no built-in logic to parse the text, find the second column, and perform mathematical addition on those numerical values.
3. **Where did you use `>>`, and what would `>` do instead?**
   I used `>>` when saving the `changed.csv` count to `evidence/edge.txt` so it appended below the `no-match.csv` count. If I had used `>`, it would have completely overwritten the file, erasing the previous result of `0`.
4. **What does `tee` do that `>` alone does not?**
   `tee` takes standard input and writes it simultaneously to a file *and* to standard output (so it prints on the screen or pipes to the next command). `>` redirects output exclusively to a file, leaving nothing to print or pipe.
5. **Why must `echo $?` immediately follow the command being checked?**
   The `$?` variable stores the exit status of the *most recently executed* command. If you run any other command before checking it, `$?` will be overwritten with the exit status of that intervening command.
6. **Why can `cat missing-file | wc -l` report pipeline status 0?**
   In a standard bash pipeline, the exit status of the entire pipeline is determined by the *last* command executed. Even if `cat` fails (returning a nonzero status), `wc -l` succeeds in counting zero lines and gracefully exits with a status of 0.

## Limits and Evidence
These commands assume simple comma-separated text without quoted commas. Saved results describe one run; they do not prove every future input works or who typed a command.
