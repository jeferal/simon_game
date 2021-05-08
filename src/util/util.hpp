#ifndef UTIL_HPP_
#define UTIL_HPP_
#include <string>

using std::string;
using namespace std;


namespace BBB {

bool fileExist (const std::string& name);
int write(string path, string filename, string value);
int write(string path, string filename, int value);
string read(string path, string filename);

//time management
long toMicroseconds(struct timespec *ts);
void sleep_until(struct timespec *ts, int delay);
void sleep_lapse(int delay);
long getCurrentMicroseconds();
/////

} /* namespace BBB */

#endif /* UTIL_HPP_ */
