#include <iostream>
#include <hiredis/hiredis.h>
#include <unistd.h> // For sleep function
#include <cstring>  // For strcmp

const char *REDIS_HOST = "127.0.0.1";
const int REDIS_PORT = 6379;
const char *TX_CHANNEL = "txchannel";
const int ACK_TIMEOUT = 10; // Seconds to wait for acknowledgment

void publishMessage(redisContext *c, const std::string &message, const std::string &ackKey) {
    redisReply *reply = (redisReply *)redisCommand(c, "PUBLISH %s %s", TX_CHANNEL, message.c_str());
    if (reply == nullptr) {
        std::cerr << "Error publishing message: " << c->errstr << std::endl;
        return;
    }
    std::cout << "Message published. Subscribers received: " << reply->integer << std::endl;
    freeReplyObject(reply);

    // Add the message to the acknowledgment tracking list
    reply = (redisReply *)redisCommand(c, "SET %s %s", ackKey.c_str(), "pending");
    if (reply == nullptr) {
        std::cerr << "Error setting acknowledgment key: " << c->errstr << std::endl;
    } else {
        std::cout << "Acknowledgment key set to pending." << std::endl;
    }
    freeReplyObject(reply);
}

bool waitForAcknowledgment(redisContext *c, const std::string &ackKey) {
    redisReply *reply;
    
    // Poll for acknowledgment
    for (int i = 0; i < ACK_TIMEOUT; ++i) {
        reply = (redisReply *)redisCommand(c, "GET %s", ackKey.c_str());
        if (reply == nullptr) {
            std::cerr << "Error checking acknowledgment: " << c->errstr << std::endl;
            return false;
        }
        
        if (strcmp(reply->str, "acknowledged")) {
            std::cout << "Acknowledgment received!" << std::endl;
            freeReplyObject(reply);
            return true;
        }
        
        freeReplyObject(reply);
        sleep(1); // Wait before checking again
    }
    
    std::cout << "Acknowledgment timeout!" << std::endl;
    return false;
}

int main() {
    // Connect to Redis server
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

    // Publish a message
    std::string message = "Hello world, how are you";
    std::string ackKey = "ack_" + message;
    publishMessage(c, message, ackKey);

    // Wait for acknowledgment
    bool acknowledged = waitForAcknowledgment(c, ackKey);
    if (acknowledged) {
        std::cout << "Message acknowledged by subscribers." << std::endl;
    } else {
        std::cout << "Message not acknowledged." << std::endl;
    }

    // Cleanup and disconnect
    redisFree(c);
    return 0;
}