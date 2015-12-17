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

#include <fstream>
#include <stdexcept>
#include <string>
#include <cmath>
#include "jsoncons/json.hpp"

#define GEODESIC_DEG_TO_M         111070.4                              // conversion [deg] -> [meter]
#define RAD_TO_DEG                57.2957795131                         // 180/pi
#define DEG_TO_RAD                1.745329251e-2                        // pi/180

#define MAJOR_VERSION           1
#define MINOR_VERSION           2

void usage(char* progname) {
  // Usage
  std::cout << progname << " v" << MAJOR_VERSION << "." << MINOR_VERSION << std::endl;
  std::cout << "Usage: " << progname << " -i [input.json] -o [output_file_basename] " << std::endl;
  exit(1);
}

int main(int argc, char** argv) {
  std::cout << "********* DISTANCE STATISTICS *********" << std::endl;

  std::string input_name, output_basename, output_fixed_name, output_dynamic_name, output_gnuplot_name;
  if (argc > 2) { /* Parse arguments, if there are arguments supplied */
    for (int i = 1; i < argc; i++) {
      if ((argv[i][0] == '-') || (argv[i][0] == '/')) {       // switches or options...
        switch (tolower(argv[i][1])) {
        case 'i':
          input_name = argv[++i];
          break;
        case 'o':
          output_basename = argv[++i];
          break;
        default:    // no match...
          std::cout << "ERROR: Flag \"" << argv[i] << "\" not recognized. Quitting..." << std::endl;
          usage(argv[0]);
        }
      }
      else {
        std::cout << "ERROR: Flag \"" << argv[i] << "\" not recognized. Quitting..." << std::endl;
        usage(argv[0]);
      }
    }
  }
  else {
    std::cout << "ERROR: No flags specified. Read usage and relaunch properly." << std::endl;
    usage(argv[0]);
  }

  // Safety checks for file manipulations
  std::ifstream input_file;
  std::ofstream output_fixed_bins, output_dynamic_bins, output_gnuplot_script;

  if (input_name.size() > 5) {
    if (input_name.substr(input_name.size() - 5, 5) != ".json") {
      std::cout << input_name << " is not a valid .json file. Quitting..." << std::endl;
      exit(2);
    }
  }
  else {
    std::cout << input_name << " is not a valid .json file. Quitting..." << std::endl;
    exit(22);
  }
  input_file.open(input_name.c_str());
  if (!input_file) {
    std::cout << "FAILED: Input file " << input_name << " could not be opened. Quitting..." << std::endl;
    exit(222);
  }
  else { std::cout << "SUCCESS: file " << input_name << " opened!\n"; }
  input_file.close();

  output_fixed_name = output_basename + "_fixed_bins.txt";
  output_dynamic_name = output_basename + "_dynamic_bins.txt";
  output_gnuplot_name = output_basename + "_dinamic_bins.plt";
  
  output_fixed_bins.open(output_fixed_name.c_str());
  output_dynamic_bins.open(output_dynamic_name.c_str());
  output_gnuplot_script.open(output_gnuplot_name.c_str());

  if (!output_fixed_bins) {
    std::cout << "FAILED: Output file " << output_fixed_name << " could not be opened. Quitting..." << std::endl;
    exit(223);
  }
  else { std::cout << "SUCCESS: file " << output_fixed_name << " opened!\n"; }
  if (!output_dynamic_bins) {
    std::cout << "FAILED: Output file " << output_dynamic_name << " could not be opened. Quitting..." << std::endl;
    exit(223);
  }
  else { std::cout << "SUCCESS: file " << output_dynamic_name << " opened!\n"; }
  if (!output_gnuplot_script) {
    std::cout << "FAILED: Output file " << output_gnuplot_name << " could not be opened. Quitting..." << std::endl;
    exit(223);
  }
  else { std::cout << "SUCCESS: file " << output_gnuplot_name << " opened!\n"; }

  // Importing json distance database
  jsoncons::json distance_records = jsoncons::json::parse_file(input_name);
  std::vector<double> distances;

  if (distance_records.is_object()) {         // object-style
    int i = 0;
    for (auto rec = distance_records.begin_members(); rec != distance_records.end_members(); ++rec, ++i) {
      try {
        if (rec->value().has_member("distance")) distances.push_back(rec->value()["distance"].as<double>());
      }
      catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
      }
    }
  }
  else if (distance_records.is_array()) {    // array-style
    for (size_t i = 0; i < distance_records.size(); ++i) {
      try {
        if (distance_records[i].has_member("distance")) distances.push_back(distance_records[i]["distance"].as<double>());
      }
      catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
      }
    }
  }

  //// Statistics on distance vector
  // Scenario 1 : fixed bins
  std::vector<double> dist_thresholds({ 1.0, 3.0, 5.0, 7.5, 15.0 });      // move to config file ?
  std::vector<int> bins(dist_thresholds.size()+1, 0);
  double max_dist = 0;

  for (size_t i = 0; i < distances.size(); i++) {  // max_dist calc
    max_dist = ((max_dist > distances[i]) ? max_dist : distances[i]);
  }
  dist_thresholds.push_back(max_dist);              // all the points will be inside this distance -> to check results

  for (size_t i = 0; i < distances.size(); i++) {
    for (size_t j = 0; j < bins.size(); j++) {
      if (distances[i] <= dist_thresholds[j]) bins[j]++;
    }
  }

  // Scenario 2 : dynamical bins
  double bin_w = 0.5;                                                     // move to config file ?
  int bin_n = int(max_dist / bin_w) + 1;
  std::vector<int> dynamic_bins(bin_n, 0);
  std::vector<double> fraction_bins, cumulative_bins;

  for (size_t i = 0; i < distances.size(); i++) dynamic_bins[int(distances[i] / bin_w)]++;

  for (auto b : dynamic_bins) fraction_bins.push_back(b / double(distances.size()));

  cumulative_bins.push_back(fraction_bins[0]);
  for (size_t i = 1; i < fraction_bins.size(); i++) cumulative_bins.push_back(cumulative_bins.back() + fraction_bins[i]);

  std::vector<double> frac_thresholds({ 0.5, 0.66, 0.75, 0.90, 0.95, 0.99, 1.00 });  // move to config file ?
  std::vector<size_t> indices;
  for (size_t i = 0; i < frac_thresholds.size(); i++) {
    for (size_t j = 0; j < cumulative_bins.size(); j++) {
      if (cumulative_bins[j] > frac_thresholds[i]) {
        indices.push_back(j);
        break;
      }
    }
  }

  //Write output file, compatible with gnuplot
  size_t counter = 0;
  for (auto a : dist_thresholds) {
    output_fixed_bins << std::setw(6) << a << "\t" << bins[counter] << std::endl;
    counter++;
  }

  counter = 0;
  for (auto a : fraction_bins) {
    output_dynamic_bins << counter << "\t";
    output_dynamic_bins << counter*bin_w << "\t";
    output_dynamic_bins << (counter+1)*bin_w << "\t";
    output_dynamic_bins << a /*fraction_bins[counter]*/ << "\t";
    output_dynamic_bins << cumulative_bins[counter] << "\t";
    output_dynamic_bins << std::endl;
    counter++;
  }


  output_fixed_bins.close();
  output_dynamic_bins.close();
  output_gnuplot_script.close();

  return 0;
}


