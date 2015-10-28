#pragma once
#include <string>
class QManager
{

	bool create_q(const std::string &_id);

	bool remove_q(const std::string &_id);

	void clean_up_q(const std::string &_id);

	void print_status_of_q(const std::string &_id)const;

	void print_status_all_qs()const;

	std::string generate_q_id();

public:
	QManager();
	~QManager();
};

