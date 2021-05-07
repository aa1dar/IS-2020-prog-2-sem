#include <iostream>
#include "pugixml.hpp"
#include <map>
#include <string>
#include <vector>
#include <sstream>
#include <set>
#include <cmath>


//FIXED dont use using namespace


//FIXED S P A C E S



std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> result;
    std::stringstream ss(s);
    std::string item;

    while (getline(ss, item, delim)) {
        result.push_back(item);
    }

    return result;
}


class database {
public:

    database() = default;

    std::multimap<std::string, std::pair<std::string, std::pair<long int, long int>>> routes_;
    std::map<std::string, double> rout_length_;
    std::set<std::string> name_of_routes_;


};

long int reform(std::string cord) {
    long int x = 0;

    for (char i: cord) {
        if (i != '.')
            x = x * 10 + ((int) i - (int) '0');
    }
    return x;
}


std::string max_length_route(database &data) {
    double maxlength = 0;
    std::string route;
    for (auto i:data.name_of_routes_) {
        if (data.rout_length_[i] > maxlength) {
            maxlength = data.rout_length_[i];
            route = i;
        }
    }
    return route;
}

std::string max_station_street(const std::map<std::string, int> &street_counts) {
    int maxcount = 0;
    std::string street_ans;

    //FIXED range based for
    for (const auto &i: street_counts) {
        auto[street, count] = i;
        if (maxcount < count) {
            maxcount = count;
            street_ans = street;
        }
    }

    return street_ans;
}

//fixed const&
std::string max_counts_station(const database &data) {
    int count = 0;
    int maxcount = 0;
    std::string route;
    for (auto i: data.name_of_routes_) {
        count = data.routes_.count(i);
        if (count > maxcount) {
            maxcount = count;
            route = i;
        }
    }

    return route;

}

double find_length(long x, long y, long xOld, long yOld) {
    double ans = 0;
    ans = sqrt((x - xOld) * (x - xOld) + (y - yOld) * (y - yOld));

    return ans;
}


int main() {

    pugi::xml_document doc;

    doc.load_file("data.xml");

    pugi::xpath_query transport_stations_query("/dataset/transport_station");

    pugi::xpath_query num_query("number/text()");
    pugi::xpath_query routes_query("routes/text()");
    pugi::xpath_query coordinates_query("coordinates/text()");
    pugi::xpath_query type_of_vehicle_query("type_of_vehicle/text()");
    pugi::xpath_query location_query("location/text()");


    database *data;
    database bus_data;
    database trolleybus_data;
    database tram_data;
    std::map<std::string, int> street_counts;
    std::set<std::string> streets;

    pugi::xpath_node_set xpath_stations = doc.select_nodes(transport_stations_query);
    for (pugi::xpath_node xpath_station : xpath_stations) {

        pugi::xml_node station = xpath_station.node();


        pugi::xml_node num = station.select_node(num_query).node();
        pugi::xml_node routes = station.select_node(routes_query).node();
        pugi::xml_node coordinates = station.select_node(coordinates_query).node();
        pugi::xml_node vehicle = station.select_node(type_of_vehicle_query).node();
        pugi::xml_node location = station.select_node(location_query).node();


        //std::cout << "name: " << name.value() << std::endl;
        //std::cout << "num: " << num.value() << std::endl;
        //std::cout << "routes: " << routes.value() << std::endl;
        //std::cout << "coordinates: " << coordinates.value() << std::endl;
        //std::cout << "vehicle : " << vehicle.value() << std::endl;
        //std::cout << "location: " << location.value() << std::endl;
        //std::cout << "-----------------------------------------------" << std::endl;

        std::vector<std::string> routes_from_xml = split(routes.value(), ',');
        std::vector<std::string> cord = split(coordinates.value(), ',');
        long int x = reform(cord[0]);
        long int y = reform(cord[1]);


        //std::cout<<"x = "<<x<<std::endl;
        //std::cout<<"y = "<<y<<std::endl;

        std::string veh = vehicle.value();
        //FIXED tralley is not a word
        if (veh == "Автобус")
            data = &bus_data;
        else if (veh == "Трамвай")
            data = &tram_data;
        else
            data = &trolleybus_data;

        std::string loc = location.value();
        if (loc != "")
            street_counts[loc] += 1;

        for (std::string i: routes_from_xml) {
            if ((*data).rout_length_.count(i) == 0) {
                (*data).rout_length_[i] = 0;
            } else {

                auto[start, ends] = (*data).routes_.equal_range(i);
                ends--;
                auto[name, cords_station] = (*ends);
                auto[station, cords] = cords_station;
                auto[xOld, yOld] = cords;

                (*data).rout_length_[i] += find_length(x, y, xOld, yOld);
            }

            (*data).routes_.insert(
                    std::pair<std::string, std::pair<std::string, std::pair<long, long>>>(i, make_pair(num.value(),
                                                                                                       std::make_pair(x,
                                                                                                                      y))));
            (*data).name_of_routes_.insert(i);


        }


    }
    std::cout << "Routes with most stops: " << std::endl;
    std::cout << "\tbus: " << max_counts_station(bus_data) << std::endl;
    std::cout << "\ttrall: " << max_counts_station(trolleybus_data) << std::endl;
    //FIXED trrr is tractor from Uncle Fedor
    std::cout << "\ttram: " << max_counts_station(tram_data) << std::endl;
    std::cout << "-------------------------" << std::endl;
    std::cout << "The most longest routes: " << std::endl;
    std::cout << "\tbus " << max_length_route(bus_data) << std::endl;
    std::cout << "\ttrall " << max_length_route(trolleybus_data) << std::endl;
    std::string answer1 = max_length_route(tram_data);
    std::cout << "\ttram " << answer1 << std::endl;
    std::cout << "--------------------------" << std::endl;
    std::cout << "Most popular street " << max_station_street(street_counts) << std::endl;


    return 0;
}
