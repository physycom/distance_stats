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


#include "class_stats.hpp"

#define MAJOR_VERSION             1
#define MINOR_VERSION             2


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
      input_files.push_back(std::string(argv[i]));
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
  Trip_Stats ave_stats, max_stats, min_stats, tot_stats;
  memset(&ave_stats, 0, sizeof(Trip_Stats));
  memset(&tot_stats, 0, sizeof(Trip_Stats));

  memset(&max_stats, 0, sizeof(Trip_Stats));
  memset(&min_stats, 0, sizeof(Trip_Stats));

  min_stats.record_cnt = std::numeric_limits<unsigned long long int>::max();
  min_stats.rdp_cnt = std::numeric_limits<unsigned long long int>::max();
  min_stats.restored_cnt = std::numeric_limits<unsigned long long int>::max();
  min_stats.others_cnt = std::numeric_limits<unsigned long long int>::max();
  min_stats.total_dist = std::numeric_limits<double>::max();
  min_stats.ave_dist_m = std::numeric_limits<double>::max();
  min_stats.max_dist_m = std::numeric_limits<double>::max();
  min_stats.min_dist_m = std::numeric_limits<double>::max();
  min_stats.record_per_10km = std::numeric_limits<int>::max();
  min_stats.total_time = std::numeric_limits<unsigned long long int>::max();
  min_stats.ave_dt_s = std::numeric_limits<int>::max();
  min_stats.max_dt_s = std::numeric_limits<int>::max();
  min_stats.min_dt_s = std::numeric_limits<int>::max();
  min_stats.record_per_h = std::numeric_limits<int>::max();

  max_stats.record_cnt = std::numeric_limits<unsigned long long int>::min();
  max_stats.rdp_cnt = std::numeric_limits<unsigned long long int>::min();
  max_stats.restored_cnt = std::numeric_limits<unsigned long long int>::min();
  max_stats.others_cnt = std::numeric_limits<unsigned long long int>::min();
  max_stats.total_dist = std::numeric_limits<double>::min();
  max_stats.ave_dist_m = std::numeric_limits<double>::min();
  max_stats.max_dist_m = std::numeric_limits<double>::min();
  max_stats.min_dist_m = std::numeric_limits<double>::min();
  max_stats.record_per_10km = std::numeric_limits<int>::min();
  max_stats.total_time = std::numeric_limits<unsigned long long int>::min();
  max_stats.ave_dt_s = std::numeric_limits<int>::min();
  max_stats.max_dt_s = std::numeric_limits<int>::min();
  max_stats.min_dt_s = std::numeric_limits<int>::min();
  max_stats.record_per_h = std::numeric_limits<int>::min();

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

        tot_stats.record_cnt += stat[i]["counter_records"].as<int>();
        tot_stats.rdp_cnt += stat[i]["counter_rdp"].as<int>();
        tot_stats.restored_cnt += stat[i]["counter_restored"].as<int>();
        tot_stats.others_cnt += stat[i]["counter_others"].as<int>();
        tot_stats.total_dist += stat[i]["total_dist_m"].as<double>();
        tot_stats.ave_dist_m += stat[i]["ave_dist_m"].as<double>();
        tot_stats.max_dist_m += stat[i]["max_dist_m"].as<double>();
        tot_stats.min_dist_m += stat[i]["min_dist_m"].as<double>();
        tot_stats.record_per_10km += stat[i]["record_per_10km"].as<int>();
        tot_stats.total_time += stat[i]["total_time_s"].as<int>();
        tot_stats.ave_dt_s += stat[i]["ave_dt_s"].as<int>();
        tot_stats.max_dt_s += stat[i]["max_dt_s"].as<int>();
        tot_stats.min_dt_s += stat[i]["min_dt_s"].as<int>();
        tot_stats.record_per_h += stat[i]["record_per_h"].as<int>();

        min_stats.record_cnt = (stat[i]["counter_records"].as<int>() < min_stats.record_cnt) ? stat[i]["counter_records"].as<int>() : min_stats.record_cnt;
        min_stats.rdp_cnt = (stat[i]["counter_rdp"].as<int>() < min_stats.rdp_cnt) ? stat[i]["counter_rdp"].as<int>() : min_stats.rdp_cnt;
        min_stats.restored_cnt = (stat[i]["counter_restored"].as<int>() < min_stats.restored_cnt) ? stat[i]["counter_restored"].as<int>() : min_stats.restored_cnt;
        min_stats.others_cnt = (stat[i]["counter_others"].as<int>() < min_stats.others_cnt) ? stat[i]["counter_others"].as<int>() : min_stats.others_cnt;
        min_stats.total_dist = (stat[i]["total_dist_m"].as<double>() < min_stats.total_dist) ? stat[i]["total_dist_m"].as<double>() : min_stats.total_dist;
        min_stats.ave_dist_m = (stat[i]["ave_dist_m"].as<double>() < min_stats.ave_dist_m) ? stat[i]["ave_dist_m"].as<double>() : min_stats.ave_dist_m;
        min_stats.max_dist_m = (stat[i]["max_dist_m"].as<double>() < min_stats.max_dist_m) ? stat[i]["max_dist_m"].as<double>() : min_stats.max_dist_m;
        min_stats.min_dist_m = (stat[i]["min_dist_m"].as<double>() < min_stats.min_dist_m) ? stat[i]["min_dist_m"].as<double>() : min_stats.min_dist_m;
        min_stats.record_per_10km = (stat[i]["record_per_10km"].as<int>() < min_stats.record_per_10km) ? stat[i]["record_per_10km"].as<int>() : min_stats.record_per_10km;
        min_stats.total_time = (stat[i]["total_time_s"].as<int>() < min_stats.total_time) ? stat[i]["total_time_s"].as<int>() : min_stats.total_time;
        min_stats.ave_dt_s = (stat[i]["ave_dt_s"].as<int>() < min_stats.ave_dt_s) ? stat[i]["ave_dt_s"].as<int>() : min_stats.ave_dt_s;
        min_stats.max_dt_s = (stat[i]["max_dt_s"].as<int>() < min_stats.max_dt_s) ? stat[i]["max_dt_s"].as<int>() : min_stats.max_dt_s;
        min_stats.min_dt_s = (stat[i]["min_dt_s"].as<int>() < min_stats.min_dt_s) ? stat[i]["min_dt_s"].as<int>() : min_stats.min_dt_s;
        min_stats.record_per_h = (stat[i]["record_per_h"].as<int>() < min_stats.record_per_h) ? stat[i]["record_per_h"].as<int>() : min_stats.record_per_h;

        max_stats.record_cnt = (stat[i]["counter_records"].as<int>() > min_stats.record_cnt) ? stat[i]["counter_records"].as<int>() : min_stats.record_cnt;
        max_stats.rdp_cnt = (stat[i]["counter_rdp"].as<int>() > min_stats.rdp_cnt) ? stat[i]["counter_rdp"].as<int>() : min_stats.rdp_cnt;
        max_stats.restored_cnt = (stat[i]["counter_restored"].as<int>() > min_stats.restored_cnt) ? stat[i]["counter_restored"].as<int>() : min_stats.restored_cnt;
        max_stats.others_cnt = (stat[i]["counter_others"].as<int>() > min_stats.others_cnt) ? stat[i]["counter_others"].as<int>() : min_stats.others_cnt;
        max_stats.total_dist = (stat[i]["total_dist_m"].as<double>() > min_stats.total_dist) ? stat[i]["total_dist_m"].as<double>() : min_stats.total_dist;
        max_stats.ave_dist_m = (stat[i]["ave_dist_m"].as<double>() > min_stats.ave_dist_m) ? stat[i]["ave_dist_m"].as<double>() : min_stats.ave_dist_m;
        max_stats.max_dist_m = (stat[i]["max_dist_m"].as<double>() > min_stats.max_dist_m) ? stat[i]["max_dist_m"].as<double>() : min_stats.max_dist_m;
        max_stats.min_dist_m = (stat[i]["min_dist_m"].as<double>() > min_stats.min_dist_m) ? stat[i]["min_dist_m"].as<double>() : min_stats.min_dist_m;
        max_stats.record_per_10km = (stat[i]["record_per_10km"].as<int>() > min_stats.record_per_10km) ? stat[i]["record_per_10km"].as<int>() : min_stats.record_per_10km;
        max_stats.total_time = (stat[i]["total_time_s"].as<int>() > min_stats.total_time) ? stat[i]["total_time_s"].as<int>() : min_stats.total_time;
        max_stats.ave_dt_s = (stat[i]["ave_dt_s"].as<int>() > min_stats.ave_dt_s) ? stat[i]["ave_dt_s"].as<int>() : min_stats.ave_dt_s;
        max_stats.max_dt_s = (stat[i]["max_dt_s"].as<int>() > min_stats.max_dt_s) ? stat[i]["max_dt_s"].as<int>() : min_stats.max_dt_s;
        max_stats.min_dt_s = (stat[i]["min_dt_s"].as<int>() > min_stats.min_dt_s) ? stat[i]["min_dt_s"].as<int>() : min_stats.min_dt_s;
        max_stats.record_per_h = (stat[i]["record_per_h"].as<int>() > min_stats.record_per_h) ? stat[i]["record_per_h"].as<int>() : min_stats.record_per_h;
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

  std::cout << "Analyzed " << n << " files" << std::endl;

  // writing results to txt and json
  std::stringstream out_basename;
  out_basename << "GLOBAL_STATS";
  std::ofstream out(out_basename.str() + ".txt");
  int cell_width = 27;
  out << std::setw(cell_width) << "AVERAGED GLOBAL STATS";                                                                 out << std::endl;
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

  out << std::setw(cell_width) << "MINIMUM GLOBAL STATS";                                                                  out << std::endl;
  out << std::setw(cell_width) << "record number";              out << std::setw(cell_width) << min_stats.record_cnt;      out << std::endl;
  out << std::setw(cell_width) << "rdp enabled number";         out << std::setw(cell_width) << min_stats.rdp_cnt;         out << std::endl;
  out << std::setw(cell_width) << "restored number";            out << std::setw(cell_width) << min_stats.restored_cnt;    out << std::endl;
  out << std::setw(cell_width) << "others number";              out << std::setw(cell_width) << min_stats.others_cnt;      out << std::endl;
  out << std::setw(cell_width) << "total covered distance (m)"; out << std::setw(cell_width) << min_stats.total_dist;      out << std::endl;
  out << std::setw(cell_width) << "total time elapsed (s)";     out << std::setw(cell_width) << min_stats.total_time;      out << std::endl;
  out << std::setw(cell_width) << "record per 10 km";           out << std::setw(cell_width) << min_stats.record_per_10km; out << std::endl;
  out << std::setw(cell_width) << "ave dist (m)";               out << std::setw(cell_width) << min_stats.ave_dist_m;      out << std::endl;
  out << std::setw(cell_width) << "min dist (m)";               out << std::setw(cell_width) << min_stats.min_dist_m;      out << std::endl;
  out << std::setw(cell_width) << "max dist (m)";               out << std::setw(cell_width) << min_stats.max_dist_m;      out << std::endl;
  out << std::setw(cell_width) << "record per hour";            out << std::setw(cell_width) << min_stats.record_per_h;    out << std::endl;
  out << std::setw(cell_width) << "ave dt (s)";                 out << std::setw(cell_width) << min_stats.ave_dt_s;        out << std::endl;
  out << std::setw(cell_width) << "min dt (s)";                 out << std::setw(cell_width) << min_stats.min_dt_s;        out << std::endl;
  out << std::setw(cell_width) << "max dt (s)";                 out << std::setw(cell_width) << min_stats.max_dt_s;        out << std::endl;

  out << std::setw(cell_width) << "MAXIMUM GLOBAL STATS";                                                                  out << std::endl;
  out << std::setw(cell_width) << "record number";              out << std::setw(cell_width) << max_stats.record_cnt;      out << std::endl;
  out << std::setw(cell_width) << "rdp enabled number";         out << std::setw(cell_width) << max_stats.rdp_cnt;         out << std::endl;
  out << std::setw(cell_width) << "restored number";            out << std::setw(cell_width) << max_stats.restored_cnt;    out << std::endl;
  out << std::setw(cell_width) << "others number";              out << std::setw(cell_width) << max_stats.others_cnt;      out << std::endl;
  out << std::setw(cell_width) << "total covered distance (m)"; out << std::setw(cell_width) << max_stats.total_dist;      out << std::endl;
  out << std::setw(cell_width) << "total time elapsed (s)";     out << std::setw(cell_width) << max_stats.total_time;      out << std::endl;
  out << std::setw(cell_width) << "record per 10 km";           out << std::setw(cell_width) << max_stats.record_per_10km; out << std::endl;
  out << std::setw(cell_width) << "ave dist (m)";               out << std::setw(cell_width) << max_stats.ave_dist_m;      out << std::endl;
  out << std::setw(cell_width) << "min dist (m)";               out << std::setw(cell_width) << max_stats.min_dist_m;      out << std::endl;
  out << std::setw(cell_width) << "max dist (m)";               out << std::setw(cell_width) << max_stats.max_dist_m;      out << std::endl;
  out << std::setw(cell_width) << "record per hour";            out << std::setw(cell_width) << max_stats.record_per_h;    out << std::endl;
  out << std::setw(cell_width) << "ave dt (s)";                 out << std::setw(cell_width) << max_stats.ave_dt_s;        out << std::endl;
  out << std::setw(cell_width) << "min dt (s)";                 out << std::setw(cell_width) << max_stats.min_dt_s;        out << std::endl;
  out << std::setw(cell_width) << "max dt (s)";                 out << std::setw(cell_width) << max_stats.max_dt_s;        out << std::endl;

  out << std::setw(cell_width) << "TOTAL GLOBAL STATS";                                                                    out << std::endl;
  out << std::setw(cell_width) << "record number";              out << std::setw(cell_width) << tot_stats.record_cnt;      out << std::endl;
  out << std::setw(cell_width) << "rdp enabled number";         out << std::setw(cell_width) << tot_stats.rdp_cnt;         out << std::endl;
  out << std::setw(cell_width) << "restored number";            out << std::setw(cell_width) << tot_stats.restored_cnt;    out << std::endl;
  out << std::setw(cell_width) << "others number";              out << std::setw(cell_width) << tot_stats.others_cnt;      out << std::endl;
  out << std::setw(cell_width) << "total covered distance (m)"; out << std::setw(cell_width) << tot_stats.total_dist;      out << std::endl;
  out << std::setw(cell_width) << "total time elapsed (s)";     out << std::setw(cell_width) << tot_stats.total_time;      out << std::endl;
  out << std::setw(cell_width) << "record per 10 km";           out << std::setw(cell_width) << tot_stats.record_per_10km; out << std::endl;
  out << std::setw(cell_width) << "ave dist (m)";               out << std::setw(cell_width) << tot_stats.ave_dist_m;      out << std::endl;
  out << std::setw(cell_width) << "min dist (m)";               out << std::setw(cell_width) << tot_stats.min_dist_m;      out << std::endl;
  out << std::setw(cell_width) << "max dist (m)";               out << std::setw(cell_width) << tot_stats.max_dist_m;      out << std::endl;
  out << std::setw(cell_width) << "record per hour";            out << std::setw(cell_width) << tot_stats.record_per_h;    out << std::endl;
  out << std::setw(cell_width) << "ave dt (s)";                 out << std::setw(cell_width) << tot_stats.ave_dt_s;        out << std::endl;
  out << std::setw(cell_width) << "min dt (s)";                 out << std::setw(cell_width) << tot_stats.min_dt_s;        out << std::endl;
  out << std::setw(cell_width) << "max dt (s)";                 out << std::setw(cell_width) << tot_stats.max_dt_s;        out << std::endl;
  out.close();

  jsoncons::json ave_json = jsonize(ave_stats, "ave");
  jsoncons::json min_json = jsonize(min_stats, "min");
  jsoncons::json max_json = jsonize(max_stats, "max");
  jsoncons::json tot_json = jsonize(tot_stats, "tot");
  out.open(out_basename.str() + "_ave.json");
  out << jsoncons::pretty_print(ave_json) << std::endl;
  out.close();
  out.open(out_basename.str() + "_min.json");
  out << jsoncons::pretty_print(min_json) << std::endl;
  out.close();
  out.open(out_basename.str() + "_max.json");
  out << jsoncons::pretty_print(max_json) << std::endl;
  out.close();
  out.open(out_basename.str() + "_tot.json");
  out << jsoncons::pretty_print(tot_json) << std::endl;
  out.close();


  return 0;
}
