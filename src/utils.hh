/**
 * File: utils.hh
 * Copyright (C) 2025 Tyler Triplett
 * License: GNU GPL 3.0 or later <https://www.gnu.org/licenses/gpl-3.0.html>
 *
 * This is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

#ifndef UTILS_HH
#define UTILS_HH

namespace util {

constexpr auto banner =
R"( _______| |__  _ __ __ _
|_  / _ \ '_ \| '__/ _` |
 / /  __/ |_) | | | (_| |
/___\___|_.__/|_|  \__,_|)";

constexpr auto help =
R"(Usage: zebra [mode option] [required option] [required option]
[optional option] ... [optional flags] ...

Options or flags may be provided in any order. All i/o options may be relative or
absolute paths. Duplicate options will result in an error, flags will not.

Modes:
-S, --Stripe <Stripe>
    Stripes file into pieces
Required :
    -i, --input <input file>
    -o, --output <ouput directory>
Optional :
   OR:
        -s, --size  <stripe size>
            Stripe size in bytes or number (can be floating point) followed by
                suffix (kb, mb, gb)
            Examples:
                -s 100000
                -s 30mb
                -s 55.35mb
        -p, --parts <number of parts>
            Number of parts a file is striped into
            Example:
                -p 10
    -n, --name <name suffix>
        Part name suffix. Parts will be named `NAME SUFFIX`_`NUMBER`.stripe
        Example:
            -n large_file  | large_file_`number`.stripe
    -e, --extension <ext>
        The extension name, default "stripe" `NUMBER`.`EXT`
        Example:
            -e part | 001.part
            -e txt  | 001.txt
Flag(s)
    -np, --no-padding <no padding>
        This will cause stripes to not have padding on their numbers.
        Example:
            "003.stripe"       | "3.stripe"
            "piece_003.stripe" | "piece_3.stripe"
    -q, --quiet <quiet>
        This will silence normal outputs, warnings will still print.
    -ne, --no-extension <no extension>
        No extension will be added. This will override <ext>

-A, --Assemble <Assemble>
    Assemble, assembles pieces back to a single file
Required (1) :
Note: Assembles all ".stripe" files in directory in lexicographical order:
    -i, --intput  <input directory>
    -o, --output  <output file>
Optional :
    -e, --extension <ext>
        The extension that is searched for.
        Example:
            -e ext | "001.`ext`"
    -n, --name  <name suffix>
        The name suffix that gets used
        Example:
            -n name | "`name`_001.stripe" will match, but
                      "`other_name`_001.stripe" will not.
Flag(s) :
    -q, --quiet <quiet>
        This will silence normal outputs, warnings will still print.
    -ne, --no-extension <no extension>
        This will exclusively use files without an extension
    -nn, --no-name <no name>
        This will exclusively use files without a prefix
Required (2) :
Note: Assembles all files in the order they provided:
    -i, --input <input files>
        Example:
            -i file.txt otherfile.txt ...
    -o, --output <output file>
Flag(s) :
    -q, --quiet <quiet>
        This will silence normal outputs, warnings will still print.

Other:
    -h, --help <help>
        Help menu)";

} /// util

#endif /// UTILS_HH
