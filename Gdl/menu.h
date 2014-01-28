/* this file is a part of Gdl 0.4f and was writen by rov, http://procvor.free.fr */

// menu exemple

// draw_menu("0|1|2|3|4|5|6|7|8|9",64,32,32,&font8,"choose a level|r0v43v PaWa !  ->",2,0xAFE1,1,1) // left cursor
// draw_menu("this|is|a|test||yipi||yo !||Gdl|is|very|powerfull||:))))))",64,32,32,&font8,"hello world !|<-",0,0xB34D,1,1) // right cursor
// draw_menu("new|load|option",64,32,32,&font8,"choose your destiny",1,0xAFE1,1,1) // no cursor

// MsgBox exemple

// show_message_box(NULL, 0, "this is\na test ...", &font8, 0xA311,1) ; // no anim, no title, just text
// show_message_box(NULL, 0, "yo chui une caira moi !\nthis is\na test ...", &font8, 0xA311,1) ; // ok, now there is a title
// show_message_box((uchar *)error_anim, 16, "yo chui une caira moi !\nthis is\na test ...", &font8, 0xA311,1) ; // add draw of an 16² animation
// MsgBox("game over ...\n\npress start to continu.") ; // draw simple MsgBox
// MsgBox("hello world !|this is\na test") ; // draw MsgBox with title

ushort make_color(uchar r, uchar v, uchar b, uchar a)
{
	return (r<<11) | (v<<6) | (b<<1) | a ;
}

ushort fade_16b_color(ushort color,uchar sens) // 0 to black, 1 to white
{
	uchar r,v,b,a ;

	r = ( color >> 11 ) & 31;
	v = ( color >> 6  ) & 31;
	b = ( color >> 1  ) & 31;
	a = ( color & 1 );

	switch(sens)
	{
		case 0 :
			//if(!a)
			{
				if(r < 31) r++;
				if(v < 31) v++;
				if(b < 31) b++;
				
				
				
				
				//a=1 ;
			} //else { a=0 ; }
		break ;

		case 1 :
			//if(a)
			{	if(r) r--;
				if(v) v--;
				if(b) b--;
				//a=0 ;
			} //else { a=1 ; }
		break ;			
	}
a=0 ;
	return (r<<11) | (v<<6) | (b<<1) | a ;	
}

void	draw_degrade_rect(ushort pos_x, ushort pos_y, ushort size_x, ushort size_y, ushort color, uchar sens)
{
	uchar fade_color = 1 ;
	ushort cptx, cpty, skip_value=0, skip=0, tmp_color=color ;
	uchar r,v,b ;
	
	if(sens > 9) { sens-=10 ; fade_color=0 ; }
	tmp_color = color ;
	
	r = (tmp_color>>11) & 31;
	v = (tmp_color>> 6) & 31;
	b = (tmp_color>> 1) & 31;

	if(fade_color)
	{	
		if(31-r > skip_value) skip_value = 31-r ;
		if(31-v > skip_value) skip_value = 31-v ;
		if(31-b > skip_value) skip_value = 31-b ;
	}
	else
	{
		skip_value=255 ;
		if(31-r < skip_value) skip_value = 31-r ;
		if(31-v < skip_value) skip_value = 31-v ;
		if(31-b < skip_value) skip_value = 31-b ;	
	}

	switch(sens)
	{
		case 0 : // fade left to right
			skip_value = (size_x/skip_value)/2 ;
		
			for(cptx=0 ; cptx<size_x ; cptx++)
			{
				for(cpty=0;cpty<size_y;cpty++)
					PutPixel_16(tmp_color,pos_x+cptx,pos_y+cpty) ;
					
				if(skip++ == skip_value)
				{
					tmp_color = fade_16b_color(tmp_color,fade_color) ; // 0 to black, 1 to white
					skip=0 ;
				}
			} ;
		break ;

		case 1 : // fade right to left
			skip_value = (size_x/skip_value)/2 ;
		
			for(cptx=size_x ; cptx ; cptx--)
			{
				for(cpty=0;cpty<size_y;cpty++)
					PutPixel_16(tmp_color,pos_x+cptx,pos_y+cpty) ;
					
				if(skip++ == skip_value)
				{
					tmp_color = fade_16b_color(tmp_color,fade_color) ; // 0 to black, 1 to white
					skip=0 ;
				}
			} ;
		break ;

		case 2 : // fade up to down
			skip_value = (size_y/skip_value)/2 ;
		
			for(cpty=0;cpty<size_y;cpty++)
			{
				for(cptx=size_x ; cptx ; cptx--)
					PutPixel_16(tmp_color,pos_x+cptx,pos_y+cpty) ;
					
				if(skip++ == skip_value)
				{
					tmp_color = fade_16b_color(tmp_color,fade_color) ; // 0 to black, 1 to white
					skip=0 ;
				}
			} ;
		break ;

		case 3 : // fade down to up
			skip_value = (size_y/skip_value)/2 ;
		
			for(cpty=size_y;cpty;cpty--)
			{
				for(cptx=size_x ; cptx ; cptx--)
					PutPixel_16(tmp_color,pos_x+cptx,pos_y+cpty) ;
					
				if(skip++ == skip_value)
				{
					tmp_color = fade_16b_color(tmp_color,fade_color) ; // 0 to black, 1 to white
					skip=0 ;
				}
			} ;
		break ;
	}
}

