#include <iostream>
#include "event.hpp"
#include "event-odb.hxx"
#include <odb/database.hxx>
#include <odb/schema-catalog.hxx>
#include <odb/transaction.hxx>
#include <odb/sqlite/database.hxx>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <set>

using namespace std;
using odb::sqlite::database;
using odb::transaction;
using odb::schema_catalog;

event::event(
    order_t o,
    const std::string& s,
    unsigned int q,
    double p): order(o), security(s), qty(q), price(p) {
    database db("test.db", SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE);
    transaction t (db.begin());

    typedef odb::query<date> query;
    boost::shared_ptr<date> a0 (db.query_one<date>(query::desc == "auto"));
    if (!a0.get()) {
        cout << "Introduced auto" << endl;
        boost::shared_ptr<date> a1(new date("auto"));
        create = a1;
        db.persist(*a1);
    } else {
        create = a0;
    }
    typedef odb::query<son> query_0;
    typedef odb::result<son> result;
    result r(db.query<son>(query_0::name == "John" || query_0::name == "Jack"));
    for (auto i : r) {
        prole.insert(boost::make_shared<son>(i));
        cout << "Inserting: " << i.id << endl;
        cout << "Inserting: " << i.name_() << endl;
    }
    t.commit();
}

int main()
{
    database db("test.db", SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE);
    transaction t0 (db.begin());
    try {
        schema_catalog::create_schema (db, "", false);
    } catch (...) {
        cout << "Already existing database" << endl;
    }
    t0.commit();
    event e(buy, "TEST", 1, 0.5);
    transaction t (db.begin());
    cout << "desc: " << e.create->desc << endl;
    event_stat es (db.query_value<event_stat> ());
    cout << "sum: " << es.sum << endl;
//    typedef odb::result<event_prole_stat_2> result;
    typedef odb::query<event_prole_stat_2> query;
    event_prole_stat_2 es2 (db.query_value<event_prole_stat_2> (query::son::name == "John"));
    cout << es2.count << "|" << es2.son_name << endl;
    //event_prole_stat eps (db.query_value<event_prole_stat> ());

    for (auto j : e.prole) {
        cout << "Already inserted: " << j->id << endl;
        cout << "Already inserted: " << j->name_() << endl;
    }

    db.persist(e);
    t.commit();
    return 0;
}
