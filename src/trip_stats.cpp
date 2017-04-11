/* Copyright 2015-2017 - Alessandro Fabbri, Stefano Sinigardi */

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


#include "jsoncons/json.hpp"

#include "class_stats.hpp"

#define MAJOR_VERSION             1
#define MINOR_VERSION             1

#define GEODESIC_DEG_TO_M         111070.4                             /* conversion [deg] -> [meter]  */
#define RAD_TO_DEG                57.2957795131                        /* 180/pi                       */

#define CAUSE_IGNITION_ON         0x88

void evaluate_stats(std::vector<Trip_Stats> &trip_stats, std::string name, jsoncons::json trip) {

  // unifying different type of json in a single pointers array
  std::vector<jsoncons::json *> trip_ptr;
  std::vector<std::vector<jsoncons::json *>> trips_ptr;
  int old_counter = 0;
  if (trip.is_array()) {
    for (size_t k = 0; k < trip.size(); k++) {
      if (
        (k > 0 && trip[k].has_member("enabling") && trip[k]["enabling"].as<std::string>() == "ignition_on")
        ||
        (k > 0 && trip[k].has_member("cause") && trip[k]["cause"].as<int>() == CAUSE_IGNITION_ON)
        ||
        (trip[k].has_member("global_index") && trip[k]["global_index"].as<int>() < old_counter)
        ) {
        old_counter = 0;
        trips_ptr.push_back(trip_ptr);
        trip_ptr.clear();
      }
      if (trip[k].has_member("global_index")) old_counter = trip[k]["global_index"].as<int>();
      trip_ptr.push_back(&(trip[k]));
    }
    trips_ptr.push_back(trip_ptr);
  }
  else if (trip.is_object()) {
    for (auto it = trip.begin_members(); it != trip.end_members(); it++) {
      if (
        (it != trip.begin_members() && it->value().has_member("enabling") && it->value()["enabling"].as<std::string>() == "ignition_on")
        ||
        (it != trip.begin_members() && it->value().has_member("cause") && it->value()["cause"].as<int>() == CAUSE_IGNITION_ON)
        ||
        (it->value().has_member("global_index") && it->value()["global_index"].as<int>() < old_counter)
        ) {
        old_counter = 0;
        trips_ptr.push_back(trip_ptr);
        trip_ptr.clear();
      }
      if (it->value().has_member("global_index")) old_counter = it->value()["global_index"].as<int>();
      trip_ptr.push_back(&(it->value()));
    }
    trips_ptr.push_back(trip_ptr);
  }
  else {
    std::cout << "BAD json, not array nor object...?!?!" << std::endl;
    exit(321);
  }
  // Safety loop to exclude 1-point trips
  size_t index = 0;
  while (index < trips_ptr.size()) {
    if (trips_ptr[index].size() < 2) {
      trips_ptr.erase(trips_ptr.begin() + index);
    }
    else index++;
  }


  // evaluating stats
  for (size_t i = 0; i < trips_ptr.size(); i++) {
    Trip_Stats ts;
    ts.trip_name = name.substr(0, name.size() - 5) + "_" + std::to_string(i);
    ts.record_cnt = (int)trips_ptr[i].size();
    ts.total_time = trips_ptr[i].back()->at("timestamp").as<int>() - trips_ptr[i][0]->at("timestamp").as<int>();
    ts.total_dist = 0;
    ts.min_dist_m = 10000.0;
    ts.max_dist_m = 0.0;
    ts.min_dt_s = 10000.0;
    ts.max_dt_s = 0.0;
    ts.others_cnt = ts.rdp_cnt = ts.restored_cnt = 0;
    for (size_t j = 1; j < trips_ptr[i].size(); j++) {
      // distances
      double dlat, dlon, ds;
      dlat = trips_ptr[i][j]->at("lat").as<double>() - trips_ptr[i][j - 1]->at("lat").as<double>();
      dlon = trips_ptr[i][j]->at("lon").as<double>() - trips_ptr[i][j - 1]->at("lon").as<double>();
      dlon = GEODESIC_DEG_TO_M*cos(dlat / RAD_TO_DEG)*dlon;
      dlat = GEODESIC_DEG_TO_M*dlat;
      ds = sqrt(dlat*dlat + dlon*dlon);
      if (ds < ts.min_dist_m) ts.min_dist_m = ds;
      if (ds > ts.max_dist_m) ts.max_dist_m = ds;
      ts.total_dist += ds;

      // times
      int dt;
      dt = trips_ptr[i][j]->at("timestamp").as<int>() - trips_ptr[i][j - 1]->at("timestamp").as<int>();
      if (dt < ts.min_dt_s) ts.min_dt_s = dt;
      if (dt > ts.max_dt_s) ts.max_dt_s = dt;

      // cause
      if (trips_ptr[i][j]->has_member("enabling")) {
        if (trips_ptr[i][j]->at("enabling").as<std::string>() == "rdp_engine")
          ts.rdp_cnt++;
        else if (trips_ptr[i][j]->at("enabling").as<std::string>() == "smart_restore")
          ts.restored_cnt++;
        else
          ts.others_cnt++;
      }
    }
    ts.ave_dist_m = ts.total_dist / (double)trips_ptr[i].size();
    ts.record_per_10km = 1e4 / ts.ave_dist_m;
    ts.ave_dt_s = (double)(ts.total_time / trips_ptr[i].size());
    ts.record_per_h = 3600. / ts.ave_dt_s;

    trip_stats.push_back(ts);
  }
}


