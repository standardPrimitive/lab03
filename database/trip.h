#ifndef TRIP_H
#define TRIP_H

#include <string>
#include <vector>
#include "Poco/JSON/Object.h"
#include <optional>

namespace database
{
    class Trip{
        private:
            long _id;
            long _route_ID;
            long _driver;
            //long _user;
            std::string _date_depart;
            std::string _travel_conditions;
            int _price;
            //доп таблица
            int _user_id;
            int _trip_id;
            

        public:
            static Trip fromJSON(const std::string & str);

            //+
            long                get_id() const;
            long                get_route_ID() const;
            long                get_driver() const;
            //long                get_user() const;
            const std::string   &get_date_depart() const;
            const std::string   &get_travel_conditions() const;
            const int           &get_price() const;
            const int            &get_user_id() const;
            const int            &get_trip_id() const;
            

            long&         id();
            long         &route_ID();
            long         &driver();
            //long         &user();
            std::string  &date_depart();
            std::string  &travel_conditions();
            int          &price();
            //-
            int          &user_id();
            int          &trip_id();
            
            static void init();
            static std::optional<Trip> read_by_id(long id);
            static std::vector<Trip> read_all();
            void save_to_mysql();
            void connect_user_to_trip();
            //static std::vector<Trip> search_route(std::string login); // узнать маршрут по поездке

            Poco::JSON::Object::Ptr toJSON() const;
    };
}

#endif