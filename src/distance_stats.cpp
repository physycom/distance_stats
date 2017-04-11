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


#include <fstream>
#include <stdexcept>
#include <string>
#include <cmath>
#include "jsoncons/json.hpp"

#define GEODESIC_DEG_TO_M         111070.4                              // conversion [deg] -> [meter]
#define RAD_TO_DEG                57.2957795131                         // 180/pi
#define DEG_TO_RAD                1.745329251e-2                        // pi/180

#define MAJOR_VERSION           1
#define MINOR_VERSION           5

void usage(char* progname) {
  std::cerr << "Usage: " << progname << " -i [input.json] -o [output_file_basename] " << std::endl;
}

int main(int argc, char** argv) {
  std::cout << "distance_stats v" << MAJOR_VERSION << "." << MINOR_VERSION << std::endl;

  std::string input_name, output_basename, output_fixed_name, output_dist_dynamic_name, output_lat_dist_dynamic_name, output_lon_dist_dynamic_name, output_angles_name,
    output_index_name, output_gnuplot_fraction_name, output_png_fraction_name, output_gnuplot_cumulative_name, output_png_cumulative_name,
    output_gnuplot_angles_name, output_png_angles_name, output_gnuplot_anglesdistances_name, output_png_anglesdistances_name;

  if (argc > 2) { /* Parse arguments, if there are arguments supplied */
    for (int i = 1; i < argc; i++) {
      if ((argv[i][0] == '-') || (argv[i][0] == '/')) {       // switches or options...
        switch (::tolower(argv[i][1])) {
        case 'i':
          input_name = argv[++i];
          break;
        case 'o':
          output_basename = argv[++i];
          break;
        default:    // no match...
          std::cerr << "ERROR: Flag \"" << argv[i] << "\" not recognized. Quitting..." << std::endl;
          usage(argv[0]);
          exit(-1);
        }
      }
      else {
        std::cerr << "ERROR: Flag \"" << argv[i] << "\" not recognized. Quitting..." << std::endl;
        usage(argv[0]);
        exit(-2);
      }
    }
  }
  else {
    std::cerr << "ERROR: No flags specified. Read usage and relaunch properly." << std::endl;
    usage(argv[0]);
    exit(-3);
  }

  // Safety checks for file manipulations
  std::ifstream input_file;
  std::ofstream output_fixed_bins, output_dist_dynamic_bins, output_lat_dist_dynamic_bins, output_lon_dist_dynamic_bins, output_angles_bins,
    output_indexes, output_gnuplot_fraction_script, output_gnuplot_cumulative_script, output_gnuplot_angles_script, output_gnuplot_anglesdistances_script;

  if (input_name.size() > 5) {
    if (input_name.substr(input_name.size() - 5, 5) != ".json") {
      std::cerr << input_name << " is not a valid .json file. Quitting..." << std::endl;
      exit(-4);
    }
  }
  else {
    std::cerr << input_name << " is not a valid .json file. Quitting..." << std::endl;
    exit(-5);
  }
  input_file.open(input_name.c_str());
  if (!input_file) {
    std::cerr << "FAILED: Input file " << input_name << " could not be opened. Quitting..." << std::endl;
    exit(-6);
  }
  else { std::cout << "SUCCESS: file " << input_name << " opened!\n"; }
  input_file.close();

  output_index_name = output_basename + "_indexes.txt";
  output_fixed_name = output_basename + "_fixed_bins.txt";
  output_dist_dynamic_name = output_basename + "_dist_dynamic_bins.txt";
  output_lat_dist_dynamic_name = output_basename + "_lat_dist_dynamic_bins.txt";
  output_lon_dist_dynamic_name = output_basename + "_lon_dist_dynamic_bins.txt";
  output_angles_name = output_basename + "_angles_bins.txt";
  output_gnuplot_fraction_name = output_basename + "_dynamic_fraction_bins.plt";
  output_png_fraction_name = output_basename + "_dynamic_fraction_bins.png";
  output_gnuplot_cumulative_name = output_basename + "_dynamic_cumulative_bins.plt";
  output_png_cumulative_name = output_basename + "_dynamic_cumulative_bins.png";
  output_gnuplot_angles_name = output_basename + "_angles_bins.plt";
  output_png_angles_name = output_basename + "_angles_bins.png";
  output_gnuplot_anglesdistances_name = output_basename + "_anglesdistances_bins.plt";
  output_png_anglesdistances_name = output_basename + "_anglesdistances_bins.png";

  output_indexes.open(output_index_name.c_str());
  output_fixed_bins.open(output_fixed_name.c_str());
  output_dist_dynamic_bins.open(output_dist_dynamic_name.c_str());
  output_lat_dist_dynamic_bins.open(output_lat_dist_dynamic_name.c_str());
  output_lon_dist_dynamic_bins.open(output_lon_dist_dynamic_name.c_str());
  output_angles_bins.open(output_angles_name.c_str());
  output_gnuplot_fraction_script.open(output_gnuplot_fraction_name.c_str());
  output_gnuplot_cumulative_script.open(output_gnuplot_cumulative_name.c_str());
  output_gnuplot_angles_script.open(output_gnuplot_angles_name.c_str());
  output_gnuplot_anglesdistances_script.open(output_gnuplot_anglesdistances_name.c_str());

  if (!output_indexes) {
    std::cerr << "FAILED: Output file " << output_index_name << " could not be opened. Quitting..." << std::endl;
    exit(-7);
  }
  if (!output_fixed_bins) {
    std::cerr << "FAILED: Output file " << output_fixed_name << " could not be opened. Quitting..." << std::endl;
    exit(-8);
  }
  if (!output_dist_dynamic_bins) {
    std::cerr << "FAILED: Output file " << output_dist_dynamic_name << " could not be opened. Quitting..." << std::endl;
    exit(-9);
  }
  if (!output_lat_dist_dynamic_bins) {
    std::cerr << "FAILED: Output file " << output_lat_dist_dynamic_name << " could not be opened. Quitting..." << std::endl;
    exit(-10);
  }
  if (!output_lon_dist_dynamic_bins) {
    std::cerr << "FAILED: Output file " << output_lon_dist_dynamic_name << " could not be opened. Quitting..." << std::endl;
    exit(-11);
  }
  if (!output_angles_bins) {
    std::cerr << "FAILED: Output file " << output_angles_name << " could not be opened. Quitting..." << std::endl;
    exit(-12);
  }
  if (!output_gnuplot_fraction_script) {
    std::cerr << "FAILED: Output file " << output_gnuplot_fraction_name << " could not be opened. Quitting..." << std::endl;
    exit(-13);
  }
  if (!output_gnuplot_cumulative_script) {
    std::cerr << "FAILED: Output file " << output_gnuplot_cumulative_name << " could not be opened. Quitting..." << std::endl;
    exit(-14);
  }
  if (!output_gnuplot_angles_script) {
    std::cerr << "FAILED: Output file " << output_gnuplot_angles_name << " could not be opened. Quitting..." << std::endl;
    exit(-15);
  }
  if (!output_gnuplot_anglesdistances_script) {
    std::cerr << "FAILED: Output file " << output_gnuplot_anglesdistances_name << " could not be opened. Quitting..." << std::endl;
    exit(-16);
  }


  // Importing json distance database
  jsoncons::json distance_records;
  try {
    distance_records = jsoncons::json::parse_file(input_name);
  }
  catch (std::exception &e) {
    std::cout << "EXCEPTION: " << e.what() << std::endl;
  }
  std::vector<double> distances, lat_distances, lon_distances, angles;

  if (distance_records.is_object()) {         // object-style
    int i = 0;
    for (auto rec = distance_records.begin_members(); rec != distance_records.end_members(); ++rec, ++i) {
      try {
        if (rec->value().has_member("distance")) distances.push_back(rec->value()["distance"].as<double>());
        if (rec->value().has_member("dst_lat")) lat_distances.push_back(rec->value()["dst_lat"].as<double>());
        if (rec->value().has_member("dst_lon")) lon_distances.push_back(rec->value()["dst_lon"].as<double>());
        if (rec->value().has_member("angle")) angles.push_back(rec->value()["angle"].as<double>());
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
        if (distance_records[i].has_member("dst_lat")) lat_distances.push_back(distance_records[i]["dst_lat"].as<double>());
        if (distance_records[i].has_member("dst_lon")) lon_distances.push_back(distance_records[i]["dst_lon"].as<double>());
        if (distance_records[i].has_member("angle")) angles.push_back(distance_records[i]["angle"].as<double>());
      }
      catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
      }
    }
  }

  //// Statistics on angle vector
  double angle_bin_w = 2.0;                                                          // move to config file ?
  int angle_bin_size = int(360.0 / angle_bin_w);
  std::vector<double> angle_distance(angle_bin_size);
  std::vector<double> angle_counter(angle_bin_size);
  for (size_t i = 0; i < angles.size(); i++) {
    int bin_index = int(fabs(angles[i]) / angle_bin_w);
    if (bin_index >= angle_bin_size) bin_index = angle_bin_size -1;
    angle_distance[bin_index] += distances[i];
    angle_counter[bin_index]++;
  }

  for (int i = 0; i < angle_bin_size; i++) {
    output_angles_bins << i << "\t" << i*angle_bin_w << "\t" << (i + 1)*angle_bin_w << "\t" << angle_counter[i] << "\t" << angle_distance[i] << std::endl;
  }


  //// Statistics on distance vector
  std::vector<double> distance_thresholds({ 1.0, 3.0, 5.0, 7.5, 15.0 });             // move to config file ?
  double bin_w = 1.0;                                                                // move to config file ?
  std::vector<double> frac_thresholds({ 0.5, 0.66, 0.75, 0.90, 0.95, 0.99, 1.00 });  // move to config file ?
  size_t Xres = 1280;                                                                // move to config file ?
  size_t Yres = 960;                                                                 // move to config file ?

  size_t counter;


  //=======================================
  // Scenario 1 : fixed bins
  std::vector<int> distances_fix_binning(distance_thresholds.size() + 1, 0);
  double max_distance = 0, inf_lat_distance = 0, sup_lat_distance = 0, inf_lon_distance = 0, sup_lon_distance = 0;

  for (size_t i = 0; i < distances.size(); i++) {  // max_distance calc
    max_distance = ((max_distance > distances[i]) ? max_distance : distances[i]);
  }
  for (size_t i = 0; i < lat_distances.size(); i++) {  // inf_lat_distance calc
    inf_lat_distance = ((inf_lat_distance < lat_distances[i]) ? inf_lat_distance : lat_distances[i]);
  }
  for (size_t i = 0; i < lat_distances.size(); i++) {  // sup_lat_distance calc
    sup_lat_distance = ((sup_lat_distance > lat_distances[i]) ? sup_lat_distance : lat_distances[i]);
  }
  for (size_t i = 0; i < lon_distances.size(); i++) {  // inf_lon_distance calc
    inf_lon_distance = ((inf_lon_distance < lon_distances[i]) ? inf_lon_distance : lon_distances[i]);
  }
  for (size_t i = 0; i < lon_distances.size(); i++) {  // sup_lon_distance calc
    sup_lon_distance = ((sup_lon_distance > lon_distances[i]) ? sup_lon_distance : lon_distances[i]);
  }
  distance_thresholds.push_back(max_distance);
  std::vector<int> lat_distances_fix_binning(distance_thresholds.size(), 0);
  std::vector<int> lon_distances_fix_binning(distance_thresholds.size(), 0);

  for (size_t i = 0; i < distances.size(); i++) {
    for (size_t j = 0; j < distance_thresholds.size(); j++) {
      if (distances[i] <= distance_thresholds[j]) distances_fix_binning[j]++;
      if (std::fabs(lat_distances[i]) <= distance_thresholds[j]) lat_distances_fix_binning[j]++;
      if (std::fabs(lon_distances[i]) <= distance_thresholds[j]) lon_distances_fix_binning[j]++;
    }
  }

  //Write output file
  counter = 0;
  for (auto a : distance_thresholds) {
    output_fixed_bins << std::setw(6) << a << "\t" << distances_fix_binning[counter] << "\t" << lat_distances_fix_binning[counter] << "\t" << lon_distances_fix_binning[counter] << std::endl;
    counter++;
  }



  //=======================================
  // Scenario 2 : dynamic bins
  int distances_n_bin = int(max_distance / bin_w) + 1;
  int lat_distances_n_bin = int((sup_lat_distance - inf_lat_distance) / bin_w) + 1;
  int lon_distances_n_bin = int((sup_lon_distance - inf_lon_distance) / bin_w) + 1;;
  std::vector<int> distance_dyn_binning(distances_n_bin, 0);
  std::vector<int> lat_distance_dyn_binning(lat_distances_n_bin, 0);
  std::vector<int> lon_distance_dyn_binning(lon_distances_n_bin, 0);
  std::vector<double> distances_fraction_bins, distances_cumulative_bins;
  std::vector<double> lat_distances_fraction_bins, lat_distances_cumulative_bins;
  std::vector<double> lon_distances_fraction_bins, lon_distances_cumulative_bins;

  for (size_t i = 0; i < distances.size(); i++) distance_dyn_binning[int(distances[i] / bin_w)]++;
  for (size_t i = 0; i < lat_distances.size(); i++) lat_distance_dyn_binning[int((lat_distances[i] - inf_lat_distance) / bin_w)]++;
  for (size_t i = 0; i < lon_distances.size(); i++) lon_distance_dyn_binning[int((lon_distances[i] - inf_lon_distance) / bin_w)]++;

  for (auto b : distance_dyn_binning) distances_fraction_bins.push_back(b / double(distances.size()));
  for (auto b : lat_distance_dyn_binning) lat_distances_fraction_bins.push_back(b / double(lat_distances.size()));
  for (auto b : lon_distance_dyn_binning) lon_distances_fraction_bins.push_back(b / double(lon_distances.size()));

  distances_cumulative_bins.push_back(distances_fraction_bins[0]);
  for (size_t i = 1; i < distances_fraction_bins.size(); i++) distances_cumulative_bins.push_back(distances_cumulative_bins.back() + distances_fraction_bins[i]);

  lat_distances_cumulative_bins.push_back(lat_distances_fraction_bins[0]);
  for (size_t i = 1; i < lat_distances_fraction_bins.size(); i++) lat_distances_cumulative_bins.push_back(lat_distances_cumulative_bins.back() + lat_distances_fraction_bins[i]);

  lon_distances_cumulative_bins.push_back(lon_distances_fraction_bins[0]);
  for (size_t i = 1; i < lon_distances_fraction_bins.size(); i++) lon_distances_cumulative_bins.push_back(lon_distances_cumulative_bins.back() + lon_distances_fraction_bins[i]);


  std::vector<size_t> distances_indexes, lat_distances_indexes, lon_distances_indexes;
  for (size_t i = 0; i < frac_thresholds.size(); i++) {
    for (size_t j = 0; j < distances_cumulative_bins.size(); j++) {
      if (distances_cumulative_bins[j] > frac_thresholds[i]) {
        distances_indexes.push_back(j);
        break;
      }
    }
    for (size_t j = 0; j < lat_distances_cumulative_bins.size(); j++) {
      if (lat_distances_cumulative_bins[j] > frac_thresholds[i]) {
        lat_distances_indexes.push_back(j);
        break;
      }
    }
    for (size_t j = 0; j < lon_distances_cumulative_bins.size(); j++) {
      if (lon_distances_cumulative_bins[j] > frac_thresholds[i]) {
        lon_distances_indexes.push_back(j);
        break;
      }
    }
  }

  //Write output file
  counter = 0;
  output_indexes << "#TOT. DISTANCE:" << std::endl;
  for (auto a : distances_indexes) {
    output_indexes << std::setw(6) << a << "\t" << frac_thresholds[counter] << "\t" << a << std::endl;
    counter++;
  }
  output_indexes << std::endl;
  counter = 0;
  output_indexes << "#LAT. DISTANCE:" << std::endl;
  for (auto a : lat_distances_indexes) {
    output_indexes << std::setw(6) << a << "\t" << frac_thresholds[counter] << "\t" << a << std::endl;
    counter++;
  }
  output_indexes << std::endl;
  counter = 0;
  output_indexes << "#LON. DISTANCE:" << std::endl;
  for (auto a : lon_distances_indexes) {
    output_indexes << std::setw(6) << a << "\t" << frac_thresholds[counter] << "\t" << a << std::endl;
    counter++;
  }
  output_indexes << std::endl;



  //Write output file, compatible with gnuplot
  counter = 0;
  output_dist_dynamic_bins << "-0.5\t-0.5\t0.5\t0.0\t0.0" << std::endl;
  for (auto a : distances_fraction_bins) {
    output_dist_dynamic_bins << counter << "\t";
    output_dist_dynamic_bins << counter*bin_w << "\t";
    output_dist_dynamic_bins << (counter + 1)*bin_w << "\t";
    output_dist_dynamic_bins << a << "\t";
    output_dist_dynamic_bins << distances_cumulative_bins[counter] << "\t";
    output_dist_dynamic_bins << std::endl;
    counter++;
  }

  counter = 0;
  for (auto a : lat_distances_fraction_bins) {
    output_lat_dist_dynamic_bins << counter << "\t";
    output_lat_dist_dynamic_bins << std::floor(inf_lat_distance) + counter*bin_w << "\t";
    output_lat_dist_dynamic_bins << std::floor(inf_lat_distance) + (counter + 1)*bin_w << "\t";
    output_lat_dist_dynamic_bins << a << "\t";
    output_lat_dist_dynamic_bins << lat_distances_cumulative_bins[counter] << "\t";
    output_lat_dist_dynamic_bins << std::endl;
    counter++;
  }

  counter = 0;
  for (auto a : lon_distances_fraction_bins) {
    output_lon_dist_dynamic_bins << counter << "\t";
    output_lon_dist_dynamic_bins << std::floor(inf_lon_distance) + counter*bin_w << "\t";
    output_lon_dist_dynamic_bins << std::floor(inf_lon_distance) + (counter + 1)*bin_w << "\t";
    output_lon_dist_dynamic_bins << a << "\t";
    output_lon_dist_dynamic_bins << lon_distances_cumulative_bins[counter] << "\t";
    output_lon_dist_dynamic_bins << std::endl;
    counter++;
  }


  //=======================================
  //Prepare gnuplot script to plot all histograms
  const std::string gnuplot_styles = R"(# Styles
