#ifndef ROUTE_H
#define ROUTE_H

#include <string>
#include <vector>
#include "Poco/JSON/Object.h"
#include <optional>
#include "user.h"

namespace database
{
    class Route{
        private:
            long _id_route;
            long _id_user;
            std::string _point_start;
            std::string _point_end;

        public:
            static Route fromJSON(const std::string &str);

            long                 get_id_route() const;
            const long          &get_id_user() const;
            const std::string   &get_point_start() const;
            const std::string   &get_point_end() const;

            long         &id_route();
            long         &id_user();
            std::string  &point_start();
            std::string  &point_end();

            static void init();
            static void add_route(Route Route); //добавление маршрута
            static std::vector<Route> read_by_user_id(long _id_user);
            static void remove_route(Route Route); // удаление маршрута
            void save_to_mysql();

            Poco::JSON::Object::Ptr toJSON() const;
            static Poco::JSON::Array::Ptr vectorToJSON(std::vector<Route>& routes);
    };
}

#endif