/**
 * @file myf.c
 * @author Tiago Tavares (tiagoatavares@ua.pt)
 * @brief file that has the 2 functions ( Father and Son functions), as well as other functions to complement it. 
 * @version 0.1
 * @date 2019-01-27
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#define _MYF_

#include <signal.h>
#include <sys/time.h>
#include <errno.h>

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#include "myf.h"

/**
 * @brief Função que atribui uma aparencia diferente ao processo "filho" para prints no terminal.
 * 
 * @param str string para print.
 */
void MessageFromChild(char *str)
{
        textcolor(RESET, RED, WHITE); //from pcolor
        printf("%s", str);
        ResetTextColors(); //from pcolor
}

/**
 * @brief Função que atribui uma aparencia diferente ao processo "Pai" para prints no terminal.
 * 
 * @param str string para print.
 */
void MessageFromParent(char *str)
{
        textcolor(RESET, GREEN, WHITE); //from pcolor
        printf("%s", str);
        ResetTextColors(); //from pcolor
}

/**
 * @brief Função que representa o processo filho. Atribui a shared memory e trata da interface gráfica.
 * 
 * @param argc nº de argumentos quando executa o programa.
 * @param argv contem quais os argumentos colocados na execução do programa.
 * @return int valor de retorno da função.
 */
int ChildMain3(int argc, char *argv[])
{
        int n, shm_id;
        //char *data;    //generic pointer to serve as link for the shared memory
        char str[100]; //string to put a message

        shm_id = GetSharedMem();
        if (shm_id == -1)
                return -1; //failure

        /* attach to the memory segment to get a pointer to it */
        data = (struct datastruct *)shmat(shm_id, (void *)0, 0);
        if (data == (struct datastruct *)(-1)) //se n conseguir associar o id...
        {
                perror("shmat");
                exit(1);
        } /*data now points to a shared area */

        data->gotostart = 1;
        data->gripper = 0;
        strcpy(data->frame, "Joint");
        data->velocidade = 10;
        data->incremento = 1;
        data->mode = 1; //assincrono-1 , sincrono-0
        data->ty = 1;
        data->resposta_done = 0;
        // -----GTK-----

        //gtk_init(NULL, NULL);
        gtk_init(&argc, &argv);

        g_timeout_add(100, (GSourceFunc)pari_UpdateMyDisplay, (gpointer)NULL);
        captureG = pari_StartImageAcquisition(argc);

        /* load the interface after a configuration file*/
        builderG = gtk_builder_new();
        gtk_builder_add_from_file(builderG, "interface.glade", NULL);

        /* connect the signals in the interface */
        gtk_builder_connect_signals(builderG, NULL);

        /* get main window Widget ID and connect special signals */
        GtkWidget *t = GTK_WIDGET(gtk_builder_get_object(builderG, "window1"));

        if (t)
        {
                g_signal_connect(G_OBJECT(t), "delete_event", G_CALLBACK(pari_delete_event), NULL);
        }

        // use signal to catch SIGINT  (CTRL-C)
        signal(SIGINT, InterceptCTRL_C);

        /* Hand control over to the main loop. */
        gtk_main(); //ciclo infinito que fica á espera de eventos

        /* detach from the mem segment */
        data->gotostart = 0;

        cvReleaseCapture(&captureG); //Release capture device.
        //cvReleaseImage(&dst_imageG);             //Release image (free pointer when no longer used)
        if (src_imageG)
                cvReleaseImage(&src_imageG); //Release image (free pointer when no longer used).

        //printf("gotostart=%d\n", data->gotostart);
        //puts("filho chegou ao fim");
        if (shmdt((void *)data) == -1)
        {
                perror("shmdt");
                exit(1);
        }

        return 0;
}

/**
 * @brief Função que representa o processo Pai. Atribui a shared memory e trata da comunicação TCP/ip com o servidor.
 * 
 * @param argc nº de argumentos quando executa o programa.
 * @param argv contem quais os argumentos colocados na execução do programa.
 * @return int valor de retorno da função.
 */
