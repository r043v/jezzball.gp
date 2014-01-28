/* this file is a part of Gdl 0.4f and was writen by rov, http://procvor.free.fr */

// palette function, great thank to ThunderZ ^__^

void black_pal(void)
{
	unsigned int i;
	for (i=0;i<255;i++)	HARDWARE_PALETTE[i]=0x0000;
}

void white_pal(void)
{
	unsigned int i;
	for (i=0;i<255;i++)	HARDWARE_PALETTE[i]=0xFFFF;
}

void random_pal(void)
{
	unsigned int i;
	for (i=0;i<255;i++)	HARDWARE_PALETTE[i]=rand()%0xffff ;
}

void rotate_palette(uint * palette)
{
	unsigned char r,v,b ;
	ushort i ;

	for(i=0;i<0xFF;i++)
	{
		b = (palette[i]>>11) & 0x1f;
		v = (palette[i]>> 6) & 0x1f;
		r = (palette[i]>> 1) & 0x1f;

		palette[i] = (r<<11) | (v<<6) | (b<<1) | 0;
	} ;
}

void set_pal(unsigned int *pal_to_go)
{
 unsigned int i;
 
	// palette 
	for (i=0;i<255;i++)	
	 HARDWARE_PALETTE[i]=pal_to_go[i];
}

void palette_backup(uint *PaletteBackup)
{
	int color;
	for (color=0;color<256;color++) 
		PaletteBackup[color]=HARDWARE_PALETTE[color];	
}

void palette_restore(uint *PaletteBackup)
{
	int color;
	for (color=0;color<256;color++) 
		HARDWARE_PALETTE[color]=PaletteBackup[color];	
	
}

void fade_out(int speed)
{
	unsigned char r,v,b;
	unsigned int temp;
	short i ;

	do {
		temp=0;
		for (i=0;i<255;i++)
		{
			r= (HARDWARE_PALETTE[i]>>11) & 0x1f;
			v= (HARDWARE_PALETTE[i]>> 6) & 0x1f;
			b= (HARDWARE_PALETTE[i]>> 1) & 0x1f;
			if (r) r--;
			if (v) v--;
			if (b) b--;
			if (r+v+b) temp++;
			HARDWARE_PALETTE[i] = (r<<11) | (v<<6) | (b<<1) | 0 ;
		}
		
		pause(speed);
		if (!temp) break;
		
	} while(1) ;
}

void fade_to_palette(short speed, unsigned int *pal_to_go)
{
 	int color;
 	int cpt=0 ;
 	unsigned char r,v,b,r2,v2,b2;
 	
 	do {
 		cpt=0 ;
 		
	 	for (color=0;color<256;color++)
	 	{
				r= (HARDWARE_PALETTE[color]>>11) & 31;
				v= (HARDWARE_PALETTE[color]>> 6) & 31;
				b= (HARDWARE_PALETTE[color]>> 1) & 31;
				r2= (pal_to_go[color]>>11) & 31;
				v2= (pal_to_go[color]>> 6) & 31;
				b2= (pal_to_go[color]>> 1) & 31;
				
				if((r==r2)&&(v==v2)&&(b==b2)) cpt++ ;
				else
				{
					if (r<r2) r++;
					else if (r>r2) r--;
					if (v<v2) v++;
					else if (v>v2) v--;
					if (b<b2) b++;
					else if (b>b2) b--;
					
					HARDWARE_PALETTE[color] = (r<<11) | (v<<6) | (b<<1) | (HARDWARE_PALETTE[color]&1) ;
	 			}
	 	} ;
		
		pause(speed);
	 } while(cpt != 256) ;
}

void fade_color(short powa, short speed, char repeat, char color_id)
{
	int color, cpt=0 ;
	unsigned short r,v,b ;

	for(cpt=0;cpt<repeat;cpt++)
	{	
		for (color=0;color<256;color++)
		{
			r= (HARDWARE_PALETTE[color]>>11) & 31;
			v= (HARDWARE_PALETTE[color]>> 6) & 31;
			b= (HARDWARE_PALETTE[color]>> 1) & 31;
	
			if(color_id == 'r')
			{
				if(powa > 0)
				{
					if(r!=0xFFFF)
						r++ ;
				} else {
						if(r!=0x0000)
							r-- ;
					}
			}
			else
			{
				if(color_id == 'v')
				{
					if(powa > 0)
					{
						if(v!=0xFFFF)
							v++ ;
					} else {
							if(v!=0x0000)
								v-- ;
						}
				}
				else
				{
					if(powa > 0)
					{
						if(b!=0xFFFF)
							b++ ;
					} else {
							if(b!=0x0000)
								b-- ;
						}
				}
			} ;
			
			HARDWARE_PALETTE[color] = (r<<11) | (v<<6) | (b<<1) | 0;
			pause(speed);
		} ;
	} ;
}


