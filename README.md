## Project status

This project is unfinished. I'm currently working on other projects, so it's on hold.

As it stands, it contains the basic features, themselves limited to the minimum of what they should provide.

## Future plans

At some point, I'd like to:

- add features;
- go deeper on the existing ones;
- change the output format so the command integrates cleanly into scripts, each command must have human output and scrip output;

## Feature status

| Command | Description | Status |
|---|---|---|
| `ispng <file>` | Indicates whether it is a PNG | **Done** |
| `list <file>` | Lists every chunk: offset, type, size, critical/ancillary class | **minimum**, missing: options, more categories in the information |
| `cat <file>` | Print the raw file as differents formats with filtering options | **minimum**, hexadecimal ok, missing: cat in differents formats, options for filtering the output |
| `integrity <file>` | Check PNG integrity | **minimum**, crc check, missing: corrupted chunks |
| `info <file>` | Provides a description of the file from various perspectives in plain text, (for example: width, heigth, compression methode, interlace, etc...)| **minimum**, ihdr ok, missing: useful informations from certain other chunks |
| `delmd <src> <dest>` | Writes a copy removing the anciliary chunks and reports the number of bytes removed | **minimum**, delete ancillary chunk ok, missing: choose what to delete or keep with greater precision |
|  `inject <type> [source]` (may change)| Inserts a chunk of the given type, reads its content from source or stdin if omitted | **Not implemented** |

New Features incoming...
