struct DbSettings {
	string_view db_name;
	int db_connection_timeout;
	bool db_allow_exceptions;
	DBLogLevel db_log_level;
	
	DbSettings& SetDbName(string_view db_name) {
		this->db_name = db_name;
		return *this;
	}
	
	DbSettings& SetDbConnectionTimeout(int db_connection_timeout) {
		this->db_connection_timeout = db_connection_timeout;
		return *this;
	}
	
	DbSettings& SetDbAllowExceptions(bool db_allow_exceptions) {
		this->db_allow_exceptions = db_allow_exceptions;
		return *this;
	}
	
	DbSettings& SetDbLogLevel(DBLogLevel db_log_level) {
		this->db_log_level = db_log_level;
		return *this;
	}
}

DBHandler CreateDBHandler(const DbSettings& db_settings) {
	DBConnector connector(db_settings.db_allow_exceptions, db_settings.db_log_level);
    DBHandler db;
    if (db_settings.db_name.starts_with("tmp."s)) {
        db = connector.ConnectTmp(db_settings.db_name, db_settings.db_connection_timeout);
    } else {
        db = connector.Connect(db_settings.db_name, db_settings.db_connection_timeout);
    }
	return db;
}

vector<Person> LoadPersons(const DbSettings& db_settings, int min_age, int max_age, string_view name_filter) {
    auto db = CreateDBHandler(db_settings);
    if (!db_settings.db_allow_exceptions && !db.IsOK()) {
        return {};
    }

    ostringstream query_str;
    query_str << "from Persons "s
              << "select Name, Age "s
              << "where Age between "s << min_age << " and "s << max_age << " "s
              << "and Name like '%"s << db.Quote(name_filter) << "%'"s;
    DBQuery query(query_str.str());

    vector<Person> persons;
    for (auto [name, age] : db.LoadRows<string, int>(query)) {
        persons.push_back({move(name), age});
    }
    return persons;
}