void draw_rect_border(ushort pos_x, ushort pos_y, ushort size_x, ushort size_y, ushort color)
{
	ushort cpt ;

	for(cpt=pos_x;cpt<pos_x+size_x;cpt++) // draw the 2 lines
	{ PutPixel_16(color,cpt,pos_y) ; PutPixel_16(color,cpt,pos_y+size_y-1) ; } ;
	
	for(cpt=pos_y+1;cpt<pos_y+size_y-1;cpt++) // draw the 2 col
	{ PutPixel_16(color,pos_x,cpt) ; PutPixel_16(color,pos_x+size_x-1,cpt) ; } ;
}


void draw_rect_with_border(ushort pos_x, ushort pos_y, ushort size_x, ushort size_y, ushort color, ushort border_color, uchar sens)
{
	draw_degrade_rect(pos_x+1, pos_y+1, size_x-2, size_y-2, color, sens) ;
	draw_rect_border(pos_x, pos_y, size_x, size_y, border_color) ;	
}



ushort draw_menu
(
	char *text_menu,
	ushort valid_key,
	ushort echap_key,
	ushort extra_key,
	struct font *font,
	char *title_text,
	uchar option_pos,
	ushort color,
	ushort degrade,
	ushort blur
)
{
	ushort cptx, cpty, cpt, cpt_item, tmp, start_y ;
	uchar  items_number, item_selected ;
	uchar *items_size, max_size ;
	long last_key_time=0 ;
	uchar cursor_size=0 ;
	//static aloc = 0 ;
	//static uchar * pos ;

	short size_x, size_y ;
	ushort tmp_color ;
	unsigned char r,v,b,a ;

	//if(!aloc) { pos = gm_zi_malloc(255) ; aloc=1 ; }

	if(!echap_key) echap_key=0xFFFF ;
	if(!extra_key) extra_key=0xFFFF ;

	// blur screen
	
	for(cpt=0;cpt<blur;cpt++)
	{
		for(cptx=2;cptx<318;cptx++)
			for(cpty=2;cpty<238;cpty++)
				if(bbp==8) blur_pixel(cptx,cpty+1) ;
				else blur_pixel_16(cptx,cpty+1) ;
		nflip^=1 ;
	
		for(cptx=3;cptx<318;cptx+=rand()%2+1)
			for(cpty=3;cpty<238;cpty+=rand()%2+1)
				if(bbp==8) blur_pixel(cptx+1,cpty) ;
				else blur_pixel_16(cptx+1,cpty) ;
		nflip^=1 ;
	} ;

	cpt = 0 ;
	tmp = 0 ;
	items_number=1 ;

	while(text_menu[cpt] != '\0')
	{
		if(text_menu[cpt] == '|') items_number++ ;
		cpt++ ;				
	} ;

	cpt = 0 ;
	while(title_text[cpt] != '\0')
	{
		if(title_text[cpt] == '|')
		{
			title_text[cpt] = '\0' ;
			cursor_size = cpt+1 ;
			cpt-- ;
		}
		cpt++ ;
	} ;

/*	if(GPSTRLEN(title_text)>1) // if too many title char to save the pos
	{
		tmp = (title_text[0]+title_text[1])/2 ;

		if(pos[tmp]) // save pos found
		{
			item_selected = pos[tmp] ;
		} else item_selected = 0 ;
	}*/

	item_selected = 0 ;

	items_size = gm_malloc(items_number) ;

	cpt = 0 ;
	cpt_item = 0 ;
	max_size = 0 ;

	while(text_menu[cpt] != '\0')
	{
		if(text_menu[cpt] == '|')
		{
			text_menu[cpt] = '\0' ;
			items_size[cpt_item] = tmp ;
			if(tmp > max_size) max_size = tmp ;
			tmp=0 ;
			cpt_item++ ;
		}
		else tmp++ ;

		cpt++ ;				
	} ;

	items_size[cpt_item] = tmp ;
	if(tmp > max_size) max_size = tmp ;
	
	if(GPSTRLEN(title_text) > max_size) max_size = GPSTRLEN(title_text) ;

	start_y = (240 - (items_number*(font->size_y+2) + 2*font->size_y))>>1 ;
	start_y += (5 + font->size_y) ;

		size_x = max_size*font->size_x + 10 ;
		size_y = items_number*(font->size_y+2) + 2*font->size_y ;

	do
	{
		refresh_key() ;
		
		switch(bbp)
		{
			case 8 :
				GpRectFill(NULL, &gpDraw[nflip], (320-size_x)>>1, (240-size_y)>>1, size_x, 3+font->size_y, color);

				GpRectFill(NULL, &gpDraw[nflip], (320-size_x)>>1, ((240-size_y)>>1) + 5 + font->size_y, size_x, size_y-(5+font->size_y), color);
				break ;

			case 16 :
				if(degrade)
				{	
					short max_color = 0, skip=0 ;
					tmp_color = color ;
					
					r = (tmp_color>>11) & 31;
					v = (tmp_color>> 6) & 31;
					b = (tmp_color>> 1) & 31;
					
					if(31-r > max_color) max_color = 31-r ;
					if(31-v > max_color) max_color = 31-v ;
					if(31-b > max_color) max_color = 31-b ;

					max_color = (size_x/max_color)/2 ;
					
					for(cptx=0;cptx<size_x;cptx++)
					{
						for(cpty=0;cpty<3+font->size_y;cpty++)	PutPixel_16(tmp_color,((320-size_x)>>1)+cptx,((240-size_y)>>1)+cpty) ;
						for(cpty=0;cpty<size_y-(5+font->size_y);cpty++)	PutPixel_16(tmp_color,((320-size_x)>>1)+cptx,(((240-size_y)>>1) + 5 + font->size_y)+cpty) ;
							
						if(skip++ == max_color)
						{
							r = ( tmp_color >> 11 ) & 31;
							v = ( tmp_color >> 6  ) & 31;
							b = ( tmp_color >> 1  ) & 31;
							a = ( tmp_color & 1 );
		
							if(!a)
							{	if(r<31) r++;
								if(v<31) v++;
								if(b<31) b++;
								a=1 ;
							} else { a=0 ; }

							tmp_color = (r<<11) | (v<<6) | (b<<1) | a ;

							skip=0 ;
						}
					} ;
				} else {
					GpRectFill16(NULL, &gpDraw[nflip], (320-size_x)>>1, (240-size_y)>>1, size_x, 3+font->size_y, color);
					GpRectFill16(NULL, &gpDraw[nflip], (320-size_x)>>1, ((240-size_y)>>1) + 5 + font->size_y, size_x, size_y-(5+font->size_y), color);
				       }
				break ;					
		}

		tmp=0 ;
		
		draw_text_center_x(font,title_text,start_y - 2*font->size_y +4) ;
		
		for(cpt=0;cpt<items_number;cpt++)
		{
			if(item_selected == cpt)
			{
				if(cursor_size)
					switch(option_pos)
					{
						case 0 : draw_text(font,title_text+cursor_size,(320-(max_size*font->size_x + 10)>>1) + 10 + items_size[cpt]*font->size_x,start_y+10*cpt+2) ; break ;
						case 2 : draw_text_rl(font,title_text+cursor_size,(320-(max_size*font->size_x + 10)>>1) + max_size*font->size_x + 10 - items_size[cpt]*font->size_x - 10,start_y+10*cpt+2) ; break ;
					}
									
				gfx_draw_method = 0 ;
			}

			switch(option_pos)
			{
				case 0 : draw_text(font,text_menu + tmp,(320-(max_size*font->size_x + 10)>>1) + 5,start_y+10*cpt+2) ; break ;
				case 1 : draw_text_center_x(font,text_menu + tmp,start_y+10*cpt+2) ; break ;
				case 2 : draw_text_rl(font,text_menu + tmp,(320-(max_size*font->size_x + 10)>>1) + max_size*font->size_x + 10 - 5,start_y+10*cpt+2) ; break ;
			}
			
			gfx_draw_method = 1 ;
			
			tmp += items_size[cpt] + 1 ;
		} ;

		if((up_front(8)||instant_key(8)&&(GpTickCountGet() > last_key_time + 150))||(up_front(2)||instant_key(2)&&(GpTickCountGet() > last_key_time + 150)))
		{
			short way ; // 0: down  1: up
			
			if(up_front(8)||instant_key(8)&&(GpTickCountGet() > last_key_time + 150))
			{ if(item_selected > 0) item_selected-- ; way=1 ; }
			if(up_front(2)||instant_key(2)&&(GpTickCountGet() > last_key_time + 150))
			{ if(item_selected < items_number-1) item_selected++ ; way=0 ; }
						
			while(!items_size[item_selected])
			{
				switch(way)
				{
					case 0 : // down
						if(item_selected < items_number-1) item_selected++ ;
						else way^=1 ;
						break ;
					case 1 : // up
						if(item_selected > 0) item_selected-- ;
						else way^=1 ;
						break ;
				}
			} ;
			
			last_key_time = GpTickCountGet() ;			
		}


		flip_screen() ;

	} while((!up_front(valid_key))&&(!up_front(echap_key))&&(!up_front(extra_key))) ;
	_free(items_size) ;

	//if(GPSTRLEN(title_text)>1) pos[(title_text[0]+title_text[1])/2] = item_selected ; // save_pos

	if(up_front(valid_key)) return item_selected ;
	if(up_front(extra_key)) return 0xFFFF ;
	
	return 0 ;
}


