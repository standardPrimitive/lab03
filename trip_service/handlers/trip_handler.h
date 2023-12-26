#ifndef TRIP_HANDLER_H
#define TRIP_HANDLER_H

#include "Poco/Net/HTTPServer.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/HTMLForm.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Timestamp.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/DateTimeFormat.h"
#include "Poco/Exception.h"
#include "Poco/ThreadPool.h"
#include "Poco/Util/ServerApplication.h"
#include "Poco/Util/Option.h"
#include "Poco/Util/OptionSet.h"
#include "Poco/Util/HelpFormatter.h"
#include <iostream>
#include <iostream>
#include <fstream>

using Poco::DateTimeFormat;
using Poco::DateTimeFormatter;
using Poco::ThreadPool;
using Poco::Timestamp;
using Poco::Net::HTMLForm;
using Poco::Net::HTTPRequestHandler;
using Poco::Net::HTTPRequestHandlerFactory;
using Poco::Net::HTTPServer;
using Poco::Net::HTTPServerParams;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;
using Poco::Net::NameValueCollection;
using Poco::Net::ServerSocket;
using Poco::Util::Application;
using Poco::Util::HelpFormatter;
using Poco::Util::Option;
using Poco::Util::OptionCallback;
using Poco::Util::OptionSet;
using Poco::Util::ServerApplication;

#include "../../database/trip.h"
#include "../../helper.h"


static bool hasSubstr(const std::string &str, const std::string &substr)
{
    if (str.size() < substr.size())
        return false;
    for (size_t i = 0; i <= str.size() - substr.size(); ++i)
    {
        bool ok{true};
        for (size_t j = 0; ok && (j < substr.size()); ++j)
            ok = (str[i + j] == substr[j]);
        if (ok)
            return true;
    }
    return false;
}

class TripHandler : public HTTPRequestHandler
{
// private:
//     bool check_id_user(const std::string &id_user, std::string &reason)
//     {
//         if (id_user.find('\t') != std::string::npos)
//         {
//             reason = "User id can't contain character tabulation";
//             return false;
//         }
//         if (id_user.find('\n') != std::string::npos)
//         {
//             reason = "User id can't contain character '\\n'";
//             return false;
//         }

//         return true;
//     };

//     bool check_description(const std::string &description, std::string &reason)
//     {
//         if (description.find('\t') != std::string::npos)
//         {
//             reason = "description can't contain character tabulation";
//             return false;
//         }
//         if (description.find('\n') != std::string::npos)
//         {
//             reason = "description can't contain character '\\n'";
//             return false;
//         }

//         return true;
//     };

//     bool check_permission(const std::string &permission, std::string &reason)
//     {
//         if (permission != "true" || permission != "false")
//         {
//             std::ostringstream oss;
//             oss << "Wrong record_permission identifier: " << permission << ".";
//             reason = oss.str();
//             return false;
//         }

//         return true;
//     };

//     bool check_author_id(const std::string &author_id, std::string &reason)
//     {
//         long id = atol(author_id.c_str());
//         if (!database::User::check_user_exists_by_id(id))
//         {
//             std::ostringstream oss;
//             oss << "User with id: " << author_id << "doesn't exist.";
//             reason = oss.str();
//             return false;
//         }

//         return true;
//     };

//     bool check_conference_id(const std::string &conference_id, std::string &reason)
//     {
//         long id = atol(conference_id.c_str());
//         if (!database::Conference::check_conference_exists_by_id(id))
//         {
//             std::ostringstream oss;
//             oss << "Conference with id: " << conference_id << "doesn't exist.";
//             reason = oss.str();
//             return false;
//         }

//         return true;
//     };

//     bool query_string_boolean_to_bool(const std::string &str) 
//     {
//         if (str == "true")
//             return true;
//         else
//             return false;
//     }

public:
    TripHandler(const std::string &format) : _format(format)
    {
    }

