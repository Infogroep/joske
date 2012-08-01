#include <stdio.h>
#include <string.h>
#include <strings.h>

#include <libircclient.h>

#define BOTNAME "joske"
#define IRCSERVER "irc.krey.net"
#define dprintf(format, ...) fprintf(stderr, format, __VA_ARGS__)

static void IG_event_handler(irc_session_t * session, const char * event,
		const char * origin, const char ** params, unsigned count) {

	//
	const char ** params_iter = params;
	unsigned count_iter = count;
	dprintf("EVENT: %s | ORIGIN: %s | PARAMS: %d", event, origin, count);
	if (count) {
		dprintf(" (%s", *params_iter++);
		while (--count_iter)
			dprintf(", %s", *params_iter++);
		dprintf("%s", ")");
	}
	dprintf("%s", "\n");
	//

	static const char * channel = "#infogroep";

	if (0 == strcmp("CONNECT", event)) {
		dprintf("Joining %s\n", channel);
		irc_cmd_join(session, channel, NULL);
		return;
	}

	if (0 == strcmp("JOIN", event)) {
		dprintf("%s\n", "Saying Hi!");
		irc_cmd_msg(session, channel, "Hello there!");
		return;
	}

	if (0 == strcmp("CHANNEL", event)) {
#define nick_maxsz 128
		static char nick[nick_maxsz];
		irc_target_get_nick(origin, nick, nick_maxsz);
		if (2 == count) {
			dprintf("%s SAID on CHANNEL %s: \"%s\"\n", nick, params[0], params[1]);
			if (0 == strcasecmp("GA WEG!", params[1])) {
				irc_cmd_msg(session, channel, "okay :(");
				irc_cmd_quit(session, "Chased away by angry people on the internet.");
			}
		}
		return;
	}

	dprintf("Event '%s' not handled\n", event);
}

static void IG_eventcode_handler(irc_session_t * session, unsigned int event,
		const char * origin, const char ** params, unsigned int count) {
	//
	const char ** params_iter = params;
	unsigned count_iter = count;
	dprintf("EVENT: %d | ORIGIN: %s | PARAMS: %d", event, origin, count);
	if (count) {
		dprintf(" (%s", *params_iter++);
		while (--count_iter)
			dprintf(", %s", *params_iter++);
		dprintf("%s", ")");
	}
	dprintf("%s", "\n");
	//
}

static void IG_dcc_chat_handler(irc_session_t * session, const char * nick,
		const char * addr, irc_dcc_t dccid) {
	dprintf("%s\n", "DCC CHAT REQUEST IGNORED");
}

static void IG_dcc_send_handler(irc_session_t * session, const char * nick,
		const char * addr, const char * filename, unsigned long size, irc_dcc_t dccid) {
	dprintf("%s\n", "DCC SEND REQUEST IGNORED");
}

int main(void) {
	irc_callbacks_t IGcallbacks;
	irc_session_t * IGsession;

	IGcallbacks.event_connect      = &IG_event_handler;
	IGcallbacks.event_nick         = &IG_event_handler;
	IGcallbacks.event_quit         = &IG_event_handler;
	IGcallbacks.event_join         = &IG_event_handler;
	IGcallbacks.event_part         = &IG_event_handler;
	IGcallbacks.event_mode         = &IG_event_handler;
	IGcallbacks.event_umode        = &IG_event_handler;
	IGcallbacks.event_topic        = &IG_event_handler;
	IGcallbacks.event_kick         = &IG_event_handler;
	IGcallbacks.event_channel      = &IG_event_handler;

	IGcallbacks.event_privmsg      = &IG_event_handler;
	IGcallbacks.event_notice       = &IG_event_handler;
	IGcallbacks.event_invite       = &IG_event_handler;
	IGcallbacks.event_ctcp_req     = &IG_event_handler;
	IGcallbacks.event_ctcp_rep     = &IG_event_handler;
	IGcallbacks.event_ctcp_action  = &IG_event_handler;
	IGcallbacks.event_unknown      = &IG_event_handler;

	IGcallbacks.event_numeric      = &IG_eventcode_handler;
	IGcallbacks.event_dcc_chat_req = &IG_dcc_chat_handler;
	IGcallbacks.event_dcc_send_req = &IG_dcc_send_handler;

	dprintf("%s\n", "Starting Up");
	dprintf("%s\n", "Creating Session");
	IGsession = irc_create_session(&IGcallbacks);
	dprintf("%s\n", "Setting Up Session");
	irc_option_set(IGsession, LIBIRC_OPTION_DEBUG);
	dprintf("%s\n", "Connecting To IRC Server");
	irc_connect(IGsession, IRCSERVER, 6667, NULL, BOTNAME, BOTNAME, BOTNAME);
	dprintf("%s\n", "Running Main Event Loop");
	irc_run(IGsession);
	dprintf("%s\n", "Destroying Session");
	irc_destroy_session(IGsession);
	dprintf("%s\n", "Shutting Down");
	return 0;
}
