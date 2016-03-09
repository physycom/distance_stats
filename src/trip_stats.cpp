/* Copyright 2015 - Alessandro Fabbri, Stefano Sinigardi */

/***************************************************************************
This file is part of distance_stats.

distance_stats is free software : you can redistribute it and / or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

distance_stats is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with distance_stats. If not, see <http://www.gnu.org/licenses/>.
***************************************************************************/


#define _CRT_SECURE_NO_WARNINGS

#include "jsoncons/json.hpp"

#define MAJOR_VERSION           1
#define MINOR_VERSION           0

void usage(char* progname) {
  std::cout << "Usage: " << progname << " -i [input.json] -o [output_file_basename] " << std::endl;
  exit(1);
}

int main(int argc, char** argv) {
  std::cout << "TripStats v" << MAJOR_VERSION << "." << MINOR_VERSION << std::endl;

  return 0;
}
