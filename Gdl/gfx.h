/* this file is a part of Gdl 0.4f and was writen by rov, http://procvor.free.fr */

//-----------------------------------------------------------------------

/* affiche une img d'un objet gfx découpé, a une certaine position
	struct gfx * gfx	: pointeur sur l'objet gfx
	short x			: position x
	short y			: position y	*/

void draw_gfx_xy(struct gfx * gfx, short x, short y, char sens, unsigned char img_id)
{
	if(gfx->transp[img_id]!=0xff)
	{	
		if(gfx->transp[img_id])	draw_sprite_cut(x,y,gfx->size_x,gfx->size_y,gfx->real_pos_x[img_id],gfx->real_pos_y[img_id],gfx->real_size_x[img_id],gfx->real_size_y[img_id],gfx->img[img_id],sens,gfx->transp_color) ;
		else 			draw_sprite_cut(x,y,gfx->size_x,gfx->size_y,gfx->real_pos_x[img_id],gfx->real_pos_y[img_id],gfx->real_size_x[img_id],gfx->real_size_y[img_id],gfx->img[img_id],sens,0) ;
	}
}

//---------------------------------------------------------------------

void draw_gfx(struct gfx * gfx, char sens, unsigned char img_id)
{
	if(gfx->transp[img_id]!=0xff)
	{	
		if(gfx->transp[img_id])	draw_sprite_cut(gfx->pos_x,gfx->pos_y,gfx->size_x,gfx->size_y,gfx->real_pos_x[img_id],gfx->real_pos_y[img_id],gfx->real_size_x[img_id],gfx->real_size_y[img_id],gfx->img[img_id],sens,gfx->transp_color) ;
		else 			draw_sprite_cut(gfx->pos_x,gfx->pos_y,gfx->size_x,gfx->size_y,gfx->real_pos_x[img_id],gfx->real_pos_y[img_id],gfx->real_size_x[img_id],gfx->real_size_y[img_id],gfx->img[img_id],sens,0) ;
	}
}


//-----------------------------------------------------------------------

/* defini une animation d'un objet gfx
	
	struct gfx * gfx	: un pointeur sur l'objet gfx
	char id_anim		: numero d'anim de l'objet gfx, de 0 à 29
	char f_start		: premiere image de l'anim sur les 100 d'un objet gfx
	char f_stop		: derniere image de l'anim
	unsigned short time	: temps entre 2 frames
	char repeat		: type d'animation  0=123 1=123212.. 2=123123.. 3=123333.. 4=123111..
	char sens		: sens de l'animation  0=123 1=321	*/

void set_anim(struct gfx * gfx,char id_anim,char f_start,char f_stop, unsigned short time, char repeat,char sens)
{
	gfx->animation[id_anim].repeat = repeat ;
	gfx->animation[id_anim].frame_start = f_start ;
	gfx->animation[id_anim].frame_stop = f_stop ;
	gfx->animation[id_anim].time_anim = time ;
	gfx->animation[id_anim].sens = sens ;
	gfx->animation[id_anim].finish = 0 ;
	gfx->animation[id_anim].last_aff_time = 0 ;
	
	if(!sens) gfx->animation[id_anim].curent_frame = f_start ; // 123
	else  gfx->animation[id_anim].curent_frame = f_stop ; // 321
}


//-----------------------------------------------------------------------

/* initialise un objet gfx et une de ses animation
	
	struct gfx * gfx	: un pointeur sur l'objet gfx
	char id_anim		: numero d'anim de l'objet gfx, de 0 à 29
	char frm_start		: premiere image de l'anim sur les 100 d'un objet gfx
	char frm_number		: nombre d'images de l'anim
	unsigned char * anim	: pointeur sur la serie d'image de l'anim (toute l'anim ds la mm images, les frames les unes a la suite des autres)
	unsigned short time	: temps entre 2 frames
	char repeat		: type d'animation  0=123 1=123212.. 2=123123.. 3=123333.. 4=123111..
	char sens		: sens de l'animation  0=123 1=321
	unsigned short sx	: taille horizontale de l'objet gfx
	unsigned short sy	: taille verticale de l'objet gfx
	unsigned short px	: position x a l'ecran de l'objet gfx
	unsigned short py	: position y a l'ecran de l'objet gfx
	unsigned short tr_color	: couleur de transparence de l'objet gfx	*/