linew = 1.2
set style line  21 lc rgb '#0072bd' lt 7 lw linew pi -1 # blue
set style line  22 lc rgb '#d95319' lt 7 lw linew pi -1 # orange
set style line  23 lc rgb '#77ac30' lt 7 lw linew pi -1 # green
set style line  24 lc rgb '#a2142f' lt 7 lw linew pi -1 # red
set style line 101 lc rgb '#000000' lt 1 lw 1                     # black
set style line 102 lc rgb '#d6d7d9' lt 1 lw 1                     # gray
# Border xy
set border 3 front ls 101
set tics nomirror out scale 0.75
set format '%g'
set border linewidth 1.5
# Grid
set grid xtics ytics back ls 102
# Plot
)";

  output_gnuplot_fraction_script << "#!/gnuplot\n";
  output_gnuplot_fraction_script << "FILE_IN_TOT='" << output_dist_dynamic_name << "'\n";
  output_gnuplot_fraction_script << "FILE_IN_LAT='" << output_lat_dist_dynamic_name << "'\n";
  output_gnuplot_fraction_script << "FILE_IN_LON='" << output_lon_dist_dynamic_name << "'\n";
  output_gnuplot_fraction_script << "FILE_OUT='" << output_png_fraction_name << "'\n";
  output_gnuplot_fraction_script << "set terminal pngcairo dashed size " << Xres << ',' << Yres << " enhanced font 'Verdana,10'\n";
  output_gnuplot_fraction_script << gnuplot_styles;
  output_gnuplot_fraction_script << "set output FILE_OUT\n";
  output_gnuplot_fraction_script << "set xlabel 'd (m)' \n";
  output_gnuplot_fraction_script << "set ylabel '{/Symbol r}'\n";
  output_gnuplot_fraction_script << "plot FILE_IN_TOT u ($2+$3)/2:4 w histeps ls 21 t 'tot',\\" << "\n";
  output_gnuplot_fraction_script << "     FILE_IN_LAT u ($2+$3)/2:4 w histeps ls 24 t 'lat',\\" << "\n";
  output_gnuplot_fraction_script << "     FILE_IN_LON u ($2+$3)/2:4 w histeps ls 23 t 'lon'";
  output_gnuplot_fraction_script << "\n";

  output_gnuplot_cumulative_script << "#!/gnuplot\n";
  output_gnuplot_cumulative_script << "FILE_IN_TOT='" << output_dist_dynamic_name << "'\n";
  output_gnuplot_cumulative_script << "FILE_IN_LAT='" << output_lat_dist_dynamic_name << "'\n";
  output_gnuplot_cumulative_script << "FILE_IN_LON='" << output_lon_dist_dynamic_name << "'\n";
  output_gnuplot_cumulative_script << "FILE_OUT='" << output_png_cumulative_name << "'\n";
  output_gnuplot_cumulative_script << "set terminal pngcairo dashed size " << Xres << ',' << Yres << " enhanced font 'Verdana,10'\n";
  output_gnuplot_cumulative_script << gnuplot_styles;
  output_gnuplot_cumulative_script << "set pointintervalbox 1.5\n";
  output_gnuplot_cumulative_script << "set key bottom right\n";
  output_gnuplot_cumulative_script << "set output FILE_OUT\n";
  output_gnuplot_cumulative_script << "set xlabel 'd (m)' \n";
  output_gnuplot_cumulative_script << "set ylabel '{/Symbol r}_{int}'\n";
  output_gnuplot_cumulative_script << "set yrange[0:1]\n";
  output_gnuplot_cumulative_script << "plot FILE_IN_TOT u ($2+$3)/2:5 w linespoints ls 21 t 'tot',\\" << "\n";
  output_gnuplot_cumulative_script << "     FILE_IN_LAT u ($2+$3)/2:5 w linespoints ls 24 t 'lat',\\" << "\n";
  output_gnuplot_cumulative_script << "     FILE_IN_LON u ($2+$3)/2:5 w linespoints ls 23 t 'lon'";
  output_gnuplot_cumulative_script << "\n";

  output_gnuplot_angles_script << "#!/gnuplot\n";
  output_gnuplot_angles_script << "FILE_IN='" << output_angles_name << "'\n";
  output_gnuplot_angles_script << "FILE_OUT='" << output_png_angles_name << "'\n";
  output_gnuplot_angles_script << "set terminal pngcairo dashed size " << Xres << ',' << Yres << " enhanced font 'Verdana,10'\n";
  output_gnuplot_angles_script << "set output FILE_OUT\n";
  output_gnuplot_angles_script << "set key bottom right\n";
  output_gnuplot_angles_script << "set angles degrees\n";
  output_gnuplot_angles_script << "set polar\n";
  output_gnuplot_angles_script << "set grid polar 15.\n";
  output_gnuplot_angles_script << "unset border\n";
  output_gnuplot_angles_script << "unset param\n";
  output_gnuplot_angles_script << "plot FILE_IN u 2:4 notitle\n";
  output_gnuplot_angles_script << "\n";

  output_gnuplot_anglesdistances_script << "#!/gnuplot\n";
  output_gnuplot_anglesdistances_script << "FILE_IN='" << output_angles_name << "'\n";
  output_gnuplot_anglesdistances_script << "FILE_OUT='" << output_png_anglesdistances_name << "'\n";
  output_gnuplot_anglesdistances_script << "set terminal pngcairo dashed size " << Xres << ',' << Yres << " enhanced font 'Verdana,10'\n";
  output_gnuplot_anglesdistances_script << "set output FILE_OUT\n";
  output_gnuplot_anglesdistances_script << "set key bottom right\n";
  output_gnuplot_anglesdistances_script << "set angles degrees\n";
  output_gnuplot_anglesdistances_script << "set polar\n";
  output_gnuplot_anglesdistances_script << "set grid polar 15.\n";
  output_gnuplot_anglesdistances_script << "unset border\n";
  output_gnuplot_anglesdistances_script << "unset param\n";
  output_gnuplot_anglesdistances_script << "set style data line\n";
  output_gnuplot_anglesdistances_script << "plot FILE_IN u 2:4 notitle\n";
  output_gnuplot_anglesdistances_script << "\n";

  output_indexes.close();
  output_fixed_bins.close();
  output_dist_dynamic_bins.close();
  output_lat_dist_dynamic_bins.close();
  output_lon_dist_dynamic_bins.close();
  output_angles_bins.close();
  output_gnuplot_fraction_script.close();
  output_gnuplot_cumulative_script.close();
  output_gnuplot_angles_script.close();
  output_gnuplot_anglesdistances_script.close();

  return 0;
}


