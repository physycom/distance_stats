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

class trip_stats {
public:
  std::string filename;
  int record_number;
  double total_dist;
  double record_per_m, ave_dist_m, min_dist_m, max_dist_m;
  double total_time;
  double record_per_s, ave_dist_s, min_dist_s, max_dist_s;
};

jsoncons::json jsonize(trip_stats trip_s) {
  jsoncons::json trip_j;
  trip_j["filename"] = trip_s.filename;
  trip_j["record_number"] = trip_s.record_number;
  trip_j["total_dist"] = trip_s.total_dist;
  trip_j["record_per_m"] = trip_s.record_per_m;
  trip_j["ave_dist_m"] = trip_s.ave_dist_m;
  trip_j["min_dist_m"] = trip_s.min_dist_m;
  trip_j["max_dist_m"] = trip_s.max_dist_m;
  trip_j["record_per_s"] = trip_s.record_per_s;
  trip_j["ave_dist_s"] = trip_s.ave_dist_s;
  trip_j["min_dist_s"] = trip_s.min_dist_s;
  trip_j["max_dist_s"] = trip_s.max_dist_s;
  return trip_j;
}

trip_stats evaluate_stats(std::string name, jsoncons::json trip) {
  trip_stats ts;
  ts.filename = name;

  // unifying different type of json in a single pointers array
  std::vector<jsoncons::json *> trip_;
  if (trip.is_array()) {
    for (size_t k = 0; k < trip.size(); k++) {
      trip_.push_back(&(trip[k]));
    }
  }
  else if (trip.is_object()) {
    for (auto it = trip.begin_members(); it != trip.end_members(); it++) {
      trip_.push_back(&(it->value()));
    }
  }
  else {
    std::cout << "BAD json, not array nor object...?!?!" << std::endl;
    exit(321);
  }

  // evaluating stats
  ts.record_number = (int) trip_.size();
  for (auto t : trip_) {

  }

  return ts;
}


///////////////////////////////////////////////////////////////////////////////////////////
void usage(char* progname) {
  std::cout << "Usage: " << progname << " input_1.json ... input_n.json " << std::endl;
  exit(1);
}

int main(int argc, char** argv) {
  std::cout << "TripStats v" << MAJOR_VERSION << "." << MINOR_VERSION << std::endl;

  // parsing command line
  std::vector<std::string> input;
  if (argc > 2) {
    for (int i = 1; i < argc; i++) {
      input.push_back(argv[i]);
    }
  }
  else {
    std::cout << "No flags specified. Read usage and relaunch properly." << std::endl;
    usage(argv[0]);
  }
  std::cout << "File(s) to parse : ";
  if (input.size() > 0) for (auto file : input) std::cout << file << "  ";
  std::cout << std::endl;

  // sanity check on input files
  std::ifstream ifile;
  size_t cell_width = 0;
  for (auto file : input) {
    cell_width = (file.size() > cell_width) ? file.size() : cell_width;
    ifile.open(file);
    if (!ifile) {
      std::cout << "Unable to open input file " << file << ". Quitting..." << std::endl;
    }
    else ifile.close();
  }
  cell_width += 3;

  // parsing input json
  std::vector<trip_stats> trips;
  for (auto file : input) {
    jsoncons::json trip_json;
    try {
      trip_json = jsoncons::json::parse_file(file);
    }
    catch (std::exception &e) {
      std::cout << e.what() << std::endl;
      continue;
    }

    trips.push_back(evaluate_stats(file, trip_json));
  }

  // writing results to txt and json
  std::ofstream out("trip_stats.txt");
  out << std::setw(cell_width) << "STATS";            for (auto t : trips) out << std::setw(cell_width) << t.filename;     out << std::endl;
  out << std::setw(cell_width) << "record number";   for (auto t : trips) out << std::setw(cell_width) << t.record_number; out << std::endl;
  out << std::setw(cell_width) << "total covered distance";   for (auto t : trips) out << std::setw(cell_width) << t.total_dist; out << std::endl;
  out << std::setw(cell_width) << "record per meter";   for (auto t : trips) out << std::setw(cell_width) << t.record_per_m; out << std::endl;
  out << std::setw(cell_width) << "average dist (m)"; for (auto t : trips) out << std::setw(cell_width) << t.ave_dist_m;   out << std::endl;
  out << std::setw(cell_width) << "min dist (m)";     for (auto t : trips) out << std::setw(cell_width) << t.min_dist_m;   out << std::endl;
  out << std::setw(cell_width) << "max dist (m)";     for (auto t : trips) out << std::setw(cell_width) << t.max_dist_m;   out << std::endl;
  out << std::setw(cell_width) << "total time elapsed"; for (auto t : trips) out << std::setw(cell_width) << t.total_time; out << std::endl;
  out << std::setw(cell_width) << "record per second"; for (auto t : trips) out << std::setw(cell_width) << t.record_per_s; out << std::endl;
  out << std::setw(cell_width) << "average dist (s)"; for (auto t : trips) out << std::setw(cell_width) << t.ave_dist_s;   out << std::endl;
  out << std::setw(cell_width) << "min dist (s)";     for (auto t : trips) out << std::setw(cell_width) << t.min_dist_s;   out << std::endl;
  out << std::setw(cell_width) << "max dist (s)";     for (auto t : trips) out << std::setw(cell_width) << t.max_dist_s;   out << std::endl;
  out.close();

  jsoncons::json stats(jsoncons::json::an_array);
  for (auto t : trips) stats.add(jsonize(t));
  out.open("trip_stats.json");
  out << jsoncons::pretty_print(stats) << std::endl;
  out.close();

  return 0;
}
