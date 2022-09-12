#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

#include "cce_osconfig.h"
#include "cce_io.h"

#define LINE_SIZE 256

static int validate_key(char *key);
static int validate_value(int key_index, char *value);
static void make_filename(char *result, char *extension, unsigned size);

static Result get_key_value(char *key, char *value, unsigned size);
static Result get_first_key(char *key, unsigned size);
static Result get_next_key(char *key, unsigned size);
static Result set_key_value(char *key, char *value);
static Result unset_key_value(char *key);
static Result clear_file();
static Result apply_file();
static Result select_file();
static Result load_file(char *filename);
static Result save_file(char *filename);
static Result get_first_file(char *filename, unsigned size);
static Result get_next_file(char *filename, unsigned size);

enum
{
	network_lan_ifname = 0,
	network_lan_proto,
	network_lan_ipaddr,
	network_lan_netmask,
	network_lan_gateway,
	num_keys
} key_indices;

static char *prefixes[num_keys] = {
	"",
	"iface eth0 inet",
	"address",
	"netmask",
	"gateway"
};

static char *keys[num_keys] = {
	"network.lan.ifname",
	"network.lan.proto",
	"network.lan.ipaddr",
	"network.lan.netmask",
	"network.lan.gateway"
};

#ifdef OSCONFIG_TEST
#define CONFIG_DIR_NAME "etc/config"
#else
#define CONFIG_DIR_NAME "/etc/config"
#endif
#define CONFIG_BASENAME "interfaces"

static char values[num_keys][LINE_SIZE];

static int current_index = 0;
static int loaded = 0;
static DIR *current_dir = NULL;

/*
 *
 */

int validate_key(char *key)
{
	int i;
	for (i = 0; i < num_keys; i++) {
		if (!strncmp(keys[i], key, LINE_SIZE)) break;
	}
	return i;
}

/*
 *
 */

int validate_value(int key_index, char *value)
{
	unsigned a, b, c, d;

	if (key_index >= num_keys) return 0;
	switch(key_index) {
	default:
		break;
	case network_lan_ifname:
		if (!strcmp(value, "eth0")) return 1;
		break;
	case network_lan_proto:
		if (!strcmp(value, "static") || !strcmp(value, "dhcp")) return 1;
		break;
	case network_lan_ipaddr:
	case network_lan_netmask:
	case network_lan_gateway:
		if (sscanf(value, "%3u.%3u.%3u.%3u", &a, &b, &c, &d) < 4) return 0;
		if (a < 256 && b < 256 && c < 256 && d < 256) return 1;
	}
	return 0;
}

/*
 *
 */

Result get_key_value(char *key, char *value, unsigned size)
{
	int index;
	if (!loaded) return OSCONFIG_NOTLOADED;
	index = validate_key(key);
	if (index < num_keys) {
		strncpy(value, values[index], size);
		return OK;
	} else {
		strncpy(value, "", size);
		return OSCONFIG_BADPARAMETER;
	}
}

/*
 *
 */

Result get_first_key(char *key, unsigned size)
{
	if (!loaded) return OSCONFIG_NOTLOADED;
	current_index = 0;
	return get_next_key(key, size);
}

/*
 *
 */

Result get_next_key(char *key, unsigned size)
{
	if (!loaded) return OSCONFIG_NOTLOADED;
	if (current_index < num_keys) {
		strncpy(key, keys[current_index], size);
		current_index++;
		return OK;
	} else {
		strncpy(key, "", size);
		return OSCONFIG_BADPARAMETER;
	}
}

/*
 *
 */

Result set_key_value(char *key, char *value)
{
	int index;
	if (!loaded) return OSCONFIG_NOTLOADED;
	index = validate_key(key);
	if (index < num_keys) {
		if (validate_value(index, value)) {
			strncpy(values[index], value, LINE_SIZE);
			return OK;
		} else {
			return OSCONFIG_BADVALUE;
		}
	} else {
		return OSCONFIG_BADPARAMETER;
	}
}

