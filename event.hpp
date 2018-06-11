#ifndef EVENT_HXX
#define EVENT_HXX
#include <string>
#include <odb/core.hxx>
#include <boost/shared_ptr.hpp>
#include <set>

using std::set;
using std::string;
using boost::shared_ptr;

enum order_t {buy, sell};

#pragma db object
class son
{
public:
    #pragma db id auto
    unsigned long id;
    son(char const * const c): name(c) {};
    string name_() {return name;}
private:
    string name;
    son() {};
    friend class odb::access;
};

#pragma db object
class date
{
public:
    date(const std::string& s): desc(s) {};
    #pragma db id auto
    unsigned long id;
    std::string desc;
    date(char const * const c) {
        const std::string s(c);
        desc = s;
    };
private:
    date() {};
    friend class odb::access;
};

#pragma db object
class event
{
public:
    event(
        order_t o,
        const std::string& s,
        unsigned int q,
        double p);
    #pragma db id auto
    unsigned long id;
    order_t order;
    std::string security;
    unsigned int qty;
    double price;
    boost::shared_ptr<date> create;
    set< boost::shared_ptr<son> > prole;
private:
    friend class odb::access;
    event() {};
};

#pragma db view object(event)
struct event_stat
{
    #pragma db column("sum(" + event::qty + ")")
    unsigned int sum;
};

#pragma db view object(event) \
                object(son)
struct event_prole_stat
{
    #pragma db column(event::id)
    unsigned long event_id;
    #pragma db column(son::name)
    std::string son_name;
};

PRAGMA_DB( view object(event) \
                object(son) \
                query((?) + "GROUP BY" + son::id))
struct event_prole_stat_2
{
    #pragma db column("count(*)")
    unsigned long count;
    #pragma db column(son::name)
    std::string son_name;
};


std::ostream& operator << (std::ostream& os, const event& e);
#endif
