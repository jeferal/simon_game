/**
 * util.h
 * Author: Jose Simo (2016)
 * Universitat Politecnica de Valencia. AI2-DISCA
 * Creative Commons.
 *
 */

#ifndef UTIL_H_
#define UTIL_H_
#include <string>
using std::string;


namespace BBB {

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

#endif /* UTIL_H_ */
