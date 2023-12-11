#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>

int hasScaled = 0;
int width, height;

void button_clicked(GtkWidget *widget, gpointer data) {
	const gchar *entry = gtk_entry_get_text(GTK_ENTRY(data));
	float scale = atof(entry);

	GtkWidget *image = g_object_get_data(G_OBJECT(widget), "image");
	GdkPixbuf *imgBuf = gtk_image_get_pixbuf(GTK_IMAGE(image));

	int debug1 = gdk_pixbuf_get_width(imgBuf);
	if (hasScaled == 0) {
		width = gdk_pixbuf_get_width(imgBuf);
		height = gdk_pixbuf_get_height(imgBuf);
		hasScaled = 1;
	}

	int scaledWidth = width * scale;
	int scaledHeight = height * scale;

	GdkPixbuf *scaleBuf = gdk_pixbuf_scale_simple(
		imgBuf,
		scaledWidth, scaledHeight,
		GDK_INTERP_NEAREST
	);

	gtk_image_set_from_pixbuf(GTK_IMAGE(image), scaleBuf);
	g_object_unref(scaleBuf);
	g_print("%.2f: %ix%i\n", scale, scaledWidth, scaledHeight);
}

int main(int argc, char *argv[]) {
	GtkWidget *window, *grid, *image, *scale, *button;
	gtk_init(&argc, &argv);

	GdkScreen *screen = gdk_screen_get_default();

	gint screenWidth = gdk_screen_get_width(screen);
	gint screenHeight = gdk_screen_get_height(screen);

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "OtterViewer");
	gtk_window_set_resizable(GTK_WINDOW(window), FALSE);

	grid = gtk_grid_new();

	g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);

	if (argv[1] != NULL) {
		// Create the Image
		image = gtk_image_new_from_file(argv[1]);
		GdkPixbuf *imgBuf = gtk_image_get_pixbuf(GTK_IMAGE(image));

		int imageWidth = gdk_pixbuf_get_width(imgBuf);
		int imageHeight = gdk_pixbuf_get_height(imgBuf);

		while (screenWidth - 24 < imageWidth || screenHeight - 24 < imageHeight) {
			imageWidth = gdk_pixbuf_get_width(imgBuf) / 2;
			imageHeight = gdk_pixbuf_get_height(imgBuf) / 2;
			GdkPixbuf *scaleBuf = gdk_pixbuf_scale_simple(
				imgBuf,
				imageWidth, imageHeight,
				GDK_INTERP_NEAREST
			);

			g_print("%ix%i\n", imageWidth, imageHeight);
			gtk_image_set_from_pixbuf(GTK_IMAGE(image), scaleBuf);
		}

		// Create the input box for scaling
		GtkWidget *scale = gtk_entry_new();
		gtk_entry_set_placeholder_text(GTK_ENTRY(scale), "1.0");

		// Create the button
		GtkWidget *button = gtk_button_new_with_label("Scale");
		g_signal_connect(button, "clicked", G_CALLBACK(button_clicked), scale);

		// Add Everything to the grid
		gtk_grid_attach(GTK_GRID(grid), image, 0, 0, 1, 1);
		gtk_grid_attach(GTK_GRID(grid), scale, 0, 1, 1, 1);
		gtk_grid_attach(GTK_GRID(grid), button, 1, 1, 1, 1);

		g_object_set_data(G_OBJECT(button), "image", image);


		gtk_container_add(GTK_CONTAINER(window), grid);
		gtk_widget_show_all(window);
		gtk_main();

		return 0;
	} else {
		printf("Please provide a valid image");
		return 1;
	}
}
