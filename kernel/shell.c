//Harsh_Talati_917920398

#include <kernel.h>



typedef struct commands							
{
	char buffer[50];
	int pointer;
} commands;
commands *index;

void read_command(int window_id, commands *index)				
{											
 	char input;									
		
	index->pointer = 0;			
  	do
	{
		input = keyb_get_keystroke(window_id,TRUE); 
		switch (input) {
		case '\b': //command for backspace
			if (index->pointer == 0)
			continue;
			index->pointer--;
			wm_print(window_id, "%c",input);
			break;
		
		case 13: //command return
			index->buffer[index->pointer]='\0';			
			wm_print(window_id, "%c");
						
			clean_buffer();
			break;
		default://Other commands
			index->buffer[index->pointer++] = input;
			wm_print(window_id,"%c",input);		
			break;
		}
	} while (input != '\r' && input != '\n');
	index->buffer[index->pointer] = '\0';
    
}


//String entered compare
BOOL tos_string(char* cmd1, char* cmd2){
while (*cmd1 == *cmd2 && *cmd2 !='\0'){
cmd1++;
cmd2++;
}
return *cmd2=='\0';
}

void executeCommand(int window_id, char* cmd)
{
	if(tos_string(cmd,"help")){
		toshelp();
		return;
	} if(tos_string(cmd,"echo ")){
		ech();
		return;
	} if(tos_string(cmd,"about")){
		tosabout();
		return;
	} if(tos_string(cmd, "cls")){
		clearshellwindow();
		return;
	} if(tos_string(cmd, "pong")){
		pongg();
		return;		
	} if(tos_string(cmd, "ps")){
		print_all_processtos();
		return;
	} if(tos_string(cmd, "sh")){
		start_shell();
		return;
	}if(tos_string(cmd, "hist")){
		//addHistory();
		return;  
	
	}wm_print(window_id, "Type help or Type something sensible\n");
	
	
}

//Function to clear the shell window
void clearshellwindow(int window_id){
	
	wm_clear(window_id);
	wm_print(window_id,"This is Harsh's TOS\n");
}

//Function to clean the buffer used
void clean_buffer(int window_id){

	clear_screen_buffer();
	
	}

//Main shell process
void main_shell_process(){
	int window_id = wm_create(12, 6, 50, 17);
	wm_print(window_id,"This is Harsh's TOS\n");
	commands *index;
	commands* point = &index;
	while(1){
	clean_buffer(window_id);
	read_command(window_id, &index);
 	char *comm;
	comm = (&point->buffer[0]);
	wm_print(window_id,"\n",comm);
	executeCommand(window_id, comm);
	resign();
	}	
}
//Echo a string
void ech(int window_id){
	commands *index;
	commands* point = index;
	char *abc = point->buffer[10];
	wm_print(window_id, &abc);
	wm_print(window_id, "\n"); }
//Launch pong Game 
void pongg(int window_id)
{
	start_pong();
}
//Fun message for the world
void tosabout(int window_id, char *cmd)
{
	wm_print(window_id, "Harsh Talati-917920398\n");
}
//Information for help
void toshelp(int window_id)
{
	wm_print( window_id,"This is TOS-HELP\n");
 	wm_print( window_id,"help                  - information in help\n");
 	wm_print( window_id,"about                 - Fun message for world\n");
	wm_print( window_id,"pong                  - Launch pong game\n");
	wm_print( window_id,"echo                  - Prints the string\n"); 	
	wm_print( window_id,"ps                    - list all processes\n");
 	wm_print( window_id,"cls                   - clear the window\n");
}	
//Shell initialization
void start_shell()
{
 	create_process(main_shell_process,6,0,"Shell");
  	resign();
}

//Functions from process.c file of kernel renamed for our shell

void print_process_headingtos(WINDOW * wnd)
{
    wm_print(wnd, "State           Active Prio Name\n");
    wm_print(wnd, "------------------------------------------------\n");
}

void print_process_detailstos(WINDOW * wnd, PROCESS p)
{
    static const char *state[] = { "READY          ",
        "ZOMBIE         ",
        "SEND_BLOCKED   ",
        "REPLY_BLOCKED  ",
        "RECEIVE_BLOCKED",
        "MESSAGE_BLOCKED",
        "INTR_BLOCKED   "
    };
    if (!p->used) {
        wm_print(wnd, "PCB slot unused!\n");
        return;
    }
    /* State */
    wm_print(wnd, state[p->state]);
    /* Check for active_proc */
    if (p == active_proc)
        wm_print(wnd, " *      ");
    else
        wm_print(wnd, "        ");
    /* Priority */
    wm_print(wnd, "  %2d", p->priority);
    /* Name */
    wm_print(wnd, " %s\n", p->name);
}

void print_processtos(WINDOW * wnd, PROCESS p)
{
    print_process_headingtos(wnd);
    print_process_detailstos(wnd, p);
}

void print_all_processtos(WINDOW * wnd)
{
    int             i;
    PCB            *p = pcb;

    print_process_headingtos(wnd);
    for (i = 0; i < MAX_PROCS; i++, p++) {
        if (!p->used)
            continue;
        print_process_detailstos(wnd, p);
    }
}






