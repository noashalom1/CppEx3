#include <string>
class Role {
    public:
        virtual std::string role_name() const = 0;
        virtual ~Role() {}
    };