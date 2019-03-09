/*File generated automatically in asus16 by tiago on seg jan 28 14:41:59 WET 2019*/
#ifdef __cplusplus
extern "C" {
#endif
/* server.c */
int main (int argc, char *argv[]);
void processclient (int sock, char *clientID);
/* auxfuncs.c */
char *Get_pos_cart_char1 ();
char *Get_pos_cart_char2 ();
char *Get_pos_joint_char ();
void ManageCTRL_C (int dummy);
void get_cartesian_positions (char *string_line);
void get_joint_positions (char *string_line);
void myerror (const char *msg);
char *vsTimeString ();
#ifdef __cplusplus
}
#endif
