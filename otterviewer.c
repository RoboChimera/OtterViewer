#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>

int hasOpened = 0;

static gboolean on_configure_event(GtkWidget *window, GdkEventConfigure *event, gpointer user_data) {
	int windowWidth = event->width;
	int windowHeight = event->height;
	int minWidth;
	int minHeight;

	GtkWidget *image = GTK_WIDGET(user_data);

	const GdkPixbuf *imgPixBuf = gtk_image_get_pixbuf(GTK_IMAGE(image));

	double scaleWidth = (double)windowWidth / gdk_pixbuf_get_width(imgPixBuf);
	double scaleHeight = (double)windowHeight / gdk_pixbuf_get_height(imgPixBuf);

	/*
	if (hasOpened == 0) {
		minWidth = event->width;
		minHeight = event->height;
		hasOpened = 1;
		g_print("%i", hasOpened);
	}
	*/

	// Use nearest-neighbor algorithm for scaling
	GdkPixbuf *scaledPixBuf = gdk_pixbuf_scale_simple(
			imgPixBuf,
			windowWidth, windowHeight,
			GDK_INTERP_NEAREST
	);


	//if (windowWidth > minWidth || windowHeight > minHeight) {
		gtk_image_set_from_pixbuf(GTK_IMAGE(image), scaledPixBuf);
		gtk_widget_set_size_request(image, scaleWidth, scaleHeight);
		gtk_window_resize(GTK_WINDOW(window), windowWidth, windowHeight);
		g_print("Window size changed to %ix%i\n", windowWidth, windowHeight);
	//}

	return TRUE;
}

int main(int argc, char *argv[]) {
	GtkWidget *window, *grid, *image;
	gtk_init(&argc, &argv);

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "OtterViewer");
	//gtk_window_set_resizable(GTK_WINDOW(window), FALSE);

	grid = gtk_grid_new();

	g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);

	if (argv[1] != NULL) {
		image = gtk_image_new_from_file(argv[1]);
		printf("%s\n", argv[1]);
		gtk_grid_attach(GTK_GRID(grid), image, 0, 0, 1, 1);
		const GdkPixbuf *imgPixBuf = gtk_image_get_pixbuf(GTK_IMAGE(image));
		printf("%ux%u\n", gdk_pixbuf_get_width(imgPixBuf), gdk_pixbuf_get_height(imgPixBuf));
		//gtk_window_set_default_size(GTK_WINDOW(window), gdk_pixbuf_get_width(imgPixBuf), gdk_pixbuf_get_height(imgPixBuf));

		g_signal_connect(G_OBJECT(window), "configure-event", G_CALLBACK(on_configure_event), image);
		//gtk_window_set_resizable(GTK_WINDOW(window), FALSE);

		gtk_container_add(GTK_CONTAINER(window), grid);
		gtk_widget_show_all(window);
		gtk_main();

		return 0;
	} else {
		printf("Please provide a valid image");
		return 1;
	}
}
