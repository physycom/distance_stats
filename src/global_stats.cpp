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


#include "jsoncons/json.hpp"

#include "class_stats.hpp"

#define MAJOR_VERSION             1
#define MINOR_VERSION             1


/////////////////////////////////// MAIN ////////////////////////////////////////
void usage(char* progname) {
  std::cout << "Usage: " << progname << " input_1.json ... input_n.json " << std::endl;
  exit(1);
}

int main(int argc, char** argv) {
  std::cout << "GlobalStats v" << MAJOR_VERSION << "." << MINOR_VERSION << std::endl;

  // parsing command line
  std::vector<std::string> input_files;
  if (argc > 1) {
    for (int i = 1; i < argc; i++) {
      input_files.push_back(argv[i]);
    }
  }
  else {
    std::cout << "No arguments supplied. Read usage and relaunch properly." << std::endl;
    usage(argv[0]);
  }

  std::vector<jsoncons::json> stats;
  for (auto inj : input_files) {
    jsoncons::json stat;
    try {
      stat = jsoncons::json::parse_file(inj);
    }
    catch (std::exception &e) {
      std::cout << e.what() << std::endl;
      exit(321);
    }
    stats.push_back(stat);
  }


  int n = 0;
  Trip_Stats ave_stats;
  memset(&ave_stats, 0, sizeof(Trip_Stats));
  ave_stats.trip_name = "**** Global Average Statistics ****";
  try {
    for (auto stat : stats) {
      for (size_t i = 0; i < stat.size(); i++) {
        n++;
        ave_stats.record_cnt += stat[i]["counter_records"].as<int>();
        ave_stats.rdp_cnt += stat[i]["counter_rdp"].as<int>();
        ave_stats.restored_cnt += stat[i]["counter_restored"].as<int>();
        ave_stats.others_cnt += stat[i]["counter_others"].as<int>();
        ave_stats.total_dist += stat[i]["total_dist_m"].as<double>();
        ave_stats.ave_dist_m += stat[i]["ave_dist_m"].as<double>();
        ave_stats.max_dist_m += stat[i]["max_dist_m"].as<double>();
        ave_stats.min_dist_m += stat[i]["min_dist_m"].as<double>();
        ave_stats.record_per_10km += stat[i]["record_per_10km"].as<int>();
        ave_stats.total_time += stat[i]["total_time_s"].as<int>();
        ave_stats.ave_dt_s += stat[i]["ave_dt_s"].as<int>();
        ave_stats.max_dt_s += stat[i]["max_dt_s"].as<int>();
        ave_stats.min_dt_s += stat[i]["min_dt_s"].as<int>();
        ave_stats.record_per_h += stat[i]["record_per_h"].as<int>();

        std::cout << n << "   " << i << "  " << stat[i]["trip_name"].as<std::string>() << "    " << ave_stats.record_per_10km << std::endl;
      }
    }
  }
  catch (std::exception &e) {
    std::cout << e.what() << std::endl;
  }
  ave_stats.record_cnt /= n;
  ave_stats.rdp_cnt /= n;
  ave_stats.restored_cnt /= n;
  ave_stats.others_cnt /= n;
  ave_stats.total_dist /= n;
  ave_stats.ave_dist_m /= n;
  ave_stats.max_dist_m /= n;
  ave_stats.min_dist_m /= n;
  ave_stats.record_per_10km /= n;
  ave_stats.total_time /= n;
  ave_stats.ave_dt_s /= n;
  ave_stats.max_dt_s /= n;
  ave_stats.min_dt_s /= n;
  ave_stats.record_per_h /= n;

  std::cout << "n = " << n << std::endl;

  // writing results to txt and json
  std::stringstream out_basename;
  out_basename << "GLOBAL_STATS";
  std::ofstream out(out_basename.str() + ".txt");
  int cell_width = 27;
  out << std::setw(cell_width) << "GLOBAL AVERAGE STATS";                                                                  out << std::endl;
  out << std::setw(cell_width) << "record number";              out << std::setw(cell_width) << ave_stats.record_cnt;      out << std::endl;
  out << std::setw(cell_width) << "rdp enabled number";         out << std::setw(cell_width) << ave_stats.rdp_cnt;         out << std::endl;
  out << std::setw(cell_width) << "restored number";            out << std::setw(cell_width) << ave_stats.restored_cnt;    out << std::endl;
  out << std::setw(cell_width) << "others number";              out << std::setw(cell_width) << ave_stats.others_cnt;      out << std::endl;
  out << std::setw(cell_width) << "total covered distance (m)"; out << std::setw(cell_width) << ave_stats.total_dist;      out << std::endl;
  out << std::setw(cell_width) << "total time elapsed (s)";     out << std::setw(cell_width) << ave_stats.total_time;      out << std::endl;
  out << std::setw(cell_width) << "record per 10 km";           out << std::setw(cell_width) << ave_stats.record_per_10km; out << std::endl;
  out << std::setw(cell_width) << "ave dist (m)";               out << std::setw(cell_width) << ave_stats.ave_dist_m;      out << std::endl;
  out << std::setw(cell_width) << "min dist (m)";               out << std::setw(cell_width) << ave_stats.min_dist_m;      out << std::endl;
  out << std::setw(cell_width) << "max dist (m)";               out << std::setw(cell_width) << ave_stats.max_dist_m;      out << std::endl;
  out << std::setw(cell_width) << "record per hour";            out << std::setw(cell_width) << ave_stats.record_per_h;    out << std::endl;
  out << std::setw(cell_width) << "ave dt (s)";                 out << std::setw(cell_width) << ave_stats.ave_dt_s;        out << std::endl;
  out << std::setw(cell_width) << "min dt (s)";                 out << std::setw(cell_width) << ave_stats.min_dt_s;        out << std::endl;
  out << std::setw(cell_width) << "max dt (s)";                 out << std::setw(cell_width) << ave_stats.max_dt_s;        out << std::endl;
  out.close();

  jsoncons::json ave_json = jsonize(ave_stats);
  out.open(out_basename.str() + ".json");
  out << jsoncons::pretty_print(ave_json) << std::endl;
  out.close();


  return 0;
}
