#ifndef __TIMER_H__
#define __TIMER_H__

#include<chrono>


class timer {
public:
	inline timer(void);
	inline timer(bool running);
	inline ~timer(void);
	inline double query(void);
	inline double start(void);
	inline double stop(void);
	inline double reset(void);
	inline double resume(void);
	inline bool is_running(void) const;
protected:
	inline std::chrono::high_resolution_clock::time_point now(void) const;
	inline double updated_interval(void);
	inline double interval(void);
	std::chrono::nanoseconds m_interval;
	std::chrono::high_resolution_clock::time_point m_measurement;
	bool m_running;
};	


inline timer::timer(void) : m_interval(std::chrono::seconds::zero()), m_measurement(now()), m_running(true) {
}

inline timer::timer(bool running) : m_interval(std::chrono::seconds::zero()), m_measurement(now()), m_running(running) {
}

inline timer::~timer(void) {
}

inline double timer::query(void) {
	if (m_running) return updated_interval();
	else return interval();
}

inline double timer::start(void) {
	if (!m_running) {
		reset();
		m_measurement = now();
		m_running = true;
		return 0.0;
	}
	return updated_interval();
}

inline double timer::stop(void) {
	if (m_running) {
		m_running = false;
		return updated_interval();
	}
	return interval();
}

inline double timer::reset(void) {
	m_measurement = now();		
	m_interval = std::chrono::seconds::zero();
	return 0.0;
}

inline double timer::resume(void) {
	if (!m_running) {
		m_measurement = now();
		m_running = true;
	}
	return interval();
}

inline bool timer::is_running(void) const {
	return m_running;
}

inline std::chrono::high_resolution_clock::time_point timer::now(void) const {
	return std::chrono::high_resolution_clock::now();
}
inline double timer::updated_interval(void) {
	auto T = now();
	m_interval += std::chrono::duration_cast<std::chrono::nanoseconds>(T - m_measurement);
	m_measurement = T;
	return std::chrono::duration<double>(m_interval).count();
}

inline double timer::interval(void) {
	return std::chrono::duration<double>(m_interval).count();
}

#endif

