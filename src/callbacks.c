/**
 * @file callbacks.c File with all callbacks needed, and auxiliar functions . 
 * @author Tiago Tavares (tiagoatavares@ua.pt)
 * @brief Callbacks to GTK, and aux functions!
 * @version 0.1
 * @date 2018-12-26
 * 
 * @copyright Copyright (c) 2018
 * 
 */
#include "myf.h"

/**
* @brief  Callback to process the delete_event (usually, window kill request)
*
* @param  window - The widget that generated the event (not used here)
* @param  event - The event description (not used here)
* @param  data - generic user data (not used here)
* @return void
*/
void pari_delete_event(GtkWidget *window, GdkEvent *event, gpointer data)
{
        g_print("you should not use the os to force leave\n");
        gtk_main_quit();
}

/**
* @brief  Callback to catch CTRL-C
*/
void InterceptCTRL_C(int a)
{
        g_print("Sair por CTRL-C\n");
        gtk_main_quit();
}

/**
 * @brief Fuction to show an aditional info window
 * 
 * @param widget The event description (not used here)
 * @param user_data generic user data (not used here)
 */
void on_button_sen_clicked(GtkWidget *widget, gpointer user_data)
{
        GtkWidget *w = GTK_WIDGET(gtk_builder_get_object(builderG, "messagedialog1"));
        gtk_widget_show(w);
}

/**
 * @brief Function to close info window
 * 
 * @param widget The event description (not used here)
 * @param user_data generic user data (not used here)
 */
void on_button_info_ok_clicked(GtkWidget *widget, gpointer user_data)
{
        GtkWidget *w = GTK_WIDGET(gtk_builder_get_object(builderG, "messagedialog1"));
        gtk_widget_hide(w);
}

/**
 * @brief  Function to process the text from entry, send to server and uptade server repply.
 *
 * @param widget - the one that originated the call
 * @param user_data - Custom data with a widget ID to fetch some info
 * @return void
 */
void on_button_send_activate(GtkWidget *widget, gpointer user_data)
{
        GtkEntry *ts = GTK_ENTRY(user_data);
        GtkLabel *lbl = GTK_LABEL(gtk_builder_get_object(builderG, "label_respostaserver")); //Get some specific label ID to change it
        GtkLabel *lbl_info = GTK_LABEL(gtk_builder_get_object(builderG, "label_info"));
        int i = 0;
        char ts_text[50];

        if (data->connection)
        {
                if (ts)
                {
                        const char *tss = gtk_entry_get_text(ts); //get the text from entry1 widget

                        strcpy(ts_text, tss);

                        while (ts_text[i] != '\0') // string so com ' ' , 'numeros', e '.'
                        {
                                if (ts_text[i] == '*')
                                {
                                        ts_text[i] = '\n';
                                }
                                i++;
                        }

                        strcpy(data->pergunta, ts_text); //copia entry para enviar!

                        data->resposta_done = 0;
                        data->pedido = 1;
                        printf("Text available in entry1: %s\n", tss);
                        //gtk_label_set_text(lbl, tss);   //set entry text in "label2" widget

                        while (data->resposta_done == 0)
                        {
                                ;
                        }

                        if (data->connection)
                        {
                                gtk_label_set_text(lbl, data->resposta); //set frame tipe in "label2" widget
                        }
                        else
                        {
                                gtk_label_set_text(lbl, "Please, connect to server first!");
                        }
                        //printf("Text available in resposta: %s\n", data->resposta);
                }
        }
        else
        {
                gtk_label_set_text(lbl_info, "Please, connect to server first!");
                ;
        }
}

/**
 * @brief Function to show all TP programs ! 
 * 
 * @param widget The event description (not used here)
 * @param user_data generic user data (not used here)
 */
void on_button_view_program_clicked(GtkWidget *widget, gpointer user_data)
{
        //GtkEntry *ts = GTK_ENTRY(user_data);
        GtkLabel *lbl = GTK_LABEL(gtk_builder_get_object(builderG, "label_respostaserver")); //Get some specific label ID to change it
        GtkLabel *lbl_info = GTK_LABEL(gtk_builder_get_object(builderG, "label_info"));
        printf("BUTTON 'view_program' Clicked");
        if (data->connection)
        {
                strcpy(data->pergunta, "LISTTPP\n");
                data->resposta_done = 0;
                data->pedido = 1;

                while (data->resposta_done == 0)
                {
                        ;
                }

                //gtk_label_set_text(lbl, data->resposta); //set frame tipe in "label2" widget
                //descomentar

                //printf("Resposta_para_callback=%s\n", data->resposta);
        }
        else
        {
                gtk_label_set_text(lbl_info, "Please, connect to server first!");
        }
}

/**
 * @brief  Function to connect/disconnect to server, and uptade info on label_status
 *
 * @param widget The event description (not used here)
 * @param user_data generic user data (not used here)
 * @return void
 */
void on_button_connect_clicked(GtkWidget *widget, gpointer user_data)
{

        if (!data->pedido_connection) // se tiver a 0
        {
                data->pedido_connection = 1;
        }
        else //se tiver a 1 desliga
        {
                data->pedido_connection = 0;
                data->connection = 0;
        }
        //GtkEntry *ts = GTK_ENTRY(user_data);
        GtkLabel *lbl = GTK_LABEL(gtk_builder_get_object(builderG, "label_status")); //Get some specific label ID to change it
        GtkLabel *lbl_info = GTK_LABEL(gtk_builder_get_object(builderG, "label_info"));
        usleep(500000);
        if (data->connection)
        {
                gtk_label_set_text(lbl, "Connected"); //set frame tipe in "label2" widget
        }
        else
        {
                gtk_label_set_text(lbl, "Not Connected");
                gtk_label_set_text(lbl_info, "Connection lost! check server status..");
        }

        //inciar o timer para real time info
        gdk_threads_add_timeout(50, my_timeout, NULL);
}