int ParentMain3(int argc, char *argv[])
{
        int socket_desc;
        int ret;
        struct hostent *serverByname;
        struct sockaddr_in server;
        char message[50];
        char server_reply[MAXCHARS + 1] = {0}; //an emptied buffer for server reply
        char server_reply_cat[MAXCHARS + 1] = {0};
        char pos_reply[MAXCHARS + 1] = {0};
        char *find_string;

        char ip[16] = {0};
        int port;

        if (argc < 2)
        {
                strcpy(ip, "192.168.0.231");

                port = 4900;
                puts("SERVER FOR FANNUC\n");
        }
        else
        {
                strcpy(ip, "127.0.0.1");
                //ip = "127.0.0.1";

                port = atoi(argv[1]);

                if (port)
                {
                        ;
                }
                else
                {
                        port = 50000;
                }

                printf("Local server, on port=%d\n", port);
        }

        int var_check_joints = 1;
        int n, shm_id;
        char str[200];     //string to put a message
        struct timeval tt; //aux variable to read system time
        int aux_get_cart = 0;

        int continue_while = 1;

        shm_id = GetSharedMem();
        if (shm_id == -1)
                return -1; //failure

        /* attach to the memory segment to get a pointer to it */
        data = (struct datastruct *)shmat(shm_id, (void *)0, 0);
        if (data == (struct datastruct *)(-1))
        {
                perror("shmat");
                exit(1);
        } //'data' now points to a shared area */

        data->pedido_connection = 0;
        usleep(500000);

        do
        {
                do
                {
                        //Estrutura a 0
                        data->pedido = 0;

                        strcpy(data->frame, "World");

                        //fill server details
                        server.sin_addr.s_addr = inet_addr(ip);

                        //Create a socket for communications
                        socket_desc = socket(AF_INET, SOCK_STREAM, 0);
                        if (socket_desc == -1)
                        {
                                printf("Could not create socket\n");
                        }

                        //fcntl(socket_desc, F_SETFL, O_NONBLOCK);
                        server.sin_family = AF_INET;
                        server.sin_port = htons(port);

                        while (!data->pedido_connection) // se nao houver pedido de conecção nao continua
                        {
                                if (!data->gotostart)
                                {
                                        break;
                                }
                        }

                        if (!data->gotostart)
                        {
                                break;
                        }

                        //Connect to remote server using the created socket
                        ret = connect(socket_desc, (struct sockaddr *)&server, sizeof(server));
                        if (ret < 0)
                        {
                                puts("connect error");
                                puts("Verifique o estado do server, e reeinicie o programa!");
                                data->pedido_connection = 0;
                                break;
                        }

                        //strcpy(data->pergunta,"Who");
                        puts("Connected to server");
                        data->connection = 1;
                        //usleep(500000);
                        while (data->connection)
                        {
                                if (!data->gotostart)
                                {
                                        break;
                                }

                                if (data->pedido != 0)
                                {
                                        //printf("pergunta=<%s>\n", data->pergunta);
                                        strcpy(message, data->pergunta);

                                        ret = send(socket_desc, message, strlen(message), 0);
                                        printf("messg enviada=<%s>\n", message);
                                        if (ret < 0)
                                        {
                                                puts("Send failed");
                                                return 1;
                                        }

                                        sprintf(str, "MSG enviada do pai para o servidor(clicked): %s\n", message);
                                        MessageFromParent(str);
                                        //printf("message=<%s>\n", message);
                                        puts("enviado!");
                                        //-----receive the answer-----------------------------------------------
                                        printf("-----Reply received\n");
                                        do
                                        {

                                                bzero(server_reply, MAXCHARS); //important when reading repeatidely

                                                ret = recv(socket_desc, server_reply, MAXCHARS, 0);
                                                if (ret < 0)
                                                {
                                                        printf("recv failed\n");
                                                }
                                                printf("para o pai=%s\n", server_reply);

                                                /** se for para mover espera até o mov estar completo **/
                                                if ((!strncmp(server_reply, "Mov", 3)) && (!strncmp(message, "MOV", 3)))
                                                {
                                                        strcpy(server_reply, "1\n");
                                                }

                                                /** se for para listar os programas**/
                                                if ((!strncmp(message, "LIST", 4)) && (!strncmp(server_reply, "Program", 7)))
                                                {
                                                        usleep(500000);
                                                        bzero(server_reply, MAXCHARS); //important when reading repeatidely
                                                        ret = recv(socket_desc, server_reply, MAXCHARS, 0);
                                                        if (ret < 0)
                                                        {
                                                                printf("recv failed\n");
                                                        }
                                                        printf("para o pai=%s\n", server_reply);
                                                }

                                        } while (!strncmp(server_reply, "Mov", 3));

                                        strcpy(data->resposta, server_reply);

                                        //printf("server_reply=%s\n", server_reply);
                                        //printf("para a sharedMemori=%s\n", data->resposta);

                                        data->resposta_done = 1;
                                        data->pedido = 0;
                                        usleep(50000);
                                }
                                else // se nao houver pedido, continua a checkar a posição atual
                                {

                                        if (var_check_joints == 1) //check juntas
                                        {
                                                strcpy(message, "GETCRJPOS\n");
                                        }
                                        else //check cartesianas
                                        {
                                                strcpy(message, "GETCRCPOS\n");
                                        }

                                        ret = send(socket_desc, message, strlen(message), 0);
                                        if (ret < 0)
                                        {
                                                puts("Send failed");
                                                return 1;
                                        }

                                        sprintf(str, "MSG enviada do pai para o servidor(mytime): <%s>\n", message);
                                        MessageFromParent(str);

                                        printf("-----Reply received\n");
                                        //printf("server_reply_clean?=<%s>\n", server_reply);

                                        usleep(50000);

                                        do
                                        {

                                                bzero(server_reply, MAXCHARS); //important when reading repeatidely
                                                ret = recv(socket_desc, server_reply, MAXCHARS, 0);

                                                if (ret < 0)
                                                {
                                                        printf("recv failed\n");
                                                }
                                                printf("para o pai=<%s>\n", server_reply);

                                                find_string = strstr(server_reply, "Current");

                                                if (find_string)
                                                {
                                                        //printf("find_string=%s\n", find_string);
                                                        strcpy(server_reply, find_string);
                                                        //printf("server_rply_from_copy=%s\n", server_reply);
                                                }

                                        } while (strncmp(server_reply, "Curren", 5));

                                        printf("==================\n");

                                        if (var_check_joints == 1) //check juntas
                                        {

                                                strncpy(pos_reply, server_reply + 20, sizeof(server_reply) - 20);
                                                printf("coordenadas_tratadas=<%s>\n", pos_reply);
                                                get_joint_positions(pos_reply);
                                        }
                                        else //check cartesianas
                                        {

                                                strncpy(pos_reply, server_reply + 17, 60);
                                                printf("coordenadas_tratadas=<%s>\n", pos_reply);
                                                get_cartesian_positions(pos_reply);
                                                //printf("out_Getcartesian_pos\n");
                                        }
                                        //limpeza
                                        //printf("limpeza\n");
                                        bzero(pos_reply, MAXCHARS); //important when reading repeatidely
                                        aux_get_cart = 0;
                                        bzero(server_reply_cat, MAXCHARS); //important when reading repeatidely
                                        bzero(server_reply, MAXCHARS);     //important when reading repeatidely
                                        //printf("server_reply_fim_de_ciclo_?=<%s>\n", server_reply);

                                        var_check_joints = (!var_check_joints);
                                        //strcpy(server_reply_cat, ".");
                                }
                                //printf("Junta1=%.3f\n", data->j1);

                                PrintRedLine();
                        }

                        //close the socket before exiting
                        close(socket_desc);
                        //puts("HEREEE whilhe parente loop!");
                } while (data->gotostart);
        } while (data->gotostart);

        /* detach from the mem segment since it is leaving */
        if (shmdt((void *)data) == -1)
        {
                perror("shmdt");
                exit(1);
        }

        return shm_id;
        //-----------------------------------------------------------------
}

