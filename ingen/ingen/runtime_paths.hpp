/*
  This file is part of Ingen.
  Copyright 2007-2012 David Robillard <http://drobilla.net/>

  Ingen is free software: you can redistribute it and/or modify it under the
  terms of the GNU Affero General Public License as published by the Free
  Software Foundation, either version 3 of the License, or any later version.

  Ingen is distributed in the hope that it will be useful, but WITHOUT ANY
  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
  A PARTICULAR PURPOSE.  See the GNU Affero General Public License for details.

  You should have received a copy of the GNU Affero General Public License
  along with Ingen.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef INGEN_RUNTIME_PATHS_HPP
#define INGEN_RUNTIME_PATHS_HPP

#include <string>

namespace Ingen {

void set_bundle_path(const char* path);
void set_bundle_path_from_code(void* function);

std::string bundle_file_path(const std::string& name);
std::string data_file_path(const std::string& name);
std::string module_path(const std::string& name, std::string dir="");

} // namespace Ingen

#endif // INGEN_RUNTIME_PATHS_HPP
