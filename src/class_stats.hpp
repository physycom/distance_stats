
#include "jsoncons/json.hpp"

class Trip_Stats {
public:
  std::string trip_name;
  unsigned long long int record_cnt, restored_cnt, rdp_cnt, others_cnt;
  unsigned long long total_time;
  double total_dist;
  double record_per_10km, ave_dist_m, min_dist_m, max_dist_m;
  double record_per_h, ave_dt_s, min_dt_s, max_dt_s;
};

jsoncons::json jsonize(Trip_Stats trip_s) {
  jsoncons::json trip_j;
  trip_j["trip_name"] = trip_s.trip_name;
  trip_j["counter_records"] = trip_s.record_cnt;
  trip_j["counter_restored"] = trip_s.restored_cnt;
  trip_j["counter_rdp"] = trip_s.rdp_cnt;
  trip_j["counter_others"] = trip_s.others_cnt;
  trip_j["total_dist_m"] = (unsigned long long int)trip_s.total_dist;
  trip_j["total_time_s"] = (unsigned long long int)trip_s.total_time;
  trip_j["record_per_10km"] = (unsigned long long int)trip_s.record_per_10km;
  trip_j["ave_dist_m"] = (unsigned long long int)trip_s.ave_dist_m;
  trip_j["min_dist_m"] = (unsigned long long int)trip_s.min_dist_m;
  trip_j["max_dist_m"] = (unsigned long long int)trip_s.max_dist_m;
  trip_j["record_per_h"] = (unsigned long long int)trip_s.record_per_h;
  trip_j["ave_dt_s"] = (unsigned long long int)trip_s.ave_dt_s;
  trip_j["min_dt_s"] = (unsigned long long int)trip_s.min_dt_s;
  trip_j["max_dt_s"] = (unsigned long long int)trip_s.max_dt_s;
  return trip_j;
}
