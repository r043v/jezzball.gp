/* this file is a part of Gdl 0.4f and was writen by rov, http://procvor.free.fr */

short test_colision_pixel(struct gfx * sprite1, struct gfx * sprite2, char anim_1, char anim_2, char count,unsigned short trace)
{
		//char buffer[30] ;
		
		char colision=0;
		
		// detection colision par zone simple
		if((sprite1->pos_y + sprite1->size_y) >= sprite2->pos_y)
			if(sprite1->pos_y <= (sprite2->pos_y + sprite2->size_y))
				if((sprite1->pos_x + sprite1->size_x) >= sprite2->pos_x)
					if(sprite1->pos_x <= (sprite2->pos_x + sprite2->size_x))
						colision=1 ;

		// si colision
		if(colision)
		{
			unsigned short nb_col = 0,cpt_x,cpt_y ;
			
			//char trace=0 ;
			
			short spr1_x=0,spr1_y=0,spr2_x=0,spr2_y=0,size_x,size_y ;

			// calcul des rectangles de colision dans les 2 sprites

			if(sprite1->pos_x < sprite2->pos_x) spr1_x = sprite2->pos_x - sprite1->pos_x ;
			else if(sprite2->pos_x < sprite1->pos_x) spr2_x = sprite1->pos_x - sprite2->pos_x ;
			
			if(sprite1->pos_y < sprite2->pos_y) spr1_y = sprite2->pos_y - sprite1->pos_y ;
			else if(sprite2->pos_y < sprite1->pos_y) spr2_y = sprite1->pos_y - sprite2->pos_y ;
			
			if(sprite1->pos_x + sprite1->size_x >= sprite2->pos_x + sprite2->size_x)
			{
				if(sprite1->pos_x > sprite2->pos_x) size_x = (sprite2->pos_x + sprite2->size_x) - (sprite1->pos_x) ;
				else size_x = sprite2->size_x ;
			}
			else if(sprite1->pos_x + sprite1->size_x < sprite2->pos_x + sprite2->size_x)	 size_x = (sprite1->pos_x + sprite1->size_x) - (sprite2->pos_x) ;
			
			
			if(sprite1->pos_y + sprite1->size_y >= sprite2->pos_y + sprite2->size_y)
			{
				if(sprite1->pos_y > sprite2->pos_y) size_y = (sprite2->pos_y + sprite2->size_y) - (sprite1->pos_y) ;
				else size_y = sprite2->size_y ;
			}
			else if(sprite1->pos_y + sprite1->size_y < sprite2->pos_y + sprite2->size_y) size_y = (sprite1->pos_y + sprite1->size_y) - (sprite2->pos_y) ;

			if(size_x > sprite1->size_x) size_x=sprite1->size_x ;
			if(size_y > sprite1->size_y) size_y=sprite1->size_y ;

			//GPSPRINTF(buffer,"%i|%i|%i|%i|%i|%i",spr1_x,spr1_y,spr2_x,spr2_y,size_x,size_y) ;
			//if(bbp==16) GpTextOut16(NULL, &gpDraw[nflip],10,10,buffer,0xB3);
			//else GpTextOut(NULL, &gpDraw[nflip],10,10,buffer,0xB3);

			// calcul du nombre de pixels en contact

			if(bbp==8)
			{
				for(cpt_x=0;cpt_x<size_x;cpt_x++)
					for(cpt_y=0;cpt_y<size_y;cpt_y++)
					{					
						if(GetAnimatedPixel(spr1_x + cpt_x,spr1_y + cpt_y,sprite1,anim_1) != sprite1->transp_color)
							if(GetAnimatedPixel(spr2_x + cpt_x,spr2_y + cpt_y,sprite2,anim_2) != sprite2->transp_color)
							{
								if(count) nb_col++ ;
								else return 1 ;
								
								if(trace) PutPixel(trace,sprite1->pos_x + spr1_x + cpt_x,sprite1->pos_y + spr1_y + cpt_y) ;
								//if(trace) blur_pixel(sprite1->pos_x + spr1_x + cpt_x,sprite1->pos_y + spr1_y + cpt_y) ;
							}
					} ;
			}
			else
			{
				for(cpt_x=0;cpt_x<size_x;cpt_x++)
					for(cpt_y=0;cpt_y<size_y;cpt_y++)
					{					
						if(GetAnimatedPixel_16(spr1_x + cpt_x,spr1_y + cpt_y,sprite1,anim_1) != sprite1->transp_color)
							if(GetAnimatedPixel_16(spr2_x + cpt_x,spr2_y + cpt_y,sprite2,anim_2) != sprite2->transp_color)
							{
								if(count) nb_col++ ;
								else return 1 ;
								
								if(trace) PutPixel_16(trace,sprite1->pos_x + spr1_x + cpt_x,sprite1->pos_y + spr1_y + cpt_y) ;
								//if(trace) blur_pixel_16(sprite1->pos_x + spr1_x + cpt_x,sprite1->pos_y + spr1_y + cpt_y) ;
							}
					} ;				
			}

			return nb_col ; // renvoi le nb de pixels en contact
		}
		
	return 0 ; // pas de colision
}