void load_anim(struct gfx * gfx,char id_anim,unsigned char frm_start,unsigned char frm_number,unsigned char * anim,unsigned short time, char repeat,char sens, unsigned short sx,unsigned short sy,unsigned short px,unsigned short py,unsigned short tr_color)
{
	short cpt,cpt_x,cpt_y ;

	short pos1,pos2,posy1,posy2 ;

	gfx->size_x = sx ;
	gfx->size_y = sy ;
	gfx->pos_x = px ;
	gfx->pos_y = py ;

	gfx->animation[id_anim].repeat = repeat ;
	gfx->animation[id_anim].frame_start = frm_start ;
	gfx->animation[id_anim].frame_stop = frm_start + frm_number - 1 ;
	gfx->animation[id_anim].time_anim = time ;
	gfx->animation[id_anim].sens = sens ;
	gfx->animation[id_anim].finish = 0 ;
	gfx->animation[id_anim].last_aff_time = 0 ;
	gfx->transp_color = tr_color ;
	
	if(!sens) gfx->animation[id_anim].curent_frame = frm_start ; // 123
	else  gfx->animation[id_anim].curent_frame = frm_start + frm_number - 1 ; // 321

	gfx->moving=0 ;
	
	for(cpt=0;cpt<frm_number;cpt++)
	{
		if(bbp==16) gfx->img[frm_start+cpt] = anim + (cpt*gfx->size_x*gfx->size_y*2) ;
		else gfx->img[frm_start+cpt] = anim + (cpt*gfx->size_x*gfx->size_y) ;

		gfx->transp[frm_start + cpt] = 0 ;

		gfx->real_size_x[frm_start + cpt] = sx ;
		gfx->real_size_y[frm_start + cpt] = sy ;
		gfx->real_pos_x[frm_start + cpt] = 0 ;
		gfx->real_pos_y[frm_start + cpt] = 0 ;

		if(tr_color)
		{
			// search real image bounds
			
			char transp_ = 1 ;
			char Atransp_ = 0 ;

			cpt_y = 1 ;
			
			do	// bound up
			{
				cpt_x = 0;

				do
				{
					unsigned short color ;
					
					cpt_x++ ;
					
					if(bbp==8)	color = *((unsigned char *)gfx->img[frm_start + cpt] + cpt_x*gfx->size_y + (gfx->size_y-cpt_y)) ;
					else		color = *((unsigned short *)gfx->img[frm_start + cpt] + cpt_x*gfx->size_y + (gfx->size_y-cpt_y)) ;
					
					if(tr_color != color)
					{
						transp_ = 0 ;
						
						if(cpt_y) gfx->real_pos_y[frm_start + cpt] = cpt_y-1 ;
						else      gfx->real_pos_y[frm_start + cpt] = cpt_y ;
					}
				} while(cpt_x<gfx->size_x-1) ;
				
				cpt_y++ ;
							
				if(cpt_y==gfx->size_y) // if non transparent pixel not found in the frame
				{
					Atransp_ = 1 ;
					transp_=0 ;
				}

			} while(transp_) ;

			transp_ = 1 ;
			cpt_x = 0 ;

			if(!Atransp_)
				do	// bound left
				{
					cpt_y=0 ;
					do
					{
						unsigned short color ;
						
						if(bbp==8)	color = *((unsigned char *)gfx->img[frm_start + cpt] + cpt_x*gfx->size_y + (gfx->size_y-cpt_y)) ;
						else		color = *((unsigned short *)gfx->img[frm_start + cpt] + cpt_x*gfx->size_y + (gfx->size_y-cpt_y)) ;
						
						if(tr_color != color)
						{
							transp_ = 0 ;
							gfx->real_pos_x[frm_start + cpt] = cpt_x ;
						}
						
						cpt_y++ ;
					} while(cpt_y<=gfx->size_y) ;
	
					cpt_x++ ;

				} while(transp_) ;
	
				transp_ = 1 ;
				cpt_y = gfx->size_y ;
	
			if(!Atransp_)			
				do	// bound down
				{
					cpt_x = 0 ;
					do
					{
						unsigned short color ;
						
						if(bbp==8)	color = *((unsigned char *)gfx->img[frm_start + cpt] + cpt_x*gfx->size_y + (gfx->size_y-cpt_y)) ;
						else		color = *((unsigned short *)gfx->img[frm_start + cpt] + cpt_x*gfx->size_y + (gfx->size_y-cpt_y)) ;
						
						if(tr_color != color)
						{
							transp_ = 0 ;
							gfx->real_size_y[frm_start + cpt] = cpt_y - gfx->real_pos_y[frm_start + cpt] ;
						}
						
						cpt_x++ ;
					} while(cpt_x<gfx->size_x) ;
					cpt_y-- ;

				} while(transp_) ;
				
				transp_ = 1 ;
				cpt_x = gfx->size_x-1 ;
	
			if(!Atransp_)
				do	// bound right
				{
					cpt_y=0 ;
					do
					{
						unsigned short color ;
						
						cpt_y++ ;
						
						if(bbp==8)	color = *((unsigned char *)gfx->img[frm_start + cpt] + cpt_x*gfx->size_y + (gfx->size_y-cpt_y)) ;
						else		color = *((unsigned short *)gfx->img[frm_start + cpt] + cpt_x*gfx->size_y + (gfx->size_y-cpt_y)) ;
						
						if(tr_color != color)
						{
							transp_ = 0 ;
							gfx->real_size_x[frm_start + cpt] = cpt_x - gfx->real_pos_x[frm_start + cpt]+1 ;
						}
						
					} while(cpt_y<gfx->size_y-1) ;
	
					cpt_x-- ;

				} while(transp_) ;
	
	
				if(Atransp_) gfx->transp[frm_start + cpt] = 0xff ;
				else
				{
					// search for transp color in the real image bound
		
					pos1 = gfx->real_pos_x[frm_start + cpt] ;
					pos2 = pos1 + gfx->real_size_x[frm_start + cpt] ;
		
					posy1 = gfx->real_pos_y[frm_start + cpt] ;
					posy2 = posy1 + gfx->real_size_y[frm_start + cpt] ;
		
					for(cpt_x=pos1;cpt_x<pos2;cpt_x++)
						for(cpt_y=posy1;cpt_y<posy2;cpt_y++)
						{
							unsigned short color ;
							
							if(bbp==8)	color = *((unsigned char *)gfx->img[frm_start + cpt] + cpt_x*gfx->size_y + (gfx->size_y-cpt_y)) ;
							else		color = *((unsigned short *)gfx->img[frm_start + cpt] + cpt_x*gfx->size_y + (gfx->size_y-cpt_y)) ;
							
							if(tr_color == color)
							{
								gfx->transp[frm_start + cpt] = 1 ;
								cpt_x = pos2 ;
								cpt_y = posy2 ;
							}
						} ;
				}
		}
	} ;
}
//-----------------------------------------------------------------------

