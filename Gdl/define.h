/* this file is a part of Gdl 0.4f and was writen by rov, http://procvor.free.fr */

#define Gdl_version	"0.4f"

int key_pressed=0 ;
int old_key = 0 ;
char bbp = 16 ;
//char frequency = 0 ; //0=66mhz 1=80mhz 2:133mhz 3:40mhz
uchar gfx_draw_method = 1 ; // display transparent gfx (1) or not (0)

#define	GPSPRINTF	(gp_str_func.sprintf)
#define	GPSTRLEN	(gp_str_func.gpstrlen)
#define	GPSTRCPY	(gp_str_func.strcpy)
#define	GPSTRCMP	(gp_str_func.compare)
#define	GPSTRCAT	(gp_str_func.strcat)
#define	GPSTRRCHR	(gp_strrchr)
#define	MEMSET		(gp_str_func.memset)

#define get_free_ram	(gp_mem_func.availablemem)
#define _free		(gp_mem_func.free)

#define ulong	unsigned long
#define uchar	unsigned char
#define ushort	unsigned short
#define uint	unsigned int

#define HARDWARE_PALETTE ((dword *)0x14A00400)

GPDRAWSURFACE	gpDraw[2] ;

short nflip = 0 ;

//-----------------------------------------------------------------------

char get_decalage(uchar size)
{
	char temp_size = 128 ;
	char decalage = 7 ;

	while(size != temp_size)
	{
		temp_size = temp_size >> 1 ; // division par 2
		decalage -- ;
	} ;
	
	return decalage ;
}

//-----------------------------------------------------------------------


// fct initialisant l'ecran de la gp et la lib
// char nb_bit : 8 ou 16 bit

void	init_screen(char nb_bit)
{
	bbp = nb_bit ;
	GpGraphicModeSet(bbp,NULL) ;
	
	GpLcdSurfaceGet(&gpDraw[0],0) ;
	GpLcdSurfaceGet(&gpDraw[1],1) ;
	
	
	nflip=0 ;
	GpSurfaceFlip(&gpDraw[0]);
	GpLcdEnable() ;
}

//-----------------------------------------------------------------------

/* affiche une image qui sera decoupée si elle sort de l'ecran
	short x, short y 	: position x et y
	char sizex, char sizey	: taille x et y de l'image
	uchar* sprite	: le pointeur sur l'image a afficher
	char sens		: si a 1 l'image est affichée a l'envers
	ushort transp	: couleur de transparence	*/

void afficher_sprite_cut(short x, short y,ushort sizex, ushort sizey, uchar* sprite, char sens, ushort transp)
{
	ushort aff_x = x ;
	ushort aff_y = y, taille_y = sizey, taille_x = sizex ;
	short debut_x = 0, debut_y = 0 ;
	
	if(x < 0)
	{
		aff_x = 0 ;
		taille_x = sizex + x ;
		debut_x = aff_x - x ;
	}
	
	if(y < 0)
	{
		aff_y = 0 ;
		taille_y = sizey + y ;
		debut_y = aff_y - y ;
	}
	
	if(x+sizex > 320)
	{
		if(x < 320) taille_x = 320 - x ;
		else return ;//taille_x = 0 ;
	}
	
	if(y+sizey > 240)
	{
		if(y < 240) taille_y = 240 - y ;
		else return ;//taille_y = 0 ;
	}

	if(transp&&gfx_draw_method)
	{
		if(bbp==16)
		{
			if(!sens) GpTransBlt16(NULL,&gpDraw[nflip], aff_x, aff_y, taille_x, taille_y, sprite, debut_x, debut_y, sizex, sizey, transp) ;
			else GpTransLRBlt16(NULL,&gpDraw[nflip], aff_x, aff_y, taille_x, taille_y, sprite, debut_x, debut_y, sizex, sizey, transp) ;
		}
		else
		{
			if(!sens) GpTransBlt(NULL,&gpDraw[nflip], aff_x, aff_y, taille_x, taille_y, sprite, debut_x, debut_y, sizex, sizey, transp) ;
			else GpTransLRBlt(NULL,&gpDraw[nflip], aff_x, aff_y, taille_x, taille_y, sprite, debut_x, debut_y, sizex, sizey, transp) ;	
		}
	}
	else	{
			if(bbp==16)
			{
				if(!sens) GpBitBlt16(NULL,&gpDraw[nflip], aff_x, aff_y, taille_x, taille_y, sprite, debut_x, debut_y, sizex, sizey) ;
				else GpBitLRBlt16(NULL,&gpDraw[nflip], aff_x, aff_y, taille_x, taille_y, sprite, debut_x, debut_y, sizex, sizey) ;
			}
			else
			{
				if(!sens) GpBitBlt(NULL,&gpDraw[nflip], aff_x, aff_y, taille_x, taille_y, sprite, debut_x, debut_y, sizex, sizey) ;
				else GpBitLRBlt(NULL,&gpDraw[nflip], aff_x, aff_y, taille_x, taille_y, sprite, debut_x, debut_y, sizex, sizey) ;	
			}
		}
}


