/* this file is a part of Gdl 0.4f and was writen by rov, http://procvor.free.fr */

//-----------------------------------------------------------------------

//  pixel function - 8bit

//-----------------------------------------------------------------------

unsigned char GetPixel(short x, short y, struct gfx * gfx, unsigned char img_id)
{
	unsigned char *p_buf = (unsigned char *)gfx->img[img_id] ;
 	return *(p_buf + x*gfx->size_y + (gfx->size_y-y));
}

//-----------------------------------------------------------------------

void SetPixel(short x, short y, struct gfx * gfx, unsigned char img_id, unsigned char color)
{
	unsigned char *p_buf = (unsigned char *)gfx->img[img_id] ;
 	*(p_buf + x*gfx->size_y + (gfx->size_y-y)) = color ;
}

//-----------------------------------------------------------------------

unsigned char GetAnimatedPixel(short x, short y, struct gfx * gfx,unsigned char anim)
{
	unsigned char *p_buf = (unsigned char *)gfx->img[gfx->animation[anim].curent_frame] ;
 	return *(p_buf + x*gfx->size_y + (gfx->size_y-y));
}

//-----------------------------------------------------------------------

void PutPixel(unsigned char color, short x, short y)
{
	unsigned char *p_buf = (unsigned char *) (gpDraw[nflip].ptbuffer);
	*(p_buf + x*240 + (239-y)) = color ;
}

//-----------------------------------------------------------------------

void PutPixel_transp(unsigned char color, short x, short y, unsigned char transp_color)
{
	unsigned char *p_buf = (unsigned char *) (gpDraw[nflip].ptbuffer);
	if(transp_color != color) *(p_buf + x*240 + (239-y)) = color ;
}

//-----------------------------------------------------------------------

unsigned char get_screen_pixel(short x, short y)
{
	unsigned char *screen = (unsigned char *) (gpDraw[nflip].ptbuffer);

	return *(screen + x*240 + (239-y)) ;
}

//-----------------------------------------------------------------------
//pas de blur en 8bit, effet chelou :D
void blur_pixel(short x, short y)
{
	unsigned char color = ( get_screen_pixel(x-1,y) + get_screen_pixel(x+1,y) + get_screen_pixel(x,y-1) + get_screen_pixel(x,y+1) ) >>2 ;
	PutPixel(color,x,y) ;
}

//-----------------------------------------------------------------------

// pixel function - 16bit

//-----------------------------------------------------------------------

unsigned short GetPixel_16(short x, short y, struct gfx * gfx, unsigned char img_id)
{
	unsigned short *p_buf = (unsigned short *)gfx->img[img_id] ;
 	return *(p_buf + x*gfx->size_y + (gfx->size_y-y));
}

//-----------------------------------------------------------------------

void SetPixel_16(short x, short y, struct gfx * gfx, unsigned char img_id, unsigned short color)
{
	unsigned short *p_buf = (unsigned short *)gfx->img[img_id] ;
	
 	*(p_buf + x*gfx->size_y + (gfx->size_y-y)) = color ;
}

//-----------------------------------------------------------------------

unsigned short GetAnimatedPixel_16(short x, short y, struct gfx * gfx,unsigned char anim)
{
	unsigned short *p_buf = (unsigned short *)gfx->img[gfx->animation[anim].curent_frame] ;
 	return *(p_buf + x*gfx->size_y + (gfx->size_y-y));
}

//-----------------------------------------------------------------------

void PutPixel_16(unsigned short color, short x, short y)
{
	unsigned short *p_buf = (unsigned short *) (gpDraw[nflip].ptbuffer);
	*(p_buf + x*240 + (239-y)) = color ;
}

//-----------------------------------------------------------------------

void PutPixel_transp_16(unsigned short color, short x, short y, unsigned short transp_color)
{
	unsigned short *p_buf = (unsigned short *) (gpDraw[nflip].ptbuffer);
	if(transp_color != color) *(p_buf + x*240 + (239-y)) = color ;
}

//-----------------------------------------------------------------------

unsigned short get_screen_pixel_16(short x, short y)
{
	unsigned short *screen = (unsigned short *) (gpDraw[nflip].ptbuffer);

	return *(screen + x*240 + (239-y)) ;
}

//-----------------------------------------------------------------------

void blur_pixel_16(short x, short y)
{
	unsigned short color = ( get_screen_pixel(x-1,y) + get_screen_pixel(x+1,y) + get_screen_pixel(x,y-1) + get_screen_pixel(x,y+1) ) >>2 ;
	PutPixel_16(color,x,y) ;
}

