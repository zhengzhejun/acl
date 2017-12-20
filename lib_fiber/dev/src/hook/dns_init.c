#include "stdafx.h"
#include "dns/dns.h"
#include "hook.h"

struct dns_resolv_conf *var_dns_conf = NULL;
struct dns_hosts *var_dns_hosts      = NULL;
struct dns_hints *var_dns_hints      = NULL;

void dns_set_read_wait(int timeout)
{
	set_read_timeout(timeout);
}

void dns_init(void)
{
	static pthread_mutex_t __lock = PTHREAD_MUTEX_INITIALIZER;
	static int __called = 0;

	(void) pthread_mutex_lock(&__lock);

	if (__called) {
		(void) pthread_mutex_unlock(&__lock);
		return;
	}

	__called++;

	int err = 0;
	var_dns_conf  = dns_resconf_local(&err);
	assert(var_dns_conf && err == 0);
	var_dns_conf->options.timeout = 1000;

	var_dns_hosts = dns_hosts_local(&err);
	assert(var_dns_hosts && err == 0);

	var_dns_hints = dns_hints_local(var_dns_conf, &err);
	assert(var_dns_hints && err == 0);

	(void) pthread_mutex_unlock(&__lock);
}