/**
 * @brief  Function to open/close gripper and uptade info on label
 *
 * @param widget The event description (not used here)
 * @param user_data generic user data (not used here)
 * @return void
 */
void on_button_gripper_clicked(GtkWidget *widget, gpointer user_data)
{
        GtkLabel *lbl_resposta = GTK_LABEL(gtk_builder_get_object(builderG, "label_info"));
        if (data->connection)
        {
                gtk_label_set_text(lbl_resposta, "____________");

                GtkLabel *lbl = GTK_LABEL(gtk_builder_get_object(builderG, "label_gripper")); //Get some specific label ID to change it
                if (!data->gripper)
                {
                        strcpy(data->pergunta, "SETDIO\n2 7 0\n"); //copia entry para enviar!
                }
                else
                {
                        strcpy(data->pergunta, "SETDIO\n2 7 1\n");
                }
                data->resposta_done = 0;
                data->pedido = 1;
                printf("Button->Send to server: %s\n", data->pergunta);
                //gtk_label_set_text(lbl, tss);   //set entry text in "label2" widget

                while (data->resposta_done == 0)
                {
                        ;
                }

                if (!strncmp(data->resposta, "1\n", 2)) // se forem iguais retorna 0
                {
                        data->gripper = !data->gripper;
                }

                if (data->gripper)
                {
                        gtk_label_set_text(lbl, "Gripper ON"); //set frame tipe in "label2" widget
                }
                else
                {
                        gtk_label_set_text(lbl, "Gripper OFF");
                }
        }
        else
        {
                gtk_label_set_text(lbl_resposta, "Please, connect to server first!");
        }
}

/**
 * @brief  Function to tell robot to go to hardware postion ! 
 *
 * @param widget The event description (not used here)
 * @param user_data generic user data (not used here)
 * @return void
 */
