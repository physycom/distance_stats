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
#define MINOR_VERSION           0

void usage(char* progname) {
  // Usage
  std::cout << progname << " v" << MAJOR_VERSION << "." << MINOR_VERSION << std::endl;
  std::cout << "Usage: " << progname << " -i [input.json] " << std::endl;
  exit(1);
}

int main(int argc, char** argv) {
  std::cout << "********* DISTANCE STATISTICS *********" << std::endl;

  std::string input_name;
  if (argc > 2) { /* Parse arguments, if there are arguments supplied */
    for (int i = 1; i < argc; i++) {
      if ((argv[i][0] == '-') || (argv[i][0] == '/')) {       // switches or options...
        switch (tolower(argv[i][1])) {
        case 'i':
          input_name = argv[++i];
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
  std::vector<int> bins(dist_thresholds.size());
  double max_dist = 0;

  for (size_t i = 0; i < distances.size(); i++) {
    max_dist = (max_dist < distances[i]) ? distances[i] : max_dist;
    for (size_t j = 0; j < bins.size(); j++) {
      if (distances[i] < dist_thresholds[j]) bins[j]++;
    }
  }

  // Scenario 2 : dynamical bins
  double bin_w = 0.5;
  int bin_n = int(max_dist / bin_w) + 1;
  std::vector<int> dynamic_bins(bin_n);
  std::vector<double> fraction_bins, cumulative_bins;

  for (size_t i = 0; i < distances.size(); i++) {
    dynamic_bins[int(distances[i] / bin_w)]++;
  }
  for (auto b : dynamic_bins) {
    fraction_bins.push_back(b / double(distances.size()));
  }
  cumulative_bins.push_back(0.0);
  double temp = 0.0;
  for (size_t i = 1; i < fraction_bins.size(); i++) {
    temp += fraction_bins[i];
    cumulative_bins.push_back( temp );
  }
  
  std::vector<double> frac_thresholds({ 0.33, 0.66, 0.75, 0.90, 0.95 });
  std::vector<int> indices;
  for (size_t i = 0; i < frac_thresholds.size(); i++) {
    for (size_t j = 0; j < cumulative_bins.size(); j++) {
      if (cumulative_bins[j] > frac_thresholds[i]) {
        indices.push_back(j);
        break;
      }
    }
  }

  
  return 0;
}