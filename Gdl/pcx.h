/* this file is a part of Gdl 0.4f and was writen by rov, http://procvor.free.fr */

//-------------------------------------------------------------------------------------
void taille_pcx(char * fichier, ushort * largeur, ushort * hauteur)
{
    ulong	p_read_count;
    F_HANDLE flot;
    unsigned width = 0, height = 0;

    GpFileOpen(fichier, OPEN_R, &flot);
    GpFileSeek(flot, FROM_BEGIN, 8, 0);
    GpFileRead(flot,&width,2,&p_read_count);
    GpFileRead(flot,&height,2,&p_read_count);
    width++;
    height++;
    *largeur = width;
    *hauteur = height;
    GpFileClose(flot);
}

void charge_palette_pcx(char * fichier, uint * palette)
{
	ulong	p_read_count;
	unsigned char r,v,b; 
	uint i;
	F_HANDLE flot;
	uchar buffer[768] ;
	ushort A=1 ;
	ushort color_read = 256 ;
	
	GpFileOpen(fichier, OPEN_R, &flot);
	
	GpFileSeek(flot, FROM_END, -769, 0);	
	GpFileRead(flot,buffer,1,&p_read_count);
	
	if(buffer[0]==12) 	// 256 color palette
	{
		GpFileRead(flot,buffer,768,&p_read_count);
	}
	else {			// 16 color palette
		color_read = 16 ;
		GpFileSeek(flot, FROM_BEGIN, 16, 0);
		GpFileRead(flot,buffer,48,&p_read_count);
	}

	for (i=0; i < color_read; i++)
	{
		r = buffer[3*i] ;
		v = buffer[3*i+1] ;
		b = buffer[3*i+2] ;

		r=r>>3 ;
		v=v>>3 ;
		b=b>>3 ;

		palette[i] = (r<<11) | (v<<6) | (b<<1) | A ;
	} ;

	GpFileClose(flot);
}

void rotate_90(uchar * buffer_src, uchar * buffer_dest, ushort largeur, ushort hauteur)
{
	ushort cptx,cpty ;
	uchar color ;
	
	for(cpty=0;cpty<largeur;cpty++)
		for(cptx=0;cptx<hauteur;cptx++)
		{
			color = get_color_buffer(cptx, (largeur-1)-cpty, largeur, buffer_src) ;
			set_color_buffer(cpty,cptx, hauteur, buffer_dest,color) ;
		} ;
}

void rotate_90_and_draw(uchar * buffer_src, uchar * buffer_dest, ushort largeur, ushort hauteur, struct font * font, ushort posx, ushort posy)
{
	ulong cptx,cpty ;
	uchar color ;
	ulong skip=0 , byte_per_percent = (largeur*hauteur)/100 , percent=0 ;
	
	for(cpty=0;cpty<largeur;cpty++)
	{	for(cptx=0;cptx<hauteur;cptx++)
		{
			color = get_color_buffer(cptx, (largeur-1)-cpty, largeur, buffer_src) ;
			set_color_buffer(cpty,cptx, hauteur, buffer_dest,color) ;
		} ;
		skip += hauteur ;
		if(skip >= byte_per_percent)
		{
			short cpt=0 ;
			percent+=skip/byte_per_percent ;
			GpRectFill(NULL, &gpDraw[nflip], posx ,posy , font->size_x<<3, font->size_y, 0xFF) ;
			draw_int(font,percent,posx,posy) ;
			for(cpt=0;cpt<10;cpt++) PutPixel(0xFF,posx-20+100-percent,posy+cpt+10) ;

			skip=0 ;
		}
	} ;
}