char anim_terminated(struct gfx * gfx,char id_anim)
{
	return gfx->animation[id_anim].finish ;
}

//-----------------------------------------------------------------------

void reset_anim(struct gfx * gfx,char id_anim)
{
	if(gfx->animation[id_anim].sens)	gfx->animation[id_anim].curent_frame = gfx->animation[id_anim].frame_stop ;
	else					gfx->animation[id_anim].curent_frame = gfx->animation[id_anim].frame_start ;
	
	
	gfx->animation[id_anim].finish = 0 ;
}

//-----------------------------------------------------------------------
// repeat -> 0=123 1=123212.. 2=123123.. 3=123333.. 4=123111..
void set_next_frame_number(unsigned char * curent_frame, unsigned char frame_start, unsigned char frame_stop, unsigned char repeat, unsigned char * sens, unsigned char * finish)
{
	//char buffer[30] ;
	//short old_frame = *curent_frame ;

	if(repeat != 1) // si pas 1232123..
	{
		if(!(*sens)) // si 123
		{				
			(*curent_frame)++ ;
			
			if( *curent_frame > frame_stop)
			{
				//*curent_frame = frame_stop ;
				
				if(!repeat) *finish=1 ;
				else	if(repeat == 2) *curent_frame = frame_start ;
					else	if(repeat >= 3) *finish=1 ;
			}
		}
		else // si 321
		{
			(*curent_frame)-- ;
			
			if( *curent_frame < frame_start)
			{
				//*curent_frame = frame_start ;
				
				if(!repeat) *finish=1 ;
				else	if(repeat == 2) *curent_frame = frame_stop ;
					else	if(repeat >= 3) *finish=1 ;
			}
		}

		if(*finish)
		{
			if(repeat == 3)
			{
				if(!(*sens))	*curent_frame = frame_stop+1 ;
				else	*curent_frame = frame_start ;
			}
			else
				if(repeat == 4)
				{
					if(*sens)	*curent_frame = frame_stop ;
					else	*curent_frame = frame_start ;
				}
		}		
	}
	else // si 123212321...
	{
		if(!(*sens))
		{
			(*curent_frame)++ ;
			
			if( *curent_frame >= frame_stop)
			{
				//*curent_frame = frame_stop ;
				
				*sens = !(*sens) ;
				*curent_frame = frame_stop ;
			}
		}
		else
		{
			(*curent_frame)-- ;
			
			if( *curent_frame <= frame_start)
			{
				//*curent_frame = frame_start ;

				*sens = !(*sens) ;
				*curent_frame = frame_start ;
			}
		}
	}

 	//GPSPRINTF(buffer,"%i|%i|%i|%i|%i|%i|%i",*curent_frame, old_frame, frame_start, frame_stop, repeat, *sens, *finish) ;
	//if(bbp==16) GpTextOut16(NULL, &gpDraw[nflip],10,25,buffer,0);
	//else GpTextOut(NULL, &gpDraw[nflip],10,25,buffer,0);


}


