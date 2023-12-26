#include "trip.h"
#include "user.h"
#include "database.h"
#include "../config/config.h"

#include <Poco/Data/MySQL/Connector.h>
#include <Poco/Data/MySQL/MySQLException.h>
#include <Poco/Data/SessionFactory.h>
#include <Poco/Data/RecordSet.h>
#include <Poco/JSON/Parser.h>
#include <Poco/Dynamic/Var.h>

#include <sstream>
#include <exception>

using namespace Poco::Data::Keywords;
using Poco::Data::Session;
using Poco::Data::Statement;

namespace database
{

    void Trip::init()
    {
        try
        {
            
            Poco::Data::Session session = database::Database::get().create_session();
            Statement create_stmt(session);

            create_stmt << "CREATE TABLE IF NOT EXISTS `Trip` (`id` INT NOT NULL AUTO_INCREMENT,"
                        << "`route_ID` int(11) NOT NULL,"
                        << "`driver` int(11) NOT NULL,"
                       // << "`user` int(11) NOT NULL,"
                        << "`date_depart` VARCHAR(256) NOT NULL,"
                        << "`travel_conditions` VARCHAR(256) NULL,"
                        << "`price` int(5) NOT NULL,"
                        << "PRIMARY KEY (`id`),"
                        //<< "FOREIGN KEY (`user`) REFERENCES `User` (`id`),"
                        << "FOREIGN KEY (`route_ID`) REFERENCES `Route` (`id_route`));",  

                now;

        }

        catch (Poco::Data::MySQL::ConnectionException &e)
        {
            std::cout << "connection:" << e.what() << std::endl;
            throw;
        }
        catch (Poco::Data::MySQL::StatementException &e)
        {

            std::cout << "statement:" << e.what() << std::endl;
            throw;
        }
    }

    Poco::JSON::Object::Ptr Trip::toJSON() const
    {
        Poco::JSON::Object::Ptr root = new Poco::JSON::Object();

        root->set("id", _id);
        root->set("route_ID", _route_ID);
        root->set("driver", _driver);
        //root->set("user", _user);
        root->set("date_depart", _date_depart);
        root->set("travel_conditions", _travel_conditions);
        root->set("price", _price);
        return root;
    }

    Trip Trip::fromJSON(const std::string &str)
    {
        Trip Trip;
        Poco::JSON::Parser parser;
        Poco::Dynamic::Var result = parser.parse(str);
        Poco::JSON::Object::Ptr object = result.extract<Poco::JSON::Object::Ptr>();

        Trip.id() = object->getValue<long>("id");
        Trip.route_ID() = object->getValue<long>("route_ID");
        Trip.driver() = object->getValue<long>("driver");
        //Trip.user() = object->getValue<long>("user");
        Trip.date_depart() = object->getValue<std::string>("date_depart");
        Trip.travel_conditions() = object->getValue<std::string>("travel_conditions");
        Trip.price() = object->getValue<int>("price");
        //-
        return Trip;
    }

    std::optional<Trip> Trip::read_by_id(long id) // получить данные по поездке
    {
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Poco::Data::Statement select(session);
            Trip a;
            select << "SELECT id, route_ID, driver, date_depart, travel_conditions, price FROM Trip where id=?",
                into(a._id),
                into(a._route_ID),
                into(a._driver),
                //into(a._user),
                into(a._date_depart),
                into(a._travel_conditions),
                into(a._price),
                use(id),
                range(0, 1); //  iterate over result set one row at a time

            select.execute();
            Poco::Data::RecordSet rs(select);
            if (rs.moveFirst()) return a;
        }