void on_button_zero_clicked(GtkWidget *widget, gpointer user_data)
{
        GtkLabel *lbl_resposta = GTK_LABEL(gtk_builder_get_object(builderG, "label_info"));
        if (data->connection)
        {
                gtk_label_set_text(lbl_resposta, "____________");

                char *str = (char *)Mov_Joints(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
                strcpy(data->pergunta, str);
                free(str);
                data->resposta_done = 0;
                data->pedido = 1;
                printf("Button->Send to server: %s\n", data->pergunta);
                //gtk_label_set_text(lbl, tss);   //set entry text in "label2" widget

                while (data->resposta_done == 0)
                {
                        ;
                }

                if (!strncmp(data->resposta, "1\n", 2)) // se forem iguais retorna 0
                {
                        gtk_label_set_text(lbl_resposta, "RESET DONE");
                }
        }
        else
        {
                gtk_label_set_text(lbl_resposta, "Please, connect to server first!");
        }
}
/**
 * @brief Function to swop frames
 * 
 * @param widget The event description (not used here)
 * @param user_data generic user data (not used here)
 */
void on_button_swop_frame_clicked(GtkWidget *widget, gpointer user_data)
{
        if (!strncmp(data->frame, "J", 1))
        {
                strcpy(data->frame, "World");
        }
        else
        {
                strcpy(data->frame, "Joint");
        }
        printf("Button swop-> coordenadas=%s\n", data->frame);
}
/**
 * @brief Function to set velocity!
 * 
 * @param widget - the one that originated the call
 * @param user_data - Custom data with a widget ID (velocity label) to fetch some info
 */
void on_button_vel_ok_clicked(GtkWidget *widget, gpointer user_data)
{
        GtkEntry *ts = GTK_ENTRY(user_data);
        GtkLabel *lbl_resposta = GTK_LABEL(gtk_builder_get_object(builderG, "label_info"));
        GtkLabel *lbl = GTK_LABEL(gtk_builder_get_object(builderG, "label_velocidade")); //Get some specific label ID to change it
        if (ts)
        {
                if (data->connection)
                {

                        const char *tss = gtk_entry_get_text(ts); //get the text from entry1 widget
                        char string[15];
                        int speed = atoi(tss);
                        //printf("Text available in entry: %s\n", tss);
                        if (speed > 0)
                        {
                                sprintf(string, "SETLSPEED\n%s\n", tss);

                                strcpy(data->pergunta, string); //copia entry para enviar!
                                data->resposta_done = 0;
                                data->pedido = 1;
                                printf("Button Veloc-> pergunta=%s\n", data->pergunta);

                                while (data->resposta_done == 0)
                                {
                                        ;
                                }

                                if (!strncmp(data->resposta, "Setting motion speed.", 21)) // se forem iguais retorna 0
                                {
                                        data->velocidade = atoi(tss);
                                }

                                gtk_label_set_text(lbl_resposta, "___________________");
                        }
                        else
                        {
                                gtk_label_set_text(lbl_resposta, "Invalid speed value");
                        }
                }
                else
                {
                        gtk_label_set_text(lbl_resposta, "Please, connect to server first!");
                }
        }
}
/**
 * @brief Function that increse increment
 * 
 * @param widget The event description (not used here)
 * @param user_data generic user data (not used here)
 */
void on_button_incremento_up_clicked(GtkWidget *widget, gpointer user_data)
{
        GtkLabel *lbl = GTK_LABEL(gtk_builder_get_object(builderG, "label_incremento")); //Get some specific label ID to change it

        data->incremento++;
        //printf("Text available in entry: %s\n", tss);

        char strinng[5];
        sprintf(strinng, "%d", data->incremento);
        printf("Button incremento(+) -> incremento=%i\n", data->incremento);

        gtk_label_set_text(lbl, strinng);
}

/**
 * @brief Function that decrese increment
 * 
 * @param widget The event description (not used here)
 * @param user_data generic user data (not used here)
 */
void on_button_incremento_down_clicked(GtkWidget *widget, gpointer user_data)
{
        GtkLabel *lbl = GTK_LABEL(gtk_builder_get_object(builderG, "label_incremento")); //Get some specific label ID to change it

        data->incremento--;
        //printf("Text available in entry: %s\n", tss);

        char strinng[5];
        sprintf(strinng, "%d", data->incremento);
        printf("Button incremento(-) -> incremento=%i\n", data->incremento);

        gtk_label_set_text(lbl, strinng);
}

/**
 * @brief Function to run a program!
 * 
 * @param widget The event description (not used here)
 */
void on_button_program_exec_clicked(GtkWidget *widget)
{
        GtkLabel *lbl = GTK_LABEL(gtk_builder_get_object(builderG, "label_info")); //Get some specific label ID to change it
        GtkComboBoxText *combobox = GTK_COMBO_BOX_TEXT(gtk_builder_get_object(builderG, "combobox"));

        if (data->connection)
        {
                const char *tss = gtk_combo_box_text_get_active_text((GtkComboBoxText *)combobox);
                char string[30];
                char tp_prog[15];
                //printf("Text available in entry: %s\n", tss);

                if (tss)
                {
                        strncpy(tp_prog, tss, strlen(tss) - 3);
                        //printf("tp_prog=%s", tp_prog);

                        sprintf(string, "RUNTPP\n%s\n 1\n", tp_prog);

                        strcpy(data->pergunta, string); //copia entry para enviar!
                        data->resposta_done = 0;
                        data->pedido = 1;
                        printf("Button exec prog-> pergunta=%s\n", data->pergunta);

                        while (data->resposta_done == 0)
                        {
                                ;
                        }

                        if (!strncmp(data->resposta, "1\n", 2)) // se forem iguais retorna 0
                        {
                                gtk_label_set_text(lbl, "Program running");
                        }
                        else
                        {
                                gtk_label_set_text(lbl, "Can not execute the program. Try again, please.");
                        }
                }
                else
                {
                        gtk_label_set_text(lbl, "Select a program, please!");
                }
        }
        else
        {
                gtk_label_set_text(lbl, "Please, connect to server first!");
        }
}

/**
 * @brief Function to stop motion!
 * 
 * @param widget The event description (not used here)
 * @param user_data generic user data (not used here)
 */
void button_stop_clicked_cb(GtkWidget *widget, gpointer user_data)
{
        GtkLabel *lbl = GTK_LABEL(gtk_builder_get_object(builderG, "label_info")); //Get some specific label ID to change it

        data->resposta_done = 0;
        data->pedido = 1;
        strcpy(data->pergunta, "MOTIONSTOP\n");

        while (data->resposta_done == 0)
        {
                ;
        }

        if (!strncmp(data->resposta, "Stopping all", 11)) // se forem iguais retorna 0
        {
                gtk_label_set_text(lbl, "Motion STOPED!");
        }
        else
        {
                gtk_label_set_text(lbl, "Can not stop the movement. Try again...");
        }
}

/**
 * @brief Function that returns a message to send , with specific joint values
 * 
 * @param J1 joint 1
 * @param J2 joint 2
 * @param J3 joint 3
 * @param J4 joint 4
 * @param J5  joint 5
 * @param J6  joint 6
 * @return char* string with specific format (Robcom prog)
 */
char *Mov_Joints(float J1, float J2, float J3, float J4, float J5, float J6)
{
        char *str_joints = (char *)malloc(100);
        char juntas[70];
        char junta1[10];
        char junta2[10];
        char junta3[10];
        char junta4[10];
        char junta5[10];
        char junta6[10];
        char junta7[2];
        char junta8[2];
        char junta9[2];
        sprintf(junta1, "%.3f", J1);
        //printf("J1=%.3f\n", J1);
        //printf("junta1_char=%s\n", junta1);
        sprintf(junta2, "%.3f", J2);
        sprintf(junta3, "%.3f", J3);
        sprintf(junta4, "%.3f", J4);
        sprintf(junta5, "%.3f", J5);
        sprintf(junta6, "%.3f", J6);
        sprintf(junta7, "%d", data->ty);
        sprintf(junta8, "%d", data->velocidade);
        sprintf(junta9, "%d", data->mode);

        sprintf(juntas, "%s  %s  %s  %s  %s  %s  %s  %s  %s", junta1, junta2, junta3, junta4, junta5, junta6, junta7, junta8, junta9);
        char *s = replace(juntas, ',', ".");
        //printf("%s",juntas);
        sprintf(str_joints, "MOVTOJPOS\n%s\n", s);

        //printf("str_joints=%s\n", str_joints);

        free(s);
        return str_joints;
}

/**
 * @brief Function that returns a message to send , with specific cartesian values
 * 
 * @param C1 X value
 * @param C2 Y value
 * @param C3 Z value
 * @param C4 W value
 * @param C5 R value
 * @param C6 P value
 * @return char* string with specific format (Robcom prog)
 */
char *Mov_cart(float C1, float C2, float C3, float C4, float C5, float C6)
{
        char *str_cartesian = (char *)malloc(100);
        char juntas[70];
        char junta1[10];
        char junta2[10];
        char junta3[10];
        char junta4[10];
        char junta5[10];
        char junta6[10];
        //char junta7[2];
        //char junta8[2];
        char junta9[2];
        sprintf(junta1, "%.3f", C1);
        //printf("%s",junta1);
        sprintf(junta2, "%.3f", C2);
        sprintf(junta3, "%.3f", C3);
        sprintf(junta4, "%.3f", C4);
        sprintf(junta5, "%.3f", C5);
        sprintf(junta6, "%.3f", C6);
        //sprintf(junta7, "%d", data->ty);
        //sprintf(junta8, "%d", data->velocidade);
        sprintf(junta9, "%d", data->mode);

        sprintf(juntas, "%s  %s  %s  %s  %s  %s  %s", junta1, junta2, junta3, junta4, junta5, junta6, junta9);
        char *s = replace(juntas, ',', ".");

        //printf("%s",juntas);
        sprintf(str_cartesian, "MOVTOCINC\n%s\n", s);

        //printf("str_cartesian=%s\n", str_cartesian);
        free(s);
        return str_cartesian;
}

/**
 * @brief replace a char on a string . adpted from: (https://stackoverflow.com/questions/12890008/replacing-character-in-a-string) 
 * 
 * @param s Original string
 * @param ch Char to be substituted
 * @param repl Char to replace the previous one
 * @return char* final string
 */
char *replace(const char *s, char ch, const char *repl)
{
        int count = 0;
        const char *t;
        for (t = s; *t; t++)
                count += (*t == ch);

        size_t rlen = strlen(repl);
        char *res = (char *)malloc(strlen(s) + (rlen - 1) * count + 1);
        char *ptr = res;
        for (t = s; *t; t++)
        {
                if (*t == ch)
                {
                        memcpy(ptr, repl, rlen);
                        ptr += rlen;
                }
                else
                {
                        *ptr++ = *t;
                }
        }
        *ptr = 0;
        return res;
}

/**
 * @brief timer to update position info 
 * 
 * @param user_data generic user data (not used here)
 */
gboolean my_timeout(gpointer user_data)
{
        //printf("time_out\n");
        GtkLabel *lblcart = GTK_LABEL(gtk_builder_get_object(builderG, "label_cartesianas"));
        GtkLabel *lbljoint = GTK_LABEL(gtk_builder_get_object(builderG, "label_joint"));
        GtkLabel *lblframe = GTK_LABEL(gtk_builder_get_object(builderG, "label_coordenadas"));
        GtkLabel *lblvelocidade = GTK_LABEL(gtk_builder_get_object(builderG, "label_velocidade"));

        GtkAdjustment *adjust1 = GTK_ADJUSTMENT(gtk_builder_get_object(builderG, "adjustment1"));
        GtkAdjustment *adjust2 = GTK_ADJUSTMENT(gtk_builder_get_object(builderG, "adjustment2"));
        GtkAdjustment *adjust3 = GTK_ADJUSTMENT(gtk_builder_get_object(builderG, "adjustment3"));
        GtkAdjustment *adjust4 = GTK_ADJUSTMENT(gtk_builder_get_object(builderG, "adjustment4"));
        GtkAdjustment *adjust5 = GTK_ADJUSTMENT(gtk_builder_get_object(builderG, "adjustment5"));
        GtkAdjustment *adjust6 = GTK_ADJUSTMENT(gtk_builder_get_object(builderG, "adjustment6"));

        double j1 = data->j1;
        double j2 = data->j2;
        double j3 = data->j3;
        double j4 = data->j4;
        double j5 = data->j5;
        double j6 = data->j6;

        gtk_adjustment_set_value(adjust1, j1);
        gtk_adjustment_set_value(adjust2, j2);
        gtk_adjustment_set_value(adjust3, j3);
        gtk_adjustment_set_value(adjust4, j4);
        gtk_adjustment_set_value(adjust5, j5);
        gtk_adjustment_set_value(adjust6, j6);

        char *str_cart = Get_pos_cart_char();
        char *str_joint = Get_pos_joint_char();

        gtk_label_set_text(lblcart, str_cart); //set entry text in "lblcart" widget
        gtk_label_set_text(lbljoint, str_joint);
        free(str_cart);
        free(str_joint);

        gtk_label_set_text(lblframe, data->frame);

        char stringg[5];
        sprintf(stringg, "%d", data->velocidade);
        gtk_label_set_text(lblvelocidade, stringg);

        if (data->connection == 1)
        {
                return TRUE;
        }
        else
        {
                return FALSE;
        }
}

/**
 * @brief Return a string with real cartesian positions (to use on GTK window)
 * 
 * @return char* Final string
 */
char *Get_pos_cart_char()
{
        char *str_cart_char = (char *)malloc(100);
        char junta1[10];
        char junta2[10];
        char junta3[10];
        char junta4[10];
        char junta5[10];
        char junta6[10];
        sprintf(junta1, "%.3f", data->c1);
        //printf("%s",junta1);
        sprintf(junta2, "%.3f", data->c2);
        sprintf(junta3, "%.3f", data->c3);
        sprintf(junta4, "%.3f", data->c4);
        sprintf(junta5, "%.3f", data->c5);
        sprintf(junta6, "%.3f", data->c6);

        sprintf(str_cart_char, "%s ; %s ; %s ; %s ; %s ; %s", junta1, junta2, junta3, junta4, junta5, junta6);

        //printf("%s",str_cart_char)
        return str_cart_char;
}

/**
 * @brief Return a string with real joint positions
 * 
 * @return char* Final string
 */
char *Get_pos_joint_char()
{
        char *str_joint_char = (char *)malloc(100);
        char junta1[10];
        char junta2[10];
        char junta3[10];
        char junta4[10];
        char junta5[10];
        char junta6[10];
        sprintf(junta1, "%.3f", data->j1);
        //printf("%s",junta1);
        sprintf(junta2, "%.3f", data->j2);
        sprintf(junta3, "%.3f", data->j3);
        sprintf(junta4, "%.3f", data->j4);
        sprintf(junta5, "%.3f", data->j5);
        sprintf(junta6, "%.3f", data->j6);

        sprintf(str_joint_char, "%s ; %s ; %s ; %s ; %s ; %s", junta1, junta2, junta3, junta4, junta5, junta6);

        //printf("%s",str_cart_char)
        return str_joint_char;
}

/**
 * @brief Function to refresh TP list programs
 * 
 * @param widget The event description (not used here)
 * @param user_data generic user data (not used here)
 */
void on_refresh_list_tp_clicked(GtkWidget *widget, gpointer user_data)

{
        char *token;
        char resposta[4086];
        char *program_list;
        int c = 1;
        char c_char[5];
        GtkLabel *lbl_resposta = GTK_LABEL(gtk_builder_get_object(builderG, "label_info"));
        GtkComboBoxText *combobox = GTK_COMBO_BOX_TEXT(gtk_builder_get_object(builderG, "combobox"));

        if (data->connection)
        {
                gtk_label_set_text(lbl_resposta, "____________");

                strcpy(data->pergunta, "LISTTPP\n");

                data->resposta_done = 0;
                data->pedido = 1;

                while (data->resposta_done == 0)
                {
                        ;
                }
                strncpy(resposta, data->resposta, 150);

                program_list = get_tp_programs(resposta);
                //printf("lista_tratada=%s\n", program_list);

                //get the first token
                token = strtok(program_list, " ");

                gtk_combo_box_text_remove_all((GtkComboBoxText *)combobox);

                // walk through other tokens
                /* optei por colocar apenas 15 programas na gtk, caso se queira todos é so tirar a segunda condicao do while (c != 15) */
                while ((token != NULL) && (c != 15))
                { // While there are more characters to process...
                        if (strcmp(token, "1"))
                        {
                                sprintf(c_char, "%d", c);
                                gtk_combo_box_text_append((GtkComboBoxText *)combobox, (const gchar *)c_char, (const gchar *)token);
                                //printf("token=%s\n",token);
                                c++;
                        }

                        token = strtok(NULL, " ");
                }

                free(program_list);
        }
        else
        {
                gtk_label_set_text(lbl_resposta, "Please, connect to server first!");
        }
}

/**
 * @brief Function quit!
 * 
 * @param widget The event description (not used here)
 * @param user_data generic user data (not used here)
 */
void on_ButtonQuit_clicked(GtkWidget *widget, gpointer user_data)
{
        gtk_main_quit();
}

//------------------------------------BUTTONS JUNTAS---------------------------------------------
/**
 * @brief Function to increse J1/X
 * 
 * @param widget The event description (not used here)
 * @param user_data generic user data (not used here)
 */
void on_button_jj1_clicked(GtkWidget *widget, gpointer user_data)
{
        GtkLabel *lbl_resposta = GTK_LABEL(gtk_builder_get_object(builderG, "label_info"));
        if (data->connection)
        {
                //printf("junta1=%d\n",data->j1);
                //printf("incremento=%d\n",data->incremento);
                float juntaa;

                //gtk_label_set_text(lbl_resposta, "____________");
                if (!strncmp(data->frame, "Jo", 2))
                {
                        juntaa = data->j1 + data->incremento;
                        //printf("juntaa=%d\n",juntaa);
                        printf("junta2_antes_do_mov_joints=%.3f\n", data->j2);
                        char *str = Mov_Joints(juntaa, data->j2, data->j3, data->j4, data->j5, data->j6);
                        printf("iam on button1\n");
                        printf("juntaa=%.3f\n", juntaa);
                        printf("junta2=%.3f\n", data->j2);

                        data->pedido = 1;
                        strcpy(data->pergunta, str);
                        free(str);
                }
                else
                {
                        juntaa = data->incremento;
                        //printf("juntaa_cart=%d\n",juntaa);
                        char *str = Mov_cart(juntaa, 0, 0, 0, 0, 0);
                        data->pedido = 1;
                        strcpy(data->pergunta, str);
                        free(str);
                }

                printf("Mov_button->Send to server: %s\n", data->pergunta);
        }
        else
        {
                gtk_label_set_text(lbl_resposta, "Please, connect to server first!");
        }
}

/**
 * @brief Function to decrese J1/X
 * 
 * @param widget The event description (not used here)
 * @param user_data generic user data (not used here)
 */
void on_button_j1_clicked(GtkWidget *widget, gpointer user_data)
{
        GtkLabel *lbl_resposta = GTK_LABEL(gtk_builder_get_object(builderG, "label_info"));
        if (data->connection)
        {
                //printf("junta1=%d\n",data->j1);
                //printf("incremento=%d\n",data->incremento);
                float juntaa;

                //gtk_label_set_text(lbl_resposta, "____________");
                if (!strncmp(data->frame, "Jo", 2))
                {
                        juntaa = data->j1 - data->incremento;
                        //printf("juntaa=%d\n",juntaa);
                        char *str = Mov_Joints(juntaa, data->j2, data->j3, data->j4, data->j5, data->j6);
                        data->pedido = 1;
                        strcpy(data->pergunta, str);
                        free(str);
                }
                else
                {
                        juntaa = data->incremento;
                        //printf("juntaa_cart=%d\n",juntaa);
                        char *str = Mov_cart(-juntaa, 0, 0, 0, 0, 0);
                        data->pedido = 1;
                        strcpy(data->pergunta, str);
                        free(str);
                }

                printf("Mov_button->Send to server: %s\n", data->pergunta);
        }
        else
        {
                gtk_label_set_text(lbl_resposta, "Please, connect to server first!");
        }
}

/**
 * @brief Function to increse J2/Y
 * 
 * @param widget The event description (not used here)
 * @param user_data generic user data (not used here)
 */
void on_button_jj2_clicked(GtkWidget *widget, gpointer user_data)
{
        GtkLabel *lbl_resposta = GTK_LABEL(gtk_builder_get_object(builderG, "label_info"));
        if (data->connection)
        {
                //printf("junta1=%d\n",data->j1);
                //printf("incremento=%d\n",data->incremento);
                float juntaa;

                //gtk_label_set_text(lbl_resposta, "____________");
                if (!strncmp(data->frame, "Jo", 2))
                {
                        juntaa = data->j2 + data->incremento;
                        //printf("juntaa=%d\n",juntaa);
                        char *str = Mov_Joints(data->j1, juntaa, data->j3, data->j4, data->j5, data->j6);
                        data->pedido = 1;
                        strcpy(data->pergunta, str);
                        free(str);
                }
                else
                {
                        juntaa = data->incremento;
                        //printf("juntaa_cart=%d\n",juntaa);
                        char *str = Mov_cart(0, juntaa, 0, 0, 0, 0);
                        data->pedido = 1;
                        strcpy(data->pergunta, str);
                        free(str);
                }

                printf("Mov_button->Send to server: %s\n", data->pergunta);
        }
        else
        {
                gtk_label_set_text(lbl_resposta, "Please, connect to server first!");
        }
}

/**
 * @brief Function to decrese J2/Y
 * 
 * @param widget The event description (not used here)
 * @param user_data generic user data (not used here)
 */
void on_button_j2_clicked(GtkWidget *widget, gpointer user_data)
{
        GtkLabel *lbl_resposta = GTK_LABEL(gtk_builder_get_object(builderG, "label_info"));
        if (data->connection)
        {
                //printf("junta1=%d\n",data->j1);
                //printf("incremento=%d\n",data->incremento);
                float juntaa;

                //gtk_label_set_text(lbl_resposta, "____________");
                if (!strncmp(data->frame, "Jo", 2))
                {
                        juntaa = data->j2 - data->incremento;
                        //printf("juntaa=%d\n",juntaa);
                        char *str = Mov_Joints(data->j1, juntaa, data->j3, data->j4, data->j5, data->j6);
                        data->pedido = 1;
                        strcpy(data->pergunta, str);
                        free(str);
                }
                else
                {
                        juntaa = data->incremento;
                        //printf("juntaa_cart=%d\n",juntaa);
                        char *str = Mov_cart(0, -juntaa, 0, 0, 0, 0);
                        data->pedido = 1;
                        strcpy(data->pergunta, str);
                        free(str);
                }

                printf("Mov_button->Send to server: %s\n", data->pergunta);
        }
        else
        {
                gtk_label_set_text(lbl_resposta, "Please, connect to server first!");
        }
}

/**
 * @brief Function to increse J3/Z
 * 
 * @param widget The event description (not used here)
 * @param user_data generic user data (not used here)
 */
void on_button_jj3_clicked(GtkWidget *widget, gpointer user_data)
{
        GtkLabel *lbl_resposta = GTK_LABEL(gtk_builder_get_object(builderG, "label_info"));
        if (data->connection)
        {
                //printf("junta1=%d\n",data->j1);
                //printf("incremento=%d\n",data->incremento);
                float juntaa;

                //gtk_label_set_text(lbl_resposta, "____________");
                if (!strncmp(data->frame, "Jo", 2))
                {
                        juntaa = data->j3 + data->incremento;
                        //printf("juntaa=%d\n",juntaa);
                        char *str = Mov_Joints(data->j1, data->j2, juntaa, data->j4, data->j5, data->j6);
                        data->pedido = 1;
                        strcpy(data->pergunta, str);
                        free(str);
                }
                else
                {
                        juntaa = data->incremento;
                        //printf("juntaa_cart=%d\n",juntaa);
                        char *str = Mov_cart(0, 0, juntaa, 0, 0, 0);
                        data->pedido = 1;
                        strcpy(data->pergunta, str);
                        free(str);
                }

                printf("Mov_button->Send to server: %s\n", data->pergunta);
        }
        else
        {
                gtk_label_set_text(lbl_resposta, "Please, connect to server first!");
        }
}

/**
 * @brief Function to decrese J3/Z
 * 
 * @param widget The event description (not used here)
 * @param user_data generic user data (not used here)
 */
void on_button_j3_clicked(GtkWidget *widget, gpointer user_data)
{
        GtkLabel *lbl_resposta = GTK_LABEL(gtk_builder_get_object(builderG, "label_info"));
        if (data->connection)
        {
                //printf("junta1=%d\n",data->j1);
                //printf("incremento=%d\n",data->incremento);
                float juntaa;

                //gtk_label_set_text(lbl_resposta, "____________");
                if (!strncmp(data->frame, "Jo", 2))
                {
                        juntaa = data->j3 - data->incremento;
                        //printf("juntaa=%d\n",juntaa);
                        char *str = Mov_Joints(data->j1, data->j2, juntaa, data->j4, data->j5, data->j6);
                        data->pedido = 1;
                        strcpy(data->pergunta, str);
                        free(str);
                }
                else
                {
                        juntaa = data->incremento;
                        //printf("juntaa_cart=%d\n",juntaa);
                        char *str = Mov_cart(0, 0, -juntaa, 0, 0, 0);
                        data->pedido = 1;
                        strcpy(data->pergunta, str);
                        free(str);
                }

                printf("Mov_button->Send to server: %s\n", data->pergunta);
        }
        else
        {
                gtk_label_set_text(lbl_resposta, "Please, connect to server first!");
        }
}

/**
 * @brief Function to increse J4/W
 * @param widget The event description (not used here)
 * @param user_data generic user data (not used here)
 */
void on_button_jj4_clicked(GtkWidget *widget, gpointer user_data)
{
        GtkLabel *lbl_resposta = GTK_LABEL(gtk_builder_get_object(builderG, "label_info"));
        if (data->connection)
        {
                //printf("junta1=%d\n",data->j1);
                //printf("incremento=%d\n",data->incremento);
                float juntaa;

                //gtk_label_set_text(lbl_resposta, "____________");
                if (!strncmp(data->frame, "Jo", 2))
                {
                        juntaa = data->j4 + data->incremento;
                        //printf("juntaa=%d\n",juntaa);
                        char *str = Mov_Joints(data->j1, data->j2, data->j3, juntaa, data->j5, data->j6);
                        data->pedido = 1;
                        strcpy(data->pergunta, str);
                        free(str);
                }
                else
                {
                        juntaa = data->incremento;
                        //printf("juntaa_cart=%d\n",juntaa);
                        char *str = Mov_cart(0, 0, 0, juntaa, 0, 0);
                        data->pedido = 1;
                        strcpy(data->pergunta, str);
                        free(str);
                }

                printf("Mov_button->Send to server: %s\n", data->pergunta);
        }
        else
        {
                gtk_label_set_text(lbl_resposta, "Please, connect to server first!");
        }
}

/**
 * @brief Function to decrese J4/W
 * 
 * @param widget The event description (not used here)
 * @param user_data generic user data (not used here)
 */
void on_button_j4_clicked(GtkWidget *widget, gpointer user_data)
{
        GtkLabel *lbl_resposta = GTK_LABEL(gtk_builder_get_object(builderG, "label_info"));
        if (data->connection)
        {
                //printf("junta1=%d\n",data->j1);
                //printf("incremento=%d\n",data->incremento);
                float juntaa;

                //gtk_label_set_text(lbl_resposta, "____________");
                if (!strncmp(data->frame, "Jo", 2))
                {
                        juntaa = data->j4 - data->incremento;
                        //printf("juntaa=%d\n",juntaa);
                        char *str = Mov_Joints(data->j1, data->j2, data->j3, juntaa, data->j5, data->j6);
                        data->pedido = 1;
                        strcpy(data->pergunta, str);
                        free(str);
                }
                else
                {
                        juntaa = data->incremento;
                        //printf("juntaa_cart=%d\n",juntaa);
                        char *str = Mov_cart(0, 0, 0, -juntaa, 0, 0);
                        data->pedido = 1;
                        strcpy(data->pergunta, str);
                        free(str);
                }

                printf("Mov_button->Send to server: %s\n", data->pergunta);
        }
        else
        {
                gtk_label_set_text(lbl_resposta, "Please, connect to server first!");
        }
}

/**
 * @brief Function to increse J5/R
 * 
 * @param widget The event description (not used here)
 * @param user_data generic user data (not used here)
 */
void on_button_jj5_clicked(GtkWidget *widget, gpointer user_data)
{
        GtkLabel *lbl_resposta = GTK_LABEL(gtk_builder_get_object(builderG, "label_info"));
        if (data->connection)
        {
                //printf("junta1=%d\n",data->j1);
                //printf("incremento=%d\n",data->incremento);
                float juntaa;

                //gtk_label_set_text(lbl_resposta, "____________");
                if (!strncmp(data->frame, "Jo", 2))
                {
                        juntaa = data->j5 + data->incremento;
                        //printf("juntaa=%d\n",juntaa);
                        char *str = Mov_Joints(data->j1, data->j2, data->j3, data->j4, juntaa, data->j6);
                        data->pedido = 1;
                        strcpy(data->pergunta, str);
                        free(str);
                }
                else
                {
                        juntaa = data->incremento;
                        //printf("juntaa_cart=%d\n",juntaa);
                        char *str = Mov_cart(0, 0, 0, 0, juntaa, 0);
                        data->pedido = 1;
                        strcpy(data->pergunta, str);
                        free(str);
                }

                printf("Mov_button->Send to server: %s\n", data->pergunta);
        }
        else
        {
                gtk_label_set_text(lbl_resposta, "Please, connect to server first!");
        }
}

/**
 * @brief Function to decrese J5/R
 * 
 * @param widget The event description (not used here)
 * @param user_data generic user data (not used here)
 */
void on_button_j5_clicked(GtkWidget *widget, gpointer user_data)
{
        GtkLabel *lbl_resposta = GTK_LABEL(gtk_builder_get_object(builderG, "label_info"));
        if (data->connection)
        {
                //printf("junta1=%d\n",data->j1);
                //printf("incremento=%d\n",data->incremento);
                float juntaa;

                //gtk_label_set_text(lbl_resposta, "____________");
                if (!strncmp(data->frame, "Jo", 2))
                {
                        juntaa = data->j5 - data->incremento;
                        //printf("juntaa=%d\n",juntaa);
                        char *str = Mov_Joints(data->j1, data->j2, data->j3, data->j4, juntaa, data->j6);
                        data->pedido = 1;
                        strcpy(data->pergunta, str);
                        free(str);
                }
                else
                {
                        juntaa = data->incremento;
                        //printf("juntaa_cart=%d\n",juntaa);
                        char *str = Mov_cart(0, 0, 0, 0, -juntaa, 0);
                        data->pedido = 1;
                        strcpy(data->pergunta, str);
                        free(str);
                }

                printf("Mov_button->Send to server: %s\n", data->pergunta);
        }
        else
        {
                gtk_label_set_text(lbl_resposta, "Please, connect to server first!");
        }
}

/**
 * @brief Function to increse J6/P
 * 
  * @param widget The event description (not used here)
 * @param user_data generic user data (not used here)
 */
void on_button_jj6_clicked(GtkWidget *widget, gpointer user_data)
{
        GtkLabel *lbl_resposta = GTK_LABEL(gtk_builder_get_object(builderG, "label_info"));
        if (data->connection)
        {
                //printf("junta1=%d\n",data->j1);
                //printf("incremento=%d\n",data->incremento);
                float juntaa;

                //gtk_label_set_text(lbl_resposta, "____________");
                if (!strncmp(data->frame, "Jo", 2))
                {
                        juntaa = data->j6 + data->incremento;
                        //printf("juntaa=%d\n",juntaa);
                        char *str = Mov_Joints(data->j1, data->j2, data->j3, data->j4, data->j5, juntaa);
                        data->pedido = 1;
                        strcpy(data->pergunta, str);
                        free(str);
                }
                else
                {
                        juntaa = data->incremento;
                        //printf("juntaa_cart=%d\n",juntaa);
                        char *str = Mov_cart(0, 0, 0, 0, 0, juntaa);
                        data->pedido = 1;
                        strcpy(data->pergunta, str);
                        free(str);
                }

                printf("Mov_button->Send to server: %s\n", data->pergunta);
        }
        else
        {
                gtk_label_set_text(lbl_resposta, "Please, connect to server first!");
        }
}

/**
 * @brief Function to decrese J6/P
 * 
 * @param widget The event description (not used here)
 * @param user_data generic user data (not used here)
 */
void on_button_j6_clicked(GtkWidget *widget, gpointer user_data)
{
        GtkLabel *lbl_resposta = GTK_LABEL(gtk_builder_get_object(builderG, "label_info"));
        if (data->connection)
        {
                //printf("junta1=%d\n",data->j1);
                //printf("incremento=%d\n",data->incremento);
                float juntaa;

                //gtk_label_set_text(lbl_resposta, "____________");
                if (!strncmp(data->frame, "Jo", 2))
                {
                        juntaa = data->j6 - data->incremento;
                        //printf("juntaa=%d\n",juntaa);
                        char *str = Mov_Joints(data->j1, data->j2, data->j3, data->j4, data->j5, juntaa);
                        data->pedido = 1;
                        strcpy(data->pergunta, str);
                        free(str);
                }
                else
                {
                        juntaa = data->incremento;
                        //printf("juntaa_cart=%d\n",juntaa);
                        char *str = Mov_cart(0, 0, 0, 0, 0, -juntaa);
                        data->pedido = 1;
                        strcpy(data->pergunta, str);
                        free(str);
                }

                printf("Mov_button->Send to server: %s\n", data->pergunta);
        }
        else
        {
                gtk_label_set_text(lbl_resposta, "Please, connect to server first!");
        }
}
//==================================CAMARA/OPENCV====================================================

/**
 * @brief Function to show camera window
 * 
 * @param widget The event description (not used here)
 * @param user_data generic user data (to window 2)
 */
void on_button_camera_clicked(GtkWidget *widget, gpointer user_data)
{
        GtkWidget *w = GTK_WIDGET(gtk_builder_get_object(builderG, "window2"));
        gtk_widget_show(w);
}

/**
 * @brief Function to hide camera window
 * 
 * @param widget The event description (not used here)
 * @param user_data generic user data (to window 2)
 */
void on_button_quit_camera_clicked(GtkWidget *widget, gpointer user_data)
{
        GtkWidget *w = GTK_WIDGET(gtk_builder_get_object(builderG, "window2"));
        gtk_widget_hide(w);
}

/**
 * @brief  Handler for the timeout
 *
 *   Create a draw event in a special widget as consequence of
 *   the timeout action and ensure periodic refresh of widget.
 *
 * @param  data just to be a proper handler. Not really used. But could be some handler.
 * @return boolean to re-arm the timeout
 */
gboolean pari_UpdateMyDisplay(gpointer data)
{
        GtkWidget *da = GTK_WIDGET(gtk_builder_get_object(builderG, "drawingareasingle"));
        gtk_widget_queue_draw(da);
        return TRUE; //to re-arm the timeout.
}

/**
 * @brief  Callback to execute in case of widget draw event
 *
 * @param  widget - the causing widget
 * @param  event - the associated event
 * @param  user_data - the usual user data
 * @return TRUE to pass events into the hierarchy (if any)
 */
gboolean on_drawingareasingle_draw(GtkWidget *widget, GdkEvent *event, gpointer user_data)
{
        pari_PerformImageAcquisition(captureG); //acquire new image
        pari_RefreshDrawingArea((char *)"drawingareasingle", src_imageG);
        return TRUE;
}