void charge_pcx_and_draw(char * fichier, unsigned char * buffer, ulong taille, struct font * font, short posx, short posy)
{
	ulong	p_read_count;
	F_HANDLE	file;
	uchar	*buf ;
	ulong position, file_pos, file_size ;
	ushort num;
	uchar ch;
	short part_number=1, curent_part=1 ;
	ulong skip=0 , byte_per_percent=taille/100 , percent=0 ;

	GpFileGetSize(fichier,&file_size) ;
	
	if(SM_OK != GpFileOpen(fichier, OPEN_R, &file))
	{
		char buffer[150] ;
		GPSPRINTF(buffer,"error|file not found\n%s\npress start to continu.",fichier) ;
		show_message_box(NULL, 0, buffer, gdl_font, 0xF4C2,11) ;
		return ;
	}

	while(get_free_ram()<(file_size/part_number)+10240) part_number++ ;
	
	if(font != NULL)	draw_int(font,part_number,posx+5*font->size_x,posy) ;
	
	buf = gm_malloc(file_size/part_number) ;
	GpFileRead(file,buf,file_size/part_number,&p_read_count);
	
	position=0;
	file_pos = 128 ;
	
	do
	{
		ch = buf[file_pos++] ;
		
		// rle
		if ((ch & 0xC0) == 0xC0)
		{
			num = ch & 0x3F ;
			
			skip+=num ;
			ch = buf[file_pos++] ;
			
			while(num--) buffer[position++]=ch ;
			
			if((file_pos >= file_size/part_number)&&(curent_part != part_number))
			{
				curent_part++ ;
				file_pos=0 ;
				GpFileRead(file,buf,file_size/part_number,&p_read_count);
			}
		}
		else	{	buffer[position++]=ch ; skip++ ;	}
		
		if((file_pos >= file_size/part_number)&&(curent_part != part_number))
		{
			curent_part++ ;
			file_pos=0 ;
			GpFileRead(file,buf,file_size/part_number,&p_read_count);
		}

		if(font != NULL)
			if(skip >= byte_per_percent)
			{
				short cpt=0 ;
				percent+=skip/byte_per_percent ;
				GpRectFill(NULL, &gpDraw[nflip], posx ,posy , font->size_x<<3, font->size_y, 0xFF) ;
				draw_int(font,percent,posx,posy) ;
				draw_int(font,curent_part,posx+3*font->size_x,posy) ;
				draw_int(font,part_number,posx+5*font->size_x,posy) ;
				
				for(cpt=0;cpt<10;cpt++) PutPixel(0xB4,posx-20+percent,posy+cpt+10) ;
	
				skip=0 ;
			}

	} while(position < taille) ;

	GpFileClose(file);
		
	_free(buf) ;
}

void charge_pcx(char * fichier, uchar * buffer, ulong taille)
{
	charge_pcx_and_draw(fichier,buffer,taille,NULL,0,0) ;
}

uchar * load_pcx_with_unrotate_and_draw(char * pcx_path,uchar ** buf_unrotate, struct font * font,short posx, short posy)
{
	uchar *pcx_data, *rotate_data ;
	ushort largeur, hauteur ;

	taille_pcx(pcx_path,&largeur,&hauteur) ;
	draw_text(font,"alocate memory ...",posx,posy) ; flip_screen() ;
	pcx_data = gm_zi_malloc(hauteur*largeur) ;
	rotate_data = gm_zi_malloc(hauteur*largeur) ;
	draw_text(font,"loading image file ..",posx,posy) ; flip_screen() ;
	charge_pcx_and_draw(pcx_path, pcx_data, largeur*hauteur,font,posx,posy) ;
	*buf_unrotate = pcx_data ;
	draw_text(font,"flip bitmap",posx,posy) ; flip_screen() ;
	rotate_90(pcx_data,rotate_data,largeur,hauteur) ;

	return rotate_data ;
}

uchar * load_pcx_for_pcx_viewer(char * pcx_path,uchar ** buf_unrotate, struct font * font)
{
	uchar *pcx_data, *rotate_data ;
	ulong taille_fichier ;
	ushort largeur, hauteur ;
	char buf[50] ;

	nflip^=1 ;
	draw_text_center_x(font,"read image info",180) ;
	
	taille_pcx(pcx_path,&largeur,&hauteur) ;
	GpFileGetSize(pcx_path,&taille_fichier) ;

	GPSPRINTF(buf,"%s, %i*%i, %i B",pcx_path+8,largeur,hauteur,taille_fichier) ;
	draw_text_center_x(font,buf,190) ;
	draw_text_center_x(font,"alocate memory..",200) ;

	rotate_data = pcx_data = NULL ;

	if(get_free_ram() < hauteur*largeur*2 + 102400)
	{
		if(get_free_ram() < hauteur*largeur + 102400)
		{	draw_text_center_x(font,"sorry, not enough memory :/",220) ;
			*buf_unrotate = NULL ;
			return NULL ;
		}
		
		GpRectFill(NULL, &gpDraw[nflip], 0 , 200 , 320, font->size_y, 0xFF) ;
		draw_text_center_x(font,"image will be unfliped...",200) ;
	
		pcx_data = gm_malloc(hauteur*largeur) ;
		rotate_data = NULL ;
	} else {	
		draw_text(font,"ok",230,200) ;
		pcx_data = gm_malloc(hauteur*largeur) ;
		rotate_data = gm_malloc(hauteur*largeur) ;
	}
	
	draw_text_center_x(font,"loading data",210) ;
	charge_pcx_and_draw(pcx_path, pcx_data, largeur*hauteur,font,235,210) ;
	*buf_unrotate = pcx_data ;

	if(rotate_data != NULL)
	{
		draw_text_center_x(font,"flip bitmap",220) ;
		rotate_90_and_draw(pcx_data,rotate_data,largeur,hauteur,font,235,210) ;
		draw_text_center_x(font,"load palette",230) ;
	} else	draw_text_center_x(font,"load palette",220) ;

	nflip^=1 ;
	return rotate_data ;
}