/**
 * @brief  Gets a shared memory and connects to it
 *
 * The function first creates a random key to identify
 * the memory and then connects to it with some specific permissions
 * @return int The Shared memory ID. Returns -1 in case of failure.
 */
int GetSharedMem()
{
        key_t key; /*it's a long int */
        int shm_id;
        key = ftok("/", 'X'); /* generate a random key based on a file name and char */
        if (key == -1)
        {
                perror("ftok");
                return -1;
        }

        /* now connect to (or possibly create) the shared memory segment with permission 644 */
        shm_id = shmget(key, sizeof(struct datastruct), 0644 | IPC_CREAT);
        if (shm_id == -1)
        {
                perror("shmget");
                return -1;
        }

        return shm_id;
}

/**
 * @brief Obtem as coordenadas de juntas, ou seja, atualiza a shared memmory.
 * 
 * @param string_line string vinda do servidor com as posicoes das juntas.
 */
void get_joint_positions(char *string_line)
{
        char mapa[6][10];
        //char *str_tok; //=strtok (str, ","); //separa as strings pelas vírgulas
        int l = 0;
        int c = 0;
        int i = 0;
        int aux = 1;
        //str_tok = strtok(string_line, " "); //separador por um espaço

        while (string_line[i] != '\0') // string so com ' ' , 'numeros', e '.'
        {
                if (string_line[i] == '\n')
                {
                        string_line[i] = ' ';
                }
                i++;
        }

        i = 0;
        //puts("HERE--------1");
        char *ptr = string_line;
        //printf("ptr=%s\n", ptr);
        while ((ptr[i] != '\0') && (l <= 5))
        { // While there are more characters to process...

                if (ptr[i] == ' ')
                {
                        if (aux == 0)
                        {
                                mapa[l][c] = '\0';
                                l++;   //for next word
                                c = 0; //for next word, init index to 0
                                aux = 1;
                        }
                }
                else
                {
                        aux = 0;
                        mapa[l][c] = ptr[i];
                        c++;
                }
                i++;

                //printf("l=%d\n",l);
                //printf("i=%d\n",i);
                //printf("%s\n",mapa[1]);
                //printf("%s\n",mapa[2]);
                //printf("%s\n",mapa[3]);
        }

        //puts("HERE--------");
        data->j1 = atof(mapa[0]);
        //printf("j1=%d\n",data->j1);
        data->j2 = atof(mapa[1]);
        //printf("j1=%d\n",data->j2);
        data->j3 = atof(mapa[2]);
        data->j4 = atof(mapa[3]);
        data->j5 = atof(mapa[4]);
        data->j6 = atof(mapa[5]);

        //printf("end of get_joints\n");

        return;
}