//-----------------------------------------------------------------------

void play_anim_xy(struct gfx * gfx,char id_anim, short x, short y, unsigned char sens)
{
	// draw the frame
	if(gfx->transp[gfx->animation[id_anim].curent_frame]!=0xff)
	{	
		if(!gfx->animation[id_anim].finish)
		{
			if(gfx->transp[gfx->animation[id_anim].curent_frame])	draw_sprite_cut(x,y,gfx->size_x,gfx->size_y,gfx->real_pos_x[gfx->animation[id_anim].curent_frame],gfx->real_pos_y[gfx->animation[id_anim].curent_frame],gfx->real_size_x[gfx->animation[id_anim].curent_frame],gfx->real_size_y[gfx->animation[id_anim].curent_frame],gfx->img[gfx->animation[id_anim].curent_frame],sens,gfx->transp_color) ;
			else 							draw_sprite_cut(x,y,gfx->size_x,gfx->size_y,gfx->real_pos_x[gfx->animation[id_anim].curent_frame],gfx->real_pos_y[gfx->animation[id_anim].curent_frame],gfx->real_size_x[gfx->animation[id_anim].curent_frame],gfx->real_size_y[gfx->animation[id_anim].curent_frame],gfx->img[gfx->animation[id_anim].curent_frame],sens,0) ;
		}
		else	if(gfx->animation[id_anim].repeat != 0)
			{
				if(gfx->transp[gfx->animation[id_anim].curent_frame])	draw_sprite_cut(x,y,gfx->size_x,gfx->size_y,gfx->real_pos_x[gfx->animation[id_anim].curent_frame],gfx->real_pos_y[gfx->animation[id_anim].curent_frame],gfx->real_size_x[gfx->animation[id_anim].curent_frame],gfx->real_size_y[gfx->animation[id_anim].curent_frame],gfx->img[gfx->animation[id_anim].curent_frame],sens,gfx->transp_color) ;
				else 							draw_sprite_cut(x,y,gfx->size_x,gfx->size_y,gfx->real_pos_x[gfx->animation[id_anim].curent_frame],gfx->real_pos_y[gfx->animation[id_anim].curent_frame],gfx->real_size_x[gfx->animation[id_anim].curent_frame],gfx->real_size_y[gfx->animation[id_anim].curent_frame],gfx->img[gfx->animation[id_anim].curent_frame],sens,0) ;
			}

	}		
		
		// if anim time elapsed, change the frame number
		if(GpTickCountGet() > (gfx->animation[id_anim].last_aff_time + gfx->animation[id_anim].time_anim))
		{	
			set_next_frame_number(&gfx->animation[id_anim].curent_frame,gfx->animation[id_anim].frame_start,gfx->animation[id_anim].frame_stop, gfx->animation[id_anim].repeat, &gfx->animation[id_anim].sens, &gfx->animation[id_anim].finish) ;
			gfx->animation[id_anim].last_aff_time = GpTickCountGet() ;
		}
}

