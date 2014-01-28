
#include "Gdl.h"  // gdl 0.4e

#include ".\gfx\ball.h"
#include ".\gfx\curseur.h"
#include ".\gfx\star.h"
#include ".\gfx\font.h"

#define enabled custom_short_var[0]
#define sens_x custom_short_var[1]
#define sens_y custom_short_var[2]

struct gfx balls[60] ;
struct gfx curseur ;
struct gfx etoile ;

unsigned char nb_balls = 2 ;
unsigned short cpt, cpt1, count=1,trace=1,points=0,colision=0  ;

short tracing = 0 ;
short origine_x, origine_y ;
short power_1, power_2 ;

uint pixels_number_per_percent = 740 ; // real : 741,29
uint pixels_number = 74000, percent ;
uint curent_p_num = 0 ;

char color_id = 0 ;

char buffer[30] ;

long time ;

uchar * game_zone ;

ushort game_color = 0xAB ;

unsigned char sens_curseur = 0 ;

short lives = 5 ;

short test_pixel_with_balls_and_put(short x, short y)
{
	short cpt_ball ;

	PutPixel(1,x,y) ;

	for(cpt_ball=0;cpt_ball<50;cpt_ball++)
		if(balls[cpt_ball].enabled)
			if(test_pixel_colision_gfx_point(&balls[cpt_ball],x,y,balls[cpt_ball].pos_x,balls[cpt_ball].pos_y,0))
				return 1 ;

			return 0 ;
}

uint set_color_buffer_for_jezzball(short x, short y, short size_y, unsigned char *buffer, unsigned char color)
{
	static uint pixel_draw = 0 ;
	uchar * yo = buffer + x*size_y + (size_y-1-y) ;

	if(*yo!=color) { *yo = color ; pixel_draw++ ; }
	return pixel_draw ;
}

void draw_fade_bg(uchar *buffer,ushort bg_color,ushort back_clr,ushort up_clr)
{
	short cptx,cpty, tmp_color, back ;

	for(cptx=0;cptx<320;cptx++)
		for(cpty=0;cpty<240;cpty++)
		{
			if(get_color_buffer(cptx,cpty,240,game_zone)==bg_color)	PutPixel_16(fade_16b_color(get_screen_pixel_16(cptx,cpty),0),cptx,cpty) ;
			else							PutPixel_16(fade_16b_color(get_screen_pixel_16(cptx,cpty),1),cptx,cpty) ;
		} ;
}

