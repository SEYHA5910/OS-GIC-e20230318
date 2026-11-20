# Lab 3 — Wildcards, Hard Links and Symbolic Links: individual report

- Student name/ID: Khim Vy Seyha / e20230318
- Ubuntu username: gic-khim-vyseyha
- My values (`oslab values lab3`): name = silver, tag = old, v1 = 894, v2 = 908
- Files: `evidence/wildcards.txt`, `evidence/links.txt`, `evidence/replacement.txt`, `evidence/checkpoint.txt`, `evidence/plus.txt`, `evidence/challenge.txt`

## What I Saw

| State | What `cat` printed for the hard link | What `cat` printed for the symbolic link | What `readlink` printed |
| :--- | :--- | :--- | :--- |
| Before the rename | `version 894` | `version 894` | `silver.txt` |
| After the rename, before the repair | `version 894` | `cat: silver-soft.txt: No such file or directory` | `silver.txt` |
| After a new file took the old name | `version 894` | `version 908` | `silver.txt` |
| After my repair | `version 894` | `version 894` | `silver-old.txt` |

**My repair (the two commands I wrote):**
```bash
rm -- silver-soft.txt
ln -s silver-old.txt silver-soft.txt
```

## Explanation

A symbolic link only stores a text path (`silver.txt`), so when a new file was created with that name, `silver-soft.txt` resolved to the new file and printed `version 908`. In contrast, a hard link points directly to the file's inode on disk rather than its name. In `evidence/replacement.txt`, `silver-hard.txt` and `silver-old.txt` still share the original inode `19409033`, while the newly created `silver.txt` has a completely different inode `19409049`. Because `silver-hard.txt` remained attached to `19409033`, it continued to read `version 894` even after the rename and replacement.

## What My Tests Do Not Prove

Running `cat` only proves that two file names currently contain the same text; it does not prove they are the same underlying file rather than two separate copies. Only `ls -li` proves that `silver-hard.txt` and `silver-old.txt` share the exact same inode number and link count.

## Plus / Challenge

- **Plus:** I expected `notes/silver[!1]*` to only match `silver2.txt`, but it also matched `silver.log` because the dot (`.`) is a character that is not `1`.
- **Challenge:** Starting with three hard links (`one.txt`, `two.txt`, `three.txt`), the link count dropped from `3` to `2` after removing `one.txt`, and from `2` to `1` after removing `two.txt`. Deleting `one.txt` did not delete the data; the data is only removed from disk when all names pointing to the inode are deleted and the link count reaches `0`.

## AI Note (optional)

An AI tool suggested using `ln -s silver-old.txt silver-soft.txt` as a relative target so the link would not break if the folder moved; I verified it by running `readlink silver-soft.txt`, `cat silver-soft.txt`, and `oslab check lab3`.