//-----------------------------------------------------------------------

void animate(struct gfx * gfx,char id_anim)
{
		set_next_frame_number(&gfx->animation[id_anim].curent_frame,gfx->animation[id_anim].frame_start,gfx->animation[id_anim].frame_stop, gfx->animation[id_anim].repeat, &gfx->animation[id_anim].sens, &gfx->animation[id_anim].finish) ;
		gfx->animation[id_anim].last_aff_time = GpTickCountGet() ;
}


//-----------------------------------------------------------------------

void auto_move_gfx(struct gfx * gfx,char id_anim)
{
	if(gfx->moving)
	{
		gfx->pos_x += gfx->move[gfx->animation[id_anim].curent_frame][0] ;
		gfx->pos_y += gfx->move[gfx->animation[id_anim].curent_frame][1] ;
	}
}

//-----------------------------------------------------------------------

/* affiche et anime une animation
	struct gfx * gfx	: l'objet gfx contenant l'animation
	char id_anim		: numero d'anim de l'objet gfx		*/

void play_anim(struct gfx * gfx,char id_anim, char sens)
{
	play_anim_xy(gfx,id_anim,gfx->pos_x,gfx->pos_y,sens) ;
}


//-----------------------------------------------------------------------

/* affiche un objet avec un certain niveau de transparence, non fonctionnel
	struct gfx * gfx	: pointeur sur l'objet gfx
	unsigned char opacite	: niveau de transparence	*/

void draw_gfx_transp(struct gfx * gfx,unsigned char opacite)
{
	short cpt_x,cpt_y;
	unsigned short color, screen_color ;
	unsigned short *screen = (unsigned short *) (gpDraw[nflip].ptbuffer);
	
	for(cpt_x=0;cpt_x<gfx->size_x;cpt_x++)
		for(cpt_y=0;cpt_y<gfx->size_y;cpt_y++)
		{
			if((cpt_x>=0)&&(cpt_x<320)&&(cpt_y>=0)&&(cpt_y<240))
			{
				color = GetPixel(cpt_x,cpt_y,gfx,0) ;
				
				if(gfx->transp_color != color)
				{
					screen_color = *(screen + (gfx->pos_x + cpt_x)*240 + 240-(cpt_y + gfx->pos_y)) ;
					
					*(screen + (gfx->pos_x + cpt_x)*240 + (239-(gfx->pos_y + cpt_y))) = opacite*( color - screen_color )/255 + screen_color ;
				}
			}
		} ;		
}

//-----------------------------------------------------------------------

/* remplace les pixels non transparents d'un sprite par ceux d'un bg, pour effacer le sprite - 16bit
	struct gfx * gfx	: pointeur sur l'objet gfx
	unsigned short *bg	: pointeur sur le bg */