/*
 *
 */

static Result unset_key_value(char *key)
{
	int index;
	if (!loaded) return OSCONFIG_NOTLOADED;
	index = validate_key(key);
	if (index < num_keys) {
		strncpy(values[index], "", LINE_SIZE);
		return OK;
	} else {
		return OSCONFIG_BADPARAMETER;
	}
}

/*
 *
 */

static Result clear_file()
{
	int i;
	if (!loaded) return OSCONFIG_NOTLOADED;
	for (i = 0; i < num_keys; i++) {
		strncpy(values[i], "", LINE_SIZE);
	}
	return OK;
}

/*
 *
 */

static Result apply_file() {
	if (!loaded) return OSCONFIG_NOTLOADED;
	system("/etc/init.d/network");
	return OK;
}

/*
 *
 */

static Result select_file() {
	Result ret;
	if (!loaded) return OSCONFIG_NOTLOADED;
	ret = save_file("");
	if (ret == OK) system("kill -s SIGUSR1 $(cat /var/run/flatfsd.pid)");
	return ret;
}

/*
 *
 */

static void make_filename(char *result, char *extension, unsigned size)
{
	snprintf(result, LINE_SIZE, CONFIG_DIR_NAME "/" CONFIG_BASENAME);
	if (strlen(extension)) {
		strncat(result, ".", LINE_SIZE);
		strncat(result, extension, LINE_SIZE);
	}
}

/*
 *
 */

static Result load_file(char *filename)
{
	char fname[LINE_SIZE];
	char line[LINE_SIZE];
	char format[LINE_SIZE];
	char value[LINE_SIZE];
	FILE *f;
	int i;
	int ret;

	make_filename(fname, filename, LINE_SIZE);
	f = fopen(fname, "r");
	if (!f) {
		if (!strlen(filename)) {
			loaded = 1;
			clear_file();
			return OK;
		}
		else return OSCONFIG_BADFILE;
	}

	loaded = 1;  // Do it here so that set_key_value() works

	clear_file();

	set_key_value(keys[network_lan_ifname], "eth0");

	while (fgets(line, LINE_SIZE, f)) {
#ifdef OSCONFIG_TEST
		printf(line);
#endif
		for (i = 0; i < num_keys; i++) {
			if (strlen(prefixes[i])) {
				sprintf(format, "%s %%s", prefixes[i]);
				ret = sscanf(line, format, value);
				if (ret == 1) {
					set_key_value(keys[i], value);
#ifdef OSCONFIG_TEST
					printf ("key [%s] format [%s]", keys[i], format);
					printf (" parsed [%s] value [%s]", value, values[i]); 
					printf (" valid %d\n", validate_value(i, value));
#endif
				}
			}
				
		}
	}

	fclose(f);

	return OK;
}

/*
 *
 */

static Result save_file(char *filename)
{
	char fname[LINE_SIZE];
	FILE *f;

	if (!loaded) return OSCONFIG_NOTLOADED;

	make_filename(fname, filename, LINE_SIZE);
	f = fopen(fname, "w");
	if (!f) return OSCONFIG_BADFILE;

	fprintf(f, "auto lo\n");
	fprintf(f, "iface lo inet loopback\n");
	fprintf(f, "#\n");
	fprintf(f, "auto %s\n", values[network_lan_ifname]);
	fprintf(f, "iface %s inet ", values[network_lan_ifname]);
	fprintf(f, "%s", values[network_lan_proto]);
	if (!strcmp(values[network_lan_proto], "dhcp")) {
		fprintf(f, "\n");
	}
	if (!strcmp(values[network_lan_proto], "static")) {
		fprintf(f, "\naddress %s\n", values[network_lan_ipaddr]);
		fprintf(f, "netmask %s\n", values[network_lan_netmask]);
		if (strlen(values[network_lan_gateway])) {
			fprintf(f, "gateway %s\n", values[network_lan_gateway]);
		}
	}

	fclose(f);

	return OK;
}