/**
 * @brief Obtem as coordenadas cartesianas, ou seja, atualiza a shared memmory.
 * 
 * @param string_line string vinda do servidor com as posicoes cartesianas do robot.
 */
void get_cartesian_positions(char *string_line)
{
        char mapa[6][10];
        //char *str_tok; //=strtok (str, ","); //separa as strings pelas vírgulas
        int l = 0;
        int c = 0;
        int i = 0;
        int aux = 1;
        //str_tok = strtok(string_line, " "); //separador por um espaço

        while (string_line[i] != '\0') // string so com ' ' , 'numeros', e '.'
        {
                if (string_line[i] == '\n')
                {
                        string_line[i] = ' ';
                }
                i++;
        }

        i = 0;
        //puts("HERE--------1");
        char *ptr = string_line;
        //printf("ptr=%s\n", ptr);
        while ((ptr[i] != '\0') && (l <= 5))
        { // While there are more characters to process...

                if (ptr[i] == ' ')
                {
                        if (aux == 0)
                        {
                                mapa[l][c] = '\0';
                                l++;   //for next word
                                c = 0; //for next word, init index to 0
                                aux = 1;
                        }
                }
                else
                {
                        aux = 0;
                        mapa[l][c] = ptr[i];
                        c++;
                }
                i++;
                //printf("i=%d\n",i);
                //printf("l=%d\n",l);

                //printf("%s\n",mapa[1]);
                //printf("%s\n",mapa[2]);
                //printf("%s\n",mapa[3]);
        }

        //passagem para a shared memory
        data->c1 = atof(mapa[0]);
        data->c2 = atof(mapa[1]);
        data->c3 = atof(mapa[2]);
        data->c4 = atof(mapa[3]);
        data->c5 = atof(mapa[4]);
        data->c6 = atof(mapa[5]);

        //printf("c1=%f\n", data->c1);
        //printf("END of get_Car_pos\n");
        return;
}

/**
 * @brief Get the tp programs object Altera a string vinda do servidor, com os programas existentes no TP, de modo a ser mais facil de processar posteriormente
 * 
 * @param string_line 
 * @return char* 
 */
