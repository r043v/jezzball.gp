/* this file is a part of Gdl 0.4f and was writen by rov, http://procvor.free.fr */

//--------------------------------------------------------------

void get_extention(char * nom, char * extention)
{
	short i=0 ;
	
	while (nom[i] != '.') { i++; }
	
	i++ ;
	extention[0] = nom[i++] ;
	extention[1] = nom[i++] ;
	extention[2] = nom[i++] ;
	extention[3] = '\0' ;
}

//----------------- gpg/sef io ----------------------------------

// get a gpg or a sef file
uchar *get_gpg(char *filename)
{
	uchar		*data;
	F_HANDLE	hfile;
	ulong		p_read_count;
	ulong		fsize;
	
	hfile = NULL;
	
	if(SM_OK != GpFileOpen(filename, OPEN_R, &hfile))
	{
		char buffer[200] ;
		GPSPRINTF(buffer,"get_gpg : error|file not found\n%s",filename) ;
		MsgBox(buffer) ;
		return NULL ;
	}
	
	if(get_free_ram() < fsize + 10240) MsgBox("get_gpg : error|can't load file due to ram") ;
	
	GpFileGetSize(filename, &fsize);
	GpFileSeek(hfile, FROM_BEGIN, 8, 0); // skip header
	data = gm_zi_malloc(fsize-8); //  * sizeof(ushort)
	GpFileRead(hfile, data, fsize-8, &p_read_count);
	GpFileClose(hfile);
	return (data);
}

//----------------- palette io ----------------------------------

uint *get_pal(char *filename)
{
	uint		*data ;
	F_HANDLE	hfile ;
	ulong		p_read_count ;
	
	hfile =  NULL ;
	if(SM_OK != GpFileOpen(filename, OPEN_R, &hfile))
	{
		char buffer[200] ;		
		GPSPRINTF(buffer,"get_pal : error|file not found\n%s",filename) ;
		MsgBox(buffer) ;
		return NULL ;
	}
	
	if(get_free_ram() < 512 + 10240) MsgBox("get_pal : error|can't load file due to ram") ;
	
	GpFileSeek(hfile, FROM_BEGIN, 16, 0);
	data = gm_zi_malloc(sizeof(uint)*0xFF) ;
	GpFileRead(hfile, data, sizeof(uint)*0xFF, &p_read_count);
	GpFileClose(hfile);
	return data ;
}

//----------- pcx bitmap file io --------------------------------

#include "./pcx.h"

//----------------- map io --------------------------------------

struct map_file_header
{
	char	header[15] ;
	char	file_version[6] ;
	
	char	auteur[20] ;
	char 	map_name[50] ;
	
	ushort	size_x, size_y ;
	ushort	size_tile_x, size_tile_y ;	
	ushort	nb_tiles ;
	ushort	transp_color ;
	char	bbp ;
	
	short 	tiles_include ; // 0 in .gpg   1 yes
	short	palette_include ; // 0 no  1 yes  2 in .pal
	
	char	gpg_url[60] ;
	char	pal_url[60] ;
	char	comment[100] ;
} ;

struct map_header
{
	struct map_file_header		file_header ;

	uchar * map_array ;
	uchar * tiles_bitmap ;
	uint  * palette ;
} ;

void free_map(struct map_header * header)
{
	gp_mem_func.free((void*)header->map_array) ;
	gp_mem_func.free((void*)header->tiles_bitmap) ;
	gp_mem_func.free((void*)header->palette) ;
}