static Result get_first_file(char *filename, unsigned size)
{
	if (current_dir) closedir(current_dir);
	current_dir = opendir(CONFIG_DIR_NAME);
	return get_next_file(filename, size);
}

static Result get_next_file(char *filename, unsigned size) {
	struct dirent * d;
	do {
		d = readdir(current_dir);
		if (d && !(strncmp(d->d_name, CONFIG_BASENAME ".", strlen(CONFIG_BASENAME) + 1))) {
			strncpy(filename, d->d_name + strlen(CONFIG_BASENAME) + 1, size);
			return OK;
		}
	} while (d);
	return OSCONFIG_BADPARAMETER;
}

/* ================================================================== */

/*
 *
 */

Result os_config_get_parameter(connection_state *state)
{
	Result ret;
	char key[LINE_SIZE];
	unsigned size = LINE_SIZE;
	ret = buf_recv(state, key, size);
	if (ret != OK) {
		string_send(state, "");
		return ret;
	}
	ret = get_key_value(key, key, size);
	if (ret != OK) {
		string_send(state, "");
		return ret;
	}
	ret = string_send(state, key);
	return ret;
}

/*
 *
 */

Result os_config_get_first_parameter(connection_state * state)
{
	Result ret = OK;
	char key[LINE_SIZE];
	unsigned size = LINE_SIZE;
	ret = get_first_key(key, size);
	if (ret != OK) {
		string_send(state, "");
		string_send(state, "");
		return ret;
	}
	ret = string_send(state, key);
	ret = get_key_value(key, key, size);
	if (ret != OK) {
		string_send(state, "");
		return ret;
	}
	ret = string_send(state, key);
	return ret;
}

/*
 *
 */

Result os_config_get_next_parameter(connection_state * state)
{
	Result ret = OK;
	char key[LINE_SIZE];
	unsigned size = LINE_SIZE;
	ret = get_next_key(key, size);
	if (ret != OK) {
		string_send(state, "");
		string_send(state, "");
		return ret;
	}
	ret = string_send(state, key);
	ret = get_key_value(key, key, size);
	if (ret != OK) {
		string_send(state, "");
		return ret;
	}
	ret = string_send(state, key);
	return ret;
}

/*
 *
 */

Result os_config_set_parameter(connection_state * state)
{
	Result ret = OK;
	char key[LINE_SIZE];
	char value[LINE_SIZE];
	unsigned size = LINE_SIZE;
	ret = buf_recv(state, key, size);
	if (ret != OK)
		return ret;
	size = LINE_SIZE;
	ret = buf_recv(state, value, size);
	if (ret != OK)
		return ret;
	ret = set_key_value(key, value);
	return ret;
}

/*
 *
 */

Result os_config_unset_parameter(connection_state * state)
{
	Result ret = OK;
	char key[LINE_SIZE];
	unsigned size = LINE_SIZE;
	ret = buf_recv(state, key, size);
	if (ret != OK)
		return ret;
	ret = unset_key_value(key);
	return ret;
}

/*
 *
 */

Result os_config_clear_file(connection_state * state)
{
	Result ret = OK;
	ret = clear_file();
	return ret;
}

/*
 *
 */

Result os_config_apply_file(connection_state * state)
{
	Result ret = OK;
	ret = apply_file();
	return ret;
}

/*
 *
 */

Result os_config_load_file(connection_state * state)
{
	Result ret = OK;
	char filename[LINE_SIZE];
	unsigned size = LINE_SIZE;
	ret = buf_recv(state, filename, size);
	if (ret != OK)
		return ret;
	ret = load_file(filename);
	return ret;
}

/*
 *
 */

