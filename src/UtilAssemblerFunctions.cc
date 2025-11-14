/**
 * File: UtilAssemblerFunctions.cc
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

#include "src/UtilAssemblerFunctions.hh"
#include <fstream>

Maybe<std::streamsize> UtilAssemblerFunctions::writeStripe(FilesL files,
    std::ofstream& out) const
{
        std::streamsize bytes = 0;
        while (files) {
                const std::string& path = files->val;
                std::ifstream file(path, std::ios::binary);
                if (!file) {
                        const auto msg = "Failed to open: " + path +
                            "\nDiscard output";
                        return makeBad<std::streamsize>(msg);
                }
                const auto size = fileSize(file);
                bytes += chunk(file, out, size);
                files = files->next;
        }
        return bytes;
}