char *get_tp_programs(char *string_line)
{
        char *program_list = (char *)malloc(900);
        strcpy(program_list, string_line);
        printf("program_list=%s\n", program_list);
        int l = 0;
        int c = 0;
        int i = 0;
        int aux = 1;

        //str_tok = strtok(string_line, " "); //separador por um espaço

        while (program_list[i] != '\0') // string so com ' ' , '1' e program names
        {
                if (program_list[i] == '\n')
                {
                        program_list[i] = ' ';
                }
                i++;
        }

        return program_list;
}

//----------------------------------OPENCV functions--------------------------
/**
 * @brief Function that returns the capture device and create an imagem where the reading of the camera will be placed
 * 
 * @param argument number of arguments! if argument=1 startaquisition with extern camara !  if argument=2 startaquisition with pc camara !
 * @return CvCapture* Capture device
 */
CvCapture *pari_StartImageAcquisition(int argument)
{
        CvCapture *capture;
        printf("argumento=%d",argument);

        if (argument > 1)
        {
                capture = cvCaptureFromCAM(0); //Capture using 1st camera: i.e., no. 0
        }
        else
        {
                capture = cvCaptureFromCAM(1);
        }

        IplImage *frame = cvQueryFrame(capture);                       //Update frame. Pointer does not change.
        src_imageG = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 3); //create image for color (8 bits, 3 channels)
        //dst_imageG = (IplImage *)cvClone(src_imageG);                  //create clone of source image
        return capture;
}
/**
 * @brief Function to make image acquisition
 * 
 * @param capt Capture device
 */

void pari_PerformImageAcquisition(CvCapture *capt)
{
        static IplImage *frame;
        frame = cvQueryFrame(capt);           //Update frame pointer.
        cvCvtColor(frame, frame, CV_BGR2RGB); // Usually opencv image is BGR, so we need to change it to RGB
        cvCopy(frame, src_imageG, NULL);      //copy frame as it is (preserve color)
                                              //cvCopy(src_imageG, dst_imageG, NULL); //copy src into dst and ready to process (admitt similar image structure)
}


/**
 * @brief  Function that converts and IplImage into a pixbuf for display purposes
 *
 *      // newer versions could use gdk_pixbuf_new_from_bytes()
 *
 * @param  image The IplImage
 * @param  dst_w width of the new pixbuf
 * @param  dst_h height of the new pixbuf
 * @return The GdkPixbuf *
 */
GdkPixbuf *pari_ConvertOpenCv2Gtk(IplImage *image, int dst_w, int dst_h)
{
        IplImage *gtkMask = image;
        GdkPixbuf *pix, *rpix;
        pix = gdk_pixbuf_new_from_data((guchar *)gtkMask->imageData,
                                       GDK_COLORSPACE_RGB,
                                       FALSE,
                                       gtkMask->depth,
                                       gtkMask->width,
                                       gtkMask->height,
                                       gtkMask->widthStep,
                                       NULL, //callback to free allocated memory: void (*GdkPixbufDestroyNotify) (guchar *pixels, gpointer fn_data);
                                       NULL  //argument to previous callback: gpointer fn_data
        );

        rpix = gdk_pixbuf_scale_simple(pix, dst_w, dst_h, GDK_INTERP_BILINEAR);
        g_object_unref(pix); //free the intermediate pixbuf...
        return rpix;
}

/**
 * @brief Copy from Iplimage to pixbuf and paint the widget
 *
 * @param  widgetName String with the widget name in glade
 * @param  img IplImage where to get the image from
 * @return void
 */
void pari_RefreshDrawingArea(char *widgetName, IplImage *img)
{
        GtkWidget *da = GTK_WIDGET(gtk_builder_get_object(builderG, widgetName));
        if (!da)
        {
                printf("failed for %s\n", widgetName);
                return;
        }

        int wd = gtk_widget_get_allocated_width(da);  //also img->width for full size always
        int ht = gtk_widget_get_allocated_height(da); //also img->height for full size always
        if (!img)
        {
                return;
        }
        GdkPixbuf *pix = pari_ConvertOpenCv2Gtk(img, wd, ht);
        cairo_t *cr = gdk_cairo_create(gtk_widget_get_window(da));
        gdk_cairo_set_source_pixbuf(cr, pix, 0, 0);
        cairo_paint(cr);
        cairo_fill(cr);
        cairo_destroy(cr);
        g_object_unref(pix); //free the pixbuf ... not sure if it works as expected
}