uchar	show_message_box(uchar * bitmap, uchar size_bmp, char * text, struct font * font, ushort color, ushort degrade) // don't put an '\n' in the title !!!!
{
	ushort cpt=0, text_pos=0, tmp=0, max_line_size=0, line_number=1, max_size_x, max_size_y ;
	ushort w_pos_x, w_pos_y, size_bmp_x=0 ;
	struct gfx gfx ;

	if(bitmap != NULL)
	{	if(size_bmp == 16)	size_bmp_x = (sizeof(bitmap)%(16*16*(bbp/8)) -1)*16 ;
		else			size_bmp_x = (sizeof(bitmap)%(32*32*(bbp/8)) -1)*32 ;
	}

	// search title, text, max line size and line number

	while(text[cpt] != '\0')
	{
		//if((cpt+1)*(font->size_x) > 280) text[cpt] = '\n' ; // jump line if to many char, to not crash...

		if(text[cpt] == '|')
		{
			text[cpt] = '\0' ;
			text_pos = tmp+1 ;
			tmp=0 ;
		}
		else tmp++ ;

		if(text[cpt] == '\n')
		{
			if(max_line_size < tmp)	{ max_line_size = tmp ; tmp=0 ; }
			line_number++ ;
		}

		cpt++ ;
	} ;

	if(max_line_size < tmp)	max_line_size = tmp ;

	// compute max window size x
	
	max_size_x = max_line_size*font->size_x ;
	
	if(bitmap != NULL) // if bitmap include
			max_size_x += (size_bmp + 10) ;

	if(text_pos) // if title include
		if(text_pos*font->size_x > max_size_x) max_size_x = text_pos*font->size_x ;
	
	max_size_x += 20 ;
	
	// compute max window size y

	max_size_y = line_number*(font->size_y+2) ;
	
	if(bitmap != NULL) // if bitmap include
	{	//max_size_x += size_bmp+10 ;
		if(size_bmp+20 > max_line_size*font->size_x) max_size_y += (size_bmp+20 - (size_bmp > max_line_size*font->size_x)) ;
	}

	if(text_pos) // if title include
		max_size_y += font->size_y + 5 ;
	
	max_size_y += 20 ;

	// compute window pos
	
	w_pos_x = (320 - max_size_x)>>1 ;
	w_pos_y = (240 - max_size_y)>>1 ;

	if((w_pos_x > 319)||(w_pos_y >= 239)) return 0 ;
	
	//if((w_pos_x < 0)||(w_pos_y < 0)) return 0 ;

	if(bitmap != NULL)
		switch(bbp)
		{
			case 8 :  load_anim(&gfx,0,0,size_bmp_x/size_bmp,bitmap,100,2,0,size_bmp,size_bmp,0,0,get_color_buffer( 0, 0, size_bmp, bitmap)) ; break ;
			case 16 : load_anim(&gfx,0,0,size_bmp_x/size_bmp,bitmap,100,2,0,size_bmp,size_bmp,0,0,get_color_buffer_16( 0, 0, size_bmp, (ushort *)bitmap)) ; break ;
		}
	do {
		//draw_int(font,size_bmp_x,5,5) ;
		
		// draw rect
	
		switch(bbp)
		{
			case 8 :
				if(text_pos) // if title, draw 2 rect
				{
					GpRectFill(NULL, &gpDraw[nflip], w_pos_x, w_pos_y, max_size_x, font->size_y+4, color);
					GpRectFill(NULL, &gpDraw[nflip], w_pos_x, w_pos_y + font->size_y+6, max_size_x, max_size_y - (font->size_y+6), color);
				}
				else	GpRectFill(NULL, &gpDraw[nflip], w_pos_x, w_pos_y, max_size_x, max_size_y, color);
				
				break ;
	
			case 16 :
				if(degrade)
				{	
					if(text_pos) // if title, draw 2 rect
					{
						draw_rect_with_border(w_pos_x, w_pos_y, max_size_x, font->size_y+4, color, 256-color, 0) ;
						draw_rect_with_border(w_pos_x, w_pos_y + font->size_y+6, max_size_x, max_size_y - (font->size_y+6), color, 256-color, 0) ;
					}
					else	draw_rect_with_border(w_pos_x, w_pos_y, max_size_x, max_size_y, color, 256-color, 0) ;
				} else  {
						if(text_pos) // if title, draw 2 rect
						{
							GpRectFill16(NULL, &gpDraw[nflip], w_pos_x, w_pos_y, max_size_x, font->size_y+4, color);
							GpRectFill16(NULL, &gpDraw[nflip], w_pos_x, w_pos_y + font->size_y+6, max_size_x, max_size_y - (font->size_y+6), color);
						}
						else	GpRectFill16(NULL, &gpDraw[nflip], w_pos_x, w_pos_y, max_size_x, max_size_y, color);
					}
				break ;					
		}
	
		// draw title, text and bitmap
		
		if(text_pos) // if title
		{	
			draw_text_center_x(font,(char *)text,w_pos_y+2) ; // draw title
			
			if(bitmap==NULL)	draw_text_center_x_multiline(font,(char *)(text+text_pos),w_pos_y + font->size_y+15) ; // draw text
			else	{
					draw_text(font,(char *)(text+text_pos),w_pos_x + size_bmp + 20,w_pos_y + font->size_y+15) ; // draw text
					play_anim_xy(&gfx,0,w_pos_x + 10,w_pos_y + font->size_y+10,0) ;
				}
			
		}
		else
		{
			if(bitmap==NULL)	draw_text_center_x_multiline(font,text+text_pos,w_pos_y+10) ; // draw text
			else	{
					draw_text(font,(char *)(text+text_pos),w_pos_x + size_bmp + 20,w_pos_y + 10) ; // draw text
					play_anim_xy(&gfx,0,w_pos_x + 10,w_pos_y+10,0) ;
				}
		}
		
		refresh_key() ;
		flip_screen() ;
	
	} while(!up_front(256)) ;

	return 1 ;
}

void MsgBox(char * text)
{
	if(show_message_box(NULL, 0, text, gdl_font, make_color(0,31,0,1), 1)==0)
		MsgBox("MsgBox : error|window size is too hight\nreduce line or bitmap size.\n\npress start to continu...") ;
}


