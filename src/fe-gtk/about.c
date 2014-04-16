#define GTK_DISABLE_DEPRECATED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fe-gtk.h"

#include <gtk/gtk.h>

#include "../common/xchat.h"
#include "../common/util.h"
#include "palette.h"
#include "pixmaps.h"
#include "gtkutil.h"
#include "about.h"

static const gchar *authors[] = {
	"Zach Thibeau <zachthibeau@pchat-irc.net>",
	"Luis Miranda <luisr142004@gmail.com>",
	"Jesse N. Richardson <flare@flare183.org>",
	"XhmikosR <xhmikosr@pchat-irc.net",
	"NishaKitty :3 <kitatake@gmail.com>",
	"Based on XChat by Peter Zelezny <zed@xchat.org>",
	NULL
};

static const gchar *artists[] = {
	"neorame for the logo",
	NULL
};


static const gchar license[] = 
	"This program is free software; you can redistribute it and/or modify "
	"it under the terms of the GNU General Public License as published by "
	"the Free Software Foundation; either version 2 of the License, or "
	"(at your option) any later version.\n"
	"\n"
	"This program is distributed in the hope that it will be useful, "
	"but WITHOUT ANY WARRANTY; without even the implied warranty of "
	"MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the "
	"GNU General Public License for more details.\n"
	"\n"
	"You should have received a copy of the GNU General Public License "
	"along with this program; if not, write to the Free Software "
	"Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.";

void
menu_about (GtkWidget * wid, gpointer sess)
{
	GdkPixbuf *about_pix;
	extern GtkWindow *parent_window;      /* maingui.c */

	about_pix = gdk_pixbuf_scale_simple(pix_xchat, 128, 128, GDK_INTERP_BILINEAR);

	gtk_show_about_dialog(parent_window,
#if GTK_CHECK_VERSION (2, 11, 0)
		"program-name", "PChat",
#else
		"name", "PChat",
#endif
		"authors", authors,
		"license", license,
		"artists", artists,
		"wrap-license", TRUE,
		"logo", about_pix,
		"copyright", "Copyright (c) 2009-2012 PChat IRC Team",
		"version", PACKAGE_VERSION,
		"website", "http://www.pchat-irc.net",
		"comments", _("An advanced graphical IRC client."),
		NULL);

	g_object_unref(about_pix);
}