// renvoi si l'objet gfx est en contact avec le pixel x y de l'ecran
short test_pixel_colision_gfx_point(struct gfx * sprite1,short x, short y, short x_gfx, short y_gfx, char anim_1)
{
		char colision=0;
		//unsigned short pixel_color ;
		
		
		// detection colision par zone simple
		if((y_gfx + sprite1->size_y) >= y)
			if(y_gfx <= y)
				if((x_gfx + sprite1->size_x) >= x)
					if(x_gfx <= x)
						colision=1 ;

		if(colision)
		{
			unsigned char tmpx = x - x_gfx ;
			unsigned char tmpy = y - y_gfx ;

			if(bbp==8)
			{
				if(GetAnimatedPixel(tmpx,tmpy,sprite1,anim_1) != sprite1->transp_color)
					return 1 ;
			}
			else
			{
				if(GetAnimatedPixel_16(tmpx,tmpy,sprite1,anim_1) != sprite1->transp_color)
					return 1 ;
			}

			return 0 ;
		}
		
	return 0 ;
}

//-----------------------------------------------------------------------

/* teste une colision entre 2 objets gfx
	struct gfx * sprite1	: pointeur sur l'objet gfx numero 1
	struct gfx * sprite2	: pointeur sur l'objet gfx numero 2
	char type		: type de colision (voir fonction pour les != types)	*/

short test_colision_zone(struct gfx * sprite1, struct gfx * sprite2,char type)
{
	// colision par zone simple
	if(type==0)
	{		
		if((sprite1->pos_y + sprite1->size_y) >= sprite2->pos_y)
			if(sprite1->pos_y <= (sprite2->pos_y + sprite2->size_y))
				if((sprite1->pos_x + sprite1->size_x) >= sprite2->pos_x)
					if(sprite1->pos_x <= (sprite2->pos_x + sprite2->size_x))
						return 1 ;

					return 0 ;					
	}
	else	
	if(type==1) // colision par zone, renvoi le mur de 1 à 9
	{
		char colision=0;
		//char buffer[50] ;
		
		// detection colision
		if((sprite1->pos_y + sprite1->size_y) >= sprite2->pos_y)
			if(sprite1->pos_y <= (sprite2->pos_y + sprite2->size_y))
				if((sprite1->pos_x + sprite1->size_x) >= sprite2->pos_x)
					if(sprite1->pos_x <= (sprite2->pos_x + sprite2->size_x))
						colision=1 ;

		// si colision
		if(colision)
		{
			// detection de la zone colision
			
			short x1 = sprite1->pos_x ;
			short y1 = sprite1->pos_y ;
			short x2 = sprite1->pos_x + sprite1->size_x ;
			short y2 = sprite1->pos_y + sprite1->size_y ;
			
			short xx1 = sprite2->pos_x ;
			short yy1 = sprite2->pos_y ;
			short xx2 = sprite2->pos_x + sprite2->size_x ;
			short yy2 = sprite2->pos_y + sprite2->size_y ;
			
			short x=0,y=0 ;

			// calcul de la position y du sprite 1 par rapport au 2 en colision
			if(y1 < yy1) y=0 ;
			else
			{
				if(y2 > yy2) y=2 ;
				else y=1 ;
			}

			// calcul de la position x du sprite 1 par rapport au 2 en colision
			if(x1 < xx1) x=0 ;
			else
			{
				if(x2 > xx2) x=2 ;
				else x=1 ;
			}

			return((y*3)+(x+1)) ;
		}
		
		return colision ;
	}
	else
	if(type==2) // colision par zone, renvoi le mur de 1 à 17
	{
		char colision=0;
		
		// detection colision
		if((sprite1->pos_y + sprite1->size_y) >= sprite2->pos_y)
			if(sprite1->pos_y <= (sprite2->pos_y + sprite2->size_y))
				if((sprite1->pos_x + sprite1->size_x) >= sprite2->pos_x)
					if(sprite1->pos_x <= (sprite2->pos_x + sprite2->size_x))
						colision=1 ;

		// si colision
		if(colision)
		{
			// detection de la zone colision
			
			short x1 = sprite1->pos_x ;
			short y1 = sprite1->pos_y ;
			short x2 = sprite1->pos_x + sprite1->size_x ;
			short y2 = sprite1->pos_y + sprite1->size_y ;
			
			short xx1 = sprite2->pos_x ;
			short yy1 = sprite2->pos_y ;
			short xx2 = sprite2->pos_x + sprite2->size_x ;
			short yy2 = sprite2->pos_y + sprite2->size_y ;

			// calcul de la position y du srite 1 par rapport au 2 en colision
			
			if(y1 < yy1)
			{
				if(x1 < xx1) colision = 2 ;
				else	if(x1 == xx1) colision = 3 ;
					else	if((x1 > xx1)&&(x2<xx2)) colision = 4 ;
						else	if(x2 == xx2) colision = 5 ;
							else	if(x2 > xx2) colision = 6 ;
			}
			else	if(y1 == yy1)
				{
					if(x1 < xx1) colision = 1 ;
					else	if(x2 > xx2) colision = 7 ;
						else colision = 4 ;	
				}
				else	if(y2 == yy2)
					{
						if(x1 < xx1) colision = 11 ;
						else	if(x2 > xx2) colision = 17 ;
							else colision = 14 ;	
					}
					else	if((y1 > yy1)&&(y2<yy2))
						{
							if(x1 < xx1) colision = 8 ;
							else	if(x2 > xx2) colision = 10 ;
								else colision = 9 ;				
						} else	if(y2 > yy2)
							{
								if(x1 < xx1) colision = 12 ;
								else	if(x1 == xx1) colision = 13 ;
									else	if((x1 > xx1)&&(x2<xx2)) colision = 14 ;
										else	if(x2 == xx2) colision = 15 ;
											else	if(x2 > xx2) colision = 16 ;
							}
				return colision ;
			}
	}
	
	return 0 ;
}

//-----------------------------------------------------------------------

