#include "user.h"
#include "database.h"
#include "../config/config.h"
#include "cache.h"

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

    void User::init()
    {
        try
        {

            Poco::Data::Session session = database::Database::get().create_session();
            Statement create_stmt(session);
            create_stmt << "CREATE TABLE IF NOT EXISTS `User` (`id` INT NOT NULL AUTO_INCREMENT,"
                        << "`first_name` VARCHAR(256) NOT NULL,"
                        << "`last_name` VARCHAR(256) NOT NULL,"
                        << "`login` VARCHAR(256) NOT NULL,"
                        << "`password` VARCHAR(256) NOT NULL,"
                        << "`email` VARCHAR(256) NOT NULL,"
                        //bdv
                        << "`phone` VARCHAR(256) NOT NULL,"
                        //<< "`status` VARCHAR(256) NOT NULL,"
                        //bdv
                        << "PRIMARY KEY (`id`),KEY `fn` (`first_name`),KEY `ln` (`last_name`),"
                        << "UNIQUE KEY `unique_email` (`email`),"
                        << "UNIQUE KEY `unique_phone` (`phone`));",
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

    Poco::JSON::Object::Ptr User::toJSON() const
    {
        Poco::JSON::Object::Ptr root = new Poco::JSON::Object();

        root->set("id", _id);
        root->set("first_name", _first_name);
        root->set("last_name", _last_name);
        root->set("email", _email);
        root->set("login", _login);
        root->set("password", _password);
        //bdv
        root->set("phone", _phone);
        //root->set("status", _status);
        //bdv
        return root;
    }

    User User::fromJSON(const std::string &str)
    {
        User user;
        Poco::JSON::Parser parser;
        Poco::Dynamic::Var result = parser.parse(str);
        Poco::JSON::Object::Ptr object = result.extract<Poco::JSON::Object::Ptr>();

        user.id() = object->getValue<long>("id");
        user.first_name() = object->getValue<std::string>("first_name");
        user.last_name() = object->getValue<std::string>("last_name");
        user.email() = object->getValue<std::string>("email");
        user.login() = object->getValue<std::string>("login");
        user.password() = object->getValue<std::string>("password");
        //bdv
        user.phone() = object->getValue<std::string>("phone");
        //user.status() = object->getValue<std::string>("status");
        //bdv
        return user;
    }

    std::optional<long> User::auth(std::string &login, std::string &password)
    {
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Poco::Data::Statement select(session);
            long id;
            select << "SELECT id FROM User where login=? and password=?",
                into(id),
                use(login),
                use(password),
                range(0, 1); //  iterate over result set one row at a time

            select.execute();
            Poco::Data::RecordSet rs(select);
            if (rs.moveFirst()) return id;
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
    std::optional<User> User::read_by_id(long id)
    {
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Poco::Data::Statement select(session);
            User a;
            select << "SELECT id, first_name, last_name, email, login, password, phone FROM User where id=?",
                into(a._id),
                into(a._first_name),
                into(a._last_name),
                into(a._email),
                into(a._login),
                into(a._password),
                //bdv
                into(a._phone),
                //into(a._status),
                //bdv
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

    std::optional<User> User::read_by_login(std::string login)
    {
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Poco::Data::Statement select(session);
            User a;
            select << "SELECT id, first_name, last_name, email, login, password, phone FROM User where login=?",
                into(a._id),
                into(a._first_name),
                into(a._last_name),
                into(a._email),
                into(a._login),
                into(a._password),
                //bdv
                into(a._phone),
                //into(a._status),
                //bdv
                use(login),
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

    std::vector<User> User::read_all()
    {
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Statement select(session);
            std::vector<User> result;
            User a;
            select << "SELECT id, first_name, last_name, email, login, password, phone FROM User",
                into(a._id),
                into(a._first_name),
                into(a._last_name),
                into(a._email),
                into(a._login),
                into(a._password),
                //bdv
                into(a._phone),
               //into(a._status),
                //bdv
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

    std::vector<User> User::search(std::string first_name, std::string last_name)
    {
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Statement select(session);
            std::vector<User> result;
            User a;
            first_name += "%";
            last_name += "%";
            select << "SELECT id, first_name, last_name, email, login, password, phone FROM User where first_name LIKE ? and last_name LIKE ?",
                into(a._id),
                into(a._first_name),
                into(a._last_name),
                into(a._email),
                into(a._login),
                into(a._password),
                //bdv
                into(a._phone),
                //into(a._status),
                //bdv
                use(first_name),
                use(last_name),
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

  bool User::check_user_exists_by_id(long id)
    {
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Statement select(session);
            int count = 0;
            
            select << "SELECT COUNT(*) FROM `User` WHERE id = ?",
                into(count),
                use(id);
            
            select.execute();

            if (count > 0)
                return true;
            else
                return false;
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

    void User::save_to_mysql()
    {

        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Poco::Data::Statement insert(session);

            insert << "INSERT INTO User (first_name,last_name,email,login,password,phone) VALUES(?, ?, ?, ?, ?, ?)", //bdv
                use(_first_name),
                use(_last_name),
                use(_email),
                use(_login),
                use(_password);
                //bdv
                use(_phone);
                //use(_status);
                //bdv

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

    std::optional<User> User::read_from_cache_by_id(long id)
    {

        try
        {
            std::string result;
            if (database::Cache::get().get(id, result))
                return fromJSON(result);
            else
                return std::optional<User>();
        }
        catch (std::exception& err)
        {
           // std::cerr << "error:" << err.what() << std::endl;
            return std::optional<User>();
        }
    }

    void User::save_to_cache()
    {
        std::stringstream ss;
        Poco::JSON::Stringifier::stringify(toJSON(), ss);
        std::string message = ss.str();
        database::Cache::get().put(_id, message);
    }

    const std::string &User::get_login() const
    {
        return _login;
    }

    const std::string &User::get_password() const
    {
        return _password;
    }

    std::string &User::login()
    {
        return _login;
    }

    std::string &User::password()
    {
        return _password;
    }

    long User::get_id() const
    {
        return _id;
    }

    const std::string &User::get_first_name() const
    {
        return _first_name;
    }

    const std::string &User::get_last_name() const
    {
        return _last_name;
    }

    const std::string &User::get_email() const
    {
        return _email;
    }

    long &User::id()
    {
        return _id;
    }

    std::string &User::first_name()
    {
        return _first_name;
    }

    std::string &User::last_name()
    {
        return _last_name;
    }

    std::string &User::email()
    {
        return _email;
    }
    //bdv
    std::string &User::phone()
    {
        return _phone;
    }

    // std::string &User::status()
    // {
    //     return _status;
    // }

    const std::string &User::get_phone() const
    {
        return _phone;
    }

    // const std::string &User::get_status() const
    // {
    //     return _status;
    // }
}