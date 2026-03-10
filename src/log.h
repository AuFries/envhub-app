#include <syslog.h>

#define LOGE(...) syslog(LOG_ERR, __VA_ARGS__)
#define LOGW(...) syslog(LOG_WARNING, __VA_ARGS__)
#define LOGI(...) syslog(LOG_INFO, __VA_ARGS__)
#define LOGD(...) syslog(LOG_DEBUG, __VA_ARGS__)