#ifndef AUTHORIZATION_H_
#define AUTHORIZATION_H_

#include "chrono"
#include "unordered_map"
#include "unordered_set"
#include "mutex"
#include "jwt.h"

class Session {

private:
    Session() {};

    struct User {
        std::chrono::_V2::system_clock::time_point created_at;
        std::chrono::minutes valid_upto;
        std::string email;
        std::string password;
    };

public:

    std::unordered_map<std::string, User>sessionMap;
    std::unordered_set<std::string>user;
    std::mutex lock;

    bool session_expire(User &u)
    {
        return (u.created_at + u.valid_upto) >= std::chrono::system_clock::now();
    }

    bool jwtExpire(std::string &jwtToken)
    {
        return jwt::decode(jwtToken).get_expires_at() >= std::chrono::system_clock::now();
    }

    std::string generateToken(std::string &email, std::string &password, std::chrono::_V2::system_clock::time_point &now)
    {
        const int arraySize = 24;
        char charArray[arraySize];

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dis(std::numeric_limits<char>::min(), std::numeric_limits<char>::max());

        std::generate(charArray, charArray + arraySize, [&]() { return static_cast<char>(dis(gen)); });

        std::string jti = jwt::base::encode<jwt::alphabet::base64url>(std::string{reinterpret_cast<const char*>(charArray), sizeof(charArray)});

        std::string token = jwt::create()
                    .set_issuer("auth0")
                    .set_type("JWT")
                    .set_id(jti)
                    .set_issued_at(now)
                    .set_expires_at(now + std::chrono::minutes(5))
                    .set_payload_claim("email", jwt::claim(email))
                    .set_payload_claim("password", jwt::claim(password))
                    .sign(jwt::algorithm::hs256("secret"));

        return token;
    }


    bool verifyJwt(std::string &email, std::string &password, std::string &jwtToken)
    {
        auto decoded = jwt::decode(jwtToken);
        std::string email_now = decoded.get_payload_claim("email").as_string();
        std::string password_now = decoded.get_payload_claim("email").as_string();

        return (email_now == email) && (password_now == password);
    }

    std::string getHash(std::string &jwtToken)
    {
        std::string id;
        for(int i=jwtToken.length()-1;i>=0 && jwtToken[i]!='.';i--) id+=jwtToken[i];
        std::cout<<"Id is "<<id<<std::endl;
        return id;
    }

    std::pair<bool,std::string> create_session(std::string &jwtToken,std::string &email, std::string &password)
    {
        User u;

        if(user.find(email)!= user.end()){
            if(jwtToken=="") return {false, "Enter token"};
            bool is_valid = verifyJwt(email,password,jwtToken);
            if(!is_valid) return {false, "Invalid token"};

            std::string id = getHash(jwtToken);
            u = sessionMap[id];
        }

        if(jwtToken!="" && !session_expire(u) && !jwtExpire(jwtToken)) return {true,jwtToken};
        if(jwtToken!="") destroy_session(jwtToken,email,password);

        std::chrono::_V2::system_clock::time_point now = std::chrono::system_clock::now();
        std::string jwtToken = generateToken(email,password,now);
        u.created_at = now;
        u.valid_upto = std::chrono::minutes(5);
        std::lock_guard<std::mutex>guard(lock);
        user.insert(email);
        sessionMap[getHash(jwtToken)] = u;
        return {true,jwtToken};
    }

    std::pair<bool,std::string> destroy_session(std::string &jwtToken,std::string &email, std::string &password)
    {

        if(jwtToken=="") return {false, "Enter token"};

        if(user.find(email)!= user.end()) return {false, "Session isnt created for this user"};

        bool is_valid = verifyJwt(email,password,jwtToken);
        if(!is_valid) return {false, "Invalid token"};

        std::string id = getHash(jwtToken);
        std::lock_guard<std::mutex>guard(lock);
        sessionMap.erase(id);
        user.erase(email);
        return {true,"Session Deleted"};
    }

    static Session& getInstance() {
        static Session instance;
        return instance;
    }
};

#endif