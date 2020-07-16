// Harsh Talati-- 917920398


#include <kernel.h>

PORT train_port;
WINDOW train_wnd;

#define hold_cycle     25
#define hold_zamboni	15
#define cc_hold	3

char* trainSpeed = '0';


void set_speed(char* speed)
{	

	char speedComm[] = "L20Sx\015";
	speedComm[4] = *speed;
	train_instruction(speedComm,"",0);
	
	//update train speed value
	trainSpeed = *speed;
}

void train_clear_mem_buffer()
{
	train_instruction("R\015", "", 0);
}

void change_directions()
{
	if(trainSpeed == '0')
	{
		train_instruction("L20D\015", "", 0);
	}
	
}

void track_switch(char* switchNum, char* position)
{
	//switchNum may be any num char from '1' to '9'
	//position may be 'G' or 'R' only
	if(switchNum < '1' || switchNum > '9')
	{
		wprintf(&train_wnd, "Invalid switch identifier\n");	
		return;
	}
	if(position != 'G' && position != 'R')
	{
		wprintf(&train_wnd, "Invalid switch position\n");	
		return;
	}
	
	
	char switchCommand[] = "M##\015";
	switchCommand[1] = switchNum;
	switchCommand[2] = position;
		
	train_instruction(switchCommand, "", 0);
}

int probe_check(char* trackSection)
{
	//buffer should be clean after every clock tick
	train_clear_mem_buffer();
	
	//Track segment query can always be double digits e.g. 05
	char trackQueryCommand[] = "C00\015";
	
	
	//single digit input, between 1 and 9
	if((trackSection[0] >= '1' && trackSection[0] <= '9') && trackSection[1] == 0)
	{
		//single digit condition succeeded
		trackQueryCommand[2] = trackSection[0];
	}
	else if(trackSection[0] == '1' && (trackSection[1] >= '0' && trackSection[1] <= '6'))
	{
		//double digit condition succeeded
		trackQueryCommand[1] = trackSection[0];
		trackQueryCommand[2] = trackSection[1];
	}
	else
	{
		//no success conditions	
		wprintf(&train_wnd, "Invalid track id entered, must be from 1-16\n");
		return -1;		
	}
	
	return train_instruction(trackQueryCommand, "0\0", 3);
	
}

int train_instruction(char* outBuf, char* inpbuf, int len_inpbuf)
{
	COM_Message msg;
	int value = 0;
	//configure message payload
	msg.input_buffer = inpbuf;	
	msg.output_buffer = outBuf;
	msg.len_input_buffer = len_inpbuf;
	
	
	send(com_port,&msg);
	
	if(len_inpbuf)
	{

	}
	
	
	if(msg.input_buffer[1] == '1')
	{
		value = 1;
	}
	
	
	sleep(hold_cycle);
	return value;
}
//	initialize the track, set switch positions
void init_track(int window_id)
{
	wm_print(window_id, "Init tracks\n");		
//	make sure train is stationary
	set_speed("0");
	
	//Set switches for zamboni to rotate
	track_switch('5', 'G');
	track_switch('8', 'G');
	track_switch('9', 'R');
	track_switch('4', 'G');
	track_switch('1', 'G');
	
}

int zamboni_avail(int window_id)
{
	int value = 0;
	int i = 0;
	int direction;
	
	//Watch for Zamboni. 
	for(i = 0; i < 6; i++)
	{
		//probe for zamboni on track 4
		if(probe_check("4"))
		{
			value = 1;
			break;
		}
	}
	if(value){
	for(i = 0; i < 3; i++)
	{
// Check for direction
		if(probe_check("7"))
		{
			direction = 10;
			break;
		}
		else
		{
			direction = 50;
			break;		
		}
	}

	if(!value && !direction)
	{
		return value;
	}
	return direction;
	}

}

int train_avail(int window_id)
{
	if(probe_check("8"))
	{
		return 1;
	}
	else if(probe_check("5"))
	{
		return 4;
	}
	else if(probe_check("12"))
	{
		return 2;
	}
	else if(probe_check("2"))
	{
		return 3;
	}	
	else 
	{
		return -1;
	}
}	

