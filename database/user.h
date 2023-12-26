#ifndef USER_H
#define USER_H

#include <string>
#include <vector>
#include "Poco/JSON/Object.h"
#include <optional>

namespace database
{
    class User{
        private:
            long _id;
            std::string _first_name;
            std::string _last_name;
            std::string _email;
            std::string _login;
            std::string _password;
            //+bdv
            std::string _phone;
            std::string _status;
            //-bdv
        public:

            static User fromJSON(const std::string & str);

            long               get_id() const;
            const std::string &get_first_name() const;
            const std::string &get_last_name() const;
            const std::string &get_email() const;
            const std::string &get_login() const;
            const std::string &get_password() const;
            //+bdv
            const std::string &get_phone() const;
            //const std::string &get_status() const;
            //-bdv
           
            long&              id();
            std::string       &first_name();
            std::string       &last_name();
            std::string       &email();
            std::string       &login();
            std::string       &password();
            //+bdv
            std::string       &phone();
            //std::string       &status();
            //-bdv

            static void init();
            static std::optional<User> read_by_id(long id);
            static std::optional<User> read_by_login(std::string login);
            static std::optional<long> auth(std::string &login, std::string &password);
            static std::vector<User> read_all();
            static std::vector<User> search(std::string first_name,std::string last_name);
            static bool check_user_exists_by_id(long id);
            void save_to_mysql();

            void save_to_cache();
            static std::optional<User> read_from_cache_by_id(long id);

            Poco::JSON::Object::Ptr toJSON() const;

    };
}

#endif





