#include "database.h"

Database::Database() {
    connect();
}

Database::~Database() {
    close();
}

void Database::connect() {
    database = QSqlDatabase::addDatabase("QSQLITE");
    QString db_path = QDir::currentPath();
    db_path =  db_path + QString("/data.db");
    cout << "Database Connected: " << db_path.toStdString() << endl;
    database.setDatabaseName(db_path);
    if (!database.open()) {
        cout << "Error: Failed to connect database." << endl;
    }
}

void Database::close() {
    database.close();
}

QSqlQuery Database::execute(QString sqlCommand) {
    QSqlQuery sqlQuery;
    if(!sqlQuery.exec(sqlCommand)) {
        cout << QString("Error: Failed to execute: %1").arg(sqlCommand).toStdString() << endl;
        cout << sqlQuery.lastError().text().toStdString() << endl;
        throw sqlQuery.lastError();
    }
    return sqlQuery;
}

//void Database::initDatabase() {
//    QString create_breaker = "CREATE TABLE breaker ("\
//        "breaker_id integer NOT NULL PRIMARY KEY autoincrement,"\
//        "username text NOT NULL,"\
//        "password text NOT NULL);";
//    this->execute(create_breaker);
//    QString creat_maker = "CREATE TABLE maker ("\
//        "breaker_id integer NOT NULL PRIMARY KEY autoincrement,"\
//        "username text NOT NULL,"\
//        "password text NOT NULL);";
//    this->execute(creat_maker);
//}
