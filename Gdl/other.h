/* this file is a part of Gdl 0.4f and was writen by rov, http://procvor.free.fr */

//-----------------------------------------------------------------------

// timer function, thx to AnataGp for that :)

void timer_dec(  struct font * font, unsigned int * s, unsigned short x, unsigned short y)
{
	static long time;
	if ( GpTickCountGet() > time + 961 ) { (*s)--; time = GpTickCountGet(); }
	draw_int(font, *s, x, y) ;	
}

void timer_inc(  struct font * font, unsigned int * s, unsigned short x, unsigned short y)
{
	static long time;
	if ( GpTickCountGet() > time + 961 ) { (*s)++; time = GpTickCountGet(); }
	draw_int(font, *s, x, y) ;	
}

//-----------------------------------------------------------------------

ushort * convert_8b_to_16b(uchar * data, ushort size_x, ushort size_y, uint * pal)
{
	ushort cptx, cpty ;
	ushort * output ;
	
	if(get_free_ram() < size_x*size_y*2 + 102400) { MsgBox("error : convert 8b to 16b|not enought ram to\ncreate the 16b buffer") ; return NULL ; } ;
	
	output = gm_malloc(size_x*size_y*2) ;
	
	for(cptx=0;cptx<size_x;cptx++)
		for(cpty=0;cpty<size_y;cpty++)
			set_color_buffer_16(cptx, cpty, size_y, output, pal[get_color_buffer(cptx, cpty, size_y, data)]) ;
	
	return output ;
}

struct font * set_system_font(struct font * font)
{
	struct font * temp ;
	temp = gdl_font ;
	
	if((font->size_x > 3)&&(font->size_x < 65)) {	gdl_font = font ;	return temp ; }
	else { MsgBox("set_system_font : error|font is invalid, can t load.") ; return font ; }
}

