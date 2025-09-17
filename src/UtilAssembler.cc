/** 
 * File: UtilAssembler.cc
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

#include "src/UtilAssembler.hh"
#include "src/Maybe.hh"
#include "src/types.hh"
#include "src/utils.hh"
#include "src/helpers.hh"
#include <fstream>
#include <iostream>

Maybe<FilesL> UtilAssembler::stripeNames() const
{
     FilesL files;
     if (!fs::exists(in_) || !fs::is_directory(in_))
          return make_bad<FilesL>("Not a directory: " + in_);
     for (const auto& file : fs::directory_iterator(in_)) {
          if (matchExt(file) && matchName(file)) {
               const auto path = fs::path(in_) / file.path().filename();
               files = cons(path.string(), files);
          }
     }
     sortN(files);
     return files;
}

bool UtilAssembler::matchExt(const fs::directory_entry& file) const
{
     const auto& ext = file.path().extension().string();
     const auto expected = "." + in_;
     return (!useExt_ && ext.empty()) || ext == expected;
}

bool UtilAssembler::matchName(const fs::directory_entry& file) const
{
     const auto& stem = file.path().stem().string();
     return name_.empty() || name_ == stemToName(stem);
}

std::string UtilAssembler::stemToName(const std::string& stem) const
{
     auto ptr = stem.end() - 1;
     while (ptr > stem.begin() && std::isdigit(*ptr))
          ptr--;
     return std::string(stem.begin(), ptr);
}

Error UtilAssembler::setArgs(const ArgMapN& map)
{
     const auto errBase = UtilBaseSingle::setArgs(map);
     if (errBase)
          return *errBase;
     const auto errExtension = setMember(map,
               {"--extension", "-e", "extension"}, ext_);
     if (errExtension)
          return *errExtension;
     const auto errName = setMember(map, {"--name", "-n", "name"}, name_);
     if (errName)
          return *errName;
     return None;
}

Error UtilAssembler::run() const 
{
     if (!silence_)
          std::cout << util::banner << "\nAssembling\n";
     const auto maybeFiles = stripeNames();
     if (!maybeFiles)
          return maybeFiles.error();
     if (!*maybeFiles)
          return "No Pieces";
     std::ofstream outFile(out_);
     if (!outFile)
          return "Failed to open: " + out_;
     const auto err = writeStripe(*maybeFiles, outFile);
     if (err)
          return *err;
     const auto size = outFile.tellp();
     if (!silence_)
          std::cout << "Wrote " << out_ << " " << size << " bytes\n";
     return err;
}

Error UtilAssembler::setFlags(const ArgMapN& map)
{
     const auto maybeQuiet = validFlag(map, {"-q", "--quiet"});
     if (!maybeQuiet)
          return maybeQuiet.error();
     if (*maybeQuiet)
          silence_ = true;
     const auto maybeNoExt = validFlag(map, {"-ne", "--no-extension"});
     if (!maybeNoExt)
          return maybeNoExt.error();
     if (*maybeNoExt)
          useExt_ = false;
     return None;
}

std::unordered_set<std::string> UtilAssembler::validArgs() const
{
     return {
          "-i", "--input",
          "-o", "--output",
          "-e", "--extension",
          "-n", "--name",
          "-q", "--quiet",
          "-ne", "--no-extension",
     };
}
