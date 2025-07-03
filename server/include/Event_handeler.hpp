#include <string>

class BaseEventHandler
{
public:
    virtual void handle(const std::string &json) = 0;
    virtual ~BaseEventHandler() = default;
};

class AddFriendEventHandler : public BaseEventHandler
{
public:
    void handle(const std::string &json) override;
};

class ChatEventHandler : public BaseEventHandler
{
public:
    void handle(const std::string &json) override;
};

// ! need singleton
class LoginEventHandler : public BaseEventHandler
{
public:
    void handle(const std::string &json) override;
};
