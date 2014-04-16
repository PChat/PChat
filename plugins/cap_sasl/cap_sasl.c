/*
* SASL authentication plugin for XChat
* Extremely primitive: only PLAIN, no error checking
*
* http://ygrek.org.ua/p/cap_sasl.html
*
* Docs:
* http://hg.atheme.org/charybdis/charybdis/file/6144f52a119b/doc/sasl.txt
* http://tools.ietf.org/html/rfc4422
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <glib.h>

#include "xchat-plugin.h"

static xchat_plugin *ph; /* plugin handle */
static const char name[] = "SASL";
static const char desc[] = "SASL authentication plugin for HexChat";
static const char version[] = "1.2";
static const char sasl_help[] = "SASL Usage:\n /SASL ADD <login> <password> <network>, enable/update SASL authentication for given network\n /SASL DEL <network>, disable SASL authentication for given network\n /SASL LIST, get the list of SASL-enabled networks\n";

struct sasl_info
{
char const* login;
char const* password;
char const* network;
};

typedef struct sasl_info sasl_info;

static int
add_info (char const* login, char const* password, char const* network)
{
char buffer[512];

sprintf (buffer, "%s:%s", login, password);
return xchat_pluginpref_set_str (ph, network, buffer);
}

static int
del_info (char const* network)
{
return xchat_pluginpref_delete (ph, network);
}

static void
print_info ()
{
char list[512];
char* token;

if (xchat_pluginpref_list (ph, list))
{
xchat_printf (ph, "%s\tSASL-enabled networks:", name);
xchat_printf (ph, "%s\t----------------------", name);
token = strtok (list, ",");
while (token != NULL)
{
xchat_printf (ph, "%s\t%s", name, token);
token = strtok (NULL, ",");
}
}
else
{
xchat_printf (ph, "%s\tThere are no SASL-enabled networks currently", name);
}
}

static sasl_info*
find_info (char const* network)
{
char buffer[512];
char* token;
sasl_info* cur = (sasl_info*) malloc (sizeof (sasl_info));

if (xchat_pluginpref_get_str (ph, network, buffer))
{
token = strtok (buffer, ":");
cur->login = g_strdup (token);
token = strtok (NULL, ":");
cur->password = g_strdup (token);
cur->network = g_strdup (network);

return cur;
}

return NULL;
}

static sasl_info*
get_info (void)
{
const char* name;
name = xchat_get_info (ph, "network");

if (name)
{
return find_info (name);
}
else
{
return NULL;
}
}

static int
authend_cb (char *word[], char *word_eol[], void *userdata)
{
if (get_info ())
{
/* omit cryptic server message parts */
xchat_printf (ph, "%s\t%s\n", name, ++word_eol[4]);
xchat_commandf (ph, "QUOTE CAP END");
}

return XCHAT_EAT_ALL;
}

/*
static int
disconnect_cb (char *word[], void *userdata)
{
xchat_printf (ph, "disconnected\n");
return XCHAT_EAT_NONE;
}
*/

static int
server_cb (char *word[], char *word_eol[], void *userdata)
{
size_t len;
char* buf;
char* enc;
sasl_info* p;

if (strcmp ("AUTHENTICATE", word[1]) == 0 && strcmp ("+", word[2]) == 0)
{
p = get_info ();

if (!p)
{
return XCHAT_EAT_NONE;
}

xchat_printf (ph, "%s\tAuthenticating as %s\n", name, p->login);

len = strlen (p->login) * 2 + 2 + strlen (p->password);
buf = (char*) malloc (len + 1);
strcpy (buf, p->login);
strcpy (buf + strlen (p->login) + 1, p->login);
strcpy (buf + strlen (p->login) * 2 + 2, p->password);
enc = g_base64_encode ((unsigned char*) buf, len);

/* xchat_printf (ph, "AUTHENTICATE %s\}", enc); */
xchat_commandf (ph, "QUOTE AUTHENTICATE %s", enc);

free (enc);
free (buf);

return XCHAT_EAT_ALL;
}

return XCHAT_EAT_NONE;
}