void erase_sprite(struct gfx * gfx,unsigned short *bg)
{
	short cpt_x,cpt_y;
	unsigned short *p_buf_0 = (unsigned short *) (gpDraw[nflip].ptbuffer);
	
	for(cpt_x=0;cpt_x<gfx->size_x;cpt_x++)
		for(cpt_y=0;cpt_y<gfx->size_y;cpt_y++)
			if((gfx->pos_x + cpt_x >= 0)&&(gfx->pos_x + cpt_x < 320)&&(gfx->pos_y + cpt_y >= 0)&&(gfx->pos_y + cpt_y < 240))
				if(GetPixel(cpt_x,cpt_y,gfx,0) != gfx->transp_color)
					*(p_buf_0 + (gfx->pos_x + cpt_x)*240 + (239-(gfx->pos_y + cpt_y))) = *(bg + (gfx->pos_x + cpt_x)*240 + 240-(cpt_y + gfx->pos_y)) ;
}

//-----------------------------------------------------------------------

/* remplace les pixels non transparents d'un sprite par une couleur - 16bit
	struct gfx * gfx	: pointeur sur l'objet gfx
	unsigned short color	: couleur a afficher sur le sprite */

void erase_sprite_color(struct gfx * gfx,unsigned short color)
{
	short cpt_x,cpt_y;
	unsigned short *p_buf_0 = (unsigned short *) (gpDraw[nflip].ptbuffer);
	
	for(cpt_x=0;cpt_x<gfx->size_x;cpt_x++)
		for(cpt_y=0;cpt_y<gfx->size_y;cpt_y++)
			if((gfx->pos_x + cpt_x >= 0)&&(gfx->pos_x + cpt_x < 320)&&(gfx->pos_y + cpt_y >= 0)&&(gfx->pos_y + cpt_y < 240))
				if(GetPixel(cpt_x,cpt_y,gfx,0) != gfx->transp_color)
					*(p_buf_0 + (gfx->pos_x + cpt_x)*240 + (239-(gfx->pos_y + cpt_y))) = color ;
}

//-----------------------------------------------------------------------

/* modifi les position x et y d'un objet gfx selon les valeurs specifié
	short dec_x		: decalage en x a effectuer sur la position de l'objet
	short dec_x		: decalage en y a effectuer sur la position de l'objet
	struct gfx * gfx	: pointeur sur l'objet gfx	*/

void move_sprite(short dec_x, short dec_y, struct gfx * gfx)
{
	if(dec_x != 0)	gfx->pos_x += dec_x ;
	if(dec_y != 0)	gfx->pos_y += dec_y ;
}

//-----------------------------------------------------------------------

void draw_gfx_on_map(struct gfx * gfx, struct map * map, char sens, unsigned char img_id)
{
	if(gfx->transp[img_id]!=0xff)
	{	
		if(gfx->transp[img_id])	draw_sprite_cut(gfx->pos_x-map->scrool_x,gfx->pos_y-map->scrool_y,gfx->size_x,gfx->size_y,gfx->real_pos_x[img_id],gfx->real_pos_y[img_id],gfx->real_size_x[img_id],gfx->real_size_y[img_id],gfx->img[img_id],sens,gfx->transp_color) ;
		else 			draw_sprite_cut(gfx->pos_x-map->scrool_x,gfx->pos_y-map->scrool_y,gfx->size_x,gfx->size_y,gfx->real_pos_x[img_id],gfx->real_pos_y[img_id],gfx->real_size_x[img_id],gfx->real_size_y[img_id],gfx->img[img_id],sens,0) ;
	}
}

//-----------------------------------------------------------------------

void play_anim_on_map(struct gfx * gfx, struct map * map, char id_anim, unsigned char sens)
{
	play_anim_xy(gfx,id_anim,gfx->pos_x-map->scrool_x,gfx->pos_y-map->scrool_y,sens) ;
}