void save_map_file(ushort size_x,ushort size_y,ushort size_tile_x,ushort size_tile_y,ushort nb_tiles, ushort transp_color, char * auteur, char * comment, char * name, char * path, short * map_array, uint * palette, uchar * tiles_bitmap,char * gpg_url, char * pal_url)
{
	struct map_file_header header ;
	F_HANDLE hfile;
	
	GPSTRCPY(header.header,"_Gdl Map file_") ;
	GPSTRCPY(header.file_version,"1.2") ;
	GPSTRCPY(header.auteur,auteur) ;
	GPSTRCPY(header.comment,comment) ;
	GPSTRCPY(header.map_name,name) ;
	GPSTRCPY(header.gpg_url,gpg_url) ;
	GPSTRCPY(header.pal_url,pal_url) ;

	
	header.size_x = size_x ;
	header.size_y = size_y ;
	header.size_tile_x = size_tile_x ;
	header.size_tile_y = size_tile_y ;
	header.nb_tiles = nb_tiles ;
	header.transp_color = transp_color ;
	header.bbp = bbp ;
	
	header.tiles_include = 1 ;
	header.palette_include = 1 ;
	
	if(palette == NULL)	 header.palette_include = 0 ;
	if(header.palette_include)	if(pal_url != NULL)	header.palette_include = 2 ;
	
	if(gpg_url != NULL)	header.tiles_include = 0 ;
	
	GpFileCreate(path,ALWAYS_CREATE, &hfile);
	GpFileWrite(hfile,&header,sizeof(struct map_file_header));
	GpFileWrite(hfile,map_array,sizeof(short)*header.size_x*header.size_y);
	
	if(header.palette_include)
	{
		if(pal_url == NULL)	GpFileWrite(hfile,palette,sizeof(uint)*0xFF);
		else
		{
			F_HANDLE pal_file;
			// create .pal file
						
			GpFileCreate(pal_url,ALWAYS_CREATE, &pal_file);
			GpFileWrite(pal_file,"Gdl palette file",16) ;
			//GpFileSeek(gpg_file, FROM_BEGIN, 8, 0);
			GpFileWrite(pal_file,palette,sizeof(uint)*0xFF);
			GpFileClose(pal_file) ;
		}
	}

	if(gpg_url == NULL)	GpFileWrite(hfile,tiles_bitmap,header.nb_tiles*header.size_tile_x*header.size_tile_y*(bbp/8));
	else
	{		
		F_HANDLE gpg_file;
		header.tiles_include = 0 ;
		
		GpFileCreate(gpg_url,ALWAYS_CREATE, &gpg_file);
		GpFileWrite(gpg_file,"Gdl_gpg_",8) ;
		GpFileWrite(gpg_file,tiles_bitmap,header.nb_tiles*header.size_tile_x*header.size_tile_y*(bbp/8));
		GpFileClose(gpg_file) ;
	}

	GpFileClose(hfile) ;
}

struct map_file_header read_map_header(char * path)
{
	F_HANDLE hfile;
	ulong	p_read_count = 0 ;
	struct map_file_header map_header ;

	if(SM_OK == GpFileOpen(path, OPEN_R, &hfile))	GpFileRead(hfile,&map_header,sizeof(struct map_file_header),&p_read_count) ;

	GpFileClose(hfile) ;

	return map_header ;
}

struct map_header read_map_file(struct map * map, char * path)
{
	F_HANDLE hfile;

	struct map_header map_header ;
	uchar * map_array ;
	uchar * tiles_bitmap ;
	uint*  palette ;

	if(SM_OK == GpFileOpen(path, OPEN_R, &hfile))
	{
		ulong size ;
		ulong	p_read_count=0;
		
		// read header
		GpFileRead(hfile,&map_header.file_header,sizeof(struct map_file_header),&p_read_count);
		
		// read map_array
		size = sizeof(short)*map_header.file_header.size_x*map_header.file_header.size_y ;
		map_array = gm_zi_malloc(2*size);
		GpFileRead(hfile,map_array,size,&p_read_count);

		if(map_header.file_header.palette_include)
		{
			size = sizeof(uint)*0xFF ;
			palette = gm_zi_malloc(size);

			if(map_header.file_header.palette_include==1)
			{
				// read palette
				GpFileRead(hfile,palette,size,&p_read_count);
			}
			else
			{
				palette = get_pal(map_header.file_header.pal_url) ;
				/*F_HANDLE pal_file;
				ulong	read_count = 0 ;

				if(SM_OK == GpFileOpen(map_header.file_header.pal_url, OPEN_R, &hfile))
				{
					GpFileSeek(pal_file, FROM_BEGIN, 16, 0);
					GpFileRead(pal_file,palette,size,&read_count);
					GpFileClose(pal_file) ;
				}*/
			}
			
		} else palette = NULL ;

		if(map_header.file_header.tiles_include)
		{
			// read tile_set bitmap
			size = sizeof(uchar)*map_header.file_header.nb_tiles*map_header.file_header.size_tile_x*map_header.file_header.size_tile_y*(map_header.file_header.bbp/8) ;
			tiles_bitmap = gm_zi_malloc(size);
			GpFileRead(hfile,tiles_bitmap,size,&p_read_count);
		}	else tiles_bitmap = get_gpg(map_header.file_header.gpg_url) ;
		
				
		GpFileClose(hfile) ;

		map_header.tiles_bitmap = tiles_bitmap ;
		map_header.map_array = map_array ;
		map_header.palette = (uint *)palette ;

		set_map(map,(short *)map_array,map_header.file_header.size_x,map_header.file_header.size_y,0,0,tiles_bitmap,map_header.file_header.nb_tiles,map_header.file_header.size_tile_x,map_header.file_header.size_tile_y,map_header.file_header.transp_color) ;
	} else { MsgBox("read_map_file : error|file not found") ; }
	