void GpMain(void *arg)
{
	struct font font8 ;

	init_screen(8) ;

	load_font(&font8,(uchar *)FONT8,8,8,0xFD,95) ;

	set_system_font(&font8) ;

	load_anim(&curseur,0,0,8,(unsigned char *)curseur_img,100,1,0,20,20,170,110,0xFD) ;
	load_anim(&etoile,0,0,1,(unsigned char *)star,100,1,0,16,16,170,110,0xFD) ;
	curseur.animation[0].frame_stop = 3 ;
	set_anim(&curseur,1,4,7,100,1,0) ;

	for(cpt=0;cpt<50;cpt++)
	{
		load_anim(&balls[cpt],0,0,1,(unsigned char *)ball,75,1,0,10,10,130,100,0xFD) ;

		if(cpt<nb_balls)
		{
			balls[cpt].enabled = 1 ;
			balls[cpt].pos_x = rand()&300 ;
			balls[cpt].pos_y = rand()&220 ;
			balls[cpt].sens_x = rand()%2 ;
			balls[cpt].sens_y = rand()%2 ;
		} else balls[cpt].enabled = 0 ;
	} ;

	srand(GpTickCountGet()) ;

	game_zone = gm_zi_malloc(320*240);

	cpt = 19200 ;

	do { ((uint *)game_zone)[cpt] = 0xffffffff ; } while(cpt--) ;

	// 315*235 = 74025 pixels

	for(cpt=1;cpt<319;cpt++)
	{
		set_color_buffer_for_jezzball(cpt,2,240,game_zone,game_color) ;
		set_color_buffer_for_jezzball(cpt,1,240,game_zone,game_color) ;

		set_color_buffer_for_jezzball(cpt,237,240,game_zone,game_color);
		set_color_buffer_for_jezzball(cpt,238,240,game_zone,game_color);
	};

	for(cpt=1;cpt<239;cpt++)
	{
		set_color_buffer_for_jezzball(2,cpt,240,game_zone,game_color) ;
		set_color_buffer_for_jezzball(1,cpt,240,game_zone,game_color) ;

		set_color_buffer_for_jezzball(318,cpt,240,game_zone,game_color);
		set_color_buffer_for_jezzball(317,cpt,240,game_zone,game_color);
	};
*/
	afficher_bg_32bit((uint *)game_zone) ;
	flip_screen() ;
	afficher_bg_32bit((uint *)game_zone) ;


	do {
			//clr_scr() ;

			afficher_bg_32bit((uint *)game_zone) ;
			//draw_fade_bg(game_zone,game_color,0xFF,0x00) ; // tooo slow but good graphic effect

			for(cpt=0;cpt<lives;cpt++)	draw_gfx_xy(&etoile,300-cpt*20,5,0,0) ;

			//draw_int(&font8,curent_p_num,5,5) ;

			for(cpt=0;cpt<50;cpt++)
			{
				if(balls[cpt].enabled)
				{
					draw_gfx(&balls[cpt],0,0) ;

					for(cpt1=0;cpt1<50;cpt1++)
						if(balls[cpt1].enabled)
							if(cpt!=cpt1)
							{
								if(test_colision_pixel(&balls[cpt],&balls[cpt1],0,0,count,trace))
								{
									balls[cpt].sens_y ^=1 ;
									balls[cpt].sens_x ^=1 ;
								}
							}

					if(balls[cpt].sens_x) // right
					{
						if(get_color_buffer(balls[cpt].pos_x+10,balls[cpt].pos_y+5,240,game_zone)!=game_color) balls[cpt].pos_x+=2 ;
						else balls[cpt].sens_x ^=1 ;
					}
					else  { // left
						if(get_color_buffer(balls[cpt].pos_x,balls[cpt].pos_y+5,240,game_zone)!=game_color) balls[cpt].pos_x-=2 ;
						else balls[cpt].sens_x ^=1 ;
					      }

					if(balls[cpt].sens_y) // down
					{
						if(get_color_buffer(balls[cpt].pos_x+5,balls[cpt].pos_y+10,240,game_zone)!=game_color) balls[cpt].pos_y+=2 ;
						else balls[cpt].sens_y ^=1 ;
					}
					else  { // up
						if(get_color_buffer(balls[cpt].pos_x+5,balls[cpt].pos_y,240,game_zone)!=game_color) balls[cpt].pos_y-=2 ;
						else balls[cpt].sens_y ^=1 ;
					      }
				}
			} ;

			refresh_key() ;
			//count_and_draw_fps() ;

		if(!tracing)
		{
			if(key_pressed&1) // gauche
			{
				curseur.pos_x-=3 ;
			}

			if(key_pressed&4) // droite
			{
				curseur.pos_x+=3 ;
			}

			if(key_pressed&8) // haut
			{
				curseur.pos_y-=3 ;
			}

			if(key_pressed&2) // bas
			{
				curseur.pos_y+=3 ;
			}
		}
		else
		{
			unsigned char finish=0 ;

			if(sens_curseur)
			{
				if(get_color_buffer(origine_x + power_2,origine_y,240,game_zone)!=game_color) power_2+=2 ;
				else finish++ ;

				if(get_color_buffer(origine_x - power_1,origine_y,240,game_zone)!=game_color) power_1+=2 ;
				else finish++ ;

				if(tracing)
					for(cpt=origine_x;cpt<origine_x + power_2;cpt++)
						if(tracing)
							if(test_pixel_with_balls_and_put(cpt,origine_y))
							{	tracing=0 ;	}//cpt=320 ;	}

				if(tracing)
					for(cpt=origine_x;cpt>origine_x - power_1;cpt--)
						if(tracing)
							if(test_pixel_with_balls_and_put(cpt,origine_y))
							{	tracing=0 ;	}//cpt=0 ;	}

				if(tracing)
					for(cpt=origine_x;cpt<origine_x + power_2;cpt++)
						if(tracing)
							if(test_pixel_with_balls_and_put(cpt,origine_y+1))
							{	tracing=0 ;	}//cpt=320 ;	}

				if(tracing)
					for(cpt=origine_x;cpt>origine_x - power_1;cpt--)
						if(tracing)
							if(test_pixel_with_balls_and_put(cpt,origine_y+1))
							{	tracing=0 ;	}//cpt=0 ;	}

				if(!tracing)	lives-- ;

			if(tracing)
				if(finish==2)
				{
					short cpt_x, cpt_y, cpt_ball, ball_found=0 ;

					tracing=0 ;

					// draw line on buffer
					for(cpt=origine_x;cpt<origine_x + power_2;cpt++) { set_color_buffer_for_jezzball(cpt,origine_y,240,game_zone,game_color) ; 	pixels_number-- ; }
					for(cpt=origine_x;cpt>origine_x - power_1;cpt--) { set_color_buffer_for_jezzball(cpt,origine_y,240,game_zone,game_color) ; 	pixels_number-- ; }
					for(cpt=origine_x;cpt<origine_x + power_2;cpt++) { set_color_buffer_for_jezzball(cpt,origine_y+1,240,game_zone,game_color) ; pixels_number-- ; }
					for(cpt=origine_x;cpt>origine_x - power_1;cpt--) { set_color_buffer_for_jezzball(cpt,origine_y+1,240,game_zone,game_color) ; pixels_number-- ; }

					// search rect up of the line

					cpt = origine_y - 1 ;
					while(get_color_buffer(origine_x,cpt,240,game_zone) != game_color) cpt-- ;

					// search for a ball in the rect up

					ball_found=0 ;
					for(cpt_ball=0;cpt_ball<50;cpt_ball++)
						if(balls[cpt_ball].enabled)
						{
							if(balls[cpt_ball].pos_x<origine_x+power_2) // test x droite
								if(balls[cpt_ball].pos_x+balls[cpt_ball].size_x>origine_x-power_1) // test x gauche
									if(balls[cpt_ball].pos_y<origine_y) // test y bas
										if(balls[cpt_ball].pos_y + balls[cpt_ball].size_y>cpt) // test y haut
										{ // ball in the rect
											ball_found = 1 ;
											cpt_ball = 50 ; // break the for
										}
						}

					// draw rect
					if(!ball_found)
						for(cpt_x=origine_x-power_1;cpt_x<origine_x+power_2;cpt_x++)
							for(cpt_y=origine_y;cpt_y>cpt;cpt_y--)
							{	set_color_buffer_for_jezzball(cpt_x,cpt_y,240,game_zone,game_color) ; 	pixels_number-- ; }

					// search rect down of the line

					cpt = origine_y + 2 ;
					while(get_color_buffer(origine_x,cpt,240,game_zone) != game_color) cpt++ ;

					// search for a ball in the rect down
					ball_found=0 ;
					for(cpt_ball=0;cpt_ball<50;cpt_ball++)
						if(balls[cpt_ball].enabled)
						{
							if(balls[cpt_ball].pos_x<origine_x+power_2) // test x droite
								if(balls[cpt_ball].pos_x+balls[cpt_ball].size_x>origine_x-power_1) // test x gauche
									if(balls[cpt_ball].pos_y + balls[cpt_ball].size_y>origine_y) // test y haut
										if(balls[cpt_ball].pos_y<cpt) // test y bas
										{ // ball in the rect
											ball_found = 1 ;
											cpt_ball = 50 ; // break the for
										}
						}

					// draw rect
					if(!ball_found)
						for(cpt_x=origine_x-power_1;cpt_x<origine_x+power_2;cpt_x++)
							for(cpt_y=origine_y;cpt_y<cpt;cpt_y++)
							{	set_color_buffer_for_jezzball(cpt_x,cpt_y,240,game_zone,game_color) ; 	pixels_number-- ; }
				}
			}
			else
			{
				if(get_color_buffer(origine_x,origine_y + power_2,240,game_zone)!=game_color) power_2+=2 ;
				else finish++ ;

				if(get_color_buffer(origine_x,origine_y - power_1,240,game_zone)!=game_color) power_1+=2 ;
				else finish++ ;

				if(tracing)
					for(cpt=origine_y;cpt<origine_y + power_2;cpt++)
						if(tracing)
							if(test_pixel_with_balls_and_put(origine_x,cpt))
							{	tracing=0 ;	}//cpt=320 ;	}

				if(tracing)
					for(cpt=origine_y;cpt>origine_y - power_1;cpt--)
						if(tracing)
							if(test_pixel_with_balls_and_put(origine_x,cpt))
							{	tracing=0 ;	}//cpt=0 ;	}

				if(tracing)
					for(cpt=origine_y;cpt<origine_y + power_2;cpt++)
						if(tracing)
							if(test_pixel_with_balls_and_put(origine_x-1,cpt))
							{	tracing=0 ;	}//cpt=320 ;	}

				if(tracing)
					for(cpt=origine_y;cpt>origine_y - power_1;cpt--)
						if(tracing)
							if(test_pixel_with_balls_and_put(origine_x-1,cpt))
							{	tracing=0 ;	}//cpt=0 ;	}

				if(!tracing)	lives-- ;

			if(tracing)
				if(finish==2)
				{
					short cpt_x, cpt_y, cpt_ball, ball_found=0 ;

					tracing=0 ;

					// draw line on buffer
					for(cpt=origine_y;cpt<origine_y + power_2;cpt++) { set_color_buffer_for_jezzball(origine_x,cpt,240,game_zone,game_color) ; 	pixels_number-- ; }
					for(cpt=origine_y;cpt>origine_y - power_1;cpt--) { set_color_buffer_for_jezzball(origine_x,cpt,240,game_zone,game_color) ; 	pixels_number-- ; }
					for(cpt=origine_y;cpt<origine_y + power_2;cpt++) { set_color_buffer_for_jezzball(origine_x-1,cpt,240,game_zone,game_color) ; pixels_number-- ; }
					for(cpt=origine_y;cpt>origine_y - power_1;cpt--) { set_color_buffer_for_jezzball(origine_x-1,cpt,240,game_zone,game_color) ; pixels_number-- ; }

					// search rect left of the line

					cpt = origine_x - 2 ;
					while(get_color_buffer(cpt,origine_y,240,game_zone) != game_color) cpt-- ;

					// search for a ball in the rect left

					ball_found=0 ;
					for(cpt_ball=0;cpt_ball<50;cpt_ball++)
						if(balls[cpt_ball].enabled)
						{
							if(balls[cpt_ball].pos_x+balls[cpt_ball].size_x > cpt) // test x gauche
								if(balls[cpt_ball].pos_x < origine_x) // test x droite
									if(balls[cpt_ball].pos_y+balls[cpt_ball].size_y > origine_y-power_1) // test y haut
										if(balls[cpt_ball].pos_y < origine_y+power_2) // test y bas
										{ // ball in the rect
											ball_found = 1 ;
											cpt_ball = 50 ; // break the for
										}
						}

					// draw rect
					if(!ball_found)
						for(cpt_x=origine_x;cpt_x>cpt;cpt_x--)
							for(cpt_y=origine_y-power_1;cpt_y<origine_y+power_2;cpt_y++)
							{	set_color_buffer_for_jezzball(cpt_x,cpt_y,240,game_zone,game_color) ; 	pixels_number-- ; }

					// search rect right of the line

					cpt = origine_x+1 ;
					while(get_color_buffer(cpt,origine_y,240,game_zone) != game_color) cpt++ ;

					// search for a ball in the rect right
					ball_found=0 ;
					for(cpt_ball=0;cpt_ball<50;cpt_ball++)
						if(balls[cpt_ball].enabled)
						{
							if(balls[cpt_ball].pos_x+balls[cpt_ball].size_x>origine_x) // test x gauche
								if(balls[cpt_ball].pos_x<cpt) // test x droite
									if(balls[cpt_ball].pos_y + balls[cpt_ball].size_y>origine_y-power_1) // test y haut
										if(balls[cpt_ball].pos_y<origine_y+power_2) // test y bas
										{ // ball in the rect
											ball_found = 1 ;
											cpt_ball = 50 ; // break the for
										}
						}

					// draw rect
					if(!ball_found)
						for(cpt_x=origine_x;cpt_x<cpt;cpt_x++)
							for(cpt_y=origine_y-power_1;cpt_y<origine_y+power_2;cpt_y++)
 							{	set_color_buffer_for_jezzball(cpt_x,cpt_y,240,game_zone,game_color) ; 	pixels_number-- ; }
				}
			}
		}

			play_anim(&curseur,sens_curseur,0) ;

			if(continu_key(16))
			{
				switch (color_id)
				{
					case 0:
			 			fade_color(1,0,1,'r') ;
			 			break ;
					case 1:
			 			fade_color(1,0,1,'v') ;
			 			break ;
					case 2:
			 			fade_color(1,0,1,'b') ;
			 			break ;
				}
			}

			if(up_front(128))
			{
				color_id++ ;
				if(color_id==3) color_id = 0 ;
			}

			if(up_front(16))
				if(up_front(128))
					GpAppExit();

				if(up_front(256))
					if(up_front(512))
					{
						gfx_draw_method ^= 1 ;
					}

				if(up_front(512))
				{
					cpt=0 ;
					while(balls[cpt].enabled) cpt++ ;

					balls[cpt].enabled = 1 ;
					balls[cpt].pos_x = rand()%290 + 10 ;
					balls[cpt].pos_y = rand()%210 + 10 ;
					balls[cpt].sens_x = rand()%2 ;
					balls[cpt].sens_y = rand()%2 ;
					nb_balls++ ;
				}

				if(up_front(256))
				{
					cpt=0 ;
					while(!(balls[cpt].enabled)) cpt++ ;
					balls[cpt].enabled = 0 ;
					nb_balls-- ;
				}

				if(!tracing)
					if(up_front(64))
					{
						tracing = 1 ;
						origine_x = curseur.pos_x + 10 ;
						origine_y = curseur.pos_y + 10 ;
						power_1 = 0 ;
						power_2 = 0 ;
					}

				if(!tracing)
					if(up_front(32))
						sens_curseur ^= 1 ;

			curent_p_num = set_color_buffer_for_jezzball(1,1,240,game_zone,game_color) ;
			percent = (curent_p_num/740)-2 ;
			draw_int(&font8,count_fps(),5,5) ;
			GPSPRINTF(buffer,"%i%",percent) ;
			draw_text_rl(&font8,buffer,45,5) ;

			flip_screen() ;

			if(!lives) key_pressed = 144 ;

	} while((!up_front(128)||!up_front(16))&&(percent < 85)) ; // L + R reboot
	GpAppExit() ;
}