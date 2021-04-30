#include <iostream>
#include "pugixml.hpp"
#include <map>
#include <string>
#include <vector>
#include <sstream>
#include <set>
#include <cmath>


//todo dont use using namespace
using namespace std;
using namespace pugi;

//todo S P A C E S



vector<string> split (const string &s, char delim) {
    vector<string> result;
    stringstream ss (s);
    string item;

    while (getline (ss, item, delim)) {
        result.push_back (item);
    }

    return result;
}



class database{
public:

    database() = default;

    multimap<string,pair<string ,pair<long int,long int>>> routes_;
    map<string,double> rout_length_;
    set<string> name_of_routes_;


};

long int reform(string cord){
    long int x = 0;

    for (char i: cord) {
        if(i!='.')
            x = x*10 + ((int)i - (int)'0') ;
    }
    return x;
}


string max_length_route(database& data){
    double maxlength = 0;
    string route;
    for(auto i:data.name_of_routes_){
        if(data.rout_length_[i]>maxlength){
            maxlength = data.rout_length_[i];
            route = i;
        }
    }
    return route;
}

string max_station_street(map<string,int> &street_counts){
    int count = 0;
    int maxcount = 0;
    string street_ans;

    //todo range based for
    for(auto i = street_counts.begin(); i!=street_counts.end();i++){
        auto [street, count] = *i;
        if(maxcount<count) {
            maxcount = count;
            street_ans = street;
        }
    }

    return street_ans;
}

//todo const&
string max_counts_station(database &data){
    int count = 0;
    int maxcount = 0;
    string route;
    for(auto i: data.name_of_routes_) {
       count = data.routes_.count(i);
       if(count>maxcount) {
           maxcount = count;
           route = i;
       }
    }

    return route;

}

double find_length(long x, long y, long xOld, long yOld){
    double ans = 0;
    ans = sqrt((x-xOld)*(x-xOld) + (y-yOld)*(y-yOld));

    return ans;
}


int main(){

    xml_document doc;

    xml_parse_result result = doc.load_file("data.xml");

    xpath_query transport_stations_query("/dataset/transport_station");

    xpath_query num_query("number/text()");
    xpath_query name_stopping_query("name_stopping/text()");
    xpath_query routes_query("routes/text()");
    xpath_query coordinates_query("coordinates/text()");
    xpath_query type_of_vehicle_query("type_of_vehicle/text()");
    xpath_query location_query("location/text()");



    database* data;
    database bus_data;
    database tralley_data;
    database tram_data;
    map<string, int> street_counts;
    set<string> streets;

    xpath_node_set xpath_stations = doc.select_nodes(transport_stations_query);
    for (xpath_node xpath_station : xpath_stations)
    {

        xml_node station = xpath_station.node();

        xml_node name = station.select_node(name_stopping_query).node();
        xml_node num = station.select_node(num_query).node();
        xml_node routes = station.select_node(routes_query).node();
        xml_node coordinates = station.select_node(coordinates_query).node();
        xml_node vehicle = station.select_node(type_of_vehicle_query).node();
        xml_node location = station.select_node(location_query).node();


        //cout << "name: " << name.value() << endl;
        //cout << "num: " << num.value() << endl;
        //cout << "routes: " << routes.value() << endl;
        //cout << "coordinates: " << coordinates.value() << endl;
        //cout << "vehicle : " << vehicle.value() << endl;
        //cout << "location: " << location.value() << endl;
        //cout << "-----------------------------------------------" << endl;

        vector<string> routes_from_xml = split(routes.value(),',');
        vector<string> cord = split(coordinates.value(),',');
        long int x = reform(cord[0]);
        long int y = reform(cord[1]);


        //cout<<"x = "<<x<<endl;
        //cout<<"y = "<<y<<endl;

        string veh = vehicle.value();
        //todo tralley is not a word
        if(veh == "Автобус")
            data = &bus_data;
        else if (veh == "Трамвай")
            data = &tram_data;
        else
            data = &tralley_data;

        string loc = location.value();
        if(loc!="")
            street_counts[loc]+=1;

        for(string i: routes_from_xml){
            if((*data).rout_length_.count(i) == 0) {
                (*data).rout_length_[i] = 0;
            } else{

                auto [start,ends] = (*data).routes_.equal_range(i);
                ends--;
                auto[name,cords_station] = (*ends);
                auto[station, cords] = cords_station;
                auto[xOld,yOld] = cords;

                (*data).rout_length_[i]+=find_length(x,y,xOld,yOld);
            }

            (*data).routes_.insert(pair<string,pair<string,pair<long,long>>>(i,make_pair(num.value(),make_pair(x,y))));
            (*data).name_of_routes_.insert(i);



        }





    }
    cout<<"Routes with most stops: "<<endl;
    cout<<"\tbus: "<<max_counts_station(bus_data)<<endl;
    cout<<"\ttrall: "<<max_counts_station(tralley_data)<<endl;
    //todo trrr is tractor from Uncle Fedor
    string trrr = max_counts_station(tram_data);
    cout<<"\ttram: "<<trrr<<endl;
    cout<<"-------------------------"<<endl;
    cout<<"The most longest routes: "<<endl;
    cout<<"\tbus "<<max_length_route(bus_data)<<endl;
    cout<<"\ttrall "<<max_length_route(tralley_data)<<endl;
    string answer1 = max_length_route(tram_data);
    cout<<"\ttram "<<answer1<<endl;
    cout<<"--------------------------"<<endl;
    cout<<"Most popular street " << max_station_street(street_counts) << endl;








    return 0;
}