	return map_header ;
}

struct map_header read_map_file_with_tile(struct map * map, char * path, uchar * tiles_bitmap)
{
	F_HANDLE hfile;

	struct map_header map_header ;
	uchar * map_array ;
	uint*  palette ;

	if(SM_OK == GpFileOpen(path, OPEN_R, &hfile))
	{
		ulong size ;
		ulong	p_read_count=0;
		
		// read header
		GpFileRead(hfile,&map_header.file_header,sizeof(struct map_file_header),&p_read_count);
		
		// read map_array
		size = sizeof(short)*map_header.file_header.size_x*map_header.file_header.size_y ;
		map_array = gm_zi_malloc(size);
		GpFileRead(hfile,map_array,size,&p_read_count);

		if(map_header.file_header.palette_include)
		{
			size = sizeof(uint)*256 ;
			palette = gm_zi_malloc(size);

			if(map_header.file_header.palette_include==1)
			{
				// read palette
				GpFileRead(hfile,palette,size,&p_read_count);
			}
			else
			{
				palette = get_pal(map_header.file_header.pal_url) ;
			}
			
		} else palette = NULL ;

		GpFileClose(hfile) ;

		map_header.tiles_bitmap = tiles_bitmap ;
		map_header.map_array = map_array ;
		map_header.palette = (uint *)palette ;

		set_map(map,(short *)map_array,map_header.file_header.size_x,map_header.file_header.size_y,0,0,tiles_bitmap,map_header.file_header.nb_tiles,map_header.file_header.size_tile_x,map_header.file_header.size_tile_y,map_header.file_header.transp_color) ;
	}
	else { MsgBox("read_map_file : error|file not found") ; }

	return map_header ;
}

struct map_header read_map_file_pcx(struct map * map, char * path, char * pcx_path) // pcx file must be 8 bits (256 colors)
{
	uchar * pcx_data ;
	pcx_data = load_pcx(pcx_path) ;

	return read_map_file_with_tile(map,path,pcx_data) ;
}

void copy_map_to_file(struct map * map, char * path, char * map_name, char * gpg_url, uint * palette, char * pal_url, char * map_author)
{
	save_map_file(map->size_x,map->size_y,map->size_tile_x,map->size_tile_y,map->nb_tiles,map->transp_color,map_author,Gdl_version,map_name,path,map->map,palette,map->tab_tiles[0].img[0],gpg_url,pal_url) ;
}


// ------------------------------------------------------------------------

// load an anim (gfx structure) from a pcx file

uchar * load_anim_from_pcx(struct gfx * gfx,char id_anim,uchar frm_size_y,uchar frm_number,ushort time, char repeat,char sens, ushort px,ushort py,char * pcx_path)
{
	uchar * anim ;
	ushort largeur, hauteur ;

	taille_pcx(pcx_path,&largeur,&hauteur) ;
	
	anim = load_pcx(pcx_path) ;

	if(anim == NULL) return NULL ;

	load_anim(gfx,id_anim,0,frm_number,anim,time,repeat,sens,frm_size_y,hauteur,px,py,get_color_buffer(0,0,hauteur,anim)) ;
	
	return anim ;
}

uchar * load_animated_tile_from_pcx(struct map * map, char * pcx_url, ushort id_tile, uchar type_anim, ushort time_anim, uchar frame_init)
{
	uchar * anim ;
	ushort largeur, hauteur ;
	uchar frm_number ;

	taille_pcx(pcx_url,&largeur,&hauteur) ;	
	anim = load_pcx(pcx_url) ;

	if(anim == NULL) return NULL ;

	frm_number = largeur/map->size_tile_x ;
	//frm_number-- ;

	load_animated_tile(map, anim, frm_number, id_tile, type_anim, time_anim, frame_init) ;
	
	return anim ;
}