Result os_config_save_file(connection_state * state)
{
	Result ret = OK;
	char filename[LINE_SIZE];
	unsigned size = LINE_SIZE;
	ret = buf_recv(state, filename, size);
	if (ret != OK)
		return ret;
	ret = save_file(filename);
	return ret;
}

/*
 *
 */

Result os_config_select_file(connection_state * state)
{
	Result ret = OK;
	char filename[LINE_SIZE];
	unsigned size = LINE_SIZE;
	ret = select_file(filename);
	return ret;
}

/*
 *
 */

Result os_config_get_first_file(connection_state * state)
{
	Result ret = OK;
	char filename[LINE_SIZE];
	unsigned size = LINE_SIZE;
	ret = get_first_file(filename, size);
	if (ret != OK) {
		string_send(state, "");
		return ret;
	}
	string_send(state, filename);
	return ret;
}

/*
 *
 */

Result os_config_get_next_file(connection_state * state)
{
	Result ret = OK;
	char filename[LINE_SIZE];
	unsigned size = LINE_SIZE;
	ret = get_next_file(filename, size);
	if (ret != OK) {
		string_send(state, "");
		return ret;
	}
	string_send(state, filename);
	return ret;
}

#ifdef OSCONFIG_TEST

Result buf_recv(connection_state *state, char *buf, unsigned size)
{
	return OK;
}

Result string_send(connection_state *state, const char *buf)
{
	return OK;
}

void test(int index, char *value)
{
	Result ret;
	char v[LINE_SIZE];
	printf ("Testing index %d for value [%s]\tresult %d\n", index, value, validate_value(index, value));
	ret = set_key_value(keys[index], value);
	printf ("Setting key %s to value [%s]\tgot [%s]\tresult %d\n", keys[index], value, values[index], ret);
	ret = get_key_value(keys[index], v, LINE_SIZE);
	printf ("Getting key %s to value [%s]\tgot [%s]\tresult %d\n", keys[index], value, v, ret);
	printf ("\n");
}

main ()
{
	char k[LINE_SIZE];
	char v[LINE_SIZE];
	Result ret;
	int i;

	ret = load_file("");
	if (ret!=OK) {printf("load\n"); return;}
	ret = get_first_key(k, LINE_SIZE);
	if (ret!=OK) {printf("get_first_key\n"); return;}
	printf("first key %s\n", k);
	ret = get_key_value(k, v, LINE_SIZE);
	if (ret!=OK) {printf("get_key_value\n"); return;}
	printf("first key value %s\n", v);
	while (OK == get_next_key(k, LINE_SIZE)) {
		printf ("next key %s\n", k);
		ret = get_key_value(k, v, LINE_SIZE);
		if (ret!=OK) {printf("get_key_value\n"); return;}
		printf ("next key value %s\n", v);
	}
	for (i = 0; i < num_keys; i++) {
		printf("value of %s is %s\n", keys[i], values[i]);
	}
	test(network_lan_proto, "static");
	save_file("static");
/*
	test(network_lan_proto, "static");
	test(network_lan_proto, " dhcp");
	test(network_lan_proto, "dhcp ");
	test(network_lan_proto, " dhcp ");
	test(network_lan_proto, "zhcp");
	test(network_lan_ipaddr, "123.123.123.123");
	test(network_lan_gateway, "123.123.123.1");
	test(network_lan_netmask, "255.255.255.0");
	test(network_lan_ipaddr, "256.255.255.0");
	test(network_lan_ipaddr, "255.255.255");
	test(network_lan_ipaddr, "255.255");
	test(network_lan_ipaddr, "255");
	test(network_lan_ipaddr, ".255...");
	test(network_lan_netmask, "255.255.255.O");
*/
	test(network_lan_proto, "dhcp");
	save_file("dhcp");
	ret = get_first_file(k, LINE_SIZE);
	while (ret == OK) {
		printf("file %s\n", k);
		ret = get_next_file(k, LINE_SIZE);
	}
	select_file();
	apply_file();
}
#endif

