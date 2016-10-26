#ifndef LOGGER_HPP_MWGF6NYP
#define LOGGER_HPP_MWGF6NYP

#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <cstdarg>
#include <string>
#include <mutex>

class Logger {
	private:
		pid_t pid;
		std::string prefix;
		bool log_time;
		std::mutex lock_;
		void vLog(const char *format, va_list arglist) {
			lock_.lock();
			if(log_time) {
				struct timeval tv;
				time_t nowtime;
				struct tm *nowtm = 0;
				char tmbuf[128] = {0};
				gettimeofday(&tv, NULL);
				nowtime = tv.tv_sec;
				nowtm = localtime(&nowtime); // NOLINT
				strftime(tmbuf, sizeof(tmbuf), "%Y-%m-%d %H:%M:%S", nowtm);
				fprintf(stderr, "[%s:%6d] %s.%03ld -- ", prefix.c_str(), pid, tmbuf, tv.tv_usec / 1000);
			} else {
				fprintf(stderr, "[%s:%6d] -- ", prefix.c_str(), pid);
			}
			vfprintf(stderr, format, arglist);
			fprintf(stderr, "\n");
			lock_.unlock();
		}

	public:
		void UpdatePID() {
			pid = getpid();
		}

		explicit Logger(const char *_prefix = "Logger")
		: prefix(_prefix), log_time(true) {
			UpdatePID();
		}

		void EnableTime(bool e) {
			log_time = e;
		}

		void SetPrefix(const char *_p) {
			prefix = std::string(_p);
		}

		void Log(const char *format, ... ) {
			va_list arglist;
			va_start(arglist, format);
			vLog(format, arglist);
			va_end(arglist);
		}

		void operator()(const char *format, ...) {
			va_list arglist;
			va_start(arglist, format);
			vLog(format, arglist);
			va_end(arglist);
		}
};


#endif /* end of include guard: LOGGER_HPP_MWGF6NYP */
