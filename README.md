# pngfall

> A command-line PNG structure inspector written in C, with no external dependencies.

pngfall reads the *skeleton* of a PNG file: it lists the chunks a file is made of, verifies their checksums, decodes the image header, and writes back a copy without the metadata. It never decompresses pixel data.

Built as a learning project to practise binary parsing, manual memory management and defensive handling of untrusted input in C.

## Build

```bash
make
```

## Commands

| Command | Description |
|---|---|
| `ispng <file>` | Tells whether the file is a PNG |
| `list <file>` | Lists every chunk: offset, type, size, critical/ancillary class |
| `chunk <file> <offset>` | Prints the chunk located at a given offset |
| `info <file>` | Decodes the image header: dimensions, bit depth, colour type, interlacing |
| `integrity <file>` | Verifies the signature and the CRC of every chunk |
| `cat <file>` | Prints the raw bytes in hexadecimal |
| `delmd <src> <dst>` | Writes a copy with all ancillary chunks removed |

Exit status is `0` on success, `1` on failure. Errors go to `stderr`, results to `stdout`.

## Example

```
$ pngfall list pngtest/exif2c08.png
------------------------------------------------------------
|OFFSET       |TYPE          |SIZE          |CLASS         |
------------------------------------------------------------
|8            |IHDR          |25            |Critical      |
------------------------------------------------------------
|33           |eXIf          |990           |Ancillary     |
------------------------------------------------------------
|1023         |IDAT          |753           |Critical      |
------------------------------------------------------------
|1776         |IEND          |12            |Critical      |
------------------------------------------------------------

$ pngfall delmd pngtest/exif2c08.png stripped.png   # 1788 -> 798 bytes
```

## Scope

pngfall works on the container, not on the image. Decompressing `IDAT`, line filters, pixel reconstruction, palettes and Adam7 de-interlacing are deliberately out of scope — that is a decoder's job, and leaving it out is what keeps this a parsing exercise.

`integrity` checks *structural* validity only. A file can pass it while being semantically invalid: PngSuite's `xc9n2c08.png` declares colour type 9, which does not exist, yet all of its checksums are correct.

## Testing

`pngtest/` contains [PngSuite](http://www.schaik.com/pngsuite/): every colour type and bit depth, interlaced variants, files split across several `IDAT` chunks, and files deliberately corrupted in the signature or in a checksum. Since the tool is pointed at files that may be damaged, it is also run against every possible truncation of a valid file.

## What I learned

- **Undefined behaviour does not mean "crash".** Writing `type[5]` in a five-element array went unnoticed for a long time: the memory it touched happened to be harmless. Not the kind of bug you find by re-reading code.
- **Where to check a return value.** The NULL check for a failed read existed, but sat *after* the pointer was used. Detecting and propagating an error is three quarters of the work — the value still has to be tested between the call and its use.
- **Parsers are tested on broken input.** Early tests all used healthy PNGs, so the failure paths were never taken and their bugs stayed invisible.
- **Untrusted lengths need bounds.** A chunk length read straight from the file feeds a seek, and in any future version an allocation. It is validated against the format maximum before use.

Also: CRC32 implemented bitwise from the reflected polynomial rather than copied from the specification's sample code.

## Feature status

| Command | Description | Status |
|---|---|---|
| `ispng <file>` | Indicates whether it is a PNG | **Done** |
| `list <file>` | Lists every chunk: offset, type, size, critical/ancillary class | **minimum**, missing: options, more categories in the information |
| `cat <file>` | Print the raw file as differents formats with filtering options | **minimum**, hexadecimal ok, missing: cat in differents formats, options for filtering the output |
| `integrity <file>` | Check PNG integrity | **minimum**, crc check, missing: check chunks format|
| `chunk <file> <offset>` | print chunk at `<offset>` | **minimum**, print chunk at offset ok, missing: printing using methods other than offset as well |
| `info <file>` | Provides a description of the file from various perspectives in plain text, (for example: width, heigth, compression methode, interlace, etc...)| **minimum**, ihdr ok, missing: useful informations from certain other chunks |
| `delmd <src> <dest>` | Writes a copy removing the anciliary chunks and reports the number of bytes removed | **minimum**, delete ancillary chunk ok, missing: choose what to delete or keep with greater precision |
|  `inject <type> [source]` (may change)| Inserts a chunk of the given type, reads its content from source or stdin if omitted | **Not implemented** |

New Features incoming...

## License

MIT.
