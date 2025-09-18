/**
 * File: UtilAssemblerBase.cc
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

#include "src/UtilAssemblerBase.hh"
#include <fstream>

Error UtilAssemblerBase::writeStripe(FilesL files, std::ofstream& out) const
{
     if (!files)
          return None;
     const std::string path = files->val;
     std::ifstream file(path, std::ios::binary);
     if (!file)
          return "Failed to open: " + path + "\nDiscard output";
     out << file.rdbuf();
     return writeStripe(files->next, out);
}