void usage(char* progname) {
  std::cout << "Usage: " << progname << " input_1.json ... input_n.json " << std::endl;
  exit(1);
}

int main(int argc, char** argv) {
  std::cout << "TripStats v" << MAJOR_VERSION << "." << MINOR_VERSION << std::endl;

  // parsing command line
  std::vector<std::string> input;
  if (argc > 1) {
    for (int i = 1; i < argc; i++) {
      input.push_back(argv[i]);
    }
  }
  else {
    std::cout << "No arguments supplied. Read usage and relaunch properly." << std::endl;
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
  if (cell_width < 27) cell_width = 27;

  // parsing input json
  std::vector<Trip_Stats> trip_stats;
  for (auto file : input) {
    jsoncons::json trip_json;
    try {
      trip_json = jsoncons::json::parse_file(file);
    }
    catch (std::exception &e) {
      std::cout << e.what() << std::endl;
      continue;
    }
    evaluate_stats(trip_stats, file, trip_json);
  }

  // writing results to txt and json
  std::stringstream out_basename;
  out_basename << input[0].substr(0, input[0].size() - 5);
  std::ofstream out(out_basename.str() + "_TRIPSTATS.txt");
  out << std::setw(cell_width) << "STATS";                      for (auto t : trip_stats) out << std::setw(cell_width) << t.trip_name;       out << std::endl;
  out << std::setw(cell_width) << "record number";              for (auto t : trip_stats) out << std::setw(cell_width) << t.record_cnt;      out << std::endl;
  out << std::setw(cell_width) << "rdp enabled number";         for (auto t : trip_stats) out << std::setw(cell_width) << t.rdp_cnt;         out << std::endl;
  out << std::setw(cell_width) << "restored number";            for (auto t : trip_stats) out << std::setw(cell_width) << t.restored_cnt;    out << std::endl;
  out << std::setw(cell_width) << "others number";              for (auto t : trip_stats) out << std::setw(cell_width) << t.others_cnt;      out << std::endl;
  out << std::setw(cell_width) << "total covered distance (m)"; for (auto t : trip_stats) out << std::setw(cell_width) << t.total_dist;      out << std::endl;
  out << std::setw(cell_width) << "total time elapsed (s)";     for (auto t : trip_stats) out << std::setw(cell_width) << t.total_time;      out << std::endl;
  out << std::setw(cell_width) << "record per 10 km";           for (auto t : trip_stats) out << std::setw(cell_width) << t.record_per_10km; out << std::endl;
  out << std::setw(cell_width) << "ave dist (m)";               for (auto t : trip_stats) out << std::setw(cell_width) << t.ave_dist_m;      out << std::endl;
  out << std::setw(cell_width) << "min dist (m)";               for (auto t : trip_stats) out << std::setw(cell_width) << t.min_dist_m;      out << std::endl;
  out << std::setw(cell_width) << "max dist (m)";               for (auto t : trip_stats) out << std::setw(cell_width) << t.max_dist_m;      out << std::endl;
  out << std::setw(cell_width) << "record per hour";            for (auto t : trip_stats) out << std::setw(cell_width) << t.record_per_h;    out << std::endl;
  out << std::setw(cell_width) << "ave dt (s)";                 for (auto t : trip_stats) out << std::setw(cell_width) << t.ave_dt_s;        out << std::endl;
  out << std::setw(cell_width) << "min dt (s)";                 for (auto t : trip_stats) out << std::setw(cell_width) << t.min_dt_s;        out << std::endl;
  out << std::setw(cell_width) << "max dt (s)";                 for (auto t : trip_stats) out << std::setw(cell_width) << t.max_dt_s;        out << std::endl;
  out.close();

  jsoncons::json stats = jsoncons::json::array();
  for (auto t : trip_stats) stats.add(jsonize(t));
  out.open(out_basename.str() + "_TRIPSTATS.json");
  out << jsoncons::pretty_print(stats) << std::endl;
  out.close();

  return 0;
}