        catch (Poco::Data::MySQL::ConnectionException &e)
        {
            std::cout << "connection:" << e.what() << std::endl;
        }
        catch (Poco::Data::MySQL::StatementException &e)
        {

            std::cout << "statement:" << e.what() << std::endl;
            
        }
        return {};
    }

    std::vector<Trip> Trip::read_all() //получить все поездки
    {
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Statement select(session);
            std::vector<Trip> result;
            Trip a;
            select << "SELECT id, route_ID, driver, date_depart, travel_conditions, price FROM Trip",
                into(a._id),
                into(a._route_ID),
                into(a._driver),
                //into(a._user),
                into(a._date_depart),
                into(a._travel_conditions),
                into(a._price),
                range(0, 1); //  iterate over result set one row at a time

            while (!select.done())
            {
                if (select.execute())
                    result.push_back(a);
            }
            return result;
        }

        catch (Poco::Data::MySQL::ConnectionException &e)
        {
            std::cout << "connection:" << e.what() << std::endl;
            throw;
        }
        catch (Poco::Data::MySQL::StatementException &e)
        {

            std::cout << "statement:" << e.what() << std::endl;
            throw;
        }
    }


    void Trip::save_to_mysql()
    {
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Poco::Data::Statement insert(session);
            //+
            insert << "INSERT INTO Trip (route_ID,driver,date_depart,travel_conditions,price) VALUES(?, ?, ?, ?, ?)",
                use(_route_ID),
                use(_driver),
                //use(_user),
                use(_date_depart),
                use(_travel_conditions),
                use(_price);
            //-
            insert.execute();
            Poco::Data::Statement select(session);
            select << "SELECT LAST_INSERT_ID()",
                into(_id),
                range(0, 1); //  iterate over result set one row at a time

            if (!select.done())
            {
                select.execute();
            }
            std::cout << "inserted:" << _id << std::endl;
        }
        catch (Poco::Data::MySQL::ConnectionException &e)
        {
            std::cout << "connection:" << e.what() << std::endl;
            throw;
        }
        catch (Poco::Data::MySQL::StatementException &e)
        {
            std::cout << "statement:" << e.what() << std::endl;
            throw;
        }
    }

    void Trip::connect_user_to_trip()
    {
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Poco::Data::Statement insert(session);
            //+
            insert << "INSERT INTO user_trip (user_id, trip_id) VALUES(?, ?)",
                use(_user_id),
                use(_trip_id);
            //-
            insert.execute();
            Poco::Data::Statement select(session);
            select << "SELECT LAST_INSERT_ID()",
                into(_id),
                range(0, 1); //  iterate over result set one row at a time

            if (!select.done())
            {
                select.execute();
            }
            std::cout << "inserted succesfully" << std::endl;
        }
        catch (Poco::Data::MySQL::ConnectionException &e)
        {
            std::cout << "connection:" << e.what() << std::endl;
            throw;
        }
        catch (Poco::Data::MySQL::StatementException &e)
        {
            std::cout << "statement:" << e.what() << std::endl;
            throw;
        }      
    }

    // std::vector<Trip> Trip::search_route(std::string login)
    // {
    //     try
    //     {
    //         Poco::Data::Session session = database::Database::get().create_session();
    //         Statement select(session);
    //         std::vector<Trip> result;
    //         Trip a;
    //         //+
    //         select << "SELECT Route.id, route_ID, driver, user, date_depart, travel_conditions, price FROM Trip INNER JOIN Route ON Trip.route_ID=Route.id GROUP BY Route.id",
    //             into(a._id),
    //             into(a._route_ID),
    //             into(a._driver),
    //             into(a._user),
    //             into(a._date_depart),
    //             into(a._travel_conditions),
    //             into(a._price),
    //             use(login),
    //             range(0, 1); //  iterate over result set one row at a time
    //         //-
    //         while (!select.done())
    //         {
    //             if (select.execute())
    //                 result.push_back(a);
    //         }

    //         return result;
    //     }

    //     catch (Poco::Data::MySQL::ConnectionException &e)
    //     {
    //         std::cout << "connection:" << e.what() << std::endl;
    //         throw;
    //     }
    //     catch (Poco::Data::MySQL::StatementException &e)
    //     {
    //         std::cout << "statement:" << e.what() << std::endl;
    //         throw;
    //     }
    // }
    //+
    long Trip::get_id() const
    {
        return _id;
    }
    
    long Trip::get_route_ID() const
    {
        return _route_ID;
    }

    long Trip::get_driver() const
    {
        return _driver;
    }

    // long Trip::get_user() const
    // {
    //     return _user;
    // }

    const std::string &Trip::get_date_depart() const
    {
        return _date_depart;
    }

    const std::string &Trip::get_travel_conditions() const
    {
        return _travel_conditions;
    }

    const int &Trip::get_price() const
    {
        return _price;
    }

    const int &Trip::get_user_id() const
    {
        return _user_id;
    }

    const int &Trip::get_trip_id() const
    {
        return _trip_id;
    }

    long &Trip::id()
    {
        return _id;
    }

    long &Trip::route_ID()
    {
        return _route_ID;
    }

    long &Trip::driver()
    {
        return _driver;
    }
    
    // long &Trip::user()
    // {
    //     return _user;
    // }

    std::string &Trip::date_depart()
    {
        return _date_depart;
    }

    std::string &Trip::travel_conditions()
    {
        return _travel_conditions;
    }

    int &Trip::price()
    {
        return _price;
    }
    int &Trip::user_id()
    {
        return _user_id;
    }
    int &Trip::trip_id()
    {
        return _trip_id;
    }
    //-
}