/*File generated automatically in asus16 by tiago on qua jan 30 17:40:17 WET 2019*/
#ifdef __cplusplus
extern "C" {
#endif
/* main.c */
int main (int argc, char *argv[]);
/* callbacks.c */
char *Get_pos_cart_char ();
char *Get_pos_joint_char ();
void InterceptCTRL_C (int a);
char *Mov_Joints (float J1, float J2, float J3, float J4, float J5, float J6);
char *Mov_cart (float C1, float C2, float C3, float C4, float C5, float C6);
void button_stop_clicked_cb (GtkWidget * widget, gpointer user_data);
gboolean my_timeout (gpointer user_data);
void on_ButtonQuit_clicked (GtkWidget * widget, gpointer user_data);
void on_button_camera_clicked (GtkWidget * widget, gpointer user_data);
void on_button_connect_clicked (GtkWidget * widget, gpointer user_data);
void on_button_gripper_clicked (GtkWidget * widget, gpointer user_data);
void on_button_incremento_down_clicked (GtkWidget * widget, gpointer user_data);
void on_button_incremento_up_clicked (GtkWidget * widget, gpointer user_data);
void on_button_info_ok_clicked (GtkWidget * widget, gpointer user_data);
void on_button_j1_clicked (GtkWidget * widget, gpointer user_data);
void on_button_j2_clicked (GtkWidget * widget, gpointer user_data);
void on_button_j3_clicked (GtkWidget * widget, gpointer user_data);
void on_button_j4_clicked (GtkWidget * widget, gpointer user_data);
void on_button_j5_clicked (GtkWidget * widget, gpointer user_data);
void on_button_j6_clicked (GtkWidget * widget, gpointer user_data);
void on_button_jj1_clicked (GtkWidget * widget, gpointer user_data);
void on_button_jj2_clicked (GtkWidget * widget, gpointer user_data);
void on_button_jj3_clicked (GtkWidget * widget, gpointer user_data);
void on_button_jj4_clicked (GtkWidget * widget, gpointer user_data);
void on_button_jj5_clicked (GtkWidget * widget, gpointer user_data);
void on_button_jj6_clicked (GtkWidget * widget, gpointer user_data);
void on_button_program_exec_clicked (GtkWidget * widget);
void on_button_quit_camera_clicked (GtkWidget * widget, gpointer user_data);
void on_button_sen_clicked (GtkWidget * widget, gpointer user_data);
void on_button_send_activate (GtkWidget * widget, gpointer user_data);
void on_button_swop_frame_clicked (GtkWidget * widget, gpointer user_data);
void on_button_vel_ok_clicked (GtkWidget * widget, gpointer user_data);
void on_button_view_program_clicked (GtkWidget * widget, gpointer user_data);
void on_button_zero_clicked (GtkWidget * widget, gpointer user_data);
gboolean on_drawingareasingle_draw (GtkWidget * widget, GdkEvent * event, gpointer user_data);
void on_refresh_list_tp_clicked (GtkWidget * widget, gpointer user_data);
gboolean pari_UpdateMyDisplay (gpointer data);
void pari_delete_event (GtkWidget * window, GdkEvent * event, gpointer data);
char *replace (const char *s, char ch, const char *repl);
/* myf.c */
int ChildMain3 (int argc, char *argv[]);
int GetSharedMem ();
void MessageFromChild (char *str);
void MessageFromParent (char *str);
int ParentMain3 (int argc, char *argv[]);
void get_cartesian_positions (char *string_line);
void get_joint_positions (char *string_line);
char *get_tp_programs (char *string_line);
GdkPixbuf *pari_ConvertOpenCv2Gtk (IplImage * image, int dst_w, int dst_h);
void pari_PerformImageAcquisition (CvCapture * capt);
void pari_RefreshDrawingArea (char *widgetName, IplImage * img);
CvCapture *pari_StartImageAcquisition (int argument);
#ifdef __cplusplus
}
#endif
