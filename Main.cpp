#include <iostream>
#include <string>
#include <chrono>
#include <sstream>
#include <functional>

typedef std::chrono::duration<float, std::milli> MILLI;

const int kLoop=1000000;
const std::string kStrUid("idid");
const std::string kStrDest("target");
const int kTaskId=1999;

template <typename F> MILLI TimeMeasurement(F f)
{
	typedef std::chrono::high_resolution_clock clock;
	std::string r;
	auto t0 = clock::now();
	f(r);
	auto t1 = clock::now();
	return MILLI(t1-t0);
}

void Comment(const std::string &strComment)
{
	std::cout << "\n-------------------------" << strComment << "-------------------\n";
}

void Cost(const MILLI &milli)
{
	std::cout << milli.count() << "ms\n";
}

void TestString()
{
	{
		auto f=[](std::string& l_czTempStr)
		{
			std::string s1="Test data1";
			for (int i = 0; i < kLoop; ++i)
			{
				l_czTempStr = s1 + "Test data2" + "Test data3";
			}
		};
		Comment("string, plain addition \"+\"");
		Cost(TimeMeasurement(f));
	}

	{
		auto f=[](std::string& l_czTempStr)
		{
			for (int i = 0; i < kLoop; ++i)
			{
				l_czTempStr =  "Test data1";
				l_czTempStr += "Test data2";
				l_czTempStr += "Test data3";
			}
		};
		Comment("string, incremental \"+=\"");
		Cost(TimeMeasurement(f));
	}

	{
		auto f=[](std::string& l_czTempStr)
		{
			for (int i = 0; i < kLoop; ++i)
			{
				l_czTempStr =  "Test data1";
				l_czTempStr.append("Test data2");
				l_czTempStr.append("Test data3");
			}
		};
		Comment("string, \"append\"");
		Cost(TimeMeasurement(f));
	}

	{
		auto f=[](std::string& l_czTempStr)
		{
			for (int i = 0; i < kLoop; ++i)
			{
				std::ostringstream oss;
				oss << "Test data1";
				oss << "Test data2";
				oss << "Test data3";
				l_czTempStr = oss.str();
			}
		};
		Comment("oss, creation in each loop, incremental");
		Cost(TimeMeasurement(f));
	}

	{
		auto f=[](std::string& l_czTempStr)
		{
			std::ostringstream oss;
			for (int i = 0; i < kLoop; ++i)
			{
				oss.str("");
				oss.clear();
				oss << "Test data1" << "Test data2" << "Test data3";
			}
			l_czTempStr = oss.str();
		};
		Comment("oss, 1 creation, plain addition");
		Cost(TimeMeasurement(f));
	}

	{
		auto f=[](std::string& l_czTempStr)
		{
			std::ostringstream oss;
			for (int i = 0; i < kLoop; ++i)
			{
				oss.str("");
				oss.clear();
				oss << "Test data1";
				oss << "Test data2";
				oss << "Test data3";
			}
			l_czTempStr = oss.str();
		};
		Comment("oss, 1 creation, incremental");
		Cost(TimeMeasurement(f));
	}

	{
		auto f=[](std::string& l_czTempStr)
		{
			std::stringstream ss;
			for (int i = 0; i < kLoop; ++i)
			{
				ss.str("");
				ss.clear();
				ss << "Test data1";
				ss << "Test data2";
				ss << "Test data3";
			}
			l_czTempStr = ss.str();
		};
		Comment("ss, 1 creation, incremental");
		Cost(TimeMeasurement(f));
	}
}

void TestStringWithConversion()
{
	{
		auto f=[](std::string& l_czTempStr)
		{
			for (int i = 0; i < kLoop; ++i)
			{
				std::string strBase("asdfghjkl");
				std::ostringstream oss;
				oss << kTaskId;

				strBase += "|uid=";
				strBase += kStrUid;
				strBase += "|destination=";
				strBase += kStrDest;
				strBase += "|taskid=";
				strBase += oss.str();
			}
		};
		Comment("string, incremental by \"+\" with one oss conversion.");
		Cost(TimeMeasurement(f));
	}

	{
		auto f=[](std::string& l_czTempStr)
		{
			for (int i = 0; i < kLoop; ++i)
			{
				std::string strBase("asdfghjkl");
				std::ostringstream oss;

				oss << "|uid=";
				oss << kStrUid;
				oss << "|destination=";
				oss << kStrDest;
				oss << "|taskid=";
				oss << kTaskId;
				strBase += oss.str();
			}
		};
		Comment("string, incremental all by oss.");
		Cost(TimeMeasurement(f));
	}

	{
		auto f=[](std::string& l_czTempStr)
		{
			for (int i = 0; i < kLoop; ++i)
			{
				std::string strBase("asdfghjkl");
				std::stringstream ss;
				ss << kTaskId;

				strBase += "|uid=";
				strBase += kStrUid;
				strBase += "|destination=";
				strBase += kStrDest;
				strBase += "|taskid=";
				strBase += ss.str();
			}
		};
		Comment("string, incremental by \"+\" with one ss conversion.");
		Cost(TimeMeasurement(f));
	}

	{
		auto f=[](std::string& l_czTempStr)
		{
			for (int i = 0; i < kLoop; ++i)
			{
				std::string strBase("asdfghjkl");
				std::stringstream ss;

				ss << "|uid=";
				ss << kStrUid;
				ss << "|destination=";
				ss << kStrDest;
				ss << "|taskid=";
				ss << kTaskId;
				strBase += ss.str();
			}
		};
		Comment("string, incremental all by ss.");
		Cost(TimeMeasurement(f));
	}
}

int main ()
{
	std:: cout << std::endl << "----------------String Comparison with " << kLoop << " loops ---------------- " << std::endl;

	// TestString();
	TestStringWithConversion();
}