void track_1(int zamboni_setting, int window_id)
{
	wm_print(window_id, "This is track-1\n");
	track_switch('4','G');
	track_switch('8','G');
	if(zamboni_setting)
	{
		while(!probe_check("7"));
	}	
	set_speed("5");
	track_switch('4','G');
	track_switch('8','G');
	track_switch('2','R');
	while(!probe_check("10"));
	set_speed("4");	
	track_switch('8','R');	
	while(!probe_check("12"));
	set_speed("0");
	track_switch('8','G');
	change_directions();
	track_switch('7','G');
	if(zamboni_setting)
	{
	while(!probe_check("10"));
	}
	set_speed("4");
	track_switch('8','R');
	set_speed("4");
	track_switch('5','R');
	track_switch('6','R');	
	while(!probe_check("7"));	
	set_speed("0");
	change_directions();
	set_speed("4");
	while(!probe_check("8"));
	set_speed("0");
	become_zombie();	
}

void track_2(int zamboni_setting, int window_id)
{
wm_print(window_id, "This is track-2\n");
	track_switch('5','G');
	track_switch('9','R');
	track_switch('1','G');
	track_switch('7','R');
	if(zamboni_setting)
	{
	while(!probe_check("10"));
	}
	change_directions();	
	set_speed("4");
	if(zamboni_setting)
	{
	while(!probe_check("3"));
	}
	track_switch('4','R');
	track_switch('3','G');
	while(!probe_check("2"));
	set_speed("0");
	track_switch('4','G');
	if(zamboni_setting)
	{
	while(!probe_check("3"));
	}
	set_speed("4");
	if(zamboni_setting)
	{
	while(!probe_check("10"));
	}
	track_switch('8','R');
	while(!probe_check("12"));
	set_speed("0");
	track_switch('8','G');
	become_zombie();
}
void track_3(int zamboni_setting, int window_id)	
{
wm_print(window_id, "This is track-3\n");
	track_switch('5','G');
	track_switch('9','R');
	track_switch('1','G');
	if(zamboni_setting)
	{
	while(!probe_check("7"));
	}
	set_speed("5");
	if(zamboni_setting)
	{
	while(!probe_check("10"));
	}
	track_switch('5','R');
	track_switch('6','G');
	set_speed("4");	
	while(!probe_check("12"));
	set_speed("0");	
	track_switch('5','G');
	change_directions();
	track_switch('7','R');
	if(zamboni_setting)
	{
	while(!probe_check("3"));
	}
	set_speed("5");
	track_switch('1','R');
	track_switch('2','G');
	while(!probe_check("2"));	
	set_speed("0");
	track_switch('1','G');
	become_zombie();
}

void track_4(int zamboni_setting, int window_id)	
{
wm_print(window_id, "This is track-4\n");
	track_switch('8','G');
	track_switch('4','G');
	if(zamboni_setting)
	{
	while(!probe_check("4"));
	}
	set_speed("5");
	track_switch('5','R');
	track_switch('6','G');	
	if(zamboni_setting)
	{
	while(!probe_check("4"));
	}
	track_switch('4','R');
	track_switch('3','R');
	while(!probe_check("5"));
	set_speed("0");
	track_switch('4','G');
	become_zombie();
}

void train_check(int train_setting, int zamboni_setting, int window_id)
{
	if(train_setting == 1)
	{
		track_1(zamboni_setting, window_id);
		wm_print(window_id, "This is Config-1\n");		
	}
	if(train_setting == 2)
	{
		track_2(zamboni_setting, window_id);		
		wm_print(window_id, "This is Config-2\n");
	}
	if(train_setting == 3)
	{
		track_3(zamboni_setting, window_id);		
		wm_print(window_id, "This is Config-3\n");
	}
	if(train_setting == 4)
	{
		track_4(zamboni_setting, window_id);		
		wm_print(window_id, "This is Config-4\n");
	}
}

void train_process(PROCESS self, PARAM param)
{
	int window_id = wm_create(1,1,50,17);
	wm_print(window_id, "Start\n");	

	COM_Message msg;
	char buffer [128];
	msg.input_buffer = buffer;

	
	//initialize tracks and switches
	init_track(window_id);
	
	
	//determine availability of Zamboni
	int zamboni_setting = zamboni_avail(window_id);
	//int zamboni_setting = 0;
	
	//determine setting of train
	int train_setting = train_avail(window_id);
	
	train_check(train_setting, zamboni_setting, window_id);
	

}

void init_train(WINDOW* wnd)
{
	train_port = create_process (train_process, 5, 0, "Train process");
}
