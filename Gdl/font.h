/* this file is a part of Gdl 0.4f and was writen by rov, http://procvor.free.fr */

void load_font(struct font * font, unsigned char * img_font, unsigned char sx, unsigned char sy, unsigned short tr_color, unsigned char nb_char)
{
	unsigned short cpt ;

	font->size_x = sx ;
	font->size_y = sy ;
	font->transp_color = tr_color ;

	for(cpt=0;cpt<nb_char;cpt++)
	{
		font->img[cpt+32] = img_font + (cpt*sx*sy*(bbp/8)) ;
	} ;
}

void load_font_(struct font * font, unsigned char * img_font, unsigned char sx, unsigned char sy, unsigned short tr_color, unsigned char nb_char, unsigned char start)
{
	unsigned short cpt ;

	font->size_x = sx ;
	font->size_y = sy ;
	font->transp_color = tr_color ;

	for(cpt=0;cpt<nb_char;cpt++)
	{
		font->img[cpt+start] = img_font + (cpt*sx*sy*(bbp/8)) ;
	} ;
}

// affiche du txt a une certaine position de l'ecran
void draw_text(struct font * font,char * text, unsigned short x, unsigned short y)
{
	unsigned short cpt;
	short cpt_txt ;
	unsigned short nb_char = GPSTRLEN(text) ;
	
	cpt_txt = -1 ;
	for(cpt=0;cpt<nb_char;cpt++)
	{	
		cpt_txt++ ;
		if(text[cpt]=='\n') { y+=(font->size_y+2) ; cpt_txt=-1 ; }
		else	afficher_sprite_cut(x + cpt_txt*font->size_x,y,font->size_x,font->size_y,font->img[text[cpt]],0,font->transp_color) ;
	} ;
}

// affiche du txt, la fin du txt se trouvera a la position specifié (alignement a droite)
void draw_text_rl(struct font * font, char * text, unsigned short x, unsigned short y)
{
	unsigned short cpt;
	short cpt_txt ;
	unsigned char nb_char = GPSTRLEN(text) ;
	
	unsigned short start_x = x - font->size_x*nb_char ;
	
	cpt_txt = -1 ;
	for(cpt=0;cpt<nb_char;cpt++)
	{	
		cpt_txt++ ;
		if(text[cpt]=='\n') { y+=font->size_y ; cpt_txt=-1 ; }
		else	afficher_sprite_cut(start_x + cpt_txt*font->size_x,y,font->size_x,font->size_y,font->img[text[cpt]],0,font->transp_color) ;
	} ;
}

void draw_text_center(struct font * font, char * text) // draw text at the center of the screen, not support multi line
{
	draw_text(font,text,((320 - (GPSTRLEN(text)*font->size_x))>>1),((240 - (font->size_y))>>1)) ;
}

void draw_text_center_x(struct font * font, char * text, unsigned short y) // draw text center on the screen height, not support multi line
{
	draw_text(font,text,((320 - (GPSTRLEN(text)*font->size_x))>>1),y) ;
}

void draw_text_center_x_multiline(struct font * font, char * text, unsigned short y) // draw text center on the screen height, not support multi line
{
	short cpt=0, size=0, size_max=0 ;
	
	// search max line size	
	while(text[cpt] != '\0')
	{
		if((text[cpt] == '\n')||(text[cpt+1] == '\0'))
		{
			if(size_max < size) size_max=size ;
			size = 0 ;
		} else size++ ;
		
		cpt++ ;
	} ;
	
	draw_text(font,text,(320 - size_max*font->size_x)>>1,y) ;
}

void draw_text_center_y(struct font * font, char * text, unsigned short x) // draw text center on the screen weight, not support multi line
{
	draw_text(font,text,x,((240 - (font->size_y))>>1)) ;
}

void draw_text_vibrate(struct font * font,char * text, unsigned short x, unsigned short y, unsigned char powa)
{
	draw_text(font,text,x+(powa*((rand()%2)-1)),y+(powa*((rand()%2)-1))) ;
}

void draw_text_center_vibrate(struct font * font,char * text, unsigned char powa)
{
	draw_text(font,text,((320 - (GPSTRLEN(text)*font->size_x))>>1)+(powa*((rand()%2)-1)),((240 - (font->size_y))>>1)+(powa*((rand()%2)-1))) ;
}

void draw_text_center_x_vibrate(struct font * font,char * text, unsigned short y, unsigned char powa)
{
	draw_text(font,text,((320 - (GPSTRLEN(text)*font->size_x))>>1)+(powa*((rand()%2)-1)),y+(powa*((rand()%2)-1))) ;
}

void draw_text_center_y_vibrate(struct font * font,char * text, unsigned short x, unsigned char powa)
{
	draw_text(font,text,x+(powa*((rand()%2)-1)),((240 - (font->size_y))>>1)+(powa*((rand()%2)-1))) ;
}

void draw_int(struct font * font, int value, unsigned short x, unsigned short y)
{
 	char buffer[6];
 	GPSPRINTF(buffer,"%i",value) ;

	draw_text(font,buffer, x, y) ;
}

void draw_int_rl(struct font * font, int value, unsigned short x, unsigned short y)
{
 	char buffer[6];
 	GPSPRINTF(buffer,"%i",value) ;

	draw_text_rl(font,buffer, x, y) ;
}

void draw_vertical_text(struct font * font, char * text, unsigned short x, unsigned short y)
{
	unsigned short cpt;
	short cpt_txt ;
	unsigned short nb_char = GPSTRLEN(text) ;

	cpt_txt = -1 ;
	for(cpt=0;cpt<nb_char;cpt++)
	{
		cpt_txt++ ;
		if(text[cpt]=='\n') { x+=font->size_x ; cpt_txt=-1 ; }
		else	afficher_sprite_cut(x,y + cpt_txt*font->size_y,font->size_x,font->size_y,font->img[text[cpt]],0,font->transp_color) ;
	} ;
}

