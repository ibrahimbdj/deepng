## Project status

This project is unfinished. I'm currently working on other projects, so it's on hold.

As it stands, it contains the basic features, themselves limited to the minimum of what they should provide.

The code is also neither optimised nor necessarily clean.

## Future plans

At some point, I'd like to:

- add features;
- go deeper on the existing ones;
- change the output format so the command integrates cleanly into scripts;
- add my own touch, so the utility has a reason to be used over another;
- explore what is rarely or poorly done in existing utilities and provide a solution.

## Feature status

| Command | Description | Status |
|---|---|---|
| `ispng <file>` | Indicates whether it is a PNG | Done |
| `list <file>` | Lists every chunk: offset, type, size, critical/ancillary class | minimum |
| `cat <file>` | Prints the raw file as different format | minimum |
| `integrity <file>` | Check PNG integrity via crc | minimum |
| `info <file>` | Provides a description of the file from various perspectives in plain text, (for example: width, heigth, compression methode, interlace, etc...)| minimum |
| `delmd <src> <dest>` | Writes a copy removing the anciliary chunks and reports the number of bytes removed | minimum |
|  `inject <type> [source]` (may change)| Inserts a chunk of the given type, reads its content from source or stdin if omitted | Not implemented |

New Features incoming...