// ------------------------------------------------------------------------






/*void read_dir(char * path, GPDIRENTRY * liste, &number_of_file, char * extention)
{
	GPDIRENTRY entry[_max_file_read] ;
	short cpt ;
	char ext[4] ;
	cpt_found = 0 ;
	
	GpDirEnumList(path,0,_max_file_read,entry,&number_of_file) ;

	for(cpt=2;cpt<number_of_file;cpt++)
	{
		get_extention(entry[cpt].name,ext) ;
		
		if(!GPSTRCMP(extention,(char*)ext))
		{
			liste[cpt_found].name = entry[cpt].name ;
			cpt_found++ ;
		}
		
		*number_of_file = cpt_found-1 ;
	} ;
	
	return liste ;
}*/


//-----------------------------------------------------------------------
/*
struct BITMAPFILEHEADER {  	    // Offset   Size
    short   bfType;                 //      0      2
    long    bfSize;                 //      2      4
    short   bfReserved1;            //      6      2
    short   bfReserved2;            //      8      2
    long    bfOffBits;              //     10      4
} BITMAPFILEHEADER;                 // Total size: 14

struct BITMAPINFOHEADER {	    // Offset   Size
    long    biSize;                 //      0      4
    long    biWidth;                //      4      4
    long    biHeight;               //      8      4
    short   biPlanes;               //     12      2
    short   biBitCount;             //     14      2
    long    biCompression;          //     16      4
    long    biSizeImage;            //     20      4
    long    biXPelsPerMeter;        //     24      4
    long    biYPelsPerMeter;        //     28      4
    long    biClrUsed;              //     32      4
    long    biClrImportant;         //     36      4
} BITMAPINFOHEADER;                 // Total size: 40

struct RGBQUAD {
	uchar	rgbBlue ;
	uchar	rgbGreen ;
	uchar	rgbRed ;
	uchar	rgbReserved ;
} RGBQUAD;

struct bmp_header
{
	struct BITMAPFILEHEADER		header1 ;
	struct BITMAPINFOHEADER		header2 ;

	uchar * bmp_data ;
	uint  * palette ;
} ;
*/
// convert bmp palette to gp palette
/*void create_palette(struct RGBQUAD * pal, uint * palette)
{
	ushort i ;

	for(i=0;i<0xFF;i++)	palette[i] = (pal[i].rgbRed<<11) | (pal[i].rgbGreen<<6) | (pal[i].rgbBlue<<1) | 0;
}*/
/*
struct bmp_header load_bmp_file(char *filename)
{
	F_HANDLE hfile;
	struct bmp_header header ;
	ulong	p_read_count;
	short cpt_x,cpt_y,cpt ;
	uint * palette ;
	struct RGBQUAD * pal ;
	uchar * data ;
	char buf ;
	
	if(SM_OK == GpFileOpen(filename, OPEN_R, &hfile))
	{
		GpFileRead(hfile,(char *)&header.header1,sizeof(struct BITMAPFILEHEADER),&p_read_count);
		GpFileRead(hfile,(char *)&header.header2,sizeof(struct BITMAPINFOHEADER),&p_read_count);

		pal = gm_zi_malloc(sizeof(struct RGBQUAD)*0xFF) ;
		GpFileRead(hfile, pal, sizeof(struct RGBQUAD)*0xFF, &p_read_count);

		create_palette(pal,palette) ;

		header.palette = palette ;

		//data = gm_zi_malloc(header.header2.biWidth*header.header2.biHeight);

		//GpFileRead(hfile, data,header.header2.biWidth*header.header2.biHeight, &p_read_count);
		
		for(cpt_x=0;cpt_x<header.header2.biWidth;cpt_x++)
			for(cpt_y=0;cpt_y<header.header2.biHeight;cpt_y++)
			{	GpFileRead(hfile,&buf,1,&p_read_count);
				set_color_buffer(cpt_x,cpt_y,header.header2.biHeight,data,buf) ;
			} ;
			
		header.bmp_data = get_gpg("gp:\\gpmm\\dstar\\tile.gpg") ;
		//header.bmp_data = data ;

		GpFileClose(hfile);
		
		//header.bmp_data = get_gpg("gp:\\gpmm\\dstar\\tile.gpg") ;
	} else GpAppExit() ;

	//wait_key(512) ;

	return header ;
}
*/

