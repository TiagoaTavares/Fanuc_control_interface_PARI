/**
 *      @file  auxfuncs.c
 *      @brief  auxiliary functions
 *
 * =====================================================================================
 */

#include "myserver.h"

void myerror(const char *msg)
{
	perror(msg);
	exit(1);
}

void ManageCTRL_C(int dummy)
{
	printf("User CTRL-C pressed. Closing server.\n");
	infinite_loopG = 0;
}

char *vsTimeString()
{
	char *auxstr = (char *)malloc(100);
	bzero(auxstr, 100);
	time_t t = time(NULL);
	struct tm *lt = localtime(&t);
	sprintf(auxstr, "%02d:%02d:%02d", lt->tm_hour, lt->tm_min, lt->tm_sec);
	return auxstr;
}

/**
 * @brief Return a string with real joint positions
 * 
 * @return char* 
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
	sprintf(junta1, "%.3f", jj1);
	//printf("%s",junta1);
	sprintf(junta2, "%.3f", j2);
	sprintf(junta3, "%.3f", j3);
	sprintf(junta4, "%.3f", j4);
	sprintf(junta5, "%.3f", j5);
	sprintf(junta6, "%.3f", j6);

	sprintf(str_joint_char, "%s %s %s %s %s %s", junta1, junta2, junta3, junta4, junta5, junta6);

	//printf("%s",str_cart_char)
	return str_joint_char;
}

/**
 * @brief Return a string with real cartesian positions (X Y Z)
 * 
 * @return char* 
 */
char *Get_pos_cart_char1()
{
	char *str_cart_char1 = (char *)malloc(100);
	char junta1[10];
	char junta2[10];
	char junta3[10];

	sprintf(junta1, "%.3f", c1);
	//printf("%s",junta1);
	sprintf(junta2, "%.3f", c2);
	sprintf(junta3, "%.3f", c3);


	sprintf(str_cart_char1, "%s %s %s", junta1, junta2, junta3);

	//printf("%s",str_cart_char)
	return str_cart_char1;
}

/**
 * @brief Return a string with real cartesian positions (W R P)
 * 
 * @return char* 
 */
char *Get_pos_cart_char2()
{
	char *str_cart_char2 = (char *)malloc(100);

	char junta4[10];
	char junta5[10];
	char junta6[10];

	sprintf(junta4, "%.3f", c4);
	sprintf(junta5, "%.3f", c5);
	sprintf(junta6, "%.3f", c6);

	sprintf(str_cart_char2, "%s %s %s", junta4, junta5, junta6);

	//printf("%s",str_cart_char)
	return str_cart_char2;
}


/**
 * @brief Get the joint positions object - Update shared memmory- joits positions
 * 
 * @param string_line 
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

		printf("jj1=%f\n",jj1);
		printf("j2=%f\n",j2);
		printf("j3=%f\n",j3);
		printf("j4=%f\n",j4);
		printf("j5=%f\n",j5);

        //puts("HERE--------");
        jj1 = atof(mapa[0]);
        //printf("jj1=%d\n",data->jj1);
        j2 = atof(mapa[1]);
        //printf("jj1=%d\n",data->j2);
        j3 = atof(mapa[2]);
        j4 = atof(mapa[3]);
        j5 = atof(mapa[4]);
        j6 = atof(mapa[5]);

		printf("jj1=%f\n",jj1);
		printf("j2=%f\n",j2);
		printf("j3=%f\n",j3);
		printf("j4=%f\n",j4);
		printf("j5=%f\n",j5);


        return;
}

/**
 * @brief Get the joint positions object - Update shared memmory- cartesian positions
 * 
 * @param string_line 
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

        //puts("HERE--------");
		printf("c1=%f\n",c1);
        c1 += atof(mapa[0]);
        printf("c1=%f\n",c1);
		printf("c2=%f\n",c2);
        c2 += atof(mapa[1]);
		printf("c2=%f\n",c2);
        //printf("jj1=%d\n",data->j2);
        c3 += atof(mapa[2]);
        c4 += atof(mapa[3]);
        //printf("C4\n");
        c5 += atof(mapa[4]);
        c6 += atof(mapa[5]);
        //printf("C6\n");
        return;
}