static int
cap_cb (char *word[], char *word_eol[], void *userdata)
{
if (get_info ())
{
/* FIXME test sasl cap */
/* this is visible in the rawlog in case someone needs it, otherwise it's just noise */
/* xchat_printf (ph, "%s\t%s\n", name, word_eol[1]); */
xchat_commandf (ph, "QUOTE AUTHENTICATE PLAIN");
}

return XCHAT_EAT_ALL;
}

static int
sasl_cmd_cb (char *word[], char *word_eol[], void *userdata)
{
const char* login;
const char* password;
const char* network;
const char* mode = word[2];

if (!g_ascii_strcasecmp ("ADD", mode))
{
login = word[3];
password = word[4];
network = word_eol[5];

if (!network || !*network)	/* only check for the last word, if it's there, the previous ones will be there, too */
{
xchat_printf (ph, "%s", sasl_help);
return XCHAT_EAT_ALL;
}

if (add_info (login, password, network))
{
xchat_printf (ph, "%s\tEnabled SASL authentication for the \"%s\" network\n", name, network);
}
else
{
xchat_printf (ph, "%s\tFailed to enable SASL authentication for the \"%s\" network\n", name, network);
}

return XCHAT_EAT_ALL;
}
else if (!g_ascii_strcasecmp ("DEL", mode))
{
network = word_eol[3];

if (!network || !*network)
{
xchat_printf (ph, "%s", sasl_help);
return XCHAT_EAT_ALL;
}

if (del_info (network))
{
xchat_printf (ph, "%s\tDisabled SASL authentication for the \"%s\" network\n", name, network);
}
else
{
xchat_printf (ph, "%s\tFailed to disable SASL authentication for the \"%s\" network\n", name, network);
}

return XCHAT_EAT_ALL;
}
else if (!g_ascii_strcasecmp ("LIST", mode))
{
print_info ();
return XCHAT_EAT_ALL;
}
else
{
xchat_printf (ph, "%s", sasl_help);
return XCHAT_EAT_ALL;
}
}

static int
connect_cb (char *word[], void *userdata)
{
if (get_info ())
{
xchat_printf (ph, "%s\tSASL enabled\n", name);
xchat_commandf (ph, "QUOTE CAP REQ :sasl");
}

return XCHAT_EAT_NONE;
}

int
xchat_plugin_init (xchat_plugin *plugin_handle, char **plugin_name, char **plugin_desc, char **plugin_version, char *arg)
{
/* we need to save this for use with any xchat_* functions */
ph = plugin_handle;

/* tell xchat our info */
*plugin_name = name;
*plugin_desc = desc;
*plugin_version = version;

xchat_hook_command (ph, "SASL", XCHAT_PRI_NORM, sasl_cmd_cb, sasl_help, 0);
xchat_hook_print (ph, "Connected", XCHAT_PRI_NORM, connect_cb, NULL);
/* xchat_hook_print (ph, "Disconnected", XCHAT_PRI_NORM, disconnect_cb, NULL); */
xchat_hook_server (ph, "CAP", XCHAT_PRI_NORM, cap_cb, NULL);
xchat_hook_server (ph, "RAW LINE", XCHAT_PRI_NORM, server_cb, NULL);
xchat_hook_server (ph, "903", XCHAT_PRI_NORM, authend_cb, NULL);
xchat_hook_server (ph, "904", XCHAT_PRI_NORM, authend_cb, NULL);
xchat_hook_server (ph, "905", XCHAT_PRI_NORM, authend_cb, NULL);
xchat_hook_server (ph, "906", XCHAT_PRI_NORM, authend_cb, NULL);
xchat_hook_server (ph, "907", XCHAT_PRI_NORM, authend_cb, NULL);

xchat_printf (ph, "%s plugin loaded\n", name);

return 1;
}

int
xchat_plugin_deinit (void)
{
xchat_printf (ph, "%s plugin unloaded\n", name);
return 1;
}