void draw_sprite_cut(short x, short y,ushort sizex, ushort sizey, short rx, short ry, short rsx, short rsy, uchar* sprite, char sens, ushort transp)
{
	short X = x+rx ;
	short Y = y+ry ;
	ushort aff_x = X ;
	ushort aff_y = Y, taille_y = rsy, taille_x = rsx ;
	short debut_x = rx, debut_y = ry ;
	
	if(X < 0)
	{
		if(X+rsx < 0) return ;
		aff_x = 0 ;
		taille_x = rsx + X ;
		debut_x = aff_x + rx - X ;
	}
	
	if(Y < 0)
	{
		if(Y+rsy < 0) return ;
		aff_y = 0 ;
		taille_y = rsy + Y ;
		debut_y = aff_y + ry - Y ;
	}
	
	if(X+rsx > 320)
	{
		if(X < 320) taille_x = 320 - X ;
		else return ;//taille_x = 0 ;
	}
	
	if(Y+rsy > 240)
	{
		if(Y < 240) taille_y = 240 - Y ;
		else return ;//taille_y = 0 ;
	}

	if(transp&&gfx_draw_method)
	{
		if(bbp==16)
		{
			if(!sens) GpTransBlt16(NULL,&gpDraw[nflip], aff_x, aff_y, taille_x, taille_y, sprite, debut_x, debut_y, sizex, sizey, transp) ;
			else GpTransLRBlt16(NULL,&gpDraw[nflip], aff_x, aff_y, taille_x, taille_y, sprite, debut_x, debut_y, sizex, sizey, transp) ;
		}
		else
		{
			if(!sens) GpTransBlt(NULL,&gpDraw[nflip], aff_x, aff_y, taille_x, taille_y, sprite, debut_x, debut_y, sizex, sizey, transp) ;
			else GpTransLRBlt(NULL,&gpDraw[nflip], aff_x, aff_y, taille_x, taille_y, sprite, debut_x, debut_y, sizex, sizey, transp) ;
		}
	}
	else	{
			if(bbp==16)
			{
				if(!sens) GpBitBlt16(NULL,&gpDraw[nflip], aff_x, aff_y, taille_x, taille_y, sprite, debut_x, debut_y, sizex, sizey) ;
				else GpBitLRBlt16(NULL,&gpDraw[nflip], aff_x, aff_y, taille_x, taille_y, sprite, debut_x, debut_y, sizex, sizey) ;
			}
			else
			{
				if(!sens) GpBitBlt(NULL,&gpDraw[nflip], aff_x, aff_y, taille_x, taille_y, sprite, debut_x, debut_y, sizex, sizey) ;
				else GpBitLRBlt(NULL,&gpDraw[nflip], aff_x, aff_y, taille_x, taille_y, sprite, debut_x, debut_y, sizex, sizey) ;
			}
		}
}

//-----------------------------------------------------------------------

// efface l'ecran de la gp, en 32 bits     merci a sBibi pour cette fonction ^__^

