/* this file is a part of Gdl 0.4f and was writen by rov, http://procvor.free.fr */

struct anim // animation : 30 anim max par gfx , au max 100 img ds un gfx
{
	uchar curent_frame ;		// position actuelle ds l'anim
	uchar frame_start ;		// premiere image de l'anim (parmi les 100 de l'objet gfx)
	uchar frame_stop ;		// derniere image de l'anim
	ushort time_anim ;	// temps en miliseconde entre chaque changement de frame
	uchar finish ;			// 0 : anim en cour   1 : anim terminé
	
	uchar repeat ; // 0=123 1=123212.. 2=123123.. 3=123333.. 4=123111..
	uchar sens ; // 0=123 1=321
	
	long last_aff_time ;		// moment du denier changement de frame
} ;


struct gfx
{
	// info image
	uchar *img[_max_image_number] ;	// tableau de pointeur sur les images (100 images max)
	uchar transp[_max_image_number] ;
	short move[_max_image_number][2] ;
	
	short real_size_x[_max_image_number], real_size_y[_max_image_number], real_pos_x[_max_image_number], real_pos_y[_max_image_number] ;
	
	uchar moving ;
	
	ushort size_x, size_y ;	// tailles x et y de l'objet
	int pos_x, pos_y ;		// position x et y de l'objet a l'ecran
	ushort transp_color ;	// couleur de transpartence de l'objet
	
	struct anim animation[_max_anim_number] ;	// les animations
	
	// variable non utilisée, pour l'utilisateur, utilisez des #define c bc mieux :) ex : #define enabled custom_short_var[0]
	long custom_var[_max_custom_long_var] ;
	short custom_short_var[_max_custom_short_var] ;
} ;

struct tile
{
	uchar * img[_max_tile_anim_size] ; // anim de 10 image max pour les tiles
	short real_size_x[_max_tile_anim_size], real_size_y[_max_tile_anim_size], real_pos_x[_max_tile_anim_size], real_pos_y[_max_tile_anim_size] ;

	uchar anim_id ;		// position actuelle ds l'anim du tile
	uchar nb_tiles ;	// nb d'image ds l'anim du tile
	uchar animated ;	// 1 : le tile est animé  0 : tile fixe
	uchar sens_anim ;	// 0 : 123  1 : 321
	uchar type_anim ;	// 0 : 123123...  1 : 123333... 2 : 123111... 3 : 1232123...
	ushort time_anim ;	// temps en millisecondes entre chaque images de l'anim
	
	uchar transp[_max_tile_anim_size] ;		// 1 : tile transparent      0 : non      0xFF tout transp
	
	long last_time ;	// moment du denier changement de frame
	
	uchar walkable ;
	
	short custom_short_var[_max_custom_short_var] ;
} ;

struct map
{
	ushort size_x ;
	ushort size_y ;

	uchar size_tile_x ;
	uchar size_tile_y ;

	uint map_x_max ; // size_x*size_tile_x - 320
	uint map_y_max ; // size_y*size_tile_x - 240

	ushort	nb_tiles ;

	int scrool_x ;
	int scrool_y ;
	
	uchar nb_tile_draw_x ;
	uchar nb_tile_draw_y ;
	
	struct tile tab_tiles[_max_tile_number] ;

	ushort transp_color ;
	
	char transp_map ;
	
	short * map ;
	
	short custom_short_var[_max_custom_short_var] ;
} ;

struct font
{
	unsigned char *img[0xFF] ;
	unsigned char size_x, size_y ;
	unsigned short transp_color ;
} ;

struct font * gdl_font ; // gdl system font
