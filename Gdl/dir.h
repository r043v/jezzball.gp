/* this file is a part of Gdl 0.4f and was writen by rov, http://procvor.free.fr */

void read_dir(char * dir, char * extention, char * file_url)
{
 	GPDIRENTRY file_list[256] ;
	ulong file_number ;
 	char * buffer ;
 	static short file_selected = 2 ;
 	static short file_map, cpt_draw=0 ;		
	short old_file_selected = file_selected ;
	static short start=2 ;
	short limit_draw=15 ;
	static char num_draw = 0 ;
	static char id_select = 0 ;
	long old_tick = 0 ;
	char ext[4] ;
	char buf[50] ;

	static char read_dir=0 ;

	buffer = file_url ;

	//if(file_selected > 16) start = file_selected-8 ;

	if(!read_dir)
	{
		nflip^=1 ;
		draw_text_center(gdl_font,"scan directory ...") ;
		nflip^=1 ;
		GpDirEnumList(dir,0,256,(GPDIRENTRY*)&file_list,&file_number) ;
		read_dir=1 ;
	}

	GPSPRINTF(buffer,"%s\\%s",dir,file_list[file_selected].name) ;

 	do {
		refresh_key() ;

		clr_scr() ;

		draw_int(gdl_font,get_free_ram(),5,5) ;

		cpt_draw = 0 ;
		num_draw = 0 ;

		GPSPRINTF(buf,"%i/%i",file_selected-1,file_number-2) ;
		draw_text(gdl_font,buf,5,15) ;
		
		for(file_map=start;(file_map<file_number)&&(num_draw<limit_draw);file_map++)
		{
			if(file_selected==file_map)
			{				
				gfx_draw_method = 0 ;
				id_select = num_draw ;
			}
			else	gfx_draw_method = 1 ;
			
			get_extention(file_list[file_map].name,ext) ;
			
			if(!GPSTRCMP(extention,(char*)ext))
			{
				if(file_selected==file_map)	GPSPRINTF(buf,"* %s *",file_list[file_selected].name) ;
				else				GPSPRINTF(buf,"%s",file_list[file_map].name) ;

				draw_text_center_x(gdl_font,buf,30+cpt_draw*10) ;
				cpt_draw++ ;
				num_draw++ ;
			}
		} ;

		gfx_draw_method = 1 ;

		if(instant_key(512)) GpAppExit() ;

		if((up_front(159))||(instant_key(144)&&(old_tick+20<GpTickCountGet()))||(instant_key(10)&&(old_tick+90<GpTickCountGet()))||(instant_key(5)&&(old_tick+250<GpTickCountGet())))
		{
			char ok = 0 ;
			old_tick = GpTickCountGet() ;
			old_file_selected = file_selected ;

			if(instant_key(4)) // right
			{
				char cpt ;
				
				for(cpt=0;cpt<10;cpt++)
				{
					if(file_selected != file_number-1)
					{
						file_selected++ ;
						while((!ok)&&(file_selected != file_number))
						{	
							get_extention(file_list[file_selected].name,ext) ;
							
							if(GPSTRCMP(extention,(char*)ext)) file_selected++ ;
							else ok=1 ;
						}
						
						if(!ok) file_selected = old_file_selected ;
						else
						{
							if(id_select < limit_draw)
								if(file_selected - start - 2 == limit_draw-6) start++ ;
						}
					}
				} ;
			}

			if(instant_key(1)) // left
			{
				char cpt ;
				
				for(cpt=0;cpt<10;cpt++)
				{
					if(file_selected != 2)
					{
						file_selected-- ;
						while((!ok)&&(file_selected != 1))
						{	
							get_extention(file_list[file_selected].name,ext) ;
							
							if(GPSTRCMP(extention,(char*)ext)) file_selected-- ;
							else ok=1 ;
						}
						
						if(!ok) file_selected = old_file_selected ;
						else
						{
								if(start)
									if(file_selected - start - 2 == 1) start-- ;
						}
					}
				} ;
			}


			if(instant_key(16)) // L
			{
				char cpt ;
				
				for(cpt=0;cpt<2;cpt++)
				{
					if(file_selected != 2)
					{
						file_selected-- ;
						while((!ok)&&(file_selected != 1))
						{	
							get_extention(file_list[file_selected].name,ext) ;
							
							if(GPSTRCMP(extention,(char*)ext)) file_selected-- ;
							else ok=1 ;
						}
						
						if(!ok) file_selected = old_file_selected ;
						else
						{
								if(start)
									if(file_selected - start - 2 == 1) start-- ;
						}
					}
				} ;
			}

			if(instant_key(128)) // R
			{
				char cpt ;
				
				for(cpt=0;cpt<2;cpt++)
				{
					if(file_selected != file_number-1)
					{
						file_selected++ ;
						while((!ok)&&(file_selected != file_number))
						{	
							get_extention(file_list[file_selected].name,ext) ;
							
							if(GPSTRCMP(extention,(char*)ext)) file_selected++ ;
							else ok=1 ;
						}
						
						if(!ok) file_selected = old_file_selected ;
						else
						{
							if(id_select < limit_draw)
								if(file_selected - start - 2 == limit_draw-6) start++ ;
						}
					}
				} ;
			}

			if(instant_key(8)) // up
			{
				if(file_selected != 2)
				{
					file_selected-- ;
					while((!ok)&&(file_selected != 1))
					{	
						get_extention(file_list[file_selected].name,ext) ;
						
						if(GPSTRCMP(extention,(char*)ext)) file_selected-- ;
						else ok=1 ;
					}
					
					if(!ok) file_selected = old_file_selected ;
					else
					{
							if(start)
								if(file_selected - start - 2 == 1) start-- ;
					}
				}
			}

			if(instant_key(2)) // down
			{
				if(file_selected != file_number-1)
				{
					file_selected++ ;
					while((!ok)&&(file_selected != file_number))
					{	
						get_extention(file_list[file_selected].name,ext) ;
						
						if(GPSTRCMP(extention,(char*)ext)) file_selected++ ;
						else ok=1 ;
					}
	
					if(!ok) file_selected = old_file_selected ;
					else
					{
						if(id_select < limit_draw)
							if(file_selected - start - 2 == limit_draw-6) start++ ;
					}
				}
			}

			if(old_file_selected != file_selected)	GPSPRINTF(buffer,"%s\\%s",dir,file_list[file_selected].name) ;
		}

		flip_screen() ;

	} while(!up_front(320)) ; // A or start
}