void	clr_scr(void)
{	
	register uint	*p_buf;
	register int	i;
	
	p_buf = (uint *)gpDraw[nflip].ptbuffer;
	
	if(bbp == 16) i = 38400;  // 16bit, on affiche 2 pixels a la fois, 38400 = 320*240/2
	else i = 19200; // 8bit, on affiche 4 pixels a la fois, 19200 = 320*240/4
	
	while (i--) p_buf[i] = 0xffffffff;
}

void	clr_scr_color(uint color)
{	
	register uint	*p_buf;
	register int	i;
	
	p_buf = (uint *)gpDraw[nflip].ptbuffer;
	
	if(bbp == 16) i = 38400;  // 16bit, on affiche 2 pixels a la fois, 38400 = 320*240/2
	else i = 19200; // 8bit, on affiche 4 pixels a la fois, 19200 = 320*240/4
	
	while (i--) p_buf[i] = color;
}

/* affiche un background de 320*240 a l'ecran, par mem copy
	ushort * bg	: pointeur sur le bg a afficher	*/

void	aff_bg_memcpy(ushort * bg)
{
	gm_memcpy(&gpDraw[nflip].ptbuffer, bg, 320*240*(bbp/8));
}

//-----------------------------------------------------------------------

/* affiche un background de 320*240 a l'ecran, en 32 bit
	uint * bg	: pointeur sur le bg a afficher	*/

void	afficher_bg_32bit(uint * bg)
{	
	register uint	*p_buf;
	register int	i;
	
	p_buf = (uint *)gpDraw[nflip].ptbuffer;

	if(bbp == 16) i = 38400;  // 16bit, on affiche 2 pixels a la fois, 38400 = 320*240/2
	else i = 19200; // 8bit, on affiche 4 pixels a la fois, 19200 = 320*240/4
	
	while (i--) p_buf[i] = bg[i] ;
}

//-----------------------------------------------------------------------

/* compte et affiche le nombre de fps */

void count_and_draw_fps(void)
{
	static long time=0 ;
	static short fps=0,fps_count=0 ;
	char buffer[20] ;
	static long last_time ;
	long tick = GpTickCountGet() ;
	short frame_time = tick - last_time ;
	
	last_time = tick ;
	
	fps++ ;
			
	if(tick > (time + 1000)) { time=tick ; fps_count=fps; fps=0 ; }
	
	GPSPRINTF(buffer,"%i|%i|%i",fps_count,frame_time,key_pressed) ;
	
	/*switch (frequency)
	{
		case 0:
 			GPSPRINTF(buffer,"%i|%i|%i|66mhz",fps_count,frame_time,key_pressed) ;
 			break ;
		case 1:
 			GPSPRINTF(buffer,"%i|%i|%i|80mhz",fps_count,frame_time,key_pressed) ;
 			break ;
		case 2:
 			GPSPRINTF(buffer,"%i|%i|%i|133mhz",fps_count,frame_time,key_pressed) ;
 			break ;
		case 3:
 			GPSPRINTF(buffer,"%i|%i|%i|40mhz",fps_count,frame_time,key_pressed) ;
 			break ;
	}*/

	//draw_text_rl(&font,buffer,315,5) ;

	if(bbp==16) GpTextOut16(NULL, &gpDraw[nflip],10,10,buffer,0xB3);
	else GpTextOut(NULL, &gpDraw[nflip],10,10,buffer,0xB3);
}

short count_fps(void)
{
	static long time=0 ;
	static short fps=0,fps_count=0 ;
	long tick = GpTickCountGet() ;
	
	fps++ ;
			
	if(tick > (time + 1000)) { time=tick ; fps_count=fps; fps=0 ; }
	
	return fps_count ;
}

//-----------------------------------------------------------------------

/* flippe les buffer d'ecran */

void flip_screen(void)
{		
	GpSurfaceFlip( &gpDraw[nflip]);
	nflip ^= 1;
}

//-----------------------------------------------------------------------

/* attend 'ms' millisecondes */

void pause(int ms)
{
	uint ticks = GpTickCountGet();
	
	while(GpTickCountGet()-ticks<ms);
}

//-----------------------------------------------------------------------

