PROJECT(PChat C)

cmake_minimum_required( VERSION 2.6.0 )

option( ENABLE_OPENSSL "Build with openssl support" ON )
option( ENABLE_PLUGINS "Build with plugin support" ON )
option( ENABLE_GTK "Build the gtk interface" ON )
option( ENABLE_TEXT "Build the text interface" OFF )

IF ( WIN32 )
	ADD_DEFINITIONS (-DWIN32)
ENDIF ( WIN32 )

SET(SRC_COMMON
    src/common/cfgfiles.c
    src/common/chanopt.c
    src/common/ctcp.c
    src/common/dcc.c
    src/common/history.c
    src/common/ignore.c
    src/common/inbound.c
    src/common/modes.c
    src/common/msproxy.c
    src/common/network.c
    src/common/notify.c
    src/common/outbound.c
    src/common/plugin-timer.c
    src/common/proto-irc.c
    src/common/server.c
    src/common/servlist.c
    src/common/strlutil.c
    src/common/text.c
    src/common/thread.c
    src/common/tree.c
    src/common/url.c
    src/common/userlist.c
    src/common/util.c
    src/common/xchat.c
)

SET SRC_MAKETE (
	src/common/make-te.c
)

IF( ENABLE_GTK )
SET SRC_FEGTK (
	src/fe-gtk/about.c
	src/fe-gtk/ascii.c
	src/fe-gtk/banlist.c
	src/fe-gtk/chanlist.c
	src/fe-gtk/chanview.c
	src/fe-gtk/custom-list.c
	src/fe-gtk/dccgui.c
	src/fe-gtk/editlist.c
	src/fe-gtk/fe-gtk.c
	src/fe-gtk/fkeys.c
	src/fe-gtk/gtkutil.c
	src/fe-gtk/ignoregui.c
	src/fe-gtk/joind.c
	src/fe-gtk/maingui.c
	src/fe-gtk/menu.c
	src/fe-gtk/notifygui.c
	src/fe-gtk/palette.c
	src/fe-gtk/pixmaps.c
	src/fe-gtk/plugin-tray.c
	src/fe-gtk/rawlog.c
	src/fe-gtk/search.c
	src/fe-gtk/servlistgui.c
	src/fe-gtk/setup.c
	src/fe-gtk/sexy-iso-codes.c
	src/fe-gtk/sexy-marshal.c
	src/fe-gtk/sexy-spell-entry.c
	src/fe-gtk/textgui.c
	src/fe-gtk/urlgrab.c
	src/fe-gtk/userlistgui.c
	src/fe-gtk/xtext.c
)

ADD_EXECUTABLE( PChat ${SRC_FEGTK} )

TARGET_LINK_LIBRARIES (
	PChat
	libpchatcommon
	${GTK_LIBRARIES}
	${GLIB_LIBRARIES}
	${GMODULE_LIBRARIES}
)
ENDIF( ENABLE_GTK )

ADD_EXECUTABLE( make-te ${SRC_MAKETE} )
ADD_LIBRARY( libpchatcommon static ${SRC_COMMON} )
TARGET_LINK_LIBRARIES (libpchatcommon
	${GLIB_LIBRARIES}
	${GMODULE_LIBRARIES}
)

