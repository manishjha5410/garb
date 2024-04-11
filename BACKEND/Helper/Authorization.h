#ifndef AUTHORIZATION_H_
#define AUTHORIZATION_H_

#include "chrono"
#include "unordered_map"
#include "unordered_set"
#include "mutex"
#include "jwt.h"

#include "sstream"

class Session {

private:

    struct User {
        std::chrono::_V2::system_clock::time_point created_at;
        std::chrono::minutes valid_upto;
        boost::json::object data;
    };

    std::unordered_map<std::string, User>sessionMap;
    std::unordered_set<int>user;
    std::mutex lock;

    Session() {};

    std::string getDateTime(std::chrono::_V2::system_clock::time_point tp)
    {
        std::stringstream ss;
        ss << std::chrono::duration_cast<std::chrono::nanoseconds>(tp - std::chrono::system_clock::from_time_t(0)).count();
        return ss.str();
    }

    std::string getHash(std::string &jwtToken)
    {
        return jwt::decode(jwtToken).get_id();
    }

    bool verifyJwt(std::string &jwtToken)
    {
        std::string hash = getHash(jwtToken);

        jwt::decoded_jwt<jwt::traits::boost_json> token = jwt::decode(jwtToken);

		auto verifier = jwt::verify()
				.allow_algorithm(jwt::algorithm::hs256("secret"))
				.with_issuer("auth0")
				.with_id(hash)
				.leeway(60UL);
                
                
        verifier.verify(token);
        
        return true;
    }

    std::string getData(std::string &jwtToken, std::string claim)
    {
        return jwt::decode(jwtToken).get_payload_claim(claim).as_string();
    }

    bool session_expire(User &u)
    {
        return (u.created_at + u.valid_upto) <= std::chrono::system_clock::now();
    }

    bool jwtExpire(std::string &jwtToken)
    {
        return jwt::decode(jwtToken).get_expires_at() <= std::chrono::system_clock::now();
    }

    std::string generateToken(int id, std::chrono::_V2::system_clock::time_point &now)
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
                    .set_payload_claim("id",jwt::claim(std::to_string(id)))
                    .sign(jwt::algorithm::hs256("secret"));

        return token;
    }

public:

    std::pair<bool,std::string> verify_session(std::string &jwtToken){

        if(jwtToken=="") return {false, "Enter token"};

        int id = stoi(getData(jwtToken,"id"));


        std::unordered_set<int>::iterator it = user.find(id);

        if(it== user.end()) return {false,"Session is not created"};

        std::string hash = getHash(jwtToken);
        User u = sessionMap[hash];

        if(!session_expire(u) && !jwtExpire(jwtToken))
            return {true,jwtToken};
        else
        {
            destroy_session(id,jwtToken);
            return {false,"Session is expired log in again"};
        }


        return {true,"User is already loggedin"};
    }

    std::pair<bool,std::string> create_session(boost::json::object &data, std::string &jwtToken)
    {
        User u;

        if(user.find(data["id"].as_int64())!= user.end())
            return {false,"Session is already creatred"};

        std::chrono::_V2::system_clock::time_point now = std::chrono::system_clock::now();
        jwtToken = generateToken(data["id"].as_int64(),now);

        u.created_at = now;
        u.valid_upto = std::chrono::minutes(5);
        u.data = data;

        std::lock_guard<std::mutex>guard(lock);
        user.insert(data["id"].as_int64());
        sessionMap[getHash(jwtToken)] = u;

        return {true,jwtToken};
    }

    std::pair<bool,std::string> destroy_session(int id,std::string &jwtToken)
    {
        std::string hash = getHash(jwtToken);
        std::lock_guard<std::mutex>guard(lock);
        sessionMap.erase(hash);
        user.erase(id);
        return {true,"Session Deleted"};
    }

    std::pair<bool,boost::json::value> return_value(std::string jwtToken, std::string key){
        if(jwtToken=="") return {false,"Enter token"};
        std::string hash = getHash(jwtToken);

        std::unordered_map<std::string, Session::User>::iterator it = sessionMap.find(hash);

        if(it == sessionMap.end()) return {false,"Session is not present"};
        
        User &u = it->second;
        boost::json::object::iterator it1 = u.data.find(key);
        if(it1==u.data.end()) return {false,boost::json::value{}};

        return {true,it1->value()};
    }

    static Session& getInstance() {
        static Session instance;
        return instance;
    }
};

#endif