//-----------------------------------------------------------------------

unsigned short get_color_buffer_16(short x, short y, short size_y, unsigned short *buffer)
{
	return *(buffer + x*size_y + (size_y-1-y)) ;
}

void set_color_buffer_16(short x, short y, short size_y, unsigned short *buffer, unsigned short color)
{
	*(buffer + x*size_y + (size_y-1-y)) = color ;
}

unsigned char get_color_buffer(short x, short y, short size_y, unsigned char *buffer)
{
	return *(buffer + x*size_y + (size_y-1-y)) ;
}

void set_color_buffer(short x, short y, short size_y, unsigned char *buffer, unsigned char color)
{
	*(buffer + x*size_y + (size_y-1-y)) = color ;
}
/*
void blur_zone_16(short x1, short y1, short x2, short y2,unsigned short *bg, char method)
{
	short x,y;
	unsigned short color ;
	
	for(x=x1;x<x2;x++)
		for(y=y1;y<y2;y++)
		{				
			if(method)	color = ( get_screen_pixel(x-1,y) + get_screen_pixel(x+1,y) + get_screen_pixel(x,y-1) + get_screen_pixel(x,y+1) ) >>2 ;
			else color = ( GetPixel(x-1,y) + GetPixel(x+1,y) + GetPixel(x,y-1) + GetPixel(x,y+1) ) >>2 ;
			
			return *(bg + x*240 + (239-y)) ;
			
			PutPixel(color,x,y) ;
		} ;
}

unsigned short get_screen_pixel(short x, short y)
{
	unsigned short *screen = (unsigned short *) (gpDraw[nflip].ptbuffer);

	return *(screen + x*240 + (239-y)) ;
}

unsigned short GetPixel(short x, short y)
{
 	unsigned short *p_buf = (unsigned short *)bg16 ; 	
 	return *(p_buf + x*240 + (239-y));
}

*/





void bitblt(ulong*img_src,short posx, short posy, ushort sizex, ushort sizey)
{
	short endx, endy, decx=0, decy=0, col_size, bpp = bbp>>3 ;

	ulong *screen_pos = (ulong *)gpDraw[nflip].ptbuffer, *img_end, *next_col_end ;

	if(posx<0)
	{
		if((endx = sizex+posx)<0) return ;
		decx = -posx ;
		posx = 0 ;
		img_src += decx*sizey*bpp ;
	}

	if(endx > 319)
	{
		if(posx > 319) return ;
		endx -= endx-320 ;
	}

	if(posy < 0)
	{
		if((endy = sizey+posy)<0) return ;
		decy = -posy ;
		posy = 0 ;
		img_src += (sizey-1-decy)*bpp ;
	}

	if(endy > 239)
	{
		if(posy > 239) return ;
		endy -= endy-240 ;
		sizey = endy - posy ;
	}

	col_size=sizey*bpp ;
	img_end = img_src + sizex*col_size ;
	screen_pos += (posx*240 + (239-posy))*bpp ;
	next_col_end = screen_pos + col_size ;

	do {	*screen_pos = *img_src ;
		img_src++ ;

		if(screen_pos == next_col_end)
		{
			screen_pos += (240 - sizey)*bpp ;
			next_col_end = screen_pos + col_size ;
		} else screen_pos++ ;		
	} while(img_src != img_end) ;
}

			//*screen_pos = *(ulong *)((ushort*)img_src + (cptx-posx+decx)*sizey + ((sizey-1)-(cpty-posy+decy))) ; // show horizontal flip
			//*screen_pos = *(ulong *)((ushort*)img_src + (endx-(cptx-posx+decx))*sizey + ((sizey-1)-(endy-(cpty-posy+decy)))) ; // show vertical flip
			//if(bbp==8)	PutPixel(get_color_buffer(cptx-posx,cpty-posy,sizey,(uchar*)img_src),cptx,cpty) ;
			//else		PutPixel_16(get_color_buffer_16(cptx-posx,cpty-posy,sizey,(ushort*)img_src),cptx,cpty) ;
		//screen_pos=(ulong *)((ushort*)gpDraw[nflip].ptbuffer+(cptx*240 + (239-cpty))*(bbp/8)) ;
		//screen_pos += (23 - sizey) ;// (240-sizey)*(bbp/8) ;
		//(ulong *)(((ushort*)gpDraw[nflip].ptbuffer) + cptx*240 + (239-cpty)) ;
		//+ ((cptx-posx)*sizey + ((sizey-1)-(endy-(cpty-posy)))/4) ;