uchar * load_pcx(char * pcx_path)
{
	uchar *pcx_data, *rotate_data, *temp ;
	ushort largeur, hauteur ;

	taille_pcx(pcx_path,&largeur,&hauteur) ;
	
	if(get_free_ram() < hauteur*largeur*2)
	{
		char buffer[100] ;
		GPSPRINTF(buffer,"load_pcx : error|can't load file due to ram.\nfree : %i\nneed : %i\n%i - %i = %i b",get_free_ram(),largeur*hauteur*2,largeur*hauteur*2,get_free_ram(),largeur*hauteur*2-get_free_ram()) ;
		MsgBox(buffer) ; return NULL ;
	}

	pcx_data = gm_malloc(hauteur*largeur) ;
	rotate_data = gm_malloc(hauteur*largeur) ;
	
	charge_pcx(pcx_path, pcx_data, largeur*hauteur) ;
	rotate_90(pcx_data,rotate_data,largeur,hauteur) ;

	_free(pcx_data) ;

	if(bbp==16) // if 16 bit, convert the 8 bit pcx to 16 bit
	{	
		uint pal[256] ;
		
		//MsgBox("load_pcx|convert 8bits image file to 16 bits") ;
		
		charge_palette_pcx(pcx_path,pal) ;
		temp = rotate_data ;
		rotate_data = (uchar *)convert_8b_to_16b(temp, largeur, hauteur, pal) ;
		_free(temp) ;
	}
	
	return rotate_data ;
}

uchar * load_pcx_with_unrotate(char * pcx_path,uchar ** buf_unrotate)
{
	uchar *pcx_data, *rotate_data ;
	ushort largeur, hauteur ;

	taille_pcx(pcx_path,&largeur,&hauteur) ;
	pcx_data = gm_zi_malloc(hauteur*largeur) ;
	rotate_data = gm_zi_malloc(hauteur*largeur) ;

	charge_pcx(pcx_path, pcx_data, largeur*hauteur) ;
	
	rotate_90(pcx_data,rotate_data,largeur,hauteur) ;

	if(bbp==16) // if 16 bit, convert the 8 bit pcx to 16 bit
	{	
		uint * pal ;
		uchar * temp ;
		
		charge_palette_pcx(pcx_path,pal) ;
		
		temp = rotate_data ;
		rotate_data = (uchar *)convert_8b_to_16b(temp, largeur, hauteur, pal) ;
		_free(temp) ;
		
		temp = pcx_data ;
		pcx_data = (uchar *)convert_8b_to_16b(temp, largeur, hauteur, pal) ;
		_free(temp) ;		
	}

	*buf_unrotate = pcx_data ;

	return rotate_data ;
}

void load_font_from_pcx(struct font * font, char * pcx_path)
{
	ushort largeur, hauteur, nb_char, tr_color ;
	uchar * img_font ;
	char buffer[150] ;

	taille_pcx(pcx_path,&largeur,&hauteur) ;
	nb_char = (largeur/hauteur) ;

	img_font = load_pcx(pcx_path) ;
	
	if(img_font != NULL)
	{
		tr_color = get_color_buffer_16(0, 0, hauteur, (ushort *)img_font) ;
	
		//GPSPRINTF(buffer,"pcx file info|%s\nlargeur %i\nhauteur %i\nnb char %i\ntransp color %X %i",pcx_path,largeur,hauteur,nb_char,tr_color,tr_color) ;
		//MsgBox(buffer) ;
	
		//load_font(font,load_pcx(pcx_path),hauteur,hauteur,,nb_char) ;
	
		load_font(font, img_font, hauteur, hauteur, tr_color, nb_char) ;
	
		//MsgBox("pcx file loaded") ;
	}
	else
	{
		GPSPRINTF(buffer,"unable to load font\n%s",pcx_path) ;
		MsgBox(buffer) ;
	}
}

void load_unsquare_font_from_pcx(struct font * font, char * pcx_path, ushort size_letter_x)
{
	ushort largeur, hauteur, nb_char, tr_color ;
	uchar * img_font ;
	//char buffer[150] ;

	taille_pcx(pcx_path,&largeur,&hauteur) ;
	nb_char = (largeur/size_letter_x) ;

	img_font = load_pcx(pcx_path) ;
	tr_color = get_color_buffer_16(0, 0, hauteur, (ushort *)img_font) ;

	//GPSPRINTF(buffer,"pcx file info|%s\nlargeur %i\nhauteur %i\nnb char %i\ntransp color %X %i",pcx_path,largeur,hauteur,nb_char,tr_color,tr_color) ;
	//MsgBox(buffer) ;

	//load_font(font,load_pcx(pcx_path),hauteur,hauteur,,nb_char) ;

	load_font(font, img_font, size_letter_x, hauteur, tr_color, nb_char) ;

	//MsgBox("pcx file loaded") ;
}