    void handleRequest(HTTPServerRequest &request,
                       HTTPServerResponse &response)
    {
        HTMLForm form(request, request.stream());
        try
        {

            if (form.has("id") && hasSubstr(request.getURI(), "/trips")) // п.8 Получение информации о поездке
            {
                long id = atol(form.get("id").c_str());
                std::optional<database::Trip> result = database::Trip::read_by_id(id);
                if (result)
                {
                    response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
                    response.setChunkedTransferEncoding(true);
                    response.setContentType("application/json");
                    std::ostream &ostr = response.send();
                    Poco::JSON::Stringifier::stringify(result->toJSON(), ostr);
                    return;
                }
                else
                {
                    response.setStatus(Poco::Net::HTTPResponse::HTTPStatus::HTTP_NOT_FOUND);
                    response.setChunkedTransferEncoding(true);
                    response.setContentType("application/json");
                    Poco::JSON::Object::Ptr root = new Poco::JSON::Object();
                    root->set("type", "/errors/not_found");
                    root->set("title", "Internal exception");
                    root->set("status", "404");
                    root->set("detail", "trip not found");
                    root->set("instance", "/trip");
                    std::ostream &ostr = response.send();
                    Poco::JSON::Stringifier::stringify(root, ostr);
                    return;
                }
            }
            else if (hasSubstr(request.getURI(), "/read_all")) // получить все
            {
                auto results = database::Trip::read_all();
                Poco::JSON::Array arr;
                for (auto s : results)
                    arr.add(s.toJSON());
                response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
                response.setChunkedTransferEncoding(true);
                response.setContentType("application/json");
                std::ostream &ostr = response.send();
                Poco::JSON::Stringifier::stringify(arr, ostr);
                return;
            }
            else if (request.getMethod() == Poco::Net::HTTPRequest::HTTP_POST && hasSubstr(request.getURI(), "/trips")) // п.6 Добавить поездку
            {
                if (form.has("route_ID") && form.has("driver") && form.has("date_depart") && form.has("travel_conditions") && form.has("price"))
                {
                    database::Trip trip;
                    trip.route_ID() = stof(form.get("route_ID"));
                    trip.driver() = stof(form.get("driver"));
                   // trip.user() = stof(form.get("user"));
                    trip.date_depart() = form.get("date_depart");
                    trip.travel_conditions() = form.get("travel_conditions");
                    trip.price() = stof(form.get("price"));
                    trip.save_to_mysql();
                    response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
                    response.setChunkedTransferEncoding(true);
                    response.setContentType("application/json");
                    std::ostream &ostr = response.send();
                    ostr << trip.get_id();
                    return;
                }
            }
            else if (request.getMethod() == Poco::Net::HTTPRequest::HTTP_POST && hasSubstr(request.getURI(), "/connect")) // п.7 Подключение пользователя к поездке (в разработке)
            {
                if (form.has("user_id") && form.has("trip_id"))
                {
                    database::Trip trip;
                    trip.user_id() = stof(form.get("user_id"));
                    trip.trip_id() = stof(form.get("trip_id"));
                    trip.connect_user_to_trip();
                    response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
                    response.setChunkedTransferEncoding(true);
                    response.setContentType("application/json");
                    std::ostream &ostr = response.send();
                    ostr << trip.get_id();
                    return;
                }
            }
        }
        catch (...)
        {
        }

        response.setStatus(Poco::Net::HTTPResponse::HTTPStatus::HTTP_NOT_FOUND);
        response.setChunkedTransferEncoding(true);
        response.setContentType("application/json");
        Poco::JSON::Object::Ptr root = new Poco::JSON::Object();
        root->set("type", "/errors/not_found");
        root->set("title", "Internal exception");
        root->set("status", Poco::Net::HTTPResponse::HTTPStatus::HTTP_NOT_FOUND);
        root->set("detail", "request not found");
        root->set("instance", "/trip");
        std::ostream &ostr = response.send();
        Poco::JSON::Stringifier::stringify(root, ostr);
    }

private:
    std::string _format;
};
#endif