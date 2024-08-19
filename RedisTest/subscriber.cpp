#include <iostream>
#include <hiredis/hiredis.h>
#include <cstring> // For strcmp

const char *REDIS_HOST = "127.0.0.1";
const int REDIS_PORT = 6379;
const char *TX_CHANNEL = "txchannel";

void handleMessage(redisContext *c, const std::string &message) {
    std::cout << "Received message: " << message << std::endl;
    
    // Publish acknowledgment
    std::string ackKey = "ack_" + message;
    std::string ack = "acknowledged";
    redisReply *reply = (redisReply *)redisCommand(c, "SET %s %s", ackKey.c_str(), ack);
    if (reply == nullptr) {
        std::cerr << "Error publishing acknowledgment: " << c->errstr << std::endl;
    } else {
        std::cout << "Acknowledgment published." << std::endl;
    }
    freeReplyObject(reply);
}

int main() {
    redisContext *c = redisConnect(REDIS_HOST, REDIS_PORT);
    if (c == nullptr || c->err) {
        if (c) {
            std::cerr << "Connection error: " << c->errstr << std::endl;
            redisFree(c);
        } else {
            std::cerr << "Connection error: can't allocate redis context" << std::endl;
        }
        return 1;
    }

    // Subscribe to the channel
    redisReply *reply = (redisReply *)redisCommand(c, "SUBSCRIBE %s", TX_CHANNEL);
    if (reply == nullptr) {
        std::cerr << "Error subscribing to channel: " << c->errstr << std::endl;
        redisFree(c);
        return 1;
    }
    freeReplyObject(reply);

    // Listen for messages and handle them
    while (redisGetReply(c, (void**)&reply) == REDIS_OK) {
        if (reply->type == REDIS_REPLY_ARRAY && reply->elements == 3) {
            if (strcmp(reply->element[0]->str, "message") == 0) {
                handleMessage(c, reply->element[2]->str);
            }
        }
        freeReplyObject(reply);
    }

    redisFree(c);
    return 0;
}