/* this file is a part of Gdl 0.4f and was writen by rov, http://procvor.free.fr */

// key detection function

char up_front(unsigned short key)
{
	if(!(old_key&key))
		if(key_pressed&key)
			return 1 ;
			
	return 0 ;
}

char up_or_time_key(unsigned short key, ushort time) // not work
{
	static long last_time = 0 ;
	
	if(!(old_key&key))
		if(key_pressed&key)
		{	last_time = GpTickCountGet() ;	return 1 ;	}
			
	if((key_pressed&key)&&((last_time+time) < GpTickCountGet()))
	{	last_time = GpTickCountGet() ;	return 1 ;	}

	return 0 ;
}

char down_front(unsigned short key)
{
	if(old_key&key)
		if(!(key_pressed&key))
			return 1 ;
			
	return 0 ;
}

char continu_key(unsigned short key)
{
	if(old_key&key)
		if(key_pressed&key)
			return 1 ;
			
	return 0 ;
}

char instant_key(unsigned short key)
{
	if(key_pressed&key)	return 1 ;

	return 0 ;
}

void refresh_key(void)
{
	old_key = key_pressed ;
	GpKeyGetEx(&key_pressed);
}

void wait_key(unsigned short key)
{
	while(!(key_pressed&key)) refresh_key() ;
}

void reset_key(void)
{
	key_pressed = old_key = 0 ;
}
