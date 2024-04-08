#ifndef AUTHORIZATION_H_
#define AUTHORIZATION_H_

#include "chrono"
#include "unordered_map"
#include <jwt.h>

class SessionHelper {
    public:
        std::string jwtToken;
        std::chrono::_V2::system_clock::time_point created_at;
        std::chrono::minutes valid_upto;
        // static std::unordered_map<std::string, Session>sessionMap;

        SessionHelper()
        {
        }

        bool session_expire()
        {
            return (created_at + valid_upto) >= std::chrono::system_clock::now();
        }

        bool jwtExpire()
        {
            return jwt::decode(jwtToken).get_expires_at() >= std::chrono::system_clock::now();
        }

        std::string getHash()
        {
            std::string id;
            for(int i=jwtToken.length()-i;i>=0 && jwtToken[i]!='.';i--) id+=jwtToken[i];
            std::cout<<"Is is "<<id<<std::endl;
            return id;
        }

        std::chrono::_V2::system_clock::time_point generateToken(std::string &email, std::string &password)
        {
            const int arraySize = 24;
            char charArray[arraySize];

            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<int> dis(std::numeric_limits<char>::min(), std::numeric_limits<char>::max());

            std::generate(charArray, charArray + arraySize, [&]() { return static_cast<char>(dis(gen)); });

            std::string jti = jwt::base::encode<jwt::alphabet::base64url>(std::string{reinterpret_cast<const char*>(charArray), sizeof(charArray)});

            std::chrono::_V2::system_clock::time_point now = std::chrono::system_clock::now();

            std::string token = jwt::create()
                        .set_issuer("auth0")
                        .set_type("JWT")
                        .set_id(jti)
                        .set_issued_at(now)
                        .set_expires_at(now + std::chrono::minutes(5))
                        .set_payload_claim("email", jwt::claim(email))
                        .set_payload_claim("password", jwt::claim(password))
                        .sign(jwt::algorithm::hs256("secret"));

            created_at = now;
            valid_upto = std::chrono::minutes(5);
            jwtToken = token;

            return now;
            // Session s(token,now);
            // s.sessionHash = s.getHash(token);
            // sessionMap.insert({s.sessionHash,s});
            // user.insert(email);
        }
};

class Session {
public:

    std::unordered_map<std::string, SessionHelper>sessionMap;
    std::unordered_map<std::string,std::chrono::_V2::system_clock::time_point>user;

    std::string create_session(std::string &jwt,std::string &email, std::string &password)
    {
        if(user.find(email)!=user.end())
        {
            SessionHelper s;
            std::chrono::_V2::system_clock::time_point now = s.generateToken(email,password);
            user[email] = now;
            sessionMap[s.getHash()] = s;
            return "Session Ceated";
        }
        return "";
    }

    static Session& getInstance() {
        static Session instance;
        return instance;
    }

    private:
    Session() {};
};

#endif