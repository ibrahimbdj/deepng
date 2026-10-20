# pngfall



**In the interest of transparency: this README was written with the help of Claude AI. The code is entirely my own**

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

pngfall works on the metadata, not on the image.

## Testing

`pngtest/` contains [PngSuite](http://www.schaik.com/pngsuite/): every colour type and bit depth, interlaced variants, files split across several `IDAT` chunks, and files deliberately corrupted in the signature or in a checksum. Since the tool is pointed at files that may be damaged, it is also run against every possible truncation of a valid file.

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
