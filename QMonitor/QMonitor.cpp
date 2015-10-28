// QMonitor.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>
#include <vector>
#include <fstream>
#include <chrono>
#include <thread>
#include <boost\algorithm\string.hpp>
#include <Logger.h>
#include <DataStructure.h>

#include <StringUtils.h>
const std::string q_names_file = R"(B:\Workspace\QMonitor\qs_monitored.txt)";
using namespace AceLogger;
std::string log_dir = "B:\\Logs";
AceLogger::Logger* AceLogger::Logger::m_instance = nullptr;
AceLogger::Logger* AceLogger::Logger::GetInstance(){
	if (m_instance == nullptr)
		m_instance = new AceLogger::Logger();
	return m_instance;
}
AceLogger::Logger::Logger(){
	isLogAvailable = false;
}

std::vector<BaseIPCMessageQ*> all_q_monitored;

void create_all_listners(std::vector<std::string> &_q_names,
	std::vector<BaseIPCMessageQ*> &_qs){
	for (auto s : _q_names){
		BaseIPCMessageQ *q = new BaseIPCMessageQ(s, true, false);
		_qs.push_back(q);
	}
}


void print_q_stats(std::vector<BaseIPCMessageQ*> &_qs){

	std::chrono::milliseconds interval(1000);
	while (true){
		for (auto q : _qs){
			if (q->connected()){
				Log("Queue\t" + q->id() + "\topen\t" + StringUtils::get_string(q->size()), LOG_STATUS, LOG_TYPE::DISPLAY);
			}
			else{
				if (q->open()){
					Log("Queue\t" + q->id() + "\tJust openned\t" + StringUtils::get_string(q->size()), LOG_STATUS, LOG_TYPE::DISPLAY);
				}
				else
					Log("Queue\t" + q->id() + "\tClosed\t" , LOG_STATUS, LOG_TYPE::DISPLAY);
			}
			
		}
		std::this_thread::sleep_for(interval);
	}
}

void clean_up(std::vector<BaseIPCMessageQ*> &_qs){
	for (auto q : _qs)
		delete q;
}

void exitApp(){
	clean_up(all_q_monitored);
}

int _tmain(int argc, _TCHAR* argv[])
{
	AceLogger::RegisterLogger(log_dir, "QMonitor", "0.0.0.1");
	atexit(exitApp);
	std::vector<std::string> q_names;
	
	std::ifstream ifs(q_names_file);
	std::string temp; 
	while (std::getline(ifs,temp)){
		boost::trim(temp);
		q_names.push_back(temp);
	}
	create_all_listners(q_names, all_q_monitored);
	print_q_stats(all_q_monitored);
	
	return 0;
}

