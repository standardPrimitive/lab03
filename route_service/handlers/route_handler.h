#ifndef ROUTE_HANDLER_H
#define ROUTE_HANDLER_H

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

#include "../../database/route.h"
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

class RouteHandler : public HTTPRequestHandler
{
private:
    bool check_id_user(const std::string &id_user, std::string &reason)
    {
        long id = atol(id_user.c_str());
        if (!database::User::check_user_exists_by_id(id))
        {
            std::ostringstream oss;
            oss << "User with id: " << id << "doesn't exist.";
            reason = oss.str();
            return false;
        }

        return true;
    };

    bool check_id_route(const std::string &id_route, std::string &reason)
    {
        if (id_route.find('\t') != std::string::npos)
        {
            reason = "id_route can't contain character tabulation";
            return false;
        }
        if (id_route.find('\n') != std::string::npos)
        {
            reason = "id_route can't contain character '\\n'";
            return false;
        }

        return true;
    };

    bool check_point_start(const std::string &point_start, std::string &reason)
    {
        if (point_start.find('\t') != std::string::npos)
        {
            reason = "point_start can't contain character tabulation";
            return false;
        }
        if (point_start.find('\n') != std::string::npos)
        {
            reason = "point_start can't contain character '\\n'";
            return false;
        }

        return true;
    };

    bool check_point_end(const std::string &point_start, std::string &reason)
    {
        if (point_start.find('\t') != std::string::npos)
        {
            reason = "point_end can't contain character tabulation";
            return false;
        }
        if (point_start.find('\n') != std::string::npos)
        {
            reason = "point_end can't contain character '\\n'";
            return false;
        }

        return true;
    };

public:
    RouteHandler(const std::string &format) : _format(format)
    {
    }

    void handleRequest(HTTPServerRequest &request,
                       HTTPServerResponse &response)
    {
        HTMLForm form(request, request.stream());
        try
        {
            if (form.has("id_user") 
                && (request.getMethod() == Poco::Net::HTTPRequest::HTTP_GET) // 5. Получение маршрутов пользователя
                && (hasSubstr(request.getURI(), "/routes")))
                {
                    long _id_user = atol(form.get("id_user").c_str());
                    auto results = database::Route::read_by_user_id(_id_user);
                    if (!results.empty())    
                        {
                            auto result_json = database::Route::vectorToJSON(results);
                            response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
                            response.setChunkedTransferEncoding(true);
                            response.setContentType("application/json");
                            std::ostream &ostr = response.send();
                            Poco::JSON::Stringifier::stringify(result_json, ostr);
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
                            root->set("detail", "user not found");
                            root->set("instance", "/user");
                            std::ostream &ostr = response.send();
                            Poco::JSON::Stringifier::stringify(root, ostr);
                            return;
                        }
                }
            else if (form.has("id_user") 
                    //&& form.has("id_route") 
                    && form.has("point_start") 
                    && form.has("point_end") 
                    && (request.getMethod() == Poco::Net::HTTPRequest::HTTP_POST) // 4. Создание маршрута пользователя
                    && (hasSubstr(request.getURI(), "/routes")))

                {
                    bool check_result = true;
                    std::string message;
                    std::string reason;

                    if (!check_id_user(form.get("id_user"), reason))
                    {
                        check_result = false;
                        message += reason;
                        message += "<br>";
                    }

                    // if (!check_id_route(form.get("id_route"), reason))
                    // {
                    //     check_result = false;
                    //     message += reason;
                    //     message += "<br>";
                    // }

                    if (!check_point_start(form.get("point_start"), reason))
                    {
                        check_result = false;
                        message += reason;
                        message += "<br>";
                    }

                    if (!check_point_end(form.get("point_end"), reason))
                    {
                        check_result = false;
                        message += reason;
                        message += "<br>";
                    }

                    if (check_result)
                    {
                        database::Route route;
                        route.id_user() = atol(form.get("id_user").c_str());
                        //route.id_route() = atol(form.get("id_route").c_str());
                        route.point_start() = form.get("point_start");
                        route.point_end() = form.get("point_end");
                        route.save_to_mysql();
                        response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
                        response.setChunkedTransferEncoding(true);
                        response.setContentType("application/json");
                        std::ostream &ostr = response.send();
                        ostr << route.get_id_route();
                        return;
                    }
                    else
                    {
                        response.setStatus(Poco::Net::HTTPResponse::HTTP_NOT_FOUND);
                        std::ostream &ostr = response.send();
                        ostr << message;
                        response.send();
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
        root->set("instance", "/route");
        std::ostream &ostr = response.send();
        Poco::JSON::Stringifier::stringify(root, ostr);
    }

private:
    std::string _format;
};
